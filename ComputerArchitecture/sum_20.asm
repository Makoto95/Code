addi r1, r0, 20
addi r2, r0, 0
addi r3, r0, 0
label:  addi r2,r2,1
add r3, r2, r3
blt r2, r1, label
add r3, r0, r3
