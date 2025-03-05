;;=============================================================
;; CS 2110 - Spring 2025
;; Homework 4 - Merge Sort
;;=============================================================
;; Name: Nicholas Stone
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
;;      if (start >= end - 1) {
;;        return;
;;      }
;;      mid = DIVIDE(start + end, 2)
;;      MERGESORT(arr, buf, start, mid)
;;      MERGESORT(arr, buf, mid, end)
;;      MERGE(arr, buf, start, mid, end)
;;  }
ADD R6, R6, -4
STR R7, R6, 2
STR R5, R6, 1
ADD R5, R6, 0
ADD R6, R6, -5
STR R0, R5, -1
STR R1, R5, -2
STR R2, R5, -3
STR R3, R5, -4
STR R4, R5, -5
LDR R0, R5, 6
LDR R1, R5, 7
ADD R1, R1, -1
NOT R1, R1
ADD R1, R1, 1
ADD R2, R0, R1
BRzp RETURNSORT
LDR R1, R5, 7
ADD R3, R0, R1
AND R4, R4, 0
ADD R4, R4, 2
ADD R6, R6, -1
STR R4, R6, 0
ADD R6, R6, -1
STR R3, R6, 0
JSR DIVIDE
LDR R2, R6, 0
ADD R6, R6, 3
LDR R3, R5, 4
LDR R4, R5, 5
ADD R6, R6, -1
STR R2, R6, 0
ADD R6, R6, -1
STR R0, R6, 0
ADD R6, R6, -1
STR R4, R6, 0
ADD R6, R6, -1
STR R3, R6, 0
JSR MERGESORT
ADD R6, R6, 5
ADD R6, R6, -1
STR R1, R6, 0
ADD R6, R6, -1
STR R2, R6, 0
ADD R6, R6, -1
STR R4, R6, 0
ADD R6, R6, -1
STR R3, R6, 0
JSR MERGESORT
ADD R6, R6, 5
ADD R6, R6, -1
STR R1, R6, 0
ADD R6, R6, -1
STR R2, R6, 0
ADD R6, R6, -1
STR R0, R6, 0
ADD R6, R6, -1
STR R4, R6, 0
ADD R6, R6, -1
STR R3, R6, 0
JSR MERGE
RETURNSORT NOP
STR R0, R5, 3
LDR R4, R5, -5
LDR R3, R5, -4
LDR R2, R5, -3
LDR R1, R5, -2
LDR R0, R5, -1
ADD R6, R5, 0
LDR R5, R6, 1
LDR R7, R6, 2
ADD R6, R6, 3

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
    ADD R6, R6, -4 
    STR R7, R6, 2
    STR R5, R6, 1
    ADD R5, R6, 0
    STR R0, R6, -1
    STR R1, R6, -2
    STR R2, R6, -3
    STR R3, R6, -4
    STR R4, R6, -5
    ADD R6, R6, -5
    LDR R0, R5, 4
    LDR R1, R5, 5
    ADD R2, R1, 0
    BRz DIVIDE_BY_ZERO
    AND R3, R3, 0
DIVIDE_LOOP
    NOT R4, R1
    ADD R4, R4, 1
    ADD R4, R0, R4
    BRn DIVIDE_END
    ADD R0, R4, 0
    ADD R3, R3, 1
    BRnzp DIVIDE_LOOP
DIVIDE_BY_ZERO
    AND R3, R3, 0
    BRnzp DIVIDE_END
DIVIDE_END
    STR R3, R5, 3
    LDR R4, R6, 0
    LDR R3, R6, 1
    LDR R2, R6, 2
    LDR R1, R6, 3
    LDR R0, R6, 4
    ADD R6, R6, 5
    ADD R6, R5, 0
    LDR R5, R6, 1
    LDR R7, R6, 2
    ADD R6, R6, 3

RET





;;  MERGE Pseudocode (see PDF for explanation and examples)
;;  MERGE(int arr[], int buf[], int start, int mid, int end) {
;;      i = start;
;;      j = mid;
;;      k = start;
;;      while (i < mid && j < end) {
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

    ADD R6, R6, -4
    STR R7, R6, 2
    STR R5, R6, 1
    ADD R5, R6, 0
    STR R0, R6, -1
    STR R1, R6, -2
    STR R2, R6, -3
    STR R3, R6, -4
    STR R4, R6, -5
    ADD R6, R6, -5
    LDR R0, R5, 4
    LDR R1, R5, 5
    LDR R2, R5, 6
    LDR R3, R5, 7
    LDR R4, R5, 8
    ADD R6, R6, -3
    STR R2, R6, 0       ; i = start
    STR R3, R6, 1       ; j = mid
    STR R2, R6, 2       ; k = start
    
MERGE_LOOP1:
    LDR R2, R6, 0
    LDR R3, R6, 1
    LDR R4, R5, 7
    NOT R4, R4
    ADD R4, R4, 1
    ADD R4, R2, R4
    BRzp MERGE_LOOP2
    LDR R4, R5, 8
    NOT R4, R4
    ADD R4, R4, 1
    ADD R4, R3, R4
    BRzp MERGE_LOOP2
    LDR R0, R5, 4
    ADD R4, R0, R2
    LDR R4, R4, 0
    LDR R0, R5, 4
    ADD R0, R0, R3
    LDR R0, R0, 0
    NOT R1, R0
    ADD R1, R1, 1
    ADD R1, R4, R1
    BRp MERGE_ELSE
    LDR R0, R6, 2
    LDR R1, R5, 5
    ADD R0, R1, R0
    STR R4, R0, 0
    LDR R0, R6, 2
    ADD R0, R0, 1
    STR R0, R6, 2
    LDR R0, R6, 0
    ADD R0, R0, 1
    STR R0, R6, 0
    BRnzp MERGE_LOOP1
    
MERGE_ELSE:
    LDR R0, R6, 1
    LDR R1, R5, 4
    ADD R0, R1, R0
    LDR R4, R0, 0
    LDR R0, R6, 2
    LDR R1, R5, 5
    ADD R0, R1, R0
    STR R4, R0, 0
    LDR R0, R6, 2
    ADD R0, R0, 1       ; k++
    STR R0, R6, 2       ; store k
    LDR R0, R6, 1       ; load j
    ADD R0, R0, 1       ; j++
    STR R0, R6, 1       ; store j
    BRnzp MERGE_LOOP1
MERGE_LOOP2:
    LDR R2, R6, 0
    LDR R3, R5, 7
    NOT R3, R3
    ADD R3, R3, 1
    ADD R3, R2, R3
    BRzp MERGE_LOOP3
    LDR R4, R5, 4
    ADD R3, R4, R2
    LDR R3, R3, 0
    LDR R4, R6, 2
    LDR R0, R5, 5
    ADD R4, R0, R4
    STR R3, R4, 0
    LDR R3, R6, 2
    ADD R3, R3, 1
    STR R3, R6, 2
    ADD R2, R2, 1
    STR R2, R6, 0
    BRnzp MERGE_LOOP2
MERGE_LOOP3:
    LDR R2, R6, 1
    LDR R3, R5, 8
    NOT R3, R3
    ADD R3, R3, 1
    ADD R3, R2, R3
    BRzp MERGE_COPY
    LDR R4, R5, 4
    ADD R3, R4, R2
    LDR R3, R3, 0
    LDR R4, R6, 2
    LDR R0, R5, 5
    ADD R4, R0, R4
    STR R3, R4, 0
    LDR R3, R6, 2
    ADD R3, R3, 1
    STR R3, R6, 2
    ADD R2, R2, 1
    STR R2, R6, 1
    BRnzp MERGE_LOOP3
MERGE_COPY:
    LDR R2, R5, 6
    LDR R3, R5, 8
MERGE_COPY_LOOP:
    NOT R4, R3
    ADD R4, R4, 1
    ADD R4, R2, R4
    BRzp MERGE_END
    LDR R0, R5, 5
    ADD R4, R0, R2
    LDR R4, R4, 0
    LDR R1, R5, 4
    ADD R0, R1, R2
    STR R4, R0, 0
    ADD R2, R2, 1
    BRnzp MERGE_COPY_LOOP
MERGE_END:
    ADD R6, R6, 3
    LDR R4, R6, 0
    LDR R3, R6, 1
    LDR R2, R6, 2
    LDR R1, R6, 3
    LDR R0, R6, 4
    ADD R6, R6, 5
    ADD R6, R5, 0
    LDR R5, R6, 1
    LDR R7, R6, 2
    ADD R6, R6, 3
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
