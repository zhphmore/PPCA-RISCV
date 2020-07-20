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
    //构造一个RISCV
    theriscv RISCV;

    //获取输入
    RISCV.Input();

    //运行
    RISCV.run();

    return 0;
}
