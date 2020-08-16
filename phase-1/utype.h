#include <iostream>
#include <iterator>
#include <map>
#include <string>
using namespace std;


class utype {
public:
    string rd = "", imme = "", opcode = "";
    map<string, string>UtypeOPCode;

    string strToBinary(string s)
    {
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

    string strToBinary2(string s, int p)
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

    utype() {
        UtypeOPCode.insert({ "lui","0110111" });
        UtypeOPCode.insert({ "auipc","0010111" });
    }

    string mc(string s) {
        int i = 0;
        string temp = "";
        while (s[i] != ' ') {
            temp = temp + s[i];
            i++;
        }
        this->opcode = UtypeOPCode[temp];

        while (s[i] == ' ') {
            i++;
        }
        temp = "";

        while (s[i] != ',') {
            temp = temp + s[i];
            i++;
        }

        rd = strToBinary(temp);
        temp = "";
        i++;

        while (s[i] == ' ') {
            i++;
        }

        while (s[i] != '\0' && s[i] != '\n' && s[i] != ' ') {
            temp = temp + s[i];
            i++;
        }
        if(temp[1]=='x' || temp[1]=='X')
        {
            stringstream ss;
            ss << hex << temp;
            unsigned n;
            ss >> n;
            bitset<32> b(n);
            imme=b.to_string();
        }
        else{imme = strToBinary2(temp, 32);}
        int x = 19;
        char im[20];
        while (x >= 0) {
            im[x] = imme[31 - x];
            x--;
        }
        x = 19;
        string wat = "";
        while (x >= 0) {
            wat = wat + im[x];
            x--;
        }

        string ans = wat + rd + opcode;
        return ans;


    }
};
