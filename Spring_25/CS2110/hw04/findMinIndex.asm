;;=============================================================
;; CS 2110 - Spring 2025
;; Homework 4 - Find Min Index
;;=============================================================
;; Name: Nicholas Stone
;;=============================================================

.orig x3000
;; Suggested Pseudocode (see PDF for explanation)
;;
;;  int minIndex = 0;
;;  int minValue = ARRAY[0];
;;  for (int i = 1; i < LENGTH; i++) {
;;      if (ARRAY[i] < minValue) {
;;          minValue = ARRAY[i];
;;          minIndex = i;
;;      }
;;  }
;;  mem[mem[RESULT]] = minIndex;
    
;; YOUR CODE HERE
AND R0, R0, #0
LD R1, ARRAY
LDR R2, R1, #0
AND R3, R3, #0
ADD R3, R3, #1
FORLOOP
    LD R4, LENGTH       ; R4 = LENGTH
    NOT R4, R4
    ADD R4, R4, #1      ; 2's complement
    ADD R4, R3, R4      ; R4 = i - LENGTH
    BRzp END_LOOP       ; If sum is positive or zero, exit
    ADD R5, R1, R3      ; R5 = R1 + R3
    LDR R5, R5, #0
    NOT R6, R2
    ADD R6, R6, #1      ; 2's complement
    ADD R6, R5, R6      ; R6 = ARRAY[i] - minValue
    BRzp SKIP           ; If sum is positive or zero, skip update
    ADD R2, R5, #0
    ADD R0, R3, #0      ; minIndex = i
SKIP
    ADD R3, R3, #1      ; increment
    BR FORLOOP
END_LOOP
    LD R7, RESULT
    STR R0, R7, #0
    HALT

;; Do not rename or remove any existing labels
;; You may change the value of LENGTH for debugging
RESULT .fill x4000
ARRAY .fill x5000
LENGTH .fill 5
.end

.orig x4000
    ANSWER .blkw 1
.end

;; You may change these values for debuggin
.orig x5000
    .fill -1
    .fill 2 
    .fill 7 
    .fill 3 
    .fill -8 
.end
