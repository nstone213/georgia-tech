;;=============================================================
;; CS 2110 - Spring 2025
;; Homework 4 - Caesar Cipher
;;=============================================================
;; Name: 
;;=============================================================

;;  In this file, you must implement the 'MOD' and 'ENCRYPT' subroutines.
    
.orig x3000
    ;; You do not need to write anything here
    LD R6, STACK_PTR
    
    ;; Pushes arguments (string to encrypt, shift)
    ADD R6, R6, -1
    LD R0, SHIFT
    STR R0, R6, 0
    
    ADD R6, R6, -1
    LD R0, STRING
    STR R0, R6, 0
    
    JSR ENCRYPT
    
    LDR R0, R6, 0
    ADD R6, R6, 3

    HALT
    
    ;; Do not rename or remove any existing labels
    ;; You may change the value of STRING, LENGTH, SHIFT for debugging
    STACK_PTR .fill xF000
    STRING .fill x4000
    SHIFT .fill 7
    ASCIIUPPERA .fill 65
    ASCIILOWERA .fill 97
    ALPHABETLEN .fill 26



MOD ;; Do not change this label! Treat this as like the name of the function in a function header
    ;; Code your implementation for the MOD subroutine here!

    ;;  MOD Pseudocode (see PDF for explanation and examples)   
;;  
;;  MOD(int a, int b) {
;;      while (a >= b) {
;;          a -= b;
;;      }
;;      return a;
;;  }
    
    RET



ENCRYPT ;; Do not change this label! Treat this as like the name of the function in a function header
        ;; Code your implementation for the ENCRYPT subroutine here!

    ;;  ENCRYPT Pseudocode (see PDF for explanation and examples)
;;
;;  ENCRYPT(String str, int k) {
;;      int length = 0;
;;      while (str[length] != 0) {
;;          length++;
;;      }
;;      for (int i = 0; i < length; i++) {
;;          char = str[i];
;;          if (char >= 'a' && char <= 'z') {
;;              char = char - 'a';
;;              char = MOD(char + k, 26);
;;              char = char + 'a';
;;          } else if (char >= 'A' && char <= 'Z') {
;;              char = char - 'A';
;;              char = MOD(char + k, 26);
;;              char = char + 'A';
;;          }
;;
;;          str[i] = char;
;;      }
;;  }
    
    RET

.end

;; You may change the value of the string for debugging
.orig x4000
    .stringz "hello"
.end
