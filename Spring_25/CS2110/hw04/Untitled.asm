.orig x3000

MAIN:
    AND R0, R0, 0
    AND R1, R1, 0
    ADD R0, R0, 5
    ADD R1, R0, 5

    JSR SUB

    ADD R0, R0, R0
    ADD R1, R1, R0
    HALT
SUB:
    ADD R0, R0, 5
    ADD R7, R7, 1
    RET
    
.end