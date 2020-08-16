#include<bits/stdc++.h>
#include<string>
#include<map>
#include<iostream>
#include<sstream>
#include<bitset>

using namespace std;

class itype
{
public:
    //string s="jalr x31 , x24, 45";
    string func3 = "";
    string opcode = "";
    string r1 = "", imm = "", rd = "";
    map<string, string> iTypeOPCode;
    map<string, string> iTypeFunc3;
    itype() {
        iTypeOPCode.insert(pair<string, string>("addi", "0010011"));
        iTypeOPCode.insert(pair<string, string>("ori", "0010011"));
        iTypeOPCode.insert(pair<string, string>("andi", "0010011"));
        iTypeOPCode.insert(pair<string, string>("lb", "0000011"));
        iTypeOPCode.insert(pair<string, string>("lh", "0000011"));
        iTypeOPCode.insert(pair<string, string>("lw", "0000011"));
        iTypeOPCode.insert(pair<string, string>("ld", "0000011"));
        iTypeOPCode.insert(pair<string, string>("jalr", "1100111"));

        iTypeFunc3.insert(pair<string, string>("addi", "000"));
        iTypeFunc3.insert(pair<string, string>("ori", "110"));
        iTypeFunc3.insert(pair<string, string>("andi", "111"));
        iTypeFunc3.insert(pair<string, string>("lb", "000"));
        iTypeFunc3.insert(pair<string, string>("lh", "001"));
        iTypeFunc3.insert(pair<string, string>("lw", "010"));
        iTypeFunc3.insert(pair<string, string>("ld", "011"));
        iTypeFunc3.insert(pair<string, string>("jalr", "000"));
    }

    string strToBinary(string s, int p)
    {
        int n = s.length();
        int val = 0;
        int pow = 1;
        int min;
        if (s[0] == 'x') { min = 1; }
        else { min = 0; }
        for (int i = n - 1; i >= min; i--)
        {
            val = val + (s[i] - '0') * pow;
            pow = pow * 10;
        }
        int binaryNum[32];
        int i = 0;
        while (i < p) {
            binaryNum[i] = val % 2;
            val = val / 2;
            i++;
        }
        char out[p + 1];
        for (int j = 0; j < p; j++)
        {
            out[j] = binaryNum[p - 1 - j] + '0';
        }
        out[p] = '\0';
        return out;
    }

    string mc(string s/*, map<string,string>variables*/) {
        string temp = "";
        int i = 0;
        imm = "";
        while (s[i] != ' ')
        {
            temp = temp + s[i];
            i++;
        }
        //cout<<temp<<"."<<endl;
        opcode = iTypeOPCode[temp];
        func3 = iTypeFunc3[temp];
        while (s[i] == ' ')
        {
            i++;
        }
        temp = s[i];
        //cout<<temp<<"."<<endl;
        i++;
        while (s[i] != ',' && s[i] != ' ')
        {
            temp = temp + s[i];
            i++;
        }
        //cout<<temp<<"."<<endl;
        rd = strToBinary(temp, 5);
        if (opcode == "0000011") {
            while (s[i] == ' ' || s[i] == ',') {
                i++;
            }
            temp = "";
            //if (s[i] == '0' || s[i] == '1' || s[i] == '2' || s[i] == '3' || s[i] == '4' || s[i] == '5' || s[i] == '6' || s[i] == '7' || s[i] == '8' || s[i] == '9') {
            while (s[i] != '(') {
                temp = temp + s[i];
                i++;
            }
            imm = strToBinary(temp, 12);
            i++;
            while (s[i] == ' ') {
                i++;
            }
            temp = "";
            while (s[i] != ')' && s[i] != ' ') {
                temp = temp + s[i];
                i++;
            }
            r1 = strToBinary(temp, 5);
        }
        else {
            while (s[i] == ' ' || s[i] == ',')
            {
                i++;
            }
            temp = s[i];
            i++;
            while (s[i] != ',' && s[i] != ' ')
            {
                temp = temp + s[i];
                i++;
            }
            //cout<<temp<<"."<<endl;
            r1 = strToBinary(temp, 5);
            while (s[i] == ' ' || s[i] == ',')
            {
                i++;
            }
            temp = s[i];
            i++;
            while (i < s.size())
            {
                temp = temp + s[i];
                i++;
            }

            //cout<<temp<<"."<<endl;
            stringstream wow(temp);
            int wow1 = 0;
            wow >> wow1;
            bitset<12> value(wow1);
            int u = 0;
            while (u < 12) {
                bitset<1>temp;
                temp[0] = value[11 - u];
                imm = imm + temp.to_string();
                u++;
            }

            //imm = strToBinary(temp, 12);
        }
        string ans = imm + r1 + func3 + rd + opcode;
        return ans;
    }
};

