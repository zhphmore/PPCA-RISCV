#ifndef PIPELINEWB_H_INCLUDED
#define PIPELINEWB_H_INCLUDED

#include "theriscv.h"

using namespace std;

void theriscv::WB(){

    //�ж��Ƿ�������
    if(memwb.ins==BUBBLE)
        return;

    //��֤x[0]�����޸�
    if(memwb.rd==0)
        return;

    switch(memwb.ins){

        //��ָ��
        case NOP:
            break;

        //��������ָ��
        case LUI:case AUIPC:
            x[memwb.rd]=memwb.res;
            break;

        //��������Ծָ��
        case JAL:case JALR:
            x[memwb.rd]=memwb.res;
            break;

        //������Ծָ��
        case BEQ:case BNE:case BLT:case BGE:case BLTU:case BGEU:
            break;

        //Loadָ��
        case LB:case LH:case LW:case LBU:case LHU:
            x[memwb.rd]=memwb.res;
            break;

        //Storeָ��
        case SB:case SH:case SW:
            break;

        //R-Iָ��
        case ADDI:case SLTI:case SLTIU:
        case XORI:case ORI:case ANDI:
        case SLLI:case SRLI:case SRAI:
            x[memwb.rd]=memwb.res;
            break;

        //R-Rָ��
        case ADD:case SUB:case SLL:case SLT:case SLTU:
        case XOR:case SRL:case SRA:case OR:case AND:
            x[memwb.rd]=memwb.res;
            break;

        case LI:
            break;

        default:
            break;
    }

    if(memwb.END)
        completed=true;
}

#endif // PIPELINEWB_H_INCLUDED
