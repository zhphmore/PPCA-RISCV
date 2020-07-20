#ifndef PIPELINEIF_H_INCLUDED
#define PIPELINEIF_H_INCLUDED

#include "theriscv.h"

using namespace std;

void theriscv::IF(){

    //�ж��Ƿ����
    if(ifid.END)
        return;

    ClearIFID();

    for(int i=0;i<4;i++){
        int k=8*(4-i)-1;
        unsigned int tmp=static_cast<unsigned int>(mem[pc+i]);
        do{
            ifid.IR[k--]=tmp%2+'0';
            tmp/=2;
        }while(tmp);
    }

    pc=ifid.npc=pc+4;

    //����IFID�Ĵ���
    //ifid.IR�Ѽ����
    //ifid.npc�Ѽ����
    //ifid.ins�Ѽ����
    //ifid.END�Ѽ����
}

#endif // PIPELINEIF_H_INCLUDED
