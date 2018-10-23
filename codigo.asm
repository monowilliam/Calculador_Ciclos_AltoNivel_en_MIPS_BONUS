lui $s0, 0x1000
ori $s0, $s0, 0x0000
add $s7, $zero, 0
add $s6, $zero, 64
bucle:
    beq $s7, $s6, exit
    lw $s1, 0($s0)
    lw $s2, 256($s0)
    lw $s3, 512($s0)
    mult $s1, $s2
    mflo $s4
    add $s4, $s4, $s3
    sw $s4, 768($s0)
    addi $s0, $s0, 4
    addi $s7, $s7, 1
    j bucle
exit:
    jr $ra