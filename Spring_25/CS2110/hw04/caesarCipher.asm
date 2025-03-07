;;=============================================================
;; CS 2110 - Spring 2025
;; Homework 4 - Caesar Cipher
;;=============================================================
;; Name: Nicholas Stone
;;=============================================================

;;  In this file, you must implement the 'MOD' and 'ENCRYPT' subroutines.
    
.orig x3000
    ;; You do not need to write anything here
    LD R6, STACK_PTR
    
    ;; Pushes arguments (string to encrypt, shift)
    ADD R6, R6, -1
    LD R0, SHIFT
    STR R0, R6, 0
    
    ADD R6, R6, -1
    LD R0, STRING
    STR R0, R6, 0
    
    JSR ENCRYPT
    
    LDR R0, R6, 0
    ADD R6, R6, 3

    HALT
    
    ;; Do not rename or remove any existing labels
    ;; You may change the value of STRING, LENGTH, SHIFT for debugging
    STACK_PTR .fill xF000
    STRING .fill x4000
    SHIFT .fill 7
    ASCIIUPPERA .fill 65
    ASCIILOWERA .fill 97
    ALPHABETLEN .fill 26



MOD ;; Do not change this label! Treat this as like the name of the function in a function header
    ;; Code your implementation for the MOD subroutine here!

    ;;  MOD Pseudocode (see PDF for explanation and examples)   
;;  
;;  MOD(int a, int b) {
;;      while (a >= b) {
;;          a -= b;
;;      }
;;      return a;
;;  }

ADD R6, R6, -1
ST R0, MOD_SAVE_R0      ; dividend, a
ST R1, MOD_SAVE_R1      ; divisor, b
ST R2, MOD_SAVE_R2      ; temp, a-b
ST R7, MOD_SAVE_R7      ; this is the return address
LDR R0, R6, 1    ; R0 = a
LDR R1, R6, 2    ; R1 = b
BRz MOD_DONE     ; If zero, return a
MOD_LOOP  ; while loop for mod subroutine
    NOT R2, R1
    ADD R2, R2, 1    ; 2's complement
    ADD R2, R0, R2
    BRn MOD_DONE     ; If sum is negative, exit
    ADD R0, R2, 0    ; R0 = a - b
    BR MOD_LOOP
MOD_DONE
    STR R0, R6, 0    ; result stored on stack
    LD R0, MOD_SAVE_R0
    LD R1, MOD_SAVE_R1
    LD R2, MOD_SAVE_R2
    LD R7, MOD_SAVE_R7
    RET
    
MOD_SAVE_R0 .blkw 1
MOD_SAVE_R1 .blkw 1
MOD_SAVE_R2 .blkw 1
MOD_SAVE_R7 .blkw 1



ENCRYPT
    ADD R6, R6, -1      ; return value space
    ST R0, ENCRYPT_SAVE_R0
    ST R1, ENCRYPT_SAVE_R1
    ST R2, ENCRYPT_SAVE_R2
    ST R3, ENCRYPT_SAVE_R3
    ST R4, ENCRYPT_SAVE_R4
    ST R5, ENCRYPT_SAVE_R5
    ST R7, ENCRYPT_SAVE_R7
    LDR R0, R6, 1
    LDR R1, R6, 2
    ADD R2, R0, 0        ; R2 = str
ENCRYPT_LOOP
    LDR R3, R2, 0        ; R3 = str[i]
    BRz ENCRYPT_DONE     ; if 0, move to ENCRYPT_DONE
    LD R4, ASCIILOWERA   ; R4 = ascii a
    NOT R4, R4
    ADD R4, R4, 1        ; 2's complement
    ADD R4, R3, R4       ; char - a
    BRn NOT_LOWERCASE    ; If negative, go to NOT_LOWERCASE
    LD R5, ALPHABETLEN
    NOT R5, R5
    ADD R5, R5, 1        ; 2's complement
    ADD R5, R4, R5       ; (char - a) - 26
    BRp NOT_LOWERCASE    ; If positive, go to NOT_LOWERCASE
    ADD R4, R4, R1       ; (char - a) + k
    ADD R6, R6, -1       ; decrement
    LD R5, ALPHABETLEN   ; R5 = 26
    STR R5, R6, 0        ; Push 26 as second argument
    ADD R6, R6, -1       ; decrement
    STR R4, R6, 0        ; (char - a) + k
    JSR MOD
    LDR R4, R6, 0
    ADD R6, R6, 2
    LD R5, ASCIILOWERA   ; R5 = ascii a
    ADD R3, R4, R5
    STR R3, R2, 0
    BR NEXT_CHAR
NOT_LOWERCASE
    LD R4, ASCIIUPPERA   ; R4 = ascii A
    NOT R4, R4
    ADD R4, R4, 1        ; 2's complement
    ADD R4, R3, R4       ; R4 = char - 'A'
    BRn NEXT_CHAR        ; If negative, break
    LD R5, ALPHABETLEN   ; R5 = 26
    NOT R5, R5
    ADD R5, R5, 1        ; 2's complement
    ADD R5, R4, R5       ; R5 = (char - A) - 26
    BRp NEXT_CHAR        ; If positive, NEXT_CHAR
    ADD R4, R4, R1
    ADD R6, R6, -1
    LD R5, ALPHABETLEN   ; R5 = 26
    STR R5, R6, 0
    ADD R6, R6, -1
    STR R4, R6, 0        ; Push (char - A) + k
    JSR MOD
    LDR R4, R6, 0        ; R4 = MOD((char - A) + k, 26)
    ADD R6, R6, 2
    LD R5, ASCIIUPPERA   ; R5 = ascii A
    ADD R3, R4, R5       ; R3 = MOD((char - A) + k, 26) + A
    STR R3, R2, 0
NEXT_CHAR
    ADD R2, R2, 1        ; increment
    BR ENCRYPT_LOOP
ENCRYPT_DONE
    STR R0, R6, 0
    LD R0, ENCRYPT_SAVE_R0
    LD R1, ENCRYPT_SAVE_R1
    LD R2, ENCRYPT_SAVE_R2
    LD R3, ENCRYPT_SAVE_R3
    LD R4, ENCRYPT_SAVE_R4
    LD R5, ENCRYPT_SAVE_R5
    LD R7, ENCRYPT_SAVE_R7
    RET
    
ENCRYPT_SAVE_R0 .blkw 1
ENCRYPT_SAVE_R1 .blkw 1
ENCRYPT_SAVE_R2 .blkw 1
ENCRYPT_SAVE_R3 .blkw 1
ENCRYPT_SAVE_R4 .blkw 1
ENCRYPT_SAVE_R5 .blkw 1
ENCRYPT_SAVE_R7 .blkw 1

.end

;; You may change the value of the string for debugging
.orig x4000
    .stringz "hello"
.end