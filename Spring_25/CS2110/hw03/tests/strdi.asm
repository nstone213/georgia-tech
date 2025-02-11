.orig x0000

AND R0, R0, 0; R0 = 0
ADD R0, R0, 9; R0 = 9

LD R1, STORING ; R1 = 3
STRDI R1, R0, -4 ;

HALT 

VALUEA .fill x0006
VALUEC .fill x0009
STORING .fill x0003

.end

;; === post-conditions ===
;; R0 = x0009, R1 = x0003, MEM[x0009] = x0003
