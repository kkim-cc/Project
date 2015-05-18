/* System definitions */
.equ RAMSTART, 0x0100 /* ATmega328p SRAM range is 0x0100-0x08FF (2KB) */
.equ RAMEND,   0x08FF
.equ SPL,      0x3d
.equ SPH,      0x3e
.equ SREG,     0x3f
.equ PIND,     0x09
.equ DDRD,     0x0a
.equ PORTD,    0x0b
.equ PINB,     0x03
.equ DDRB,     0x04
.equ PORTB,    0x05
.equ TCCR0A,   0x24
.equ TCCR0B,   0x25
.equ TCNT0,    0x26
.equ OCR0A,    0x27
.equ TIMSK0,   0x6e     /* ext I/O space */
.equ CS,       0
.equ CS_1024,  0b101
.equ TOIE0,    0
.equ WGM,      0
.equ OCIE0A,   1


.section .vectors


/* on ATmega328p each vector is 2 words (4B), and JMP instruction fits perfectly */
vectors:
    jmp     init            /* reset */
    jmp     nullisr         /* ext_int0 */
    jmp     nullisr         /* ext_int1 */
    jmp     nullisr         /* pcint0 */
    jmp     nullisr         /* pcint1 */
    jmp     nullisr         /* pcint2 */
    jmp     nullisr         /* WDT */
    jmp     nullisr         /* timer2 cmpA */
    jmp     nullisr         /* timer2 cmpB */
    jmp     nullisr         /* timer2 overflow */
    jmp     nullisr         /* timer1 capture */
    jmp     nullisr         /* timer1 cmpA */
    jmp     nullisr         /* timer1 cmpB */
    jmp     nullisr         /* timer1 overflow */
    jmp     timerisr        /* timer0 cmpA */
    jmp     nullisr         /* timer0 cmpB */
    jmp     nullisr         /* timer0 overflow */
    jmp     nullisr         /* SPI tx complete */
    jmp     nullisr         /* USART rx complete */
    jmp     nullisr         /* USART dr empty */
    jmp     nullisr         /* USART tx complete */
    jmp     nullisr         /* ADC */
    jmp     nullisr         /* EEPROM ready */
    jmp     nullisr         /* analog cmp */
    jmp     nullisr         /* 2-wire serial */
    jmp     nullisr         /* store PM */


.section .text


nullisr:
    reti


/* ----------------------------------
 * Timer ISR
 * ---------------------------------- */
timerisr:
    push    r0
    push    r20
    in      r0, SREG
    lds     r20, tick
    inc     r20
    cpi     r20, 125
    brne    keepcounting
    clr     r20
    /* decrement system time (MM:SS) */
    push    r21
    lds     r21, systime+1
    cpi     r21, 0
    brne    nooverflow_s
    push    r22
    lds     r22, systime+0
    cpi     r22, 0
    breq    keepcounting
    dec     r22         /* decrement minutes*/
    sts     systime+0, r22
    pop     r22
    ldi     r21, 60
nooverflow_s:
    dec     r21         /* decrement seconds */
    sts     systime+1, r21
    pop     r21
keepcounting:
    sts     tick, r20
    out     SREG, r0
    pop     r20
    pop     r0
    reti


/* ----------------------------------
 * system init
 * ---------------------------------- */
init:
    /* setup a stack */
    ldi     r17, hi8(RAMEND)
    ldi     r16, lo8(RAMEND)
    out     SPH, r17
    out     SPL, r16
    /* copy .data to RAM; the following symbols are defined in the linker script */
    ldi     r31, hi8(_data_start_lma)
    ldi     r30, lo8(_data_start_lma)
    ldi     r29, hi8(_data_start_vma)
    ldi     r28, lo8(_data_start_vma)
    ldi     r16, lo8(_data_size)
copydata:
    lpm     r0, z+
    st      y+, r0
    dec     r16
    brne    copydata
    /* config peripherals */
    rcall   ser_init
    rcall   timer_init
    /* interrupts live, run main*/
    sei             /* SEt Interrupts - interrupt enable */
                    /* cli : CLear Interrupts - interrupt disable */
    rjmp    main

    
/* ----------------------------------
 * main program
 * r19:r20 check timer regsiter
 * ---------------------------------- */
main:
    ldi     r25, hi8(str_start)
    ldi     r24, lo8(str_start)
    rcall   ser_print
    
    sbi     DDRB,  0    /* configure as output     Set Bit in I/O Register */
    cbi     PORTB, 0    /* initial output is low   Clear Bit in I/O Register*/

    sbi     PORTD, 7    /* activate pull-up */
    cbi     DDRD,  7    /* config as input */
    nop                 /* synchronize */
    
    cbi     PORTB, 0    /* CBI – Clear Bit in I/O Register */

    lds     r19, systime
    lds     r20, systime+1
    
wait_down:
    rcall   chk_timer   /* call if interrupts enable */
    in      r16, PIND
    sbrc    r16, 7      /* SBRC – Skip if Bit in Register is Cleared */
    rjmp    wait_down
    /* wait for release */
wait_up:
    rcall   chk_timer   /* call if interrupts enable */
    in      r16, PIND
    sbrs    r16, 7      /* SBRS – Skip if Bit in Register is Set */
    rjmp    wait_up
    
    /* call reset timer */
    rcall   reset_timer
    
    rjmp    wait_down


/* ----------------------------------
 * Reset timer
 * ---------------------------------- */
reset_timer:
    push    r16
    /* init timer */
    ldi     r16, 0
    sts     systime+1, r16
    ldi     r16, 3
    sts     systime+0, r16
    /* set interrupts enable */
    sei
    /* turn light off */
    cbi     PORTB, 0        /* CBI – Clear Bit in I/O Register */
    pop     r16
    ret

/* ----------------------------------
 * Check timer
 * r19:r20(mm:ss) check timer regsiter [input/output]
 * if(timer && change time) print time 
 * if(timer && times up) then 
 *           stop interrupt 
 *           clear r18 
 *           turn light on 
 *           print "Coffee is ready..."
 * 
 * ---------------------------------- */
chk_timer:
    push    r18
    brid    chk_timer_end
    /* check timer */
    lds     r18, systime+1
    cp      r18, r20
    breq    chk_timer_end
    /* save current timer and print */
    lds     r19, systime+0
    lds     r20, systime+1
    rcall   print_time
    /* check times up */
    cpi     r19, 0
    brne    chk_timer_end
    cpi     r20, 0
    brne    chk_timer_end
    /* stop interrupts */
    cli
    /* turn light on.. */
    sbi     PORTB, 0        /* SBI – Set Bit in I/O Register */
    /* print "Coffee is ready..." */
    push    r25
    push    r24
    ldi     r24, 10
    rcall   ser_tx    
    ldi     r24, 13
    rcall   ser_tx
    ldi     r25, hi8(str_timesup)
    ldi     r24, lo8(str_timesup)
    rcall   ser_print
    pop     r24
    pop     r25
chk_timer_end:
    pop     r18
    ret
/* ----------------------------------
 * ---------------------------------- */

/* ----------------------------------
 * Print system time
 * ---------------------------------- */
print_time:
    cli                     /* ensure atomic access to both bytes of the clock */
    lds     r24, systime
    rcall   ser_decnum
    ldi     r24, ':'
    rcall   ser_tx
    lds     r24, systime+1
    rcall   ser_decnum
    sei
    ldi     r24, 13         /* 10/13 */
    rcall   ser_tx
    ret


/* ----------------------------------
 * config the timer
 * ---------------------------------- */
timer_init:
    /* 16MHz clock scaled down by 1024 increments every 64us,
       if I count 125 of those then I get a tick of 8ms */
    ldi     r16, 125            /* output compare A */
    out     OCR0A, r16
    ldi     r16, (2<<WGM)       /* mode CTC (clear timer on compare) */
    out     TCCR0A, r16
    ldi     r16, (1<<OCIE0A)    /* interrupt enable for compare A */
    sts     TIMSK0, r16
    ldi     r16, (CS_1024 << CS)
    out     TCCR0B, r16         /* clock select */
    ret


/* ----------------------------------
 * delay function
 * ---------------------------------- */
delay:
    push    r28         /* save them before use */
    push    r29
delay_again:
    clr     r28
    clr     r29
delay_loop:
    adiw    r28, 1      /* this word add uses pair R29:R28 */
    brcc    delay_loop  /* count up to 0xFFFF */
    dec     r24
    brne    delay_again
    pop     r29         /* restore, in reverse order */
    pop     r28
    ret


.section .data


str_start:
    .asciz "Timer program...\n\r"
str_timesup:
    .asciz "Coffee is ready...\n\r"
    
/* Use the below location (variable) to store the state of the button */
button:
    .byte 1     /* 1 == released, 0 == pressed */
tick:
    .byte 0
systime:
    .byte 3, 0   /* MM:SS */

