;;=============================================================
;; CS 2110 - Spring 2025
;; Homework 4 - Merge Sort
;;=============================================================
;; Name: 
;;=============================================================

;;  In this file, you must implement the 'MERGESORT', 'MERGE', and 'DIVIDE' subroutines.
    
.orig x3000
    ;; You do not need to write anything here
    LD R6, STACK_PTR

    ;; Pushes arguments (starting address of array to sort,
    ;;                   starting address of buffer,
    ;;                   lower index to sort,
    ;;                   upper index to sort)
    ;;=============================================================

    LD R6, STACK_PTR
    LD R0, ARRAY
    LD R1, BUF
    AND R2, R2, 0   ; start = 0
    LD R3, LENGTH   ; end = length
    
    ADD R6, R6, -1  ; push args
    STR R3, R6, 0
    ADD R6, R6, -1
    STR R2, R6, 0
    ADD R6, R6, -1
    STR R1, R6, 0
    ADD R6, R6, -1
    STR R0, R6, 0
    
    JSR MERGESORT
    
    HALT
    
    ARRAY   .fill x4000
    BUF     .fill x5000
    LENGTH  .fill 4
    STACK_PTR .fill xF000


MERGESORT   ;; Do not change this label! Treat this as like the name of the function in a function header
            ;; Code your implementation for the MERGESORT subroutine here!

;; MERGESORT PSEUDOCODE: Pseudocode (see PDF for explanation and examples)   
;; MERGESORT (int[] arr, int[] buf, int start, int end) {
;;      if (start >= end) {
;;        return;
;;      }
;;      mid = DIVIDE(start + end, 2)
;;      MERGESORT(arr, buf, start, mid)
;;      MERGESORT(arr, buf, mid, end)
;;      MERGE(arr, buf, start, mid, end)
;;  }
    
    
    RET
    
;;  DIVIDE Pseudocode (see PDF for explanation and examples)   
;;
;;  DIVIDE(int a, int b)
;;  {
;;      if (b == 0)
;;      {
;;          return 0;
;;      }
;;    
;;      int quotient = 0;
;;      while (a >= b)
;;      {
;;          a -= b;
;;          quotient++;
;;      }
;;    
;;      return quotient;
;;  }

DIVIDE  ;; Do not change this label! Treat this this like the name of the function in a function header
        ;; Code your implementation for the DIVIDE subroutine here!
    
    RET


;;  MERGE Pseudocode (see PDF for explanation and examples)
;;  MERGE(int arr[], int buf[], int start, int mid, int end) {
;;      int i = start;
;;      int j = mid;
;;      int k = start;
;;      while (i < mid && j < end):
;;          if arr[i] <= arr[j] {
;;              buf[k] = arr[i];
;;              k++;
;;              i++;
;;          } else {
;;              buf[k] = arr[j];
;;              k++;
;;              j++;
;;          }
;;      }
;;      while (i < mid) {
;;          buf[k] = arr[i];
;;          k++;
;;          i++;
;;      }
;;      while (j < end) {
;;          buf[k] = arr[j];
;;          k++;
;;          j++;
;;      }
;;      for (i = start; i < end; i++) {
;;          arr[i] = buf[i];
;;      }
;;  }

MERGE   ;; Do not change this label! Treat this as like the name of the function in a function header
        ;; Code your implementation for the MERGE subroutine here!
    RET

.end
    
;; You may change the values in these arrays for debugging. Remember to change the value of LENGTH as well. 
.orig x4000
    .fill 5 
    .fill 2 
    .fill 3 
    .fill 1
.end

.orig x5000
    .fill 5 
    .fill 2 
    .fill 3 
    .fill 1
.end
