#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "CPUs.h"


class Instruction
{
public:
    Instruction();
    /** =================== 数据传送指令 ================== **/

    /** mov指令 **/
    //immediate value to Register
    void mov(CPUs *cp, MicroCom::Regs target, int imValue);
    //immediate value to Memory
    void mov(CPUs *cp, MemoryUnit m, int imValue);
    //Register to Register
    void mov(CPUs *cp, MicroCom::Regs target, MicroCom::Regs source);
    //Memory to Register & Register to Memory
    void mov(CPUs *cp, MicroCom::Dir, MicroCom::Regs reg, MemoryUnit m);

    /** in/out指令 **/
    void in(CPUs *cp, MicroCom::Regs reg, int addr=-1);
    void out(CPUs *cp, MicroCom::Regs reg, int addr=-1);

    /** 出入栈指令 **/
    //push Register on stack
    void push(CPUs *cp, MicroCom::Regs source);
    //push Memory on stack
    void push(CPUs *cp, MemoryUnit m);
    //pop stack to Register
    void pop(CPUs *cp, MicroCom::Regs target);
    //pop stack to Memory
    void pop(CPUs *cp, MemoryUnit m);

    /** XCHG交换指令 **/
    //Register to Memory & Memory to Register
    void xChg(CPUs *cp, MicroCom::Dir, MicroCom::Regs reg, MemoryUnit m);
    //Register to Register
    void xChg(CPUs *cp, MicroCom::Regs target, MicroCom::Regs source);

    /** 地址目标传送指令(Address Object Transfers) **/
    /* 该类指令要求源操作数只能是存储单元,且目的操作数只能是寄存器 */
    // LEA取地址有效指令(Load Effective Address)
    void lea(CPUs *cp, MicroCom::Regs target, MemoryUnit m);
    // LDS将双字指针送到寄存器和DS指令(Load Pointer Using DS)
    void lds(CPUs *cp, MicroCom::Regs target, MemoryUnit m);
    // LES将双字指针送到寄存器和ES指令(Load Pointer Using ES)
    void les(CPUs *cp, MicroCom::Regs target, MemoryUnit m);

    /** 标志位传送指令(Flag Transfers) **/
    //PUSHF标志入栈指令(Push Flag onto Stack)
    void pushf(CPUs *cp);
    //POPF标志出栈指令(Pop Flag off Stack)
    void popf(CPUs *cp);

    /** =================== 算术运算指令 ================== **/

    /** 加法指令(Addition) **/
    // ADD加法指令(Addition) & ADC带进位的加法指令
    //immediate value to Register
    void add(CPUs *cp, MicroCom::Regs target, int imValue);
    //immediate value to Memory
    void add(CPUs *cp, int imValue, MemoryUnit m, bool isCarry);
    //Register to Register
    void add(CPUs *cp, MicroCom::Regs target, MicroCom::Regs source);
    //Memory to Register & Register to Memory
    void add(CPUs *cp, MicroCom::Dir, MicroCom::Regs reg, MemoryUnit m, bool isCarry=false);

    /** 寻址指令 **/
    int getPhyAddr(CPUs *cp, MemoryUnit m);
};

#endif // INSTRUCTION_H
