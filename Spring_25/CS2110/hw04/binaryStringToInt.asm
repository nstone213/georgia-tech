;;=============================================================
;; CS 2110 - Spring 2025
;; Homework 4 - Binary String to Int 
;;=============================================================
;; Name: 
;;=============================================================

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

    ;; YOUR CODE HERE
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
