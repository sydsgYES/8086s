#include "Instruction.h"

Instruction::Instruction()
{

}


/** mov指令 **/
//mov 立即数寻址（将直接寻址与立即数寻址合并，通过bool isAddressing区分）
void Instruction::mov(CPUs *cp, MicroCom::Regs target, int value){
    cp->setRegValue(target,value);
    return;
}

//
void Instruction::mov(CPUs *cp, MemoryUnit m, int imValue){
    int phyAddr = getPhyAddr(cp, m);
    cp->writeBusCycle(phyAddr,imValue);
    return;
}


//mov 寄存器→寄存器
void Instruction::mov(CPUs *cp, MicroCom::Regs target, MicroCom::Regs source){
    int value = cp->getRegValue(source);
    cp->setRegValue(target,value);
    return;
}


void Instruction::mov(CPUs *cp, MicroCom::Dir dir, MicroCom::Regs reg, MemoryUnit m){
    int phyAddr = getPhyAddr(cp, m);
    int value = 0;
    if(dir == MicroCom::In){
        value = cp->readBusCycle(phyAddr);
        cp->setRegValue(reg,value);
    }
    else{
        value = cp->getRegValue(reg);
        cp->writeBusCycle(phyAddr,value);
    }
    return;
}


/** 出入栈指令 **/
//push
//源操作数为寄存器
void Instruction::push(CPUs *cp, MicroCom::Regs source){
    int value = cp->getRegValue(source);
    int newSpValue = cp->getRegValue(MicroCom::sp)-2;
    int stackAddr = cp->getRegValue(MicroCom::ss)*16 + newSpValue;
    cp->writeBusCycle(stackAddr,value);
    cp->setRegValue(MicroCom::sp,newSpValue);
    return;
}

//源操作数为存储单元
void Instruction::push(CPUs *cp, MemoryUnit m){
    int phyAddr = getPhyAddr(cp, m);
    int value = cp->readBusCycle(phyAddr);
    int newSpValue = cp->getRegValue(MicroCom::sp)-2;
    int stackAddr = cp->getRegValue(MicroCom::ss)*16 + newSpValue;
    cp->setRegValue(MicroCom::sp,newSpValue);
    cp->writeBusCycle(stackAddr,value);
}

//目的操作数为寄存器
void Instruction::pop(CPUs *cp, MicroCom::Regs target){
    int stackAddr = getPhyAddr(cp, MemoryUnit(MicroCom::sp));
    int value = cp->readBusCycle(stackAddr);
    cp->setRegValue(target,value);
    cp->setRegValue(MicroCom::sp,cp->getRegValue(MicroCom::sp)+2);
    return;
}

//目的操作数为存储单元
void Instruction::pop(CPUs *cp, MemoryUnit m){
    int stackAddr = getPhyAddr(cp, MemoryUnit(MicroCom::sp));
    int value = cp->readBusCycle(stackAddr);
    int tPhyAddr = getPhyAddr(cp, m);
    cp->writeBusCycle(tPhyAddr,value);
    cp->setRegValue(MicroCom::sp,cp->getRegValue(MicroCom::sp)-2);
    return;
}

/** 获取物理地址函数 **/
int Instruction::getPhyAddr(CPUs *cp, MemoryUnit m){
    if(m.based==MicroCom::no){
        return m.addr_offset;
    }
    int phyAddr = 0;
    int segAddr = 0;
    if(m.prefixed != MicroCom::no){
        segAddr = cp->getRegValue(m.prefixed);
    }
    else{
        if(m.based == MicroCom::bp || m.based == MicroCom::sp){
            segAddr = cp->getRegValue(MicroCom::ss);
        }
        else{
            segAddr = cp->getRegValue(MicroCom::ds);
        }
    }
    phyAddr = segAddr*16 + cp->getRegValue(m.based) + m.addr_offset;
    if(m.indexed != MicroCom::no){
        phyAddr += cp->getRegValue(m.indexed);
    }
    return phyAddr;
}


void Instruction::add(CPUs *cp, MicroCom::Dir dir, MicroCom::Regs reg, MemoryUnit m, bool isCarry){
    int phyAddr = getPhyAddr(cp, m);
    int mryValue = cp->readBusCycle(phyAddr);
    int regValue = cp->getRegValue(reg);
    if(isCarry){
        regValue += cp->getRegValue(MicroCom::flags,0);
    }
    if(dir == MicroCom::In){
        cp->setRegValue(reg,regValue+mryValue);
    }
    else{
        cp->writeBusCycle(phyAddr,regValue+mryValue);
    }
    return;
}


/****************************************************
 - Function: read the peripherals（外设）'s data
 - Description: 对标志位无影响
 - Input: address of the peripheral
 - Output: null
*****************************************************/
void Instruction::in(CPUs *cp, MicroCom::Regs reg, int addr){
    int inPortAddr = 0;
    if(addr==-1){   //间接寻址
        inPortAddr = cp->getRegValue(MicroCom::dx);
    }
    else{           //直接寻址
        inPortAddr = addr;
    }
    cp->setRegValue(reg,cp->readBusCycle(inPortAddr,false));
}



/****************************************************
 - Function：write the AX's data to peripherals（外设）
 - Description：对标志位无影响
 - Input：address of the peripheral
 - Output：null
*****************************************************/
void Instruction::out(CPUs *cp, MicroCom::Regs reg, int addr){
    int outPortAddr = 0;
    int value = cp->getRegValue(reg);
    if(addr==-1){   //间接寻址
        outPortAddr = cp->getRegValue(MicroCom::dx);
    }
    else{           //直接寻址
        outPortAddr = addr;
    }
    cp->writeBusCycle(outPortAddr,value,false);
}


/** 标志位传送指令(Flag Transfers) **/
/****************************************************
 - Function：Push Flag on Stack
 - Description：对标志位无影响
 - Input：无
 - Output：SP←(SP-2)
*****************************************************/
void Instruction::pushf(CPUs *cp){
    int flagsValue = cp->getRegValue(MicroCom::flags);
    int stackAddr = getPhyAddr(cp, {MicroCom::sp})-2;
    int spNewValue = cp->getRegValue(MicroCom::sp)-2;
    cp->setRegValue(MicroCom::sp,spNewValue);
    cp->writeBusCycle(stackAddr,flagsValue);
    return;
}

/****************************************************
 - Function：Pop Flag off Stack
 - Description：对标志位无影响
 - Input：无
 - Output：SP←(SP+2)
*****************************************************/
void Instruction::popf(CPUs *cp){
    int stackAddr = this->getPhyAddr(cp, {MicroCom::sp});
    int spNewValue = cp->getRegValue(MicroCom::sp)+2;
    int flagsValue = cp->readBusCycle(stackAddr);
    cp->setRegValue(MicroCom::sp,spNewValue);
    cp->setRegValue(MicroCom::flags,flagsValue);
    return;
}
