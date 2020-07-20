#include <iostream>
#include <cstring>
#include "theriscv.hpp"
#include "pipelineIF.hpp"
#include "pipelineID.hpp"
#include "pipelineEX.hpp"
#include "pipelineMEM.hpp"
#include "pipelineWB.hpp"

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
