;;===============================================================
;; CS 2110 - Spring 2025
;; Homework 4 - Binary String to Int 
;;=============================================================
;; Name: Nicholas Stone
;;===========================================================

.orig x3000
;; Suggested Pseudocode (see PDF for explanation)
;;
;;  int length = 0;
;;  while (BINARYSTRING[length] != 0) {
;;      length++;
;;  }
;;  int result = 0;
;;  for (int i = 0; i < length; i++) {
;;      result = result << 1;
;;      result += BINARYSTRING[i] - 48;
;;  }
;;  mem[mem[RESULTADDR]] = result;


;; Step 1: Find the length of the binary string
    AND R0, R0, #0   ; length = 0
    LD R1, BINARYSTRING  ; Load the address of the binary string

FIND_LENGTH
    LDR R2, R1, #0   ; Load character from BINARYSTRING[length]
    BRZ DONE_LENGTH  ; If it's null ('\0'), stop
    ADD R0, R0, #1   ; length++
    ADD R1, R1, #1   ; Move to next character
    BR FIND_LENGTH   ; Repeat

DONE_LENGTH
    AND R3, R3, #0   ; result = 0
    LD R1, BINARYSTRING  ; Reload address of the binary string
    AND R4, R4, #0   ; i = 0

;; Step 2: Convert the binary string to an integer
CONVERT_LOOP
    NOT R5, R4       ; Compute -i
    ADD R5, R5, #1
    ADD R5, R5, R0   ; Check if i < length
    BRZ DONE_CONVERT ; If i == length, stop

    ADD R3, R3, R3   ; result = result << 1 (shift left)
    LDR R2, R1, #0   ; Load BINARYSTRING[i]
    LD R6, ASCIIDIG  ; Load ASCII '0' (48)
    NOT R6, R6       
    ADD R6, R6, #1   ; Compute -48
    ADD R2, R2, R6   ; Convert ASCII to integer (0 or 1)
    ADD R3, R3, R2   ; result += (BINARYSTRING[i] - '0')

    ADD R1, R1, #1   ; Move to next character
    ADD R4, R4, #1   ; i++
    BR CONVERT_LOOP  ; Repeat

DONE_CONVERT
    LD R5, RESULTADDR ; Load memory location to store result
    STR R3, R5, #0   ; Store result in memory

    HALT

;; Do not rename or remove any existing labels
;; You may change the value of LENGTH for debugging
BINARYSTRING .fill x5000
RESULTADDR .fill x4000
ASCIIDIG .fill 48
.end

.orig x4000
    .blkw 1
.end

;; You may change the value of the string for debugging
.orig x5000
    .stringz "11000"
.end
