;;=============================================================
;; CS 2110 - Spring 2025
;; Homework 4 - Integer Square Root
;;=============================================================
;; Name: 
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
    ;; YOUR CODE HERE
    HALT

;; Do not rename or remove any existing labels
;; You may change the value of N for debugging
N .fill 20
RESULT .fill x4000
.end

.orig x4000
    .blkw 1
.end
