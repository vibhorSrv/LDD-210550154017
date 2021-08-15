#ifndef MOD_IOCTL2_H
#define MOD_IOCTL2_H

/**
 * This structure will contain the operands and the operator to be performed on them
 */ 
struct oper_container
{
    int operand1, operand2;
    char operator;
};

#define MAGIC_NUM 'O'

#define IOCT_CALC_PARCEL _IOWR(MAGIC_NUM, 21, struct oper_container)
#define IOCT_RES _IOR(MAGIC_NUM, 22, int)

#endif