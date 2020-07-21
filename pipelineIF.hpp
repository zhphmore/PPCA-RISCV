#ifndef PIPELINEIF_H_INCLUDED
#define PIPELINEIF_H_INCLUDED

#include "theriscv.hpp"

using namespace std;

void theriscv::IF(){

    //判断是否结束
    if(ifid.END)
        return;

    ClearIFID();

    for(int i=0;i<4;i++){
        int k=8*(4-i)-1;
        unsigned int tmp=static_cast<unsigned int>(mem[pc+i]);
        ifid.iru+=(tmp << (8*i));
        do{
            ifid.IR[k--]=tmp%2+'0';
            tmp/=2;
        }while(tmp);
    }

    pc=ifid.npc=pc+4;

    //整理IFID寄存器
    //ifid.IR已计算好
    //ifid.npc已计算好
    //ifid.ins已计算好
    //ifid.END已计算好
}

#endif // PIPELINEIF_H_INCLUDED
