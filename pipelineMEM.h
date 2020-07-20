#ifndef PIPELINEMEM_H_INCLUDED
#define PIPELINEMEM_H_INCLUDED

#include "theriscv.h"

using namespace std;

void theriscv::MEM(){

    //判断是否结束
    if(memwb.END)
        return;

    ClearMEMWB();

    //判断是否是气泡
    if(exmem.ins==BUBBLE){
        memwb.ins=BUBBLE;
        return;
    }

    char change[5];//访存LS指令会用到
    memset(change,0,5);

    switch(exmem.ins){

        //空指令
        case NOP:
            break;

        //整数创建指令
        case LUI:case AUIPC:
            memwb.res=exmem.val;
            break;

        //非条件跳跃指令
        case JAL:case JALR:
            memwb.res=exmem.val;
            break;

        //条件跳跃指令
        case BEQ:case BNE:case BLT:case BGE:case BLTU:case BGEU:
            break;

        //Load指令
        case LB:
            memwb.res=mem[exmem.val];
        case LH:
            memwb.res=(static_cast<int>(mem[exmem.val+1])<<8)+static_cast<int>(mem[exmem.val]);
        case LW:
            change[3]=mem[exmem.val];
            change[2]=mem[exmem.val+1];
            change[1]=mem[exmem.val+2];
            change[0]=mem[exmem.val+3];
            memmove(&memwb.res,change,sizeof(memwb.res));
        case LBU:
            change[3]=mem[exmem.val];
            memmove(&memwb.res,change,sizeof(memwb.res));
        case LHU:
            change[3]=mem[exmem.val];
            change[2]=mem[exmem.val+1];
            memmove(&memwb.res,change,sizeof(memwb.res));
            break;

        //Store指令
        case SB:
            mem[exmem.val]=x[exmem.rs2];
            break;
        case SH:
            memmove(change,&x[exmem.rs2],sizeof(x[exmem.rs2]));
            mem[exmem.val]=change[3];
            mem[exmem.val+1]=change[2];
            break;
        case SW:
            memmove(change,&x[exmem.rs2],sizeof(x[exmem.rs2]));
            mem[exmem.val]=change[3];
            mem[exmem.val+1]=change[2];
            mem[exmem.val+2]=change[1];
            mem[exmem.val+3]=change[0];
            break;

        //R-I指令
        case ADDI:case SLTI:case SLTIU:
        case XORI:case ORI:case ANDI:
        case SLLI:case SRLI:case SRAI:
            memwb.res=exmem.val;
            break;

        //R-R指令
        case ADD:case SUB:case SLL:case SLT:case SLTU:
        case XOR:case SRL:case SRA:case OR:case AND:
            memwb.res=exmem.val;
            break;

        default:
            memwb.res=0;
    }

    //整理MEMWB寄存器
    memwb.ins=exmem.ins;
    memwb.rd=exmem.rd;
    //memwb.res已计算好
    memwb.END=exmem.END;
}

#endif // PIPELINEMEM_H_INCLUDED
