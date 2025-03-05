;;=============================================================
;; CS 2110 - Spring 2025
;; Homework 4 - Integer Square Root
;;=============================================================
;; Name: Nicholas Stone
;;=============================================================


;; Suggested Pseudocode (see PDF for explanation)
;;
;;  int L = 0;
;;  int a = 1;
;;  int d = 3;
;;
;;  while (a <= N) {
;;      a = a + d;  // Compute the next perfect square
;;      d = d + 2;  // Increase the difference between squares
;;      L = L + 1;  // Increment L
;;  }
;;
;;  mem[mem[RESULT]] = L;
    
.orig x3000
    AND R0, R0, #0   ; L = 0
    ADD R1, R0, #1   ; a = 1
    ADD R2, R0, #3   ; d = 3
    LD R3, N         ; N
WHILELOOP
    NOT R4, R1       ; -a
    ADD R4, R4, #1
    ADD R4, R4, R3   ; Check while condition
    BRN FIN          ; If negative, leave
    ADD R1, R1, R2   ; a = a + d
    ADD R2, R2, #2   ; d = d + 2
    ADD R0, R0, #1   ; L = L + 1
    BR WHILELOOP     ; Go to top of loop
FIN
    LD R5, RESULT    ; L's mem address
    STR R0, R5, #0   ; Store L at RESULT
    HALT

;; Do not rename or remove any existing labels
;; You may change the value of N for debugging
N .fill 20
RESULT .fill x4000
.end

.orig x4000
    .blkw 1
.end
