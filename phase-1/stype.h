#include <iostream>
#include <iterator>
#include <map>
#include <string>
using namespace std;



class stype {
public:
    string opcode = "", rs1 = "", rs2 = "", func3 = "", imme = "";
    map<string, string> STypeOPCode;
    map<string, string> STypeFunc3;

    stype() {
        STypeOPCode.insert({ "sb", "0100011" });
        STypeOPCode.insert({ "sh", "0100011" });
        STypeOPCode.insert({ "sw", "0100011" });
        STypeOPCode.insert({ "sd", "0100011" });

        STypeFunc3.insert({ "sb", "000" });
        STypeFunc3.insert({ "sh", "001" });
        STypeFunc3.insert({ "sw", "010" });
        STypeFunc3.insert({ "sd", "011" });
    }

    string strToBinary(string s) {
        int n = s.length();
        int val = 0;
        int pow = 1;
        for (int i = n - 1; i > 0; i--)
        {
            val = val + (s[i] - '0') * pow;
            pow = pow * 10;
        }

        int binaryNum[5];
        int i = 0;
        while (i < 5) {
            binaryNum[i] = val % 2;
            val = val / 2;
            i++;
        }
        char out[6];
        for (int j = 0; j < 5; j++)
        {
            out[j] = binaryNum[4 - j] + '0';
        }
        out[5] = '\0';
        return out;
    }
    string decimalStrToBinaryStr(string s) {
        int n = s.length();
        int val = 0;
        int pow = 1;
        for (int i = n - 1; i >= 0; i--)
        {
            val = val + (s[i] - '0') * pow;
            pow = pow * 10;
        }

        int binaryNum[12];
        int i = 0;
        while (i < 12) {
            binaryNum[i] = val % 2;
            val = val / 2;
            i++;
        }
        char out[13];
        for (int j = 0; j < 12; j++)
        {
            out[j] = binaryNum[11 - j] + '0';
        }
        out[12] = '\0';
        return out;
    }

    string mc(string s) {
        int i = 0;
        string temp = "";
        while (s[i] != ' ') {
            temp = temp + s[i];
            i++;
        }
        this->opcode = STypeOPCode[temp];
        this->func3 = STypeFunc3[temp];

        i++;

        //while (s[i] == ' ') {
         //   i++;
           // cout<<i<<endl;
     //  }

        temp = "";
        while (s[i] != ',') {
            temp = temp + s[i];
            i++;
        }

        i++;

        rs2 = strToBinary(temp);


        temp = "";

        while (s[i] == ' ') {
            i++;
        }


        while (s[i] != '(') {
            temp = temp + s[i];
            i++;
        }

        imme = decimalStrToBinaryStr(temp);

        i++;

        temp = "";



        while (s[i] != ')' && s[i] != ' ') {

            temp = temp + s[i];
            i++;
        }


        rs1 = strToBinary(temp);
        char imme1[8], imme2[5];
        i = 0;

        while (i < 7) {
            imme1[i] = imme[i];
            i++;
        }
        int j = 0;
        while (j < 5) {
            imme2[j] = imme[j + 7];
            j++;
        }
        imme1[7]='\0';
        string ans;
        ans = imme1 + rs2 + rs1 + func3 + imme2[0] + imme2[1] + imme2[2] + imme2[3] + imme2[4];
        int x = 0;
        ans += opcode;
        return ans;
    }

};
