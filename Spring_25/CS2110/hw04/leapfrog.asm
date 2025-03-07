;;=============================================================
;; CS 2110 - Spring 2025
;; Homework 4 - Leapfrog
;;=============================================================
;; Name: Nicholas Stone
;;=============================================================

.orig x3000
;; Suggested Pseudocode (see PDF for explanation)
;;
;; inc = x0100
;; func_addr = starting address of function
;; copy_addr = func_addr + inc
;; start_copy = copy_addr
;; stop_addr = xFE00  
;; if (start_copy - stop_addr == 0):
;;      HALT
;; curr = mem[func_addr]
;; while (curr != 0) {
;;      curr = mem[func_addr]
;;      mem[copy_addr] = curr
;;      func_addr++
;;      copy_addr++
;; }
;; mem[copy_addr] = 0
;; PC = start_copy
;; .fill 0

;; YOUR CODE HERE
FUNC_ADDR
    LD R0, INC          ; INC into R0
    LEA R1, FUNC_ADDR   ; FUNC_ADDR into R1
    ADD R2, R1, R0      ; FUNC_ADDR + inc
    ADD R6, R2, #0      ; START_COPY
    LD R3, STOP_ADDR    ; STOP_ADDR into R3
    NOT R3, R3          ; beginning of if statement       
    ADD R3, R3, #1      ; 2's complement
    ADD R4, R2, R3      ; R2 - STOP_ADDR
    BRz HALT_PROGRAM    ; If sum is zero, exit
COPYLOOP
    LDR R5, R1, #0      ; Load FUNC_ADDR into R5
    BRz ENDCOPY        ; If 0, ENDCOPY (part of while loop)
    STR R5, R2, #0
    ADD R1, R1, #1      ; Increment FUNC_ADDR
    ADD R2, R2, #1      ; Increment copied adder
    ADD R4, R2, R3      ; copied adder - STOP_ADDR
    BRz HALT_PROGRAM    ; If sum is zero, exit
    BR COPYLOOP
ENDCOPY
    AND R5, R5, #0      ; 0 to R5
    STR R5, R2, #0      ; Store 0 at copied adder
    JMP R6              ; Jump to copied adder
HALT_PROGRAM
    HALT

INC        .FILL x0100
STOP_ADDR  .FILL xFE00
START_COPY .BLKW 1
.fill 0

.end