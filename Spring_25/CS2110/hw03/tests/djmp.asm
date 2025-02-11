.orig x0000

AND R1, R1, 0; R1 = 0
ADD R1, R1, 9; R1 = 9

LD R0, SRC ; R0 = x0006
ADD R0, R0, R0 ; R0 = 2R0 = x000C

STR R1, R0, 1; mem[2R0 + 1] = mem[x000D] <- R1 (9)

LD R0, SRC ; R0 = x0006
DJMP R0 ;; PC <= mem[2 * SR + 1]  

AND R0, R0, 0 ; clear R0
AND R1, R1, 0 ; clear R1 

HALT

SRC .fill x0006

.end


;; === post-conditions ===
;; PC: x000A, R0: x0006, R1: x0009, mem[x000D] = 9
;; If R0 = R1 = 0 then DJMP failed!
