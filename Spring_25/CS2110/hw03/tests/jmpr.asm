.orig x0000

AND R0, R0, 0 ; R0 = 0
ADD R0, R0, 6 ; R0 = 6

AND R2, R2, 0 ; R2 = 0
ADD R2, R2, 5 ; R2 = 5
ADD R2, R2, 5 ; R2 = 10

JMPR R0, 2 ; jump to HALT

AND R0, R0, 0 ; proper execution jumps over these two lines
AND R2, R2, 0

HALT
.end

;; === post-conditions ===
;; R0 = x0006, R2 = x000A, PC = x0009
;; If R0 = R2 = 0 then JMP failed!
