.equ UCSR0A,0xc0    /* serial regs */
.equ UCSR0B,0xc1
.equ UDR0,  0xc6
.equ UBRR0L,0xc4
.equ RXEN0, 4       /* serial bits */
.equ TXEN0, 3       /* serial bits */
.equ UDRE0, 5
.equ RXC0,  7

/* system clock is 16.000MHz, to get 9600 baud use this formula:
 * baudval = (system freq/(16*baud))-1 */
.equ BAUDVAL, 103

.global ser_tx
.global ser_rx
.global ser_init
.global ser_print
.global ser_hexnum
.global ser_readline
.global ser_decnum

/* read a full line */
ser_readline:
    movw    r28, r24
input_next:
    rcall   ser_rx
    push    r24
    rcall   ser_tx                  /* echo it back */
    pop     r24
    cpi     r24, 0x0d               /* enter? */
    breq    gotline
    st      y+, r24
    rjmp    input_next
gotline:
    ldi     r24, 0x0a
    rcall   ser_tx                  /* CR was already echoed, insert LF too */
    ldi     r24, 0                  /* terminate the string */
    st      y, r24
    ret

/* print a byte's value in ASCII, as hex */
ser_hexnum:
    push    r16
    mov     r16, r24
    swap    r24
    andi    r24, 0x0f
    adiw    r24, 0x30   /* might affect R25 but who cares */
    cpi     r24, 0x3a
    brlo    belowa
    adiw    r24, 0x07
belowa:
    rcall   ser_tx
    mov     r24, r16
    andi    r24, 0x0f
    adiw    r24, 0x30
    cpi     r24, 0x3a
    brlo    belowa2
    adiw    r24, 0x07
belowa2:
    rcall   ser_tx
    pop     r16
    ret
    
/* print a byte's value in ASCII, as decimal */
/* r17 high byte, r16 low byte */
ser_decnum:
    push    r16
    push    r17
    ldi     r17, 0x00
    mov     r16, r24
decnum_loop:
    cpi     r16, 0x0a
    brlt    decnum_end
    inc     r17
    subi    r16, 0x0a
    rjmp    decnum_loop
decnum_end:
    mov     r24, r17
    adiw    r24, 0x30
    rcall   ser_tx
    mov     r24, r16
    adiw    r24, 0x30
    rcall   ser_tx
    pop     r17
    pop     r16
    ret

/* print a string */
ser_print:
    push    r28
    push    r29
    push    r22         /* save 2nd param, in case ser_tx clobbers it */
    movw    r28, r24    /* Y = string pointer */
ser_print_next:
    ld      r24, Y+
    tst     r24
    breq    ser_print_lineend
    rcall   ser_tx
    rjmp    ser_print_next
ser_print_lineend:
    pop     r22
    cpi     r22, 1
    brne    ser_print_done
    ldi     r24, 10
    rcall   ser_tx
    ldi     r24, 13
    rcall   ser_tx
ser_print_done:
    pop     r29
    pop     r28
    ret

/* receive a byte */
ser_rx:
    lds     r18, UCSR0A
    sbrs    r18, RXC0
    rjmp    ser_rx
    lds     r24, UDR0
    ret

/* transmit a byte */
ser_tx:
    lds     r18, UCSR0A
    sbrs    r18, UDRE0
    rjmp    ser_tx
    sts     UDR0, r24
    ret

/* init */
ser_init:
    ldi     r18, lo8(BAUDVAL)
    sts     UBRR0L, r18
    /* not necessary to write UBRR0H because it's 0 on init, and my baud
       value will be below 256 for rates faster than 2400 */
    ldi     r18, (1<<RXEN0) | (1<<TXEN0)
    sts     UCSR0B, r18
    ret

