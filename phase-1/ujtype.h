#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include<bitset>
using namespace std;
class ujtype {
public:
    string opcode = "1101111", rd = "", imme = "";
    map<string, int> labToimm;

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


    string mc(string s, int pc) {

        int i = 0;
        string temp = "";
        while (s[i] != ' ') {
            i++;
        }

        i++;
        while (s[i] != ',') {
            temp = temp + s[i];
            i++;
        }
        rd = strToBinary(temp, 5);
        i++;

        temp = "";
        while (s[i] == ' ') {
            i++;
        }

        while (s[i] != ' ' && s[i] != '\n' && s[i] != '\0') {
            temp = temp + s[i];
            i++;
        }

        int why = 4*(labToimm[temp] - pc);
        bitset<21>value(why);
        //imme = strToBinary(labToimm[temp],21);
        int u = 0;
        while (u < 21) {
            bitset<1> temp;
            temp[0] = value[20 - u];
            imme = imme + temp.to_string();
            u++;
        }
        string imme1 = "";
        imme1 = imme1 + imme[0] + imme[10] + imme[11] + imme[12] + imme[13] + imme[14] + imme[15] + imme[16] + imme[17] + imme[18] + imme[19] + imme[9] + imme[1] + imme[2] + imme[3] + imme[4] + imme[5] + imme[6] + imme[7] + imme[8];

        string ans = imme1 + rd + opcode;
        return ans;
    }
};
