#ifndef PIPELINEID_H_INCLUDED
#define PIPELINEID_H_INCLUDED

#include "theriscv.hpp"

using namespace std;

void theriscv::ID(){

    //判断是否结束
    if(idex.END)
        return;

    ClearIDEX();

    //判断是否是气泡
    if(ifid.ins==BUBBLE){
        idex.ins=BUBBLE;
        return;
    }

    //解码指令类型
    unsigned int opcode=ToUInt(ifid.IR,25,31);
    unsigned int func;
    unsigned int head;
    unsigned int immtmp1,immtmp2,immtmp3,immtmp4;

    switch(opcode){

        //空指令
        case 0:
            idex.ins=NOP;
            break;

        //整数创建指令
        case 55://"0110111"
            idex.ins=LUI;
            idex.rd=ToUInt(ifid.IR,20,24);
            idex.imm=(ifid.iru>>12) << 12;
            break;

        case 23://"0010111"
            idex.ins=AUIPC;
            idex.rd=ToUInt(ifid.IR,20,24);
            idex.imm=(ifid.iru>>12) << 12;
            break;

        //非条件跳跃指令
        case 111://"1101111"
            idex.ins=JAL;
            idex.rd=ToUInt(ifid.IR,20,24);
            immtmp1=((ifid.iru>> 12) & 255) << 12;
            immtmp2=((ifid.iru>> 20) & 1) << 11;
            immtmp3=((ifid.iru>> 21) & 1023) << 1;
            immtmp4=((ifid.iru>> 31) & 1) << 20;
            idex.imm=immtmp1+immtmp2+immtmp3+immtmp4;
            idex.imm=signextend(idex.imm,20);
            pc=ifid.npc-4+idex.imm;
            break;

        case 103://"1100111"
            func=ToUInt(ifid.IR,17,19);
            if(func==0){
                idex.ins=JALR;
                idex.rs1=ToUInt(ifid.IR,12,16);
                idex.rd=ToUInt(ifid.IR,20,24);
                idex.imm=(ifid.iru>>20) & 4095;
                idex.imm=signextend(idex.imm,11);
            }
            else{
                idex.ins=WRONG;
            }
            break;

        //条件跳跃指令
        case 99://"1100011"
            func=ToUInt(ifid.IR,17,19);
            idex.rs1=ToUInt(ifid.IR,12,16);
            idex.rs2=ToUInt(ifid.IR,7,11);
            immtmp1=((ifid.iru>> 7) & 1) << 11;
            immtmp2=((ifid.iru>> 8) & 15) << 1;
            immtmp3=((ifid.iru>> 25) & 63) << 5;
            immtmp4=((ifid.iru>> 31) & 1) << 12;
            idex.imm=immtmp1+immtmp2+immtmp3+immtmp4;
            idex.imm=signextend(idex.imm,12);

            switch(func){
                case 0://"000"
                    idex.ins=BEQ;
                    break;
                case 1://"001"
                    idex.ins=BNE;
                    break;
                case 4://"100"
                    idex.ins=BLT;
                    break;
                case 5://"101"
                    idex.ins=BGE;
                    break;
                case 6://"110"
                    idex.ins=BLTU;
                    break;
                case 7://"111"
                    idex.ins=BGEU;
                    break;
                default:
                    idex.ins=WRONG;
            }

            if(Predict()){
                pc=ifid.npc-4+idex.imm;
                idex.pred=true;
            }

            break;

        //Load指令
        case 3://"0000011"
            func=ToUInt(ifid.IR,17,19);
            idex.rs1=ToUInt(ifid.IR,12,16);
            idex.rd=ToUInt(ifid.IR,20,24);
            idex.imm=(ifid.iru>>20) & 4095;
            idex.imm=signextend(idex.imm,11);
            
            switch(func){
                case 0://"000"
                    idex.ins=LB;
                    break;
                case 1://"001"
                    idex.ins=LH;
                    break;
                case 2://"010"
                    idex.ins=LW;
                    break;
                case 4://"100"
                    idex.ins=LBU;
                    break;
                case 5://"101"
                    idex.ins=LHU;
                    break;
                default:
                    idex.ins=WRONG;
            }
            break;

        //Store指令
        case 35://"0100011"
            func=ToUInt(ifid.IR,17,19);
            idex.rs1=ToUInt(ifid.IR,12,16);
            idex.rs2=ToUInt(ifid.IR,7,11);
            immtmp1=(ifid.iru>>7) & 31;
            immtmp2=((ifid.iru>>25) & 127) << 5;
            idex.imm=immtmp1+immtmp2;
            idex.imm=signextend(idex.imm,11);

            switch(func){
                case 0://"000"
                    idex.ins=SB;
                    break;
                case 1://"001"
                    idex.ins=SH;
                    break;
                case 2://"010"
                    idex.ins=SW;
                    break;
                default:
                    idex.ins=WRONG;
            }
            break;

        //R-I指令
        case 19://"0010011"
            func=ToUInt(ifid.IR,17,19);
            head=ToUInt(ifid.IR,0,6);
            idex.rs1=ToUInt(ifid.IR,12,16);
            idex.rd=ToUInt(ifid.IR,20,24);
            switch(func){
                case 0://"000"
                    idex.ins=ADDI;
                    idex.imm=(ifid.iru>>20) & 4095;
                    idex.imm=signextend(idex.imm,11);
                    //设置的结束指令
                    if((idex.imm==255) && (idex.rs1==0) && (idex.rd==10)){
                        idex.END=true;
                        ifid.END=true;
                    }
                    break;
                case 2://"010"
                    idex.ins=SLTI;
                    idex.imm=(ifid.iru>>20) & 4095;
                    idex.imm=signextend(idex.imm,11);
                    break;
                case 3://"011"
                    if(idex.rs1>=0 && idex.imm>=0){
                        idex.ins=SLTIU;
                        idex.imm=(ifid.iru>>20) & 4095;
                        idex.imm=signextend(idex.imm,11);
                    }
                    else
                        idex.ins=WRONG;
                    break;
                case 4://"100"
                    idex.ins=XORI;
                    idex.imm=(ifid.iru>>20) & 4095;
                    idex.imm=signextend(idex.imm,11);
                    break;
                case 6://"110"
                    idex.ins=ORI;
                    idex.imm=(ifid.iru>>20) & 4095;
                    idex.imm=signextend(idex.imm,11);
                    break;
                case 7://"111"
                    idex.ins=ANDI;
                    idex.imm=(ifid.iru>>20) & 4095;
                    idex.imm=signextend(idex.imm,11);
                    break;
                case 1://"001"
                    if(head==0){
                        idex.ins=SLLI;
                        idex.immu=(ifid.iru>>20) & 31;
                    }
                    else
                        idex.ins=WRONG;
                    break;
                case 5://"101"
                    if(head==0){
                        idex.ins=SRLI;
                        idex.immu=(ifid.iru>>20) & 31;
                    }
                    else if(head==32){
                        idex.ins=SRAI;
                        idex.immu=(ifid.iru>>20) & 31;
                    }
                    else
                        idex.ins=WRONG;
                    break;
                default:
                    idex.ins=WRONG;
            }
            break;

        //R-R指令
        case 51://"0110011"
            func=ToUInt(ifid.IR,17,19);
            head=ToUInt(ifid.IR,0,6);
            idex.rs1=ToUInt(ifid.IR,12,16);
            idex.rs2=ToUInt(ifid.IR,7,11);
            idex.rd=ToUInt(ifid.IR,20,24);
            
            switch(func){
                case 0://"000"
                    if(head==0)
                        idex.ins=ADD;
                    else if(head==32)
                        idex.ins=SUB;
                    else
                        idex.ins=WRONG;
                    break;
                case 1://"001"
                    if(head==0)
                        idex.ins=SLL;
                    else
                        idex.ins=WRONG;
                    break;
                case 2://"010"
                    if(head==0)
                        idex.ins=SLT;
                    else
                        idex.ins=WRONG;
                    break;
                case 3://"011"
                    if(head==0)
                        idex.ins=SLTU;
                    else
                        idex.ins=WRONG;
                    break;
                case 4://"100"
                    if(head==0)
                        idex.ins=XOR;
                    else
                        idex.ins=WRONG;
                    break;
                case 5://"101"
                    if(head==0)
                        idex.ins=SRL;
                    else if(head==32)
                        idex.ins=SRA;
                    else
                        idex.ins=WRONG;
                    break;
                case 6://"110"
                    if(head==0)
                        idex.ins=OR;
                    else
                        idex.ins=WRONG;
                    break;
                case 7://"111"
                    if(head==0)
                        idex.ins=AND;
                    else
                        idex.ins=WRONG;
                    break;
                default:
                    idex.ins=WRONG;
            }
        break;

        default:
            idex.ins=WRONG;
    }

    //整理IDEX寄存器
    idex.npc=ifid.npc;
    //idex.ins已计算好
    //idex.rs1已计算好
    //idex.rs2已计算好
    //idex.rd已计算好
    //idex.imm已计算好
    //idex.immu已计算好
    //idex.pred已计算好
    //idex.END已计算好
}

#endif // PIPELINEID_H_INCLUDED
