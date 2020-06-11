# hitsz-arch-lab
HITSZ 计算机体系结构 课程实验

课程文档：https://hitsz-lab.gitee.io/computer-arch/

### 运行方式

1、运行util/logisim.exe，也可在Linux/MacOS上自行运行同等版本logisim(2.15.0.2)。

2、在logisim电路仿真软件File菜单栏中，点击“open”，选择相应目录(lab1/lab2)下的“SoC.circ”。类似软件函数封装与调用，在电路中，也可以对功能较为复杂的电路封装成多个模块的子电路/部件。在本次实验中，SoC电路中的层次关系如下：

```
- SoC
    |- BusModel : I/O总线模块  
    |- CPU：单周期RISC-V CPU  
        |-- AluModel: ALU模块
        |-- Branch：分支跳转模块
        |-- ControlModel：控制模块
        |-- ImmModel：立即数扩展模块
        |-- RegFile：寄存器堆
    |- Three-stage CPU：三级流水RISC-V CPU 
    |- DMEM：数据存储器
    	|-- DMEMCache-直接相联
    	|-- DMEMCache-全相联
    |- IMEM：指令存储器
    |- KeyTTYDriver：键盘和TTY终端驱动模块
    |- LedDriver：7段数码管驱动模块
```

### Resources

[Green Sheet](http://inst.eecs.berkeley.edu/~cs61c/fa18/img/riscvcard.pdf)

[ISA Manual](https://content.riscv.org/wp-content/uploads/2017/05/riscv-spec-v2.2.pdf)

[What do the different wire colors mean](http://www.cburch.com/logisim/docs/2.7/en/html/guide/bundles/colors.html)

[cs61c.org](cs61c.org)