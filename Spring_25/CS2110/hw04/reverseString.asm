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
    HALT

;; Do not rename or remove any existing labels
;; You may change the value of LENGTH for debugging
STRING .fill x4000
.end

;; You may change the value of the string for debugging
.orig x4000
    .stringz "hello"
.end
