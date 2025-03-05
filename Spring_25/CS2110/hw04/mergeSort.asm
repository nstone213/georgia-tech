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
;;      if (start >= end - 1) {
;;        return;
;;      }
;;      mid = DIVIDE(start + end, 2)
;;      MERGESORT(arr, buf, start, mid)
;;      MERGESORT(arr, buf, mid, end)
;;      MERGE(arr, buf, start, mid, end)
;;  }
    ADD R6, R6, -4 ; Allocate space
    STR R7, R6, 2 ; Save Ret Addr
    STR R5, R6, 1 ; Save Old FP
    ADD R5, R6, 0 ; Copy SP to FP
    ADD R6, R6, -5 ; Make room for saved regs & l.v. 1-n
    STR R0, R5, -1 ; Save R0
    STR R1, R5, -2 ; Save R1
    STR R2, R5, -3 ; Save R2
    STR R3, R5, -4 ; Save R3
    STR R4, R5, -5 ; Save R4
    
    ;method here
    LDR R0, R5, 6 ;R0 = start
    LDR R1, R5, 7 ;R1 = end
    ADD R1, R1, -1
    NOT R1, R1
    ADD R1, R1, 1 ;R1 = -end
;;    if (start >= end) {
;;        return;
;;      }
    ADD R2, R0, R1 ;;start - end for cc logic
    BRzp RETURNSORT ;;return if start - end < 0
    ;mid = DIVIDE(start + end, 2)
    LDR R1, R5, 7 ;R1 = end
    ADD R3, R0, R1 ;R3 = start + end
    AND R4, R4, 0
    ADD R4, R4, 2 ;R4 = 2
    ADD R6, R6, -1  ; push args
    STR R4, R6, 0 ;pushing 2
    ADD R6, R6, -1
    STR R3, R6, 0 ;pushing start + end
    JSR DIVIDE
    ;divide does its thing and pushes return value on stack
    ;ATP R0 = start, R1 = end, R2 = unused, R3 = start + end, R4 = 2
    LDR R2, R6, 0 ;R2 = return of divide = mid
    ADD R6, R6, 3 ;pop return and 2 args
    LDR R3, R5, 4 ;R3 = arr
    LDR R4, R5, 5 ;R4 = buf
    ;R0 = start, R1 = end, R2 = mid, R3 = arr, R4 = buf
    
    ;MERGESORT(arr, buf, start, mid)
    ;push args from mid to arr
    ADD R6, R6, -1
    STR R2, R6, 0 ;pushing mid
    ADD R6, R6, -1
    STR R0, R6, 0 ;pushing start
    ADD R6, R6, -1
    STR R4, R6, 0 ;pushing buf
    ADD R6, R6, -1
    STR R3, R6, 0 ;pushing arr
    JSR MERGESORT
    ;mergesort does its thing, returns with return val and args
    ADD R6, R6, 5 ;pop return value and 4 args
    
    ;R0 = start, R1 = end, R2 = mid, R3 = arr, R4 = buf
    ;MERGESORT(arr, buf, mid, end)
    ;push args from end to arr
    ADD R6, R6, -1
    STR R1, R6, 0 ;pushing end
    ADD R6, R6, -1
    STR R2, R6, 0 ;pushing mid
    ADD R6, R6, -1
    STR R4, R6, 0 ;pushing buf
    ADD R6, R6, -1
    STR R3, R6, 0 ;pushing arr
    JSR MERGESORT
    ;mergesort does its thing, returns with return val and args
    ADD R6, R6, 5 ;pop return value and 4 args
    
    ;MERGE(arr, buf, start, mid, end)
    ;R0 = start, R1 = end, R2 = mid, R3 = arr, R4 = buf
    ;push args from end to arr
    ADD R6, R6, -1
    STR R1, R6, 0 ;pushing end
    ADD R6, R6, -1
    STR R2, R6, 0 ;pushing mid
    ADD R6, R6, -1
    STR R0, R6, 0 ;pushing start
    ADD R6, R6, -1
    STR R4, R6, 0 ;pushing buf
    ADD R6, R6, -1
    STR R3, R6, 0 ;pushing arr
    JSR MERGE
    ;merge does its thing, returns val and 5 args
    ADD R6, R6, 6 ;pop return value and 5 args
    
    ;;teardown
    RETURNSORT NOP
    STR R0, R5, 3 ; save return value on stack
    LDR R4, R5, -5 ; Restore R4
    LDR R3, R5, -4 ; Restore R3
    LDR R2, R5, -3 ; Restore R2
    LDR R1, R5, -2 ; Restore R1
    LDR R0, R5, -1 ; Restore R0
    ADD R6, R5, 0 ; Restore SP
    LDR R5, R6, 1 ; Restore FP
    LDR R7, R6, 2 ; Restore RA
    ADD R6, R6, 3 ; Pop RA,FP,1st local variable
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
    ADD R6, R6, -4      ; allocate space for return value, ret addr, old frame ptr, and 1 local var
    STR R7, R6, 2       ; save return address
    STR R5, R6, 1       ; save old frame pointer
    ADD R5, R6, 0       ; set up new frame pointer
    STR R0, R6, -1      ; save R0
    STR R1, R6, -2      ; save R1
    STR R2, R6, -3      ; save R2
    STR R3, R6, -4      ; save R3
    STR R4, R6, -5      ; save R4
    ADD R6, R6, -5      ; adjust stack pointer
    LDR R0, R5, 4       ; Load a (first parameter)
    LDR R1, R5, 5       ; Load b (second parameter)
    ADD R2, R1, 0       ; Copy b to R2
    BRz DIVIDE_BY_ZERO  ; If b == 0, handle division by zero
    AND R3, R3, 0       ; R3 = quotient = 0
DIVIDE_LOOP
    NOT R4, R1          ; R4 = ~b
    ADD R4, R4, 1       ; R4 = -b
    ADD R4, R0, R4      ; R4 = a - b
    BRn DIVIDE_END      ; If a < b, exit loop
    ADD R0, R4, 0       ; a = a - b (R4 already contains a-b)
    ADD R3, R3, 1       ; quotient++
    BRnzp DIVIDE_LOOP
DIVIDE_BY_ZERO
    AND R3, R3, 0       ; R3 = 0
    BRnzp DIVIDE_END
DIVIDE_END
    STR R3, R5, 3       ; Store result in return value slot
    LDR R4, R6, 0       ; restore R4
    LDR R3, R6, 1       ; restore R3
    LDR R2, R6, 2       ; restore R2
    LDR R1, R6, 3       ; restore R1
    LDR R0, R6, 4       ; restore R0
    ADD R6, R6, 5       ; adjust stack pointer
    ADD R6, R5, 0       ; restore stack pointer
    LDR R5, R6, 1       ; restore old frame pointer
    LDR R7, R6, 2       ; restore return address
    ADD R6, R6, 3       ; pop off return value, ret addr, and old frame ptr
    RET                 ; return to caller





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

    ADD R6, R6, -4      ; allocate space for return value, ret addr, old frame ptr, and 1 local var
    STR R7, R6, 2       ; save return address
    STR R5, R6, 1       ; save old frame pointer
    ADD R5, R6, 0       ; set up new frame pointer
    STR R0, R6, -1      ; save R0
    STR R1, R6, -2      ; save R1
    STR R2, R6, -3      ; save R2
    STR R3, R6, -4      ; save R3
    STR R4, R6, -5      ; save R4
    ADD R6, R6, -5      ; adjust stack pointer
    LDR R0, R5, 4       ; Load arr (array address)
    LDR R1, R5, 5       ; Load buf (buffer address)
    LDR R2, R5, 6       ; Load start
    LDR R3, R5, 7       ; Load mid
    LDR R4, R5, 8       ; Load end
    ADD R6, R6, -3      ; Make space for i, j, k
    STR R2, R6, 0       ; i = start
    STR R3, R6, 1       ; j = mid
    STR R2, R6, 2       ; k = start
    
MERGE_LOOP1:
    LDR R2, R6, 0       ; Load i
    LDR R3, R6, 1       ; Load j
    LDR R4, R5, 7       ; Load mid
    NOT R4, R4          ; R4 = ~mid
    ADD R4, R4, 1       ; R4 = -mid
    ADD R4, R2, R4      ; R4 = i - mid
    BRzp MERGE_LOOP2    ; If i >= mid, exit loop
    LDR R4, R5, 8       ; Load end
    NOT R4, R4          ; R4 = ~end
    ADD R4, R4, 1       ; R4 = -end
    ADD R4, R3, R4      ; R4 = j - end
    BRzp MERGE_LOOP2    ; If j >= end, exit loop
    LDR R0, R5, 4       ; Reload array address to R0
    ADD R4, R0, R2      ; R4 = arr + i = address of arr[i]
    LDR R4, R4, 0       ; R4 = value of arr[i]
    LDR R0, R5, 4       ; Reload array address to R0
    ADD R0, R0, R3      ; R0 = arr + j = address of arr[j]
    LDR R0, R0, 0       ; R0 = value of arr[j]
    NOT R1, R0          ; R1 = ~arr[j]
    ADD R1, R1, 1       ; R1 = -arr[j]
    ADD R1, R4, R1      ; R1 = arr[i] - arr[j]
    BRp MERGE_ELSE      ; If arr[i] > arr[j], go to else part
    LDR R0, R6, 2       ; Load k
    LDR R1, R5, 5       ; Reload buffer address to R1
    ADD R0, R1, R0      ; R0 = buf + k = address of buf[k]
    STR R4, R0, 0       ; buf[k] = arr[i]
    LDR R0, R6, 2       ; Load k
    ADD R0, R0, 1       ; k++
    STR R0, R6, 2       ; Store k
    LDR R0, R6, 0       ; Load i
    ADD R0, R0, 1       ; i++
    STR R0, R6, 0       ; Store i
    BRnzp MERGE_LOOP1   ; Continue loop
    
MERGE_ELSE:
    LDR R0, R6, 1       ; Load j
    LDR R1, R5, 4       ; Reload array address to R1
    ADD R0, R1, R0      ; R0 = arr + j = address of arr[j]
    LDR R4, R0, 0       ; R4 = value of arr[j]
    LDR R0, R6, 2       ; Load k
    LDR R1, R5, 5       ; Reload buffer address to R1
    ADD R0, R1, R0      ; R0 = buf + k = address of buf[k]
    STR R4, R0, 0       ; buf[k] = arr[j]
    LDR R0, R6, 2       ; Load k
    ADD R0, R0, 1       ; k++
    STR R0, R6, 2       ; Store k
    LDR R0, R6, 1       ; Load j
    ADD R0, R0, 1       ; j++
    STR R0, R6, 1       ; Store j
    BRnzp MERGE_LOOP1   ; Continue loop
MERGE_LOOP2:
    LDR R2, R6, 0       ; Load i
    LDR R3, R5, 7       ; Load mid
    NOT R3, R3          ; R3 = ~mid
    ADD R3, R3, 1       ; R3 = -mid
    ADD R3, R2, R3      ; R3 = i - mid
    BRzp MERGE_LOOP3    ; If i >= mid, exit loop
    LDR R4, R5, 4       ; Reload array address
    ADD R3, R4, R2      ; R3 = arr + i = address of arr[i]
    LDR R3, R3, 0       ; R3 = value of arr[i]
    LDR R4, R6, 2       ; Load k
    LDR R0, R5, 5       ; Reload buffer address
    ADD R4, R0, R4      ; R4 = buf + k = address of buf[k]
    STR R3, R4, 0       ; buf[k] = arr[i]
    LDR R3, R6, 2       ; Load k
    ADD R3, R3, 1       ; k++
    STR R3, R6, 2       ; Store k
    ADD R2, R2, 1       ; i++
    STR R2, R6, 0       ; Store i
    BRnzp MERGE_LOOP2   ; Continue loop
MERGE_LOOP3:
    LDR R2, R6, 1       ; Load j
    LDR R3, R5, 8       ; Load end
    NOT R3, R3          ; R3 = ~end
    ADD R3, R3, 1       ; R3 = -end
    ADD R3, R2, R3      ; R3 = j - end
    BRzp MERGE_COPY     ; If j >= end, exit loop
    LDR R4, R5, 4       ; Reload array address
    ADD R3, R4, R2      ; R3 = arr + j = address of arr[j]
    LDR R3, R3, 0       ; R3 = value of arr[j]
    LDR R4, R6, 2       ; Load k
    LDR R0, R5, 5       ; Reload buffer address
    ADD R4, R0, R4      ; R4 = buf + k = address of buf[k]
    STR R3, R4, 0       ; buf[k] = arr[j]
    LDR R3, R6, 2       ; Load k
    ADD R3, R3, 1       ; k++
    STR R3, R6, 2       ; Store k
    ADD R2, R2, 1       ; j++
    STR R2, R6, 1       ; Store j
    BRnzp MERGE_LOOP3   ; Continue loop
MERGE_COPY:
    LDR R2, R5, 6       ; Load start (i = start)
    LDR R3, R5, 8       ; Load end
MERGE_COPY_LOOP:
    NOT R4, R3          ; R4 = ~end
    ADD R4, R4, 1       ; R4 = -end
    ADD R4, R2, R4      ; R4 = i - end
    BRzp MERGE_END      ; If i >= end, exit loop
    LDR R0, R5, 5       ; Reload buffer address
    ADD R4, R0, R2      ; R4 = buf + i = address of buf[i]
    LDR R4, R4, 0       ; R4 = value of buf[i]
    LDR R1, R5, 4       ; Reload array address
    ADD R0, R1, R2      ; R0 = arr + i = address of arr[i]
    STR R4, R0, 0       ; arr[i] = buf[i]
    ADD R2, R2, 1       ; i++
    BRnzp MERGE_COPY_LOOP ; Continue loop
MERGE_END:
    ADD R6, R6, 3       ; Free space for i, j, k
    LDR R4, R6, 0       ; restore R4
    LDR R3, R6, 1       ; restore R3
    LDR R2, R6, 2       ; restore R2
    LDR R1, R6, 3       ; restore R1
    LDR R0, R6, 4       ; restore R0
    ADD R6, R6, 5       ; adjust stack pointer
    ADD R6, R5, 0       ; restore stack pointer
    LDR R5, R6, 1       ; restore old frame pointer
    LDR R7, R6, 2       ; restore return address
    ADD R6, R6, 3       ; pop off return value, ret addr, and old frame ptr
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
