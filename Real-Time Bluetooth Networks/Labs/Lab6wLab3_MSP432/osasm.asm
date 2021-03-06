;/*****************************************************************************/
; OSasm.asm: low-level OS commands, written in assembly                       */
; Runs on LM4F120/TM4C123/MSP432
; Lab 6 starter file
; March 2, 2016

        .thumb
        .text
        .align 2
        .global  RunPt            ; currently running thread
        .global  StartOS
        .global  SysTick_Handler
        .global  Scheduler

RunPtAddr .field RunPt,32
SysTick_Handler:  .asmfunc      ; 1) Saves R0-R3,R12,LR,PC,PSR
    CPSID   I                  ; 2) Prevent interrupt during switch
       ;YOU IMPLEMENT THIS (same as Lab 2)
    CPSIE   I                  ; 9) tasks run with interrupts enabled
    BX      LR                 ; 10) restore R0-R3,R12,LR,PC,PSR

       .endasmfunc
StartOS: .asmfunc
    LDR     R0, RunPtAddr      ; currently running thread
       ;YOU IMPLEMENT THIS (same as Lab 2)
    CPSIE   I                  ; Enable interrupts at processor level
    BX      LR                 ; start first thread

       .endasmfunc
      .end
