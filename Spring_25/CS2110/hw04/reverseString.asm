;;=============================================================
;; CS 2110 - Spring 2025
;; Homework 4 - Reverse String 
;;=============================================================
;; Name: Nicholas Stone
;;=============================================================

.orig x3000
;; Suggested Pseudocode (see PDF for explanation)
;;
;; int length = 0;
;; while (STRING[length] != 0) {
;;      length++;
;; }
;;
;;
;; int start = 0;
;; int end = length - 1;

;; while (start < end) {
;;      char temp = STRING[start];
;;      STRING[start] = STRING[end];
;;      STRING[end] = temp;

;;      start++;
;;      end--;
;;}

;; YOUR CODE HERE
AND R0, R0, #0          ; R0 = 0, length = 0
LD R1, STRING           ; Load STRING to R1

LOOP         ; iterates until loop reaches /0
    ADD R2, R1, R0      ; R2 = address of STRING
    LDR R3, R2, #0      ; R3 = STRING
    BRz WHILE_DONE      ; If /0, exit to WHILE_DONE
    ADD R0, R0, #1      ; length adding 1
    BR LOOP
WHILE_DONE
    AND R2, R2, #0      ; start index goes back to 0
    ADD R3, R0, #-1     ; end becomes length (R0) - 1
SWAP_LOOP
    NOT R4, R3
    ADD R4, R4, #1      ; 2's complements R4
    ADD R4, R2, R4      ; R4 = start - end
    BRzp SWAP_DONE      ; If positive or zero, exit to swap_done
    ADD R5, R1, R2      ; R5 = address of STRING[start]
    LDR R6, R5, #0      ; R6 = STRING[start]
    ADD R7, R1, R3      ; R7 = address of STRING[end]
    LDR R4, R7, #0      ; R4 = STRING[end]
    STR R4, R5, #0
    STR R6, R7, #0
    ADD R2, R2, #1      ; R2 + 1
    ADD R3, R3, #-1     ; R3 - 1
    BR SWAP_LOOP
SWAP_DONE
    HALT

;; Do not rename or remove any existing labels
;; You may change the value of LENGTH for debugging
STRING .fill x4000
.end

;; You may change the value of the string for debugging
.orig x4000              ; head to x4000 for result
    .stringz "hello"
.end
