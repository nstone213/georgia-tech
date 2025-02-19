;;=============================================================
;; CS 2110 - Spring 2025
;; Homework 4 - Find Min Index
;;=============================================================
;; Name: 
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
