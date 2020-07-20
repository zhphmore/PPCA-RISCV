#ifndef THERISCV_H_INCLUDED
#define THERISCV_H_INCLUDED

#include <stdio.h>
#include <cstring>
#include <memory.h>

#define MAX_MEM 50000

using namespace std;

class theriscv{
private:

    //ָ������
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

    //x0~x31�Ĵ���
    int x[32];
    //pc�Ĵ���
    unsigned int pc;

    //IFID�Ĵ���
    struct IFID{
        char IR[32];
        unsigned int npc;
        OrderType ins;
        bool END;
    }ifid;
    //IDEX�Ĵ���
    struct IDEX{
        unsigned int npc;
        OrderType ins;
        unsigned int rs1;
        unsigned int rs2;
        unsigned int rd;
        int imm;
        unsigned int immu;
        bool pred;//��֧Ԥ��
        bool END;//��������
    }idex;
    //EXMEM�Ĵ���
    struct EXMEM{
        unsigned int npc;
        OrderType ins;
        unsigned int rs2;
        unsigned int rd;
        int val;
        bool END;//��������
    }exmem;
    //MEMWB�Ĵ���
    struct MEMWB{
        OrderType ins;
        unsigned int rd;
        int res;
        bool END;//��������
    }memwb;

    //�ڴ�
    unsigned char mem[MAX_MEM];

    //��֧Ԥ���λ������
    struct COUNTER{
        int num;
        char history[10000];
        int currentsize;
    }counter;

    //���н�����־
    bool completed;

    //�Ѷ������ַ���ת��Ϊ�޷���ʮ��������
    unsigned int ToUInt(const char* str,const int a,const int b);
    //�Ѷ������ַ���ת��Ϊ�з���ʮ��������
    int ToInt(const char* str,const int a,const int b);
    //��ʮ�������ַ���ת��Ϊʮ��������
    int HexToDec(const char* str,const int a,const int b);
    //������ˮ�Ĵ���
    void ClearIFID();
    void ClearIDEX();
    void ClearEXMEM();
    void ClearMEMWB();
    //��λ����������
    bool Predict();
    void CounterAdd();
    void CounterSub();
    void CounterYes();
    void CounterNo();

public:

    //���캯��
    theriscv();
    //���㺯��
    void ClearReg();
    void ClearMem();
    void ClearCounter();
    //����
    void Input();
    void InputConfirm();
    //�弶��ˮ����
    void IF();
    void ID();
    void EX();
    void MEM();
    void WB();
    //����
    void run();

};

//���캯��
theriscv::theriscv(){
    ClearReg();
    ClearMem();
    ClearCounter();
    completed=false;
}

//����IFID�Ĵ���
void theriscv::ClearIFID(){
    memset(ifid.IR,0,32);
    ifid.npc=0;
    ifid.ins=NOP;
    ifid.END=false;
}

//����IDEX�Ĵ���
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

//����EXMEM�Ĵ���
void theriscv::ClearEXMEM(){
    exmem.npc=0;
    exmem.ins=NOP;
    exmem.rs2=0;
    exmem.rd=0;
    exmem.val=0;
    exmem.END=false;
}

//����MEMWB�Ĵ���
void theriscv::ClearMEMWB(){
    memwb.ins=NOP;
    memwb.rd=0;
    memwb.res=0;
    memwb.END=false;
}

//����Ĵ���
void theriscv::ClearReg(){
    //����x0~x31
    memset(x,0,32);
    //����pc
    pc=0;
    //������ˮ�Ĵ���
    ClearIFID();
    ClearIDEX();
    ClearEXMEM();
    ClearMEMWB();
}

//�����ڴ�
void theriscv::ClearMem(){
    memset(mem,0,MAX_MEM);
}

//Ԥ���Ƿ���ת
bool theriscv::Predict(){
    if(counter.num>=3)
        return true;
    return false;
}

//�����λ������
void theriscv::ClearCounter(){
    counter.num=0;
    memset(counter.history,0,10000);
    counter.currentsize=0;
}

//��λ����������
void theriscv::CounterAdd(){
    if(counter.num!=4)
        counter.num++;
}

//��λ����������
void theriscv::CounterSub(){
    if(counter.num!=0)
        counter.num--;
}

//��¼��λ�������жϳɹ�
void theriscv::CounterYes(){
    counter.history[counter.currentsize++]=1;
}

//��¼��λ�������ж�ʧ��
void theriscv::CounterNo(){
    counter.history[counter.currentsize++]=0;
}

//�Ѷ������ַ���ת��Ϊ�޷���ʮ��������
unsigned int theriscv::ToUInt(const char* str,const int a,const int b){
    int dec=0;
    for(int i=a;i<=b;i++){
        dec=(dec<<1)+(str[i]-'0');
    }
    return dec;
}

//�Ѷ������ַ���ת��Ϊ�з���ʮ��������
int theriscv::ToInt(const char* str,const int a,const int b){
    int dec=0;
    for(int i=a+1;i<=b;i++){
        dec=(dec<<1)+(str[i]-'0');
    }
    if(str[a]=='1')
        dec=-dec;
    return dec;
}

//��ʮ�������ַ���ת��Ϊʮ��������
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

//����
void theriscv::Input(){
    int adtmp;
    char tmpstr[12];
    memset(tmpstr,0,12);
    while(cin >> tmpstr){
        if(tmpstr[0]=='@'){
            adtmp=HexToDec(tmpstr,1,8);
            mem[adtmp++]=HexToDec(tmpstr,9,10);
        }
        else{
            mem[adtmp++]=HexToDec(tmpstr,0,1);
        }
    }
}

//�������
void theriscv::InputConfirm(){
    for(int i=0;i<MAX_MEM;i++)
        if(mem[i]){
            printf("%d",mem[i]);
            cout << ' ';
        }
}

//����
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
