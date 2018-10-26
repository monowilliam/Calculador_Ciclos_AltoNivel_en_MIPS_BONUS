lui $s0, 0x1000
ori $s0, $s0, 0x0020 #dirección base en registro s0
addi $s1, $0, 10
addi $s2, $0, 0 # i fila
sll $t1, $s1, 2
addi $t3, $0,40
    fila:
        beq $s2, $s1, fin
        addi $s3, $0, 0 # j columna
        Colu:
            beq $s3, $s1, incfila
            sll $t2, $s3, 2 #j*4
            mul $t4, $s2, $t3 #i*40
            add $t4, $t4, $t2 # calcular el offset exacto (j*4) + (i*40)
            add $t4, $t4, $s0 # suma base + offset
            add $t1, $t1, $s3
            add $t1, $t1, $s2
            lw $s5, 0($t4)
            add $t1, $t1, $s5
            sw $t1, 0($t4)
            addi $s3, $s3,1
            j Colu
    incfila: 
        addi $s2, $s2,1
        j fila
fin:
    jr $ra