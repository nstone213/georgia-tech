;;=============================================================
;; CS 2110 - Spring 2025
;; Homework 4 - Caesar Cipher
;;=============================================================
;; Name: 
;;=============================================================

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
    ;; Allocate space for return value
    ADD R6, R6, -1
    
    ;; Save registers
    ST R0, MOD_SAVE_R0
    ST R1, MOD_SAVE_R1
    ST R2, MOD_SAVE_R2
    ST R7, MOD_SAVE_R7
    
    ;; Get parameters from stack
    LDR R0, R6, 1    ;; R0 = a (first argument)
    LDR R1, R6, 2    ;; R1 = b (second argument)
    
    ;; Check if b is zero
    BRz MOD_DONE     ;; If b is zero, return a
    
MOD_LOOP
    ;; Check if a < b
    NOT R2, R1       ;; R2 = ~b
    ADD R2, R2, 1    ;; R2 = -b
    ADD R2, R0, R2   ;; R2 = a - b
    BRn MOD_DONE     ;; If a - b < 0, exit loop
    
    ;; a = a - b
    ADD R0, R2, 0    ;; R0 = a - b
    
    BR MOD_LOOP      ;; Repeat the loop
    
MOD_DONE
    ;; Return a
    STR R0, R6, 0    ;; Store result on stack (return value)
    
    ;; Restore registers
    LD R0, MOD_SAVE_R0
    LD R1, MOD_SAVE_R1
    LD R2, MOD_SAVE_R2
    LD R7, MOD_SAVE_R7
    RET
    
;; Save locations for registers
MOD_SAVE_R0 .blkw 1
MOD_SAVE_R1 .blkw 1
MOD_SAVE_R2 .blkw 1
MOD_SAVE_R7 .blkw 1



ENCRYPT
    ;; Save registers
    ADD R6, R6, -1      ;; Make space for return value
    
    ST R0, ENCRYPT_SAVE_R0
    ST R1, ENCRYPT_SAVE_R1
    ST R2, ENCRYPT_SAVE_R2
    ST R3, ENCRYPT_SAVE_R3
    ST R4, ENCRYPT_SAVE_R4
    ST R5, ENCRYPT_SAVE_R5
    ST R7, ENCRYPT_SAVE_R7
    
    ;; Get parameters from stack
    LDR R0, R6, 1    ;; R0 = str (pointer to string)
    LDR R1, R6, 2    ;; R1 = k (shift amount)
    
    ;; Initialize R2 as our working pointer
    ADD R2, R0, 0    ;; R2 = str (pointer to current character)
    
ENCRYPT_LOOP
    ;; Load the current character
    LDR R3, R2, 0    ;; R3 = str[i]
    
    ;; Check if we've reached the end of the string (null terminator)
    BRz ENCRYPT_DONE
    
    ;; Check if character is lowercase ('a' to 'z')
    LD R4, ASCIILOWERA   ;; R4 = 'a' (ASCII 97)
    NOT R4, R4           ;; R4 = ~'a'
    ADD R4, R4, 1        ;; R4 = -'a'
    ADD R4, R3, R4       ;; R4 = char - 'a'
    
    BRn NOT_LOWERCASE    ;; If char < 'a', not lowercase
    
    ;; Check if char <= 'z' (char - 'a' < 26)
    LD R5, ALPHABETLEN   ;; R5 = 26
    NOT R5, R5           ;; R5 = ~26
    ADD R5, R5, 1        ;; R5 = -26
    ADD R5, R4, R5       ;; R5 = (char - 'a') - 26
    BRp NOT_LOWERCASE    ;; If (char - 'a') > 26, not lowercase
    
    ;; Process lowercase letter
    ;; R4 already contains (char - 'a')
    
    ;; Add shift amount
    ADD R4, R4, R1       ;; R4 = (char - 'a') + k
    
    ;; Push arguments for MOD function (R4 and 26)
    ADD R6, R6, -1
    LD R5, ALPHABETLEN   ;; R5 = 26
    STR R5, R6, 0        ;; Push 26 as second argument
    
    ADD R6, R6, -1
    STR R4, R6, 0        ;; Push (char - 'a') + k as first argument
    
    ;; Call MOD function
    JSR MOD
    
    ;; Get result and clean stack
    LDR R4, R6, 0        ;; R4 = MOD((char - 'a') + k, 26)
    ADD R6, R6, 2        ;; Clean up stack
    
    ;; Convert back to ASCII
    LD R5, ASCIILOWERA   ;; R5 = 'a'
    ADD R3, R4, R5       ;; R3 = MOD((char - 'a') + k, 26) + 'a'
    
    ;; Store the encrypted character back
    STR R3, R2, 0
    
    BR NEXT_CHAR
    
NOT_LOWERCASE
    ;; Check if character is uppercase ('A' to 'Z')
    LD R4, ASCIIUPPERA   ;; R4 = 'A' (ASCII 65)
    NOT R4, R4           ;; R4 = ~'A'
    ADD R4, R4, 1        ;; R4 = -'A'
    ADD R4, R3, R4       ;; R4 = char - 'A'
    
    BRn NEXT_CHAR        ;; If char < 'A', not a letter
    
    ;; Check if char <= 'Z' (char - 'A' < 26)
    LD R5, ALPHABETLEN   ;; R5 = 26
    NOT R5, R5           ;; R5 = ~26
    ADD R5, R5, 1        ;; R5 = -26
    ADD R5, R4, R5       ;; R5 = (char - 'A') - 26
    BRp NEXT_CHAR        ;; If (char - 'A') > 26, not uppercase
    
    ;; Process uppercase letter
    ;; R4 already contains (char - 'A')
    
    ;; Add shift amount
    ADD R4, R4, R1       ;; R4 = (char - 'A') + k
    
    ;; Push arguments for MOD function (R4 and 26)
    ADD R6, R6, -1
    LD R5, ALPHABETLEN   ;; R5 = 26
    STR R5, R6, 0        ;; Push 26 as second argument
    
    ADD R6, R6, -1
    STR R4, R6, 0        ;; Push (char - 'A') + k as first argument
    
    ;; Call MOD function
    JSR MOD
    
    ;; Get result and clean stack
    LDR R4, R6, 0        ;; R4 = MOD((char - 'A') + k, 26)
    ADD R6, R6, 2        ;; Clean up stack
    
    ;; Convert back to ASCII
    LD R5, ASCIIUPPERA   ;; R5 = 'A'
    ADD R3, R4, R5       ;; R3 = MOD((char - 'A') + k, 26) + 'A'
    
    ;; Store the encrypted character back
    STR R3, R2, 0
    
NEXT_CHAR
    ;; Move to next character
    ADD R2, R2, 1        ;; R2 points to next character
    BR ENCRYPT_LOOP
    
ENCRYPT_DONE
    ;; Set return value (the original string pointer)
    STR R0, R6, 0
    
    ;; Restore registers
    LD R0, ENCRYPT_SAVE_R0
    LD R1, ENCRYPT_SAVE_R1
    LD R2, ENCRYPT_SAVE_R2
    LD R3, ENCRYPT_SAVE_R3
    LD R4, ENCRYPT_SAVE_R4
    LD R5, ENCRYPT_SAVE_R5
    LD R7, ENCRYPT_SAVE_R7
    RET
    
;; Save locations for registers
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