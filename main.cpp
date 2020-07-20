#include <iostream>
#include <cstring>
#include "theriscv.h"
#include "pipelineIF.h"
#include "pipelineID.h"
#include "pipelineEX.h"
#include "pipelineMEM.h"
#include "pipelineWB.h"

using namespace std;

int main()
{
    //����һ��RISCV
    theriscv RISCV;

    //��ȡ����
    RISCV.Input();

    //����
    RISCV.run();

    return 0;
}
