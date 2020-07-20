#ifndef PIPELINEWB_H_INCLUDED
#define PIPELINEWB_H_INCLUDED

#include "theriscv.h"

using namespace std;

void theriscv::WB(){

    //判断是否是气泡
    if(memwb.ins==BUBBLE)
        return;

    //保证x[0]不被修改
    if(memwb.rd==0)
        return;

    switch(memwb.ins){

        //空指令
        case NOP:
            break;

        //整数创建指令
        case LUI:case AUIPC:
            x[memwb.rd]=memwb.res;
            break;

        //非条件跳跃指令
        case JAL:case JALR:
            x[memwb.rd]=memwb.res;
            break;

        //条件跳跃指令
        case BEQ:case BNE:case BLT:case BGE:case BLTU:case BGEU:
            break;

        //Load指令
        case LB:case LH:case LW:case LBU:case LHU:
            x[memwb.rd]=memwb.res;
            break;

        //Store指令
        case SB:case SH:case SW:
            break;

        //R-I指令
        case ADDI:case SLTI:case SLTIU:
        case XORI:case ORI:case ANDI:
        case SLLI:case SRLI:case SRAI:
            x[memwb.rd]=memwb.res;
            break;

        //R-R指令
        case ADD:case SUB:case SLL:case SLT:case SLTU:
        case XOR:case SRL:case SRA:case OR:case AND:
            x[memwb.rd]=memwb.res;
            break;

        case LI:
            break;

        default:
            break;
    }

    if(memwb.END){
        completed=true;
        unsigned int result=(((unsigned int)x[memwb.rd]) & 255u);
        cout << result;
    }
}

#endif // PIPELINEWB_H_INCLUDED
