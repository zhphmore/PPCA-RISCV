#ifndef THERISCV_H_INCLUDED
#define THERISCV_H_INCLUDED

#include <stdio.h>
#include <cstring>
#include <memory.h>

#define MAX_MEM 50000

using namespace std;

class theriscv{
private:

    //指令类型
    enum OrderType{
        NOP=0,
        LUI=1,AUIPC,
        JAL=3,JALR,
        BEQ=5,BNE,BLT,BGE,BLTU,BGEU,
        LB=11,LH,LW,LBU,LHU,
        SB=16,SH,SW,
        ADDI=19,SLTI,SLTIU,XORI,ORI,ANDI,SLLI=25,SRLI,SRAI,
        ADD=28,SUB,SLL,SLT,SLTU,XOR,SRL,SRA,OR,AND,
        LI=38,
        BUBBLE=39,
        WRONG=40
    };

    //x0~x31寄存器
    int x[32];
    //pc寄存器
    unsigned int pc;

    //IFID寄存器
    struct IFID{
        char IR[32];
        unsigned int npc;
        OrderType ins;
        bool END;
    }ifid;
    //IDEX寄存器
    struct IDEX{
        unsigned int npc;
        OrderType ins;
        unsigned int rs1;
        unsigned int rs2;
        unsigned int rd;
        int imm;
        unsigned int immu;
        bool pred;//分支预测
        bool END;//结束运行
    }idex;
    //EXMEM寄存器
    struct EXMEM{
        unsigned int npc;
        OrderType ins;
        unsigned int rs2;
        unsigned int rd;
        int val;
        bool END;//结束运行
    }exmem;
    //MEMWB寄存器
    struct MEMWB{
        OrderType ins;
        unsigned int rd;
        int res;
        bool END;//结束运行
    }memwb;

    //内存
    unsigned char mem[MAX_MEM];

    //分支预测二位计数器
    struct COUNTER{
        int num;
        char history[10000];
        int currentsize;
    }counter;

    //运行结束标志
    bool completed;

    //把二进制字符串转换为无符号十进制数字
    unsigned int ToUInt(const char* str,const int a,const int b);
    //把二进制字符串转换为有符号十进制数字
    int ToInt(const char* str,const int a,const int b);
    //把十六进制字符串转换为十进制数字
    int HexToDec(const char* str,const int a,const int b);
    //归零流水寄存器
    void ClearIFID();
    void ClearIDEX();
    void ClearEXMEM();
    void ClearMEMWB();
    //二位计数器操作
    bool Predict();
    void CounterAdd();
    void CounterSub();
    void CounterYes();
    void CounterNo();

public:

    //构造函数
    theriscv();
    //归零函数
    void ClearReg();
    void ClearMem();
    void ClearCounter();
    //输入
    void Input();
    void InputConfirm();
    //分支预测准确率
    void Precision();
    //五级流水函数
    void IF();
    void ID();
    void EX();
    void MEM();
    void WB();
    //运行
    void run();

};

//构造函数
theriscv::theriscv(){
    ClearReg();
    ClearMem();
    ClearCounter();
    completed=false;
}

//归零IFID寄存器
void theriscv::ClearIFID(){
    memset(ifid.IR,0,sizeof(char)*32);
    ifid.npc=0;
    ifid.ins=NOP;
    ifid.END=false;
}

//归零IDEX寄存器
void theriscv::ClearIDEX(){
    idex.npc=0;
    idex.ins=NOP;
    idex.rs1=0;
    idex.rs2=0;
    idex.rd=0;
    idex.imm=0;
    idex.immu=0;
    idex.pred=false;
    idex.END=false;
}

//归零EXMEM寄存器
void theriscv::ClearEXMEM(){
    exmem.npc=0;
    exmem.ins=NOP;
    exmem.rs2=0;
    exmem.rd=0;
    exmem.val=0;
    exmem.END=false;
}

//归零MEMWB寄存器
void theriscv::ClearMEMWB(){
    memwb.ins=NOP;
    memwb.rd=0;
    memwb.res=0;
    memwb.END=false;
}

//归零寄存器
void theriscv::ClearReg(){
    //归零x0~x31
    memset(x,0,sizeof(int)*32);
    //归零pc
    pc=0;
    //归零流水寄存器
    ClearIFID();
    ClearIDEX();
    ClearEXMEM();
    ClearMEMWB();
}

//归零内存
void theriscv::ClearMem(){
    memset(mem,0,sizeof(unsigned char)*MAX_MEM);
}

//预测是否跳转
bool theriscv::Predict(){
    if(counter.num>=3)
        return true;
    return false;
}

//归零二位计数器
void theriscv::ClearCounter(){
    counter.num=0;
    memset(counter.history,0,sizeof(char)*10000);
    counter.currentsize=0;
}

//二位计数器增加
void theriscv::CounterAdd(){
    if(counter.num!=4)
        counter.num++;
}

//二位计数器减少
void theriscv::CounterSub(){
    if(counter.num!=0)
        counter.num--;
}

//记录二位计数器判断成功
void theriscv::CounterYes(){
    counter.history[counter.currentsize++]=1;
}

//记录二位计数器判断失败
void theriscv::CounterNo(){
    counter.history[counter.currentsize++]=0;
}

//分支预测准确率
void theriscv::Precision(){
    if(!counter.currentsize){
        cout << "No record!" << endl;
        return;
    }
    double rate;
    int correct=0;
    for(int i=0;i<counter.currentsize;i++){
        if(counter.history[i]==1)
            correct++;
    }
    rate=correct / counter.currentsize;
    cout << "Precision:" << rate << endl;
}

//把二进制字符串转换为无符号十进制数字
unsigned int theriscv::ToUInt(const char* str,const int a,const int b){
    int dec=0;
    for(int i=a;i<=b;i++){
        dec=(dec<<1)+(str[i]-'0');
    }
    return dec;
}

//把二进制字符串转换为有符号十进制数字
int theriscv::ToInt(const char* str,const int a,const int b){
    int dec=0;
    for(int i=a+1;i<=b;i++){
        dec=(dec<<1)+(str[i]-'0');
    }
    if(str[a]=='1')
        dec=-dec;
    return dec;
}

//把十六进制字符串转换为十进制数字
int theriscv::HexToDec(const char* str,const int a,const int b){
    int dec=0;
    for(int i=a;i<=b;i++){
        if((str[i]>='0')&&(str[i]<='9'))
            dec=(dec<<4)+(str[i]-'0');
        else if((str[i]>='a')&&(str[i]<='f'))
            dec=(dec<<4)+10+(str[i]-'a');
        else if((str[i]>='A')&&(str[i]<='F'))
            dec=(dec<<4)+10+(str[i]-'A');
        else
            dec=dec<<4;
    }
    return dec;
}

//输入
void theriscv::Input(){
    int adtmp;
    char tmpstr[12];
    memset(tmpstr,0,sizeof(char)*12);
    while(cin >> tmpstr){
        if(tmpstr[0]=='#')
            break;
        if(tmpstr[0]=='@'){
            adtmp=HexToDec(tmpstr,1,8);
            //mem[adtmp++]=HexToDec(tmpstr,9,10);
        }
        else{
            mem[adtmp++]=HexToDec(tmpstr,0,1);
        }
        memset(tmpstr,0,sizeof(char)*12);
    }
}

//检查输入
void theriscv::InputConfirm(){
    for(int i=0;i<MAX_MEM;i++)
        if(mem[i]){
            printf("%d",mem[i]);
            cout << ' ';
        }
}

//运行
void theriscv::run(){
    while(!completed){
        void WB();
        void MEM();
        void EX();
        void ID();
        void IF();
    }
}

#endif // THERISCV_H_INCLUDED
