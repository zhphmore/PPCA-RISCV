#ifndef PIPELINEID_H_INCLUDED
#define PIPELINEID_H_INCLUDED

#include "theriscv.h"

using namespace std;

void theriscv::ID(){

    //�ж��Ƿ����
    if(idex.END)
        return;

    ClearIDEX();

    //�ж��Ƿ�������
    if(ifid.ins==BUBBLE){
        idex.ins=BUBBLE;
        return;
    }

    //����ָ������
    unsigned int opcode=ToUInt(ifid.IR,25,31);
    unsigned int func;
    unsigned int head;
    char immtmp[21];

    switch(opcode){

        //��ָ��
        case 0:
            idex.ins=NOP;
            break;

        //��������ָ��
        case 55://"0110111"
            idex.ins=LUI;
            idex.rd=ToUInt(ifid.IR,20,24);
            idex.imm=ToInt(ifid.IR,0,19);
            break;

        case 23://"0010111"
            idex.ins=AUIPC;
            idex.rd=ToUInt(ifid.IR,20,24);
            idex.imm=ToInt(ifid.IR,0,19);
            break;

        //��������Ծָ��
        case 111://"1101111"
            idex.ins=JAL;
            idex.rd=ToUInt(ifid.IR,20,24);
            for(int i=1;i<=10;i++)
                immtmp[i+11]=ifid.IR[i];
            immtmp[11]=ifid.IR[11];
            for(int i=12;i<=19;i++)
                immtmp[i-11]=ifid.IR[i];
            immtmp[0]=ifid.IR[0];
            immtmp[0]='0';
            idex.imm=ToInt(immtmp,0,20);
            pc=ifid.npc-4+idex.imm;
            break;

        case 103://"1100111"
            func=ToUInt(ifid.IR,17,19);
            if(func==0){
                idex.ins=JALR;
                idex.rs1=ToUInt(ifid.IR,12,16);
                idex.rd=ToUInt(ifid.IR,20,24);
                idex.imm=ToUInt(ifid.IR,0,11);
            }
            else{
                idex.ins=WRONG;
            }
            break;

        //������Ծָ��
        case 99://"1100011"
            func=ToUInt(ifid.IR,17,19);
            idex.rs1=ToUInt(ifid.IR,12,16);
            idex.rs2=ToUInt(ifid.IR,7,11);
            immtmp[0]=ifid.IR[0];
            immtmp[1]=ifid.IR[24];
            for(int i=1;i<=6;i++)
                immtmp[i+1]=ifid.IR[i];
            for(int i=20;i<=23;i++)
                immtmp[i-12]=ifid.IR[i];
            immtmp[12]='0';
            idex.imm=ToUInt(immtmp,0,12);

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

        //Loadָ��
        case 3://"0000011"
            func=ToUInt(ifid.IR,17,19);
            idex.rs1=ToUInt(ifid.IR,12,16);
            idex.rd=ToUInt(ifid.IR,20,24);
            idex.imm=ToInt(ifid.IR,0,11);
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

        //Storeָ��
        case 35://"0100011"
            func=ToUInt(ifid.IR,17,19);
            idex.rs1=ToUInt(ifid.IR,12,16);
            idex.rs2=ToUInt(ifid.IR,7,11);
            for(int i=0;i<=6;i++)
                immtmp[i]=ifid.IR[i];
            for(int i=20;i<=24;i++)
                immtmp[i-13]=ifid.IR[i];
            idex.imm=ToInt(ifid.IR,0,11);
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

        //R-Iָ��
        case 17://"0010011"
            func=ToUInt(ifid.IR,17,19);
            head=ToUInt(ifid.IR,0,6);
            idex.rs1=ToUInt(ifid.IR,12,16);
            idex.rd=ToUInt(ifid.IR,20,24);
            switch(func){
                case 0://"000"
                    idex.ins=ADDI;
                    idex.imm=ToInt(ifid.IR,0,11);
                    //���õĽ���ָ��
                    if((idex.imm==255) && (idex.rs1==0) && (idex.rd==10)){
                        idex.END=true;
                        ifid.END=true;
                    }
                    break;
                case 2://"010"
                    idex.ins=SLTI;
                    idex.imm=ToInt(ifid.IR,0,11);
                    break;
                case 3://"011"
                    if(idex.rs1>=0 && idex.imm>=0){
                        idex.ins=SLTIU;
                        idex.immu=ToUInt(ifid.IR,0,11);
                    }
                    else
                        idex.ins=WRONG;
                    break;
                case 4://"100"
                    idex.ins=XORI;
                    idex.imm=ToInt(ifid.IR,0,11);
                    break;
                case 6://"110"
                    idex.ins=ORI;
                    idex.imm=ToInt(ifid.IR,0,11);
                    break;
                case 7://"111"
                    idex.ins=ANDI;
                    idex.imm=ToInt(ifid.IR,0,11);
                    break;
                case 1://"001"
                    if(head==0){
                        idex.ins=SLLI;
                        idex.immu=ToUInt(ifid.IR,7,11);
                    }
                    else
                        idex.ins=WRONG;
                    break;
                case 5://"101"
                    if(head==0){
                        idex.ins=SRLI;
                        idex.immu=ToUInt(ifid.IR,7,11);
                    }
                    else if(head==32){
                        idex.ins=SRAI;
                        idex.immu=ToUInt(ifid.IR,7,11);
                    }
                    else
                        idex.ins=WRONG;
                    break;
                default:
                    idex.ins=WRONG;
            }
            break;

        //R-Rָ��
        case 54://"0110011"
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

    //����IDEX�Ĵ���
    idex.npc=ifid.npc;
    //idex.ins�Ѽ����
    //idex.rs1�Ѽ����
    //idex.rs2�Ѽ����
    //idex.rd�Ѽ����
    //idex.imm�Ѽ����
    //idex.immu�Ѽ����
    //idex.pred�Ѽ����
    //idex.END�Ѽ����
}

#endif // PIPELINEID_H_INCLUDED
