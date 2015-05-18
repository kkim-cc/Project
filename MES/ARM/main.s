/*
 * SET/MES A5
 *
 * This program implements an interactive calculator with a few simple math functions.
 */

/* -------------------------------------
 * Init code
 * ------------------------------------- */
init:   ldr     sp, =0x20400
        bl      ser_init
        b       main


/* -------------------------------------
 * Main program
 * ------------------------------------- */
main:   adr     r0, str_start
        bl      ser_print       /* print str_start text */
prompt: mov     r0, #'>'
        bl      ser_tx          /* print '>' */
        adr     r0, input
        bl      ser_readline    /* get user input (command name) */
        bl      newline
        adr     r4, cmd_table   /* match the input with registered commands */
nextcmd:adr     r0, input
        ldr     r1, [r4], #12 
        cmp     r1, #0
        beq     prompt          /* ignore unrecognized commands */
        bl      strcmp
        cmp     r0, #0
        bne     nextcmd
        /* found a match, call handler function */
        ldr     r0, [r4, #-4]   /* param table */
        ldr     r2, [r4, #-8]   /* fn pointer */
        adr     lr, done
        mov     pc, r2
done:   ldr     r0, [r4, #-4]
        ldr     r0, [r0, #12]
        bl      ser_hexnum      /* print result */
        bl      newline
        b       prompt


/* ------------------------------------
 * String comparison
 * Input: R0 - first string ptr.
 *        R1 - second string ptr.
 * local: r2 - length of first string
 * Output: R0 - 0 if equal, 1 otherwise
 * ------------------------------------ */
strcmp: /* TODO */
        /* prolog: save registers */
        stmfd   sp!, {r2,r5,r6,lr}

        /* get length && save to r2*/
        mov     r6, r0 
        mov     r2, #0
s_len:  add     r2, #1
        ldrb    r5, [r0], #1
        cmp     r5, #0
        bne     s_len
        mov     r0, r6          /* restore original value */
        
s_next: cmp     r2, #0          /* terminating condition for the loop */
        beq     s_done
        ldrb    r5, [r0], #1    /* load the chars */
        ldrb    r6, [r1], #1
        sub     r2, #1
        subs    r5, r5, r6      /* take a difference between the chars */
        beq     s_next
s_done: mov     r0, r5

        /* epilog: restore registers */
        ldmfd   sp!, {r2,r5,r6,lr}
        mov     pc, lr

/* ------------------------------------
 * Helper: print a newline
 * r0 - used local variable
 * ------------------------------------ */
newline:/* TODO */
        stmfd   sp!, {r1,r2,lr} /* push r1,r2,lr to stack */
        mov     r0, #13
        bl      ser_tx
        mov     r0, #10
        bl      ser_tx
        ldmfd   sp!, {r1,r2,lr} /* pop r1,r2,lr from stack */
        mov     pc, lr


/* ------------------------------------
 * Absolute value
 * Input: R0 - descriptor with params
 * Output: R0 - output base address 
 * ------------------------------------ */
abs:    /* TODO */
        stmfd   sp!, {r1,r4,r5,lr} /* push r1,r4,r5,lr to stack */
        
        ldr     r1, [r0, #4]    /* load r1 from the address in r0 + 4 */
        cmp     r1, #0          /* compare r1, 0 */
        bgt     abs_skip        /* if greater then 0 go to skip */
        rsb     r1, r1, #0      /* reverse subtract */
abs_skip:   
        str     r1, [r0, #12]   /* store r1 to the address in r0 + 12 */
        ldmfd   sp!, {r1,r4,r5,lr} /* pop r1,r4,r5,lr from stack */
        mov     pc, lr          /* return */


/* ------------------------------------
 * Sum of a series
 * Input: R0 - descriptor with params
 * Output: R0 - output base address
 * ------------------------------------ */
sum:    /* TODO */
        stmfd   sp!, {r1-r5,lr} /* push r1-r5,lr to stack */
        
        ldr     r2, [r0, #4]    /* load r2 from the address in r0 + 4 */
        mov     r1, #0          /* initialize r1 register */
sum_next:
        add     r1, r1, r2      /* r1 = r1 + r2 */
        subs    r2, r2, #1      /* r2 = r2 - 1 */
        cmp     r2, #0
        bne     sum_next        /* if r2 != 0 then go to next */
        str     r1, [r0, #12]   /* store r1 to the address in r0 + 12 */
      
        ldmfd   sp!, {r1-r5,lr} /* pop r1-r5,lr from stack */
        mov     pc, lr          /* return */


/* ------------------------------------
 * Greatest Common Divisor
 * Input: R0 - descriptor with params
 * Output: r0 - output base address 
 * r1 - used local variable
 * r2 - used local variable
 * ------------------------------------ */
gcd:    stmfd   sp!, {r1-r5,lr} /* push r1-r5,lr to stack */
        
        ldr     r1, [r0, #4]    /* load r1 from the address in r0 + 4 */
        ldr     r2, [r0, #8]    /* load r2 from the address in r0 + 8 */
loop_gcd:
        cmp     r1, r2          /* compare r1 and r2 */
        subgt   r1, r1, r2      /* if r1 > r2 then r1 = r1 - r2 */
        sublt   r2, r2, r1      /* if r1 < r2 then r2 = r2 - r1 */
        bne     loop_gcd        /* go to loop_gcd if r2 not equal r1 */
        str     r1, [r0, #12]   /* store r1 to the address in r0 + 12 */
        
        ldmfd   sp!, {r1-r5,lr} /* pop r1-r5,lr from stack */
        mov     pc, lr          /* return */


/* ------------------------------------
 * Factorial
 * Input: R0 - descriptor with params
 * Output: r0 - output base address 
 * r1 - used local variable
 * r2 - used local variable
 * ------------------------------------ */
fact:   /* TODO */
        stmfd   sp!, {r1-r5,lr} /* push r1-r5,lr to stack */
        
        ldr     r1, [r0, #4]    /* load r1 from the address in r0 + 4 */
        mov     r2, r1          /* r2 = r1*/
loop_fact:
        sub     r1, r1, #1      /* r1 = r1 - 1 */
        mul     r3, r2, r1      /* r3 = r2 * r1 */
        mov     r2, r3          /* r2 = r3 */
        cmp     r1, #1          /* compare r1, 1*/
        bgt     loop_fact       /* go to loop_fact if r1 > 1 */
        str     r2, [r0, #12]   /* store r2 to the address in r0 + 12 */
        
        ldmfd   sp!, {r1-r5,lr} /* pop r1-r5,lr from stack */
        mov     pc, lr          /* return */


/* -------------------------------------
 * Data
 * ------------------------------------- */
input:  .space 32,0

/* 
 Data structure describing calculator commands:
   struct calc {
     uint8_t cmdid;
     uint32_t p[2];
     uint32_t result;
   }
 Below is an array of these structures, with some sample value for input params.
*/
calcdata:
p_abs:  .byte 0, 0, 0, 0    /* cmd ID (not used currently) and padding */
        .word -5, 0, 0      /* input params and result */
p_sum:  .byte 0, 0, 0, 0
        .word 15, 0, 0
p_gcd:  .byte 0, 0, 0, 0
        .word 18, 48, 0
p_fact: .byte 0, 0, 0, 0
        .word 7, 0, 0

str_abs:    .asciz "abs"
str_sum:    .asciz "sum"
str_gcd:    .asciz "gcd"
str_fact:   .asciz "fact"
            .align 4

/*
 Data structure mapping command names to handler function and parameters:
   struct cmd_table {
     char *cmdname;
     void *fn;
     struct calc *params;
   }
 Below is an array of these structures...
*/
cmd_table:
        .word str_abs,  abs,  p_abs
        .word str_sum,  sum,  p_sum
        .word str_gcd,  gcd,  p_gcd
        .word str_fact, fact, p_fact
        .word 0     /* sentinel */

str_start:
        .asciz "Starting calculator in interactive mode...\n"
        .align 4

