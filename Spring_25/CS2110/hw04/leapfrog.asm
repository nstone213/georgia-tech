;;=============================================================
;; CS 2110 - Spring 2025
;; Homework 4 - Leapfrog
;;=============================================================
;; Name: 
;;=============================================================

.orig x3000
;; Suggested Pseudocode (see PDF for explanation)
;;
;; inc = x0100
;; func_addr = starting address of function
;; copy_addr = func_addr + inc
;; start_copy = copy_addr
;; stop_addr = xFE00  
;; if (start_copy - stop_addr == 0):
;;      HALT
;; curr = mem[func_addr]
;; while (curr != 0) {
;;      curr = mem[func_addr]
;;      mem[copy_addr] = curr
;;      func_addr++
;;      copy_addr++
;; }
;; mem[copy_addr] = 0
;; PC = start_copy
;; .fill 0

;; YOUR CODE HERE

        .fill 0
.end