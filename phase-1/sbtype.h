#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include<bitset>
using namespace std;
class sbtype {
public:
    string opcode = "", rs1 = "", rs2 = "",  func3 = "";
    map<string, string> SBTypeOPcode;
    map<string, string> SBTypeFunc3;
    map<string, int> labToimm;

    sbtype() {
        SBTypeOPcode.insert({ "beq","1100011" });
        SBTypeOPcode.insert({ "blt","1100011" });
        SBTypeOPcode.insert({ "bne","1100011" });
        SBTypeOPcode.insert({ "bge","1100011" });

        SBTypeFunc3.insert({ "beq","000" });
        SBTypeFunc3.insert({ "bne","001" });
        SBTypeFunc3.insert({ "blt","100" });
        SBTypeFunc3.insert({ "bge","101" });

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

    string mc(string s,int pc) {

        int i = 0; string imme=""; 
        string temp = "";
        while (s[i] != ' ') {
            temp = temp + s[i];
            i++;

        }

        this->opcode = SBTypeOPcode[temp];
        this->func3 = SBTypeFunc3[temp];

        i++;
        temp = "";
        while (s[i] != ',') {
            temp = temp + s[i];
            i++;
        }
        rs1 = strToBinary(temp, 5);
        i++;
        temp = "";
        while (s[i] == ' ') {
            i++;
        }

        while (s[i] != ',') {
            temp = temp + s[i];
            i++;
        }

        i++;
        rs2 = strToBinary(temp, 5);
        while (s[i] == ' ') {
            i++;
        }
        temp = "";

        while (i < s.size()) {
            temp = temp + s[i];
            i++;
        }

        int why = 4*(labToimm[temp] - pc);
        bitset<13>value(why);
        //imme = strToBinary(labToimm[temp],21);
        int u = 0;
        while (u < 13) {
            bitset<1> temp;
            temp[0]=value[12-u];
            imme = imme + temp.to_string();
            u++;
        }
        string imme1 = "";
        imme1 = imme1 + imme[0] + imme[2] + imme[3] + imme[4] + imme[5] + imme[6] + imme[7];

        string imme2 = "";
        imme2 = imme2 + imme[8] + imme[9] + imme[10] + imme[11] + imme[1];
        string ans = imme1 + rs2 + rs1 + func3 + imme2 + opcode;
        return ans;

    }
};

