#include <bits/stdc++.h>
#include "InsIndex.h"
#include "setControl.h"
#include "alu.h"
using namespace std;

bitset<32> addBits(bitset<32> a, bitset<32> b)
{
    int32_t d = (int32_t)a.to_ulong() + (int32_t)b.to_ulong();
    bitset<32> c(d);
    return c;
} //just a function for bitset addition
class ins_mem
{
public:
    bitset<100000> mem;
};

class RegisterFile
{
private:
    bitset<32> reg[32];
    bool write = false;

public:
    bitset<32> readData(bitset<5> regNum)
    {
        return reg[(int)(regNum.to_ulong())];
    } //reading from A and B
    void writeData(bitset<5> regNum, bitset<32> regData, bool write)
    {
        if (write == 1)
        {
            reg[(int)(regNum.to_ulong())] = regData;
            write = true;
        }
        return;
    } //write into C
};

class PC
{
    bitset<32> currAdd = 0;

public:
    void set(bitset<32> add)
    {
        currAdd = add;
        return;
    }
    bitset<32> get()
    {
        return currAdd;
    }
};
class Register
{
    bitset<32> value;

public:
    void set(bitset<32> v)
    {
        value = v;
    }
    bitset<32> get()
    {
        return value;
    }
};
class Mux
{
    int state;

public:
    bitset<32> select(bitset<32> A, bitset<32> B, int control)
    {
        if (control == 0)
        {
            this->state = control;
            return A;
        }
        else if (control == 1)
        {
            this->state = control;
            return B;
        }
    }
    bitset<32> select(bitset<32> A, bitset<32> B, bitset<32> C, int control)
    {
        if (control == 0)
        {
            this->state = control;
            return A;
        }
        else if (control == 1)
        {
            this->state = control;
            return B;
        }
        else if (control == 2)
        {
            this->state = control;
            return C;
        }
    }
};

#include "PMI.h"
class b1 //64//fetch-decode
{
public:
    bitset<32> IR;
    bitset<32> pcTemp;
};
class b2 //155//decode-alu
{
public:
    bitset<5> rs1;
    bitset<5> rs2;
    bitset<5> rd;
    bitset<32> imm;
    bitset<32> r1;
    bitset<32> r2;
    bitset<32> pcTemp;
    //control: 12 bits
    bool RF_write;
    bool cMuxB;
    bool cMuxY;
    bool cMuxMA;
    bool deMuxPMI;
    bool cMuxPC;
    bool cMuxINC;
    bitset<5> ALU_op;
};
class b3 //113//alu-mem
{
public:
    bitset<32> alu_out;
    bitset<32> alu_in;
    bitset<5> rd;
    bitset<32> pcTemp;
    //control:
    bool RF_write;
    bool cMuxB;
    bool cMuxY;
    bool cMuxMA;
    bool deMuxPMI;
    bool cMuxPC;
    bool cMuxINC;
    bitset<5> ALU_op;
};
class b4 //38//mem-reg
{
public:
    bitset<32> output;
    bitset<5> rd;
    //control:
    bool RF_write;
    bool cMuxB;
    bool cMuxY;
    bool cMuxMA;
    bool deMuxPMI;
    bool cMuxPC;
    bool cMuxINC;
    bitset<5> ALU_op;
};
RegisterFile regFile;
class GlobalStates
{
public:
    bitset<5> rs1;
    bitset<5> rs2;
    bitset<5> rd;
    bitset<3> func3;
    bitset<7> func7;
    bitset<7> opcode;
    bitset<12> imm1;
    bitset<20> imm2;
    bitset<32> SXT_imm;
    bitset<32> IR;
    bitset<32> MuxY_MD;
    int insType;
    int currInsIndex;
    int stage = -1;
    bool writeDone = 0, samedes = 0;
    b1 B1;
    b2 B2;
    b3 B3;
    b4 B4;
    bitset<32> tempMAR, manualForward1,manualForward2;
    Mux MuxPC, MuxINC, MuxY, MuxB;
    bool deMuxPMI;
    PC myPC, pcTemp;
    Register RA, RB, RZ, RY, RM;
    bitset<32> nextMA = 0;
    ALU myALU;
    InsIndex myIndices;
    PMI myPmi;
    InsRecog recog;
    bitset<32> division(bitset<32> i)
    {
        bitset<32> j = myPmi.bitset_to_int(i) / 8;
        return j;
    }
    bitset<32> Mul(bitset<32> m)
    {
        bitset<32> n = myPmi.bitset_to_int(m) * 8;
        return n;
    }
    void setIR(bitset<32> in)
    {
        this->IR = in;
    }
    void Fetch(bitset<32> _nextMA, bitset<100000> &mem1)
    {
        tempMAR = myPmi.MAR; //
        IR = myPmi.dMEM_access(mem1);
        bitset<32> MuxPC_result = MuxPC.select(RA.get(), _nextMA, 1);
        myPC.set(MuxPC_result);
        bitset<32> MuxINC_result = MuxINC.select(32 * 5, SXT_imm, 0);
        nextMA = addBits(myPC.get(), MuxINC_result);
        pcTemp.set(myPC.get());
        /////////
        myPmi.set(RZ.get(), nextMA, 1);
        stage = 0;
        writeDone = 0;
        //pipeline reg
        B1.IR = IR;
        B1.pcTemp = pcTemp.get();
    }

    bitset<32> SXT12(bitset<12> str)
    {
        bitset<32> final;
        for (int i = 0; i < 12; i++)
        {
            final[i] = str[i];
        }
        if (str[11])
        {
            for (int i = 31; i >= 12; i--)
            {
                final[i] = 1;
            }
        }
        else
        {
            for (int i = 31; i >= 12; i--)
            {
                final[i] = 0;
            }
        }
        return final;
    }

    bitset<32> SXT20(bitset<20> str)
    {
        bitset<32> final;
        for (int i = 0; i < 20; i++)
        {
            final[i] = str[i];
        }
        if (str[19])
        {
            for (int i = 31; i >= 20; i--)
            {
                final[i] = 1;
            }
        }
        else
        {
            for (int i = 31; i >= 20; i--)
            {
                final[i] = 0;
            }
        }

        return final;
    }

    void Decode()
    {

        IR = B1.IR;
        pcTemp.set(B1.pcTemp);

        bitset<32> ins = this->IR;
        if ((!ins[6]) & ins[5] & ins[4] & (!ins[3]) & (!ins[2]))
        {
            this->insType = 1;
        }
        else if ((!ins[6] & !ins[5] & ins[4] & !ins[3] & !ins[2]) || (ins[6] & ins[5] & !ins[4] & !ins[3] & ins[2]) || (!ins[6] & !ins[5] & !ins[4] & !ins[3] & !ins[2]))
        {
            this->insType = 2;
        }
        else if (ins[6] & ins[5] & !ins[4] & !ins[3] & !ins[2])
        {
            this->insType = 3;
        }
        else if (!ins[6] & ins[5] & !ins[4] & !ins[3] & !ins[2])
        {
            this->insType = 4;
        }
        else if ((ins[6]) & ins[5] & !ins[4] & ins[3] & ins[2])
        {
            this->insType = 5;
        }
        else if ((!ins[6] & ins[5] & ins[4] & !ins[3] & ins[2]) || (!ins[6] & !ins[5] & ins[4] & !ins[3] & ins[2]))
        {
            this->insType = 6;
        }
        else
        {
            this->insType = 0;
        }

        if (this->insType == 1)
        {
            // RType | opcode (7) | rd (5) | funct3 | rs1(5) | rs2 (5) | funct7 |
            for (int i = 0; i < 7; i++)
            {
                opcode[i] = IR[i];
            }
            for (int i = 0; i < 5; i++)
            {
                rd[i] = IR[7 + i];
            }
            for (int i = 0; i < 3; i++)
            {
                func3[i] = IR[12 + i];
            }
            for (int i = 0; i < 5; i++)
            {
                rs1[i] = IR[15 + i];
            }
            for (int i = 0; i < 5; i++)
            {
                rs2[i] = IR[20 + i];
            }
            for (int i = 0; i < 7; i++)
            {
                func7[i] = IR[25 + i];
            }
        }
        if (this->insType == 2)
        {
            // IType 0->31 | opcode (7) | rd (5) | funct3 | rs1(5) | immediate(12) |
            for (int i = 0; i < 7; i++)
            {
                opcode[i] = IR[i];
            }
            for (int i = 0; i < 5; i++)
            {
                rd[i] = IR[7 + i];
            }
            for (int i = 0; i < 3; i++)
            {
                func3[i] = IR[12 + i];
            }
            for (int i = 0; i < 5; i++)
            {
                rs1[i] = IR[15 + i];
            }
            for (int i = 0; i < 12; i++)
            {
                imm1[i] = IR[20 + i];
            }
            SXT_imm = SXT12(imm1);
        }
        if (this->insType == 3)
        {
            // SBType imm[12] | imm [10:5] | rs2 | rs1 | funct3 | imm[4:1] | imm[11] | opcode
            for (int i = 0; i < 7; i++)
            {
                opcode[i] = IR[i];
            }
            imm1[10] = IR[7];

            for (int i = 0; i < 4; i++)
            {
                imm1[i] = IR[8 + i];
            }
            for (int i = 0; i < 3; i++)
            {
                func3[i] = IR[12 + i];
            }
            for (int i = 0; i < 5; i++)
            {
                rs1[i] = IR[15 + i];
            }
            for (int i = 0; i < 5; i++)
            {
                rs2[i] = IR[20 + i];
            }
            for (int i = 0; i < 6; i++)
            {
                imm1[i + 4] = IR[25 + i];
            }
            imm1[11] = IR[31];
            SXT_imm = SXT12(imm1);
        }
        if (this->insType == 4)
        {
            // SType immediate (7) | rs2 (5) | rs1 (5) | func3 | immediate (5) | opcode (7) |
            // rs1 replaced by rd to symbolize reading on that register, rs2 replaced by rs1 to leave room for writing
            for (int i = 0; i < 7; i++)
            {
                opcode[i] = IR[i];
            }
            for (int i = 0; i < 5; i++)
            {
                imm1[i] = IR[7 + i];
            }
            for (int i = 0; i < 3; i++)
            {
                func3[i] = IR[12 + i];
            }
            for (int i = 0; i < 5; i++)
            {
                rs1[i] = IR[15 + i]; // destination register is rs1  store rs1, 0(rs2)
            }
            for (int i = 0; i < 5; i++)
            {
                rs2[i] = IR[20 + i]; // source register is rs2 store rs1, 0(rs2)
            }
            for (int i = 0; i < 7; i++)
            {
                imm1[i + 5] = IR[25 + i];
            }
            SXT_imm = SXT12(imm1);
        }
        if (this->insType == 5)
        {
            // UJType imm[20][10:1][11][19:12] | rd[11:7] | opcode[6:0]
            for (int i = 0; i < 7; i++)
            {
                opcode[i] = IR[i];
            }
            for (int i = 0; i < 5; i++)
            {
                rd[i] = IR[7 + i];
            }
            for (int i = 0; i < 8; i++)
            {
                imm2[11 + i] = IR[12 + i];
            }
            imm2[10] = IR[20];
            for (int i = 0; i < 10; i++)
            {
                imm2[i] = IR[21 + i];
            }
            imm2[19] = IR[31];
            SXT_imm = SXT20(imm2);
        }
        if (this->insType == 6)
        {
            // UType imm[31:12] | rd[11:7] | opcode[6:0]
            for (int i = 0; i < 7; i++)
            {
                opcode[i] = IR[i];
            }
            for (int i = 0; i < 5; i++)
            {
                rd[i] = IR[i + 7];
            }
            for (int i = 0; i < 20; i++)
            {
                imm2[i] = IR[12 + i];
            }
            SXT_imm = SXT20(imm2);
        }

        recog.set(opcode, func3, func7);
        myIndices.set(recog.get_con());
        stage = 1;

        B2.rs1 = rs1;
        B2.rs2 = rs2;
        B2.rd = rd;
        B2.imm = SXT_imm;
        B2.r1 = regFile.readData(rs1);
        B2.r2 = regFile.readData(rs2);
        B2.pcTemp = pcTemp.get();
        B2.RF_write = recog.RF_write;
        B2.cMuxB = recog.cMuxB;
        B2.cMuxY = recog.cMuxY;
        B2.cMuxMA = recog.cMuxMA;
        B2.deMuxPMI = recog.deMuxPMI;
        B2.cMuxPC = recog.cMuxPC;
        B2.cMuxINC = recog.cMuxINC;
        B2.ALU_op = recog.ALU_op;
    }

    void ALU()
    {
        rs1 = B2.rs1;
        rs2 = B2.rs2;
        rd = B2.rd;
        SXT_imm = B2.imm;
        pcTemp.set(B2.pcTemp);
        recog.RF_write = B2.RF_write;
        recog.cMuxB = B2.cMuxB;
        recog.cMuxY = B2.cMuxY;
        recog.cMuxMA = B2.cMuxMA;
        recog.deMuxPMI = B2.deMuxPMI;
        recog.cMuxPC = B2.cMuxPC;
        recog.cMuxINC = B2.cMuxINC;
        recog.ALU_op = (int)B2.ALU_op.to_ulong();

        int temp;
        if (!samedes)
        {
            RA.set(regFile.readData(rs1));
            RB.set(regFile.readData(rs2));
        }
        else
        {
            RA.set(manualForward1);
            RB.set(manualForward2);
            samedes=0;
        }
        RM.set(RB.get());
        ///////
        bitset<32> MuxB_result = MuxB.select(RB.get(), SXT_imm, recog.cMuxB);
        RZ.set(myALU.operate(RA.get(), MuxB_result, recog.ALU_op, pcTemp.get()));
        recog.setCon(myALU.getCon());

        /*if(myIndices.get()==19){
            bitset<32> MuxPC_result = MuxPC.select(RA.get(), nextMA, 0); 
            myPC.set(MuxPC_result);
            bitset<32> MuxINC_result = MuxINC.select(32, SXT_imm, 1); 
            nextMA = addBits(myPC.get(), MuxINC_result);
            myPmi.set(RZ.get(), nextMA, 1);
        }
        else if(myIndices.get()==30||myALU.getCon()==1){
            bitset<32> MuxPC_result = MuxPC.select(RA.get(),division(pcTemp.get()), 1);
            myPC.set(MuxPC_result);
            bitset<32> MuxINC_result = MuxINC.select(32, SXT_imm <<=1, 1);
            nextMA = Mul(addBits(myPC.get(), MuxINC_result));
            myPmi.set(RZ.get(), nextMA, 1); 
        }*/
        stage = 2;

        B3.alu_out = RZ.get();
        B3.alu_in = RM.get();
        B3.rd = rd;
        B3.pcTemp = pcTemp.get();
        B3.RF_write = recog.RF_write;
        B3.cMuxB = recog.cMuxB;
        B3.cMuxY = recog.cMuxY;
        B3.cMuxMA = recog.cMuxMA;
        B3.deMuxPMI = recog.deMuxPMI;
        B3.cMuxPC = recog.cMuxPC;
        B3.cMuxINC = recog.cMuxINC;
        B3.ALU_op = recog.ALU_op;
    }

    void MemAccess()
    {
        RZ.set(B3.alu_out);
        RM.set(B3.alu_in);
        rd = B3.rd;
        pcTemp.set(B3.pcTemp);
        recog.RF_write = B3.RF_write;
        recog.cMuxB = B3.cMuxB;
        recog.cMuxY = B3.cMuxY;
        recog.cMuxMA = B3.cMuxMA;
        recog.deMuxPMI = B3.deMuxPMI;
        recog.cMuxPC = B3.cMuxPC;
        recog.cMuxINC = B3.cMuxINC;
        recog.ALU_op = (int)B3.ALU_op.to_ulong();

        myPmi.set(RZ.get(), nextMA, recog.cMuxMA);
        bitset<32> MuxY_result = MuxY.select(RZ.get(), myPmi.getData(RM.get(), recog.ALU_op), pcTemp.get(), recog.cMuxY);
        RY.set(MuxY_result);
        myPmi.set(RZ.get(), nextMA, 1);
        stage = 3;

        B4.output = RY.get();
        B4.rd = rd;
        B4.RF_write = recog.RF_write;
    }

    void RegUpdate()
    {
        RY.set(B4.output);
        rd = B4.rd;
        recog.RF_write = B4.RF_write;

        regFile.writeData(rd, RY.get(), recog.RF_write);
        stage = 4;
        writeDone = 1;
    }
    void next(bitset<32> _nextMA, bitset<100000> &mem1)
    {
        switch (stage)
        {
        case -1:
            Fetch(_nextMA, mem1);
            writeDone = 0;
            cout << "fetchdone for" << endl;
            break;
        case 0:
            Decode();
            cout << "decodedone for" << endl;
            break;
        case 1:
            ALU();
            cout << "aludone for" << endl;
            break;
        case 2:
            MemAccess();
            cout << "memdone for" << endl;
            break;
        case 3:
            RegUpdate();
            writeDone = 1;
            cout << "regupdatedone for" << endl;
            break;
        default:
            stage = -1;
            next(_nextMA, mem1);
            cout << "stagereset" << endl;
            break;
        }
    }
};
string HexToBin(string hexdec)
{
    long int i = 2;
    string ans = "";
    while (hexdec[i])
    {

        switch (hexdec[i])
        {
        case '0':
            ans = ans + "0000";
            break;
        case '1':
            ans = ans + "0001";
            break;
        case '2':
            ans = ans + "0010";
            break;
        case '3':
            ans = ans + "0011";
            break;
        case '4':
            ans = ans + "0100";
            break;
        case '5':
            ans = ans + "0101";
            break;
        case '6':
            ans = ans + "0110";
            break;
        case '7':
            ans = ans + "0111";
            break;
        case '8':
            ans = ans + "1000";
            break;
        case '9':
            ans = ans + "1001";
            break;
        case 'A':
        case 'a':
            ans = ans + "1010";
            break;
        case 'B':
        case 'b':
            ans = ans + "1011";
            break;
        case 'C':
        case 'c':
            ans = ans + "1100";
            break;
        case 'D':
        case 'd':
            ans = ans + "1101";
            break;
        case 'E':
        case 'e':
            ans = ans + "1110";
            break;
        case 'F':
        case 'f':
            ans = ans + "1111";
            break;
        default:
            cout << "\nInvalid hexadecimal digit "
                 << hexdec[i];
        }
        i++;
    }
    return ans;
}
bitset<32> reverse(bitset<32> b)
{
    for (int i = 0; i < 16; ++i)
    {
        bool t = b[i];
        b[i] = b[32 - i - 1];
        b[32 - i - 1] = t;
    }
    return b;
}
bitset<32> get_ins(bitset<100000> &mem1, int index)
{
    bitset<32> temp;
    for (int i = 0; i < 32; i++)
    {
        temp[i] = mem1[index + i];
    }
    return (temp);
}