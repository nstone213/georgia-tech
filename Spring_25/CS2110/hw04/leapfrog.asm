;;=============================================================
;; CS 2110 - Spring 2025
;; Homework 4 - Leapfrog
;;=============================================================
;; Name: Nicholas Stone
;;=============================================================

.ORIG x3000
FUNC_ADDR
    LD R0, INC          ; INC into R0
    LEA R1, FUNC_ADDR   ; FUNC_ADDR into R1
    ADD R2, R1, R0      ; FUNC_ADDR + inc
    ADD R6, R2, #0      ; Load address of START_COPY
    LD R3, STOP_ADDR    ; STOP_ADDR into R3
    NOT R3, R3          ; -(STOP_ADDR)
    ADD R3, R3, #1
    ADD R4, R2, R3      ; R2 - STOP_ADDR
    BRz HALT_PROGRAM    ; If copied adder >= STOP_ADDR, halt

COPY_LOOP
    LDR R5, R1, #0      ; Load FUNC_ADDR into R5
    BRz END_COPY        ; If 0, END_COPY
    STR R5, R2, #0      ; Store current instruction at copied adder
    ADD R1, R1, #1      ; Increment FUNC_ADDR
    ADD R2, R2, #1      ; Increment copied adder
    ADD R4, R2, R3      ; Recompute copied adder - STOP_ADDR
    BRz HALT_PROGRAM    ; If copied adder >= STOP_ADDR, halt
    BR COPY_LOOP        ; Repeat loop

END_COPY
    AND R5, R5, #0      ; 0 to R5
    STR R5, R2, #0      ; Store 0 at copied adder
    JMP R6              ; Jump to copied adder

HALT_PROGRAM
    HALT                ; Halt

; Data values
INC        .FILL x0100
STOP_ADDR  .FILL xFE00
START_COPY .BLKW 1
.FILL 0

.end