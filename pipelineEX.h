#ifndef PIPELINEEX_H_INCLUDED
#define PIPELINEEX_H_INCLUDED

#include "theriscv.h"

using namespace std;

void theriscv::EX(){

    //判断是否结束
    if(exmem.END)
        return;

    ClearEXMEM();

    //判断是否是气泡
    if(idex.ins==BUBBLE){
        exmem.ins=BUBBLE;
        return;
    }

    //判断数据冒险
    int xvalrs1,xvalrs2;
    if(idex.rs1==0)
        xvalrs1=0;
    else if((idex.rs1!=0) && (idex.rs1==memwb.rd))
        xvalrs1=memwb.res;
    else
        xvalrs1=x[idex.rs1];

    if(idex.rs2==0)
        xvalrs2=0;
    else if((idex.rs2!=0) && (idex.rs2==memwb.rd))
        xvalrs2=memwb.res;
    else
        xvalrs2=x[idex.rs2];

    exmem.npc=idex.npc;

    switch(idex.ins){

        //空指令
        case NOP:
            break;

        //整数创建指令
        case LUI:
            exmem.val=idex.imm<<12;
            break;
        case AUIPC:
            exmem.val=idex.npc-4+(idex.imm<<12);
            break;

        //非条件跳跃指令
        case JAL:
            exmem.val=idex.npc;
            exmem.npc=idex.npc-4+idex.imm;
            break;
        case JALR:
            exmem.val=idex.npc;
            exmem.npc=idex.npc-4+((xvalrs1+idex.imm)/2)*2;
            pc=exmem.npc;
            ifid.ins=BUBBLE;
            break;

        //条件跳跃指令
        case BEQ:
            if(xvalrs1==xvalrs2){//会跳
                CounterAdd();
                if(idex.pred){
                    CounterYes();
                    exmem.npc=idex.npc-4+idex.imm;
                }
                else{
                    CounterNo();
                    pc=exmem.npc=idex.npc-4+idex.imm;
                    ifid.ins=BUBBLE;
                }

            }
            else{//不会跳
                CounterSub();
                if(idex.pred){
                    CounterNo();
                    pc=idex.npc;
                    ifid.ins=BUBBLE;
                }
                else
                    CounterYes();
            }

            break;
        case BNE:
            if(xvalrs1!=xvalrs2){//会跳
                CounterAdd();
                if(idex.pred){
                    CounterYes();
                    exmem.npc=idex.npc-4+idex.imm;
                }
                else{
                    CounterNo();
                    pc=exmem.npc=idex.npc-4+idex.imm;
                    ifid.ins=BUBBLE;
                }

            }
            else{//不会跳
                CounterSub();
                if(idex.pred){
                    CounterNo();
                    pc=idex.npc;
                    ifid.ins=BUBBLE;
                }
                else
                    CounterYes();
            }

            break;
        case BLT:
            if(xvalrs1<xvalrs2){//会跳
                CounterAdd();
                if(idex.pred){
                    CounterYes();
                    exmem.npc=idex.npc-4+idex.imm;
                }
                else{
                    CounterNo();
                    pc=exmem.npc=idex.npc-4+idex.imm;
                    ifid.ins=BUBBLE;
                }

            }
            else{//不会跳
                CounterSub();
                if(idex.pred){
                    CounterNo();
                    pc=idex.npc;
                    ifid.ins=BUBBLE;
                }
                else
                    CounterYes();
            }

            break;
        case BGE:
            if(xvalrs1>xvalrs2){//会跳
                CounterAdd();
                if(idex.pred){
                    CounterYes();
                    exmem.npc=idex.npc-4+idex.imm;
                }
                else{
                    CounterNo();
                    pc=exmem.npc=idex.npc-4+idex.imm;
                    ifid.ins=BUBBLE;
                }

            }
            else{//不会跳
                CounterSub();
                if(idex.pred){
                    CounterNo();
                    pc=idex.npc;
                    ifid.ins=BUBBLE;
                }
                else
                    CounterYes();
            }

            break;
        case BLTU:
            if(static_cast<unsigned int>(xvalrs1) < static_cast<unsigned int>(xvalrs2)){//会跳
                CounterAdd();
                if(idex.pred){
                    CounterYes();
                    exmem.npc=idex.npc-4+idex.imm;
                }
                else{
                    CounterNo();
                    pc=exmem.npc=idex.npc-4+idex.imm;
                    ifid.ins=BUBBLE;
                }

            }
            else{//不会跳
                CounterSub();
                if(idex.pred){
                    CounterNo();
                    pc=idex.npc;
                    ifid.ins=BUBBLE;
                }
                else
                    CounterYes();
            }

            break;
        case BGEU:
            if(static_cast<unsigned int>(xvalrs1) > static_cast<unsigned int>(xvalrs2)){//会跳
                CounterAdd();
                if(idex.pred){
                    CounterYes();
                    exmem.npc=idex.npc-4+idex.imm;
                }
                else{
                    CounterNo();
                    pc=exmem.npc=idex.npc-4+idex.imm;
                    ifid.ins=BUBBLE;
                }

            }
            else{//不会跳
                CounterSub();
                if(idex.pred){
                    CounterNo();
                    pc=idex.npc;
                    ifid.ins=BUBBLE;
                }
                else
                    CounterYes();
            }

            break;

        //Load指令
        case LB:
            exmem.val=xvalrs1+idex.imm;
            break;
        case LH:
            exmem.val=xvalrs1+idex.imm;
            break;
        case LW:
            exmem.val=xvalrs1+idex.imm;
            break;
        case LBU:
            exmem.val=xvalrs1+idex.imm;
            break;
        case LHU:
            exmem.val=xvalrs1+idex.imm;
            break;

        //Store指令
        case SB:
            exmem.val=xvalrs1+idex.imm;
            break;
        case SH:
            exmem.val=xvalrs1+idex.imm;
            break;
        case SW:
            exmem.val=xvalrs1+idex.imm;
            break;

        //R-I指令
        case ADDI:
            exmem.val=xvalrs1+idex.imm;
            break;
        case SLTI:
            exmem.val=(xvalrs1<idex.imm)?1:0;
            break;
        case SLTIU:
            exmem.val=(static_cast<unsigned int>(xvalrs1)<idex.immu)?1:0;
            break;
        case XORI:
            exmem.val=xvalrs1^idex.imm;
            break;
        case ORI:
            exmem.val=xvalrs1|idex.imm;
            break;
        case ANDI:
            exmem.val=xvalrs1&idex.imm;
            break;
        case SLLI:
            exmem.val=xvalrs1<<idex.immu;
            break;
        case SRLI:
            exmem.val=static_cast<unsigned int>(xvalrs1)>>idex.immu;
            break;
        case SRAI:
            exmem.val=xvalrs1>>idex.immu;
            break;

        //R-R指令
        case ADD:
            exmem.val=xvalrs1+xvalrs2;
            break;
        case SUB:
            exmem.val=xvalrs1-xvalrs2;
            break;
        case SLL:
            exmem.val=xvalrs1 << static_cast<unsigned int>(xvalrs2);
            break;
        case SLT:
            exmem.val=(xvalrs1<xvalrs2)?1:0;
            break;
        case SLTU:
            exmem.val=(static_cast<unsigned int>(xvalrs1) < static_cast<unsigned int>(xvalrs2))?1:0;
            break;
        case XOR:
            exmem.val=xvalrs1^xvalrs2;
            break;
        case SRL:
            exmem.val=static_cast<unsigned int>(xvalrs1) >> static_cast<unsigned int>(xvalrs2);
            break;
        case SRA:
            exmem.val=xvalrs1 >> static_cast<unsigned int>(xvalrs2);
            break;
        case OR:
            exmem.val=xvalrs1|xvalrs2;
            break;
        case AND:
            exmem.val=xvalrs1&xvalrs2;
            break;

        case LI:
            break;

        default:
            exmem.val=0;
    }

    //整理EXMEM寄存器
    //exmem.npc已计算好
    exmem.ins=idex.ins;
    exmem.rs2=idex.rs2;
    exmem.rd=idex.rd;
    //exmem.val已计算好
    exmem.END=idex.END;
}

#endif // PIPELINEEX_H_INCLUDED
