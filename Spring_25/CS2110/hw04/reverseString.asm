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
    ;; Calculate the length of the string
    AND R0, R0, #0          ; R0 = length = 0
    LD R1, STRING           ; R1 = base address of STRING

    LENGTH_LOOP
        ADD R2, R1, R0      ; R2 = address of STRING[length]
        LDR R3, R2, #0      ; R3 = STRING[length]
        BRz LENGTH_DONE      ; If STRING[length] == 0, exit loop
        ADD R0, R0, #1       ; length++
        BR LENGTH_LOOP

    LENGTH_DONE

    ;; Initialize start and end pointers
    AND R2, R2, #0          ; R2 = start = 0
    ADD R3, R0, #-1         ; R3 = end = length - 1

    ;; Swap characters until start >= end
    SWAP_LOOP
        NOT R4, R3
        ADD R4, R4, #1      ; R4 = -end
        ADD R4, R2, R4      ; R4 = start - end
        BRzp SWAP_DONE       ; If start >= end, exit loop

        ;; Load STRING[start] into R5
        ADD R5, R1, R2      ; R5 = address of STRING[start]
        LDR R6, R5, #0      ; R6 = STRING[start]

        ;; Load STRING[end] into R7
        ADD R7, R1, R3      ; R7 = address of STRING[end]
        LDR R4, R7, #0      ; R4 = STRING[end]

        ;; Swap STRING[start] and STRING[end]
        STR R4, R5, #0      ; STRING[start] = STRING[end]
        STR R6, R7, #0      ; STRING[end] = STRING[start]

        ;; Increment start and decrement end
        ADD R2, R2, #1      ; start++
        ADD R3, R3, #-1     ; end--
        BR SWAP_LOOP

    SWAP_DONE
    HALT

;; Do not rename or remove any existing labels
;; You may change the value of LENGTH for debugging
STRING .fill x4000
.end

;; You may change the value of the string for debugging
.orig x4000
    .stringz "hello"
.end
