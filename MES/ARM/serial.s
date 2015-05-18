/*
 * UART driver for ARM IntegratorCP
 */


/* UART registers */
.equ UARTBASE,  0x16000000
.equ DR_OFFSET, 0x00
.equ FR_OFFSET, 0x18
.equ BIT_BUSY,  3
.equ BIT_RXFE,  4


/* Export these functions */
.global ser_init
.global ser_tx
.global ser_rx
.global ser_print
.global ser_readline
.global ser_hexnum


/* ------------------------------------
 * Initialize the UART
 * ------------------------------------ */
ser_init:
        /* on reset, FIFOs are disabled but TX and RX is enabled;
           so I don't have to change anything to get going; baud
           rate doesn't matter for the emulator - it all goes to
           stdio
        */
        mov     pc, lr


/* ------------------------------------
 * Send out a single byte
 * ------------------------------------ */
ser_tx: ldr     r1, =UARTBASE
txwait: ldr     r2, [r1, #FR_OFFSET]
        tst     r2, #(1<<BIT_BUSY)
        bne     txwait
        strb    r0, [r1, #DR_OFFSET]
        mov     pc, lr


/* ------------------------------------
 * Receive a byte (blocking)
 * ------------------------------------ */
ser_rx: ldr     r1, =UARTBASE
rxwait: ldr     r2, [r1, #FR_OFFSET]
        tst     r2, #(1<<BIT_RXFE)
        bne     rxwait
        ldrb    r0, [r1]
        mov     pc, lr


/* ------------------------------------
 * Print a string
 * ------------------------------------ */
ser_print:
        str     r4, [sp, #-4]!
        str     lr, [sp, #-4]!
        mov     r4, r0
print_next:
        ldrb    r0, [r4], #1
        cmp     r0, #0
        beq     print_done
        bl      ser_tx
        b       print_next
print_done:
        ldr     lr, [sp], #4
        ldr     r4, [sp], #4
        mov     pc, lr


/* ------------------------------------
 * Read a line of text (until <enter>)
 * Input: R0 - address of buffer for storing text
 * ------------------------------------ */
ser_readline:
        str     r4, [sp, #-4]!
        str     lr, [sp, #-4]!
        mov     r4, r0
input_next:
        bl      ser_rx
        cmp     r0, #0xD        /* enter => '\r' */
        beq     gotline
        strb    r0, [r4], #1
        bl      ser_tx
        b       input_next
gotline:mov     r0, #0          /* insert null at the end*/
        strb    r0, [r4]
        ldr     lr, [sp], #4
        ldr     r4, [sp], #4
        mov     pc, lr


/* ------------------------------------
 * Print a value in hex
 * Input: R0 - value to be printed in hex using ASCII
 * ------------------------------------ */
ser_hexnum:
        str     r4, [sp, #-4]!
        str     r5, [sp, #-4]!
        str     lr, [sp, #-4]!
        mov     r5, #28         /* initially shift out this many bits to get MSByte */
        mov     r4, r0          /* print header "0x" */
        mov     r0, #'0'
        bl      ser_tx
        mov     r0, #'x'
        bl      ser_tx
again:  mov     r0, r4, LSR r5  /* shift out unwanted digits */
        and     r0, r0, #0xf    /* mask out to leave a single digit */
        add     r0, r0, #0x30   /* convert to ASCII 0-9 */
        cmp     r0, #0x3a
        blo     belowa
        add     r0, #0x7        /* handle range A-F */
belowa: bl      ser_tx          /* print this converted digit */
        subs    r5, r5, #4      /* next time shift out one less digit */
        bpl     again
        ldr     lr, [sp], #4
        ldr     r5, [sp], #4
        ldr     r4, [sp], #4
        mov     pc, lr
