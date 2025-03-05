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
    ;; Initialize minIndex and minValue
    AND R0, R0, #0          ; R0 = minIndex = 0
    LD R1, ARRAY            ; R1 = base address of ARRAY
    LDR R2, R1, #0          ; R2 = minValue = ARRAY[0]
    AND R3, R3, #0          ; R3 = i = 1 (loop counter)
    ADD R3, R3, #1

    ;; Loop through the array
    LOOP
        LD R4, LENGTH       ; R4 = LENGTH
        NOT R4, R4
        ADD R4, R4, #1      ; R4 = -LENGTH
        ADD R4, R3, R4      ; R4 = i - LENGTH
        BRzp END_LOOP        ; If i >= LENGTH, exit loop

        ;; Load ARRAY[i] into R5
        ADD R5, R1, R3      ; R5 = address of ARRAY[i]
        LDR R5, R5, #0      ; R5 = ARRAY[i]

        ;; Compare ARRAY[i] with minValue
        NOT R6, R2
        ADD R6, R6, #1      ; R6 = -minValue
        ADD R6, R5, R6      ; R6 = ARRAY[i] - minValue
        BRzp SKIP_UPDATE     ; If ARRAY[i] >= minValue, skip update

        ;; Update minValue and minIndex
        ADD R2, R5, #0      ; minValue = ARRAY[i]
        ADD R0, R3, #0      ; minIndex = i

        SKIP_UPDATE
        ADD R3, R3, #1      ; i++
        BR LOOP

    END_LOOP

    ;; Store the result
    LD R7, RESULT           ; R7 = address of RESULT
    STR R0, R7, #0          ; mem[RESULT] = minIndex
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
