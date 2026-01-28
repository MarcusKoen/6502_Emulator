#include <iostream>
#include <stdlib.h>
//test
using namespace std;


using Byte = unsigned char;//8 bits
using Word = unsigned short;//16 bits

using u32 = unsigned int;

struct Mem
{
    static constexpr u32 MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];

    void Initialize()
    {

        for(u32 i = 0;i < MAX_MEM;i++)
        {
            Data[i] = 0;
        }
    }


    Byte operator[](u32 Address) const
    {

        return Data[Address];
    }

    //Write 1 byte
    Byte& operator[](u32 Address)
    {

        return Data[Address];
    }

};

struct CPU
{


    Word PC; //Program Counter

    Word SP; //Stack Pointer

    Byte A,X,Y; //Registers

    Byte C : 1;//bitfield carry
    Byte Z : 1;//Zero
    Byte I : 1;//
    Byte D : 1;//Decimal
    Byte B : 1;
    Byte V : 1;//Overflow
    Byte N : 1;//Negative


    void Reset(Mem& memory)
    {

        PC = 0xFFFC;//Set Program Counter
        SP = 0x0100;//Set Stack Pointer
        C = Z = I = B = V = N = D = 0;//Clear flags
        A = X = Y = 0;//Initialize registers
        memory.Initialize();


    }


    Byte FetchByte(u32& Cycles, Mem& memory)
    {

        Byte Data = memory[PC];
        PC++;
        Cycles--;

        return Data;
    }

    static constexpr Byte
        INS_LDA_IM = 0xA9;

    void Execute(u32 Cycles, Mem& memory)
    {
       while (Cycles > 0)
       {

           Byte Ins = FetchByte(Cycles, memory);

           switch(Ins)
           {

           case INS_LDA_IM:
            {

                Byte Value = FetchByte(Cycles, memory);
                A = Value;
                Z = (A == 0);
                N = (A & 0b10000000)>0;
            }break;
           default :
            {
                printf("Instruction not handled %d \n",Ins);
            }break;
           }
       }

    }
};


int main()
{

    Mem mem;
    CPU cpu;
    cpu.Reset(mem);
    mem[0xFFFC] = CPU::INS_LDA_IM;
    mem[0xFFFD] = 0x42;


    cpu.Execute(2, mem);//clock tick



    return 0;
}
