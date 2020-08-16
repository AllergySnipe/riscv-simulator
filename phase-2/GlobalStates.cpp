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
    bitset<10000000> mem;
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

    Mux MuxPC, MuxINC, MuxY, MuxB;
    RegisterFile regFile;
    bool deMuxPMI;
    PC myPC, pcTemp;
    Register RA, RB, RZ, RY, RM;
    bitset<32> nextMA = 0;
    ALU myALU;
    InsIndex myIndices;
    PMI myPmi;
    InsRecog recog;
    bitset<32>division(bitset<32>i){
        bitset<32>j=myPmi.bitset_to_int(i)/8;
        return j;
    }
    bitset<32>Mul(bitset<32>m){
        bitset<32>n=myPmi.bitset_to_int(m)*8;
        return n;
    }
    void setIR(bitset<32> in)
    {
        this->IR = in;
    }
    void Fetch(bitset<32> _nextMA, bitset<10000000> &mem1)
    {
        //IAG
        IR = myPmi.dMEM_access(mem1);
        bitset<32> MuxPC_result = MuxPC.select(RA.get(), _nextMA, 1); //mux control
        myPC.set(MuxPC_result);
        bitset<32> MuxINC_result = MuxINC.select(32, SXT_imm <<= 1, 0); //imm dekhna hai // mux ka input configure karo
        nextMA = addBits(myPC.get(), MuxINC_result);
        pcTemp.set(myPC.get());
        /////////
        myPmi.set(RZ.get(), nextMA, 1); // set muxMA control
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
    }

    void ALU()
    {
        //actually decode ka code
        //regFile.writeData(rs1,bitset<32>(string("10")),1);
        //regFile.writeData(rs2,bitset<32>(string("100")),1);
        int temp;
        RA.set(regFile.readData(rs1));
        RB.set(regFile.readData(rs2));
        RM.set(RB.get());
        ///////
        bitset<32> MuxB_result = MuxB.select(RB.get(), SXT_imm, recog.cMuxB);      //mux control//imm
        RZ.set(myALU.operate(RA.get(), MuxB_result, myIndices.get(), pcTemp.get())); ///ALU_op config karo
        recog.setCon(myALU.getCon());

        if(myIndices.get()==19){
            bitset<32> MuxPC_result = MuxPC.select(RA.get(), nextMA, 0); //mux control
            myPC.set(MuxPC_result);
            bitset<32> MuxINC_result = MuxINC.select(32, SXT_imm, 1); //imm dekhna hai // mux ka input configure karo
            nextMA = addBits(myPC.get(), MuxINC_result);
            myPmi.set(RZ.get(), nextMA, 1); // set muxMA control
        }
        else if(myIndices.get()==30||myALU.getCon()==1){
            bitset<32> MuxPC_result = MuxPC.select(RA.get(),division(pcTemp.get()), 1); //mux control
            myPC.set(MuxPC_result);
            bitset<32> MuxINC_result = MuxINC.select(32, SXT_imm <<=1, 1); //imm dekhna hai // mux ka input configure karo
            nextMA = Mul(addBits(myPC.get(), MuxINC_result));
            myPmi.set(RZ.get(), nextMA, 1); // set muxMA control
        } 
    }

    void MemAccess()
    {
        myPmi.set(RZ.get(), nextMA, recog.cMuxMA);
        bitset<32> MuxY_result = MuxY.select(RZ.get(), myPmi.getData(RM.get(), myIndices.get()), pcTemp.get(), recog.cMuxY);
        RY.set(MuxY_result);
        myPmi.set(RZ.get(), nextMA, 1);
    
    }

    void RegUpdate()
    {
        regFile.writeData(rd, RY.get(), recog.RF_write);
    }
};
/*
int main()
{
    cout << "shuru kaaro" <<endl;
    GlobalStates gs;
    cout << "gs created" <<endl;
    gs.setIR(bitset<32>(string("00000000001100000000000110010011")));
   gs.Decode();
   gs.ALU();
   gs.MemAccess();
   gs.RegUpdate();
    cout << "main all exec" << endl;
   cout << gs.regFile.readData(gs.rd)<< endl;
    //cout << gs.
}
*/
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
GlobalStates gs;
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
bitset<32> get_ins(bitset<10000000> &mem1, int index)
{
    bitset<32> temp;
    for (int i = 0; i < 32; i++)
    {
        temp[i] = mem1[index + i];
    }
    return (temp);
}
int main()
{
    ifstream input;
    ofstream output;
    input.open("output.mc");
    bitset<10000000> mem1;
    long long int k = 0;
    string curr;
    vector<string> pcs, inss;
    while (getline(input, curr))
    {
        string temp = "";
        int i = 0;
        while (curr[i] != ' ')
        {
            temp = temp + curr[i];
            i++;
        }
        string pc = HexToBin(temp);
        while (curr[i] == ' ')
        {
            i++;
        }
        temp = "";
        while (curr[i] != ' ' && curr[i] != '\n' && curr[i] != '\0')
        {
            temp = temp + curr[i];
            i++;
        }
        string ins;
        ins = HexToBin(temp);
        int x = 0;
        while (x < 32)
        {
            int b = int(ins[31 - x] - '0');
            mem1[k + x] = b;
            x++;
        }
        k = k + 32;
    }

    while (1)
    {
        cout<<endl;
        gs.Fetch(gs.nextMA, mem1);
        cout << "FETCH: Fetch instruction" <<endl; 
        if (gs.IR == 0)
            break;    
        gs.Decode();
        cout << "DECODE: operation is "<< gs.recog.get_con()  << ", first operand is r" <<(gs.rs1).to_ulong()<<", second operand is r" <<(gs.rs2).to_ulong()<<" and destination is r"<<(gs.rd).to_ulong()<<endl;
        cout << "DECODE: read rs1="<<gs.myPmi.bitset_to_int(gs.regFile.readData(gs.rs1))<<" , rs2="<<gs.myPmi.bitset_to_int(gs.regFile.readData(gs.rs2))<<endl;
        
        gs.ALU();
        cout<< "ALU: execute "<< gs.recog.get_con() <<" "<<gs.myPmi.bitset_to_int(gs.regFile.readData(gs.rs1))<< " , "<<gs.myPmi.bitset_to_int(gs.regFile.readData(gs.rs2))<<endl;
        gs.MemAccess();
        if(gs.myPmi.flag==1)   { cout << "NO MEMORY ACCESS" <<endl;}
        else {cout<< "MEM ACCESS: get data from address "<< gs.RZ.get()<< " with offset " << gs.SXT_imm<<endl;}
        
        gs.RegUpdate();
        if(gs.recog.RF_write==1){
        cout << "WRITEBACK: write "<< gs.myPmi.bitset_to_int(gs.regFile.readData(gs.rd))<< "to r"<<(gs.rd).to_ulong()<<endl;}
        else{
            cout<<"NO WRITEBACK TO REGISTER FILE"<<endl;
        }
    }
    //cout << gs.regFile.readData(gs.rd);
}