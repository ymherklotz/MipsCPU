    .text
    .globl main

main:
    li $8, 10
    li $9, 0
    li $10, 17
loop:
    beq $9, $8, end
    add $10, $10, $9
    addi $9, $9, 1
    j loop
end:    
    ori $2, $0, 10
    syscall
