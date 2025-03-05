;;=============================================================
;; CS 2110 - Spring 2025
;; Homework 4 - Binary String to Int 
;;=============================================================
;; Name: Nicholas Stone
;;=============================================================

.orig x3000
;; Suggested Pseudocode (see PDF for explanation)
;;
;; int length = 0;
;;  while (BINARYSTRING[length] != 0) {
;;      length++;
;;  }
;;  int result = 0;
;;  for (int i = 0; i < length; i++) {
;;      result = result << 1;
;;      result += BINARYSTRING[i] - 48;
;;  }
;;  mem[mem[RESULTADDR]] = result;

;; YOUR CODE HERE
AND R0, R0, #0       ; R0 = 0
LD R1, BINARYSTRING  ; Load binarystring
LENGTH
    LDR R2, R1, #0
    BRZ FIN_LENGTH  ; If '\0', stop
    ADD R0, R0, #1   ; R0 + 1
    ADD R1, R1, #1   ; Increment
    BR LENGTH
FIN_LENGTH
    AND R3, R3, #0   ; R3 = 0
    LD R1, BINARYSTRING
    AND R4, R4, #0   ; i = 0
CONVERT_LOOP
    NOT R5, R4       ; -i
    ADD R5, R5, #1   ; 2's complement
    ADD R5, R5, R0   ; Check if i < length
    BRZ FIN_CONVERT ; If sum is 0, stop
    ADD R3, R3, R3   ; shift left, x2
    LDR R2, R1, #0
    LD R6, ASCIIDIG  ; ASCII '0' (48)
    NOT R6, R6       
    ADD R6, R6, #1   ; 2's complement
    ADD R2, R2, R6   ; ASCII to int 0 or 1
    ADD R3, R3, R2
    ADD R1, R1, #1   ; increment
    ADD R4, R4, #1   ; increment
    BR CONVERT_LOOP
FIN_CONVERT
    LD R5, RESULTADDR
    STR R3, R5, #0
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
    .stringz "11001"
.end
