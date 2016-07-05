addi r1, r0, 20
addi r2, r0, 1
addi r3, r0, 0
label:  add r3, r1, r3
bgs r2, r1, label
end: j end
