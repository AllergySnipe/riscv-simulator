#include<iostream>
#include<map>
#include<string>
#include<vector>

using namespace std;

class rtype {

public:
	string opcode = "0110011";
	string func3 = "";
	string func7 = "";
	string rd;
	string rs1;
	string rs2;

	map<string, string> rfunc3;
	map<string, string> rfunc7;

	rtype() {

		rfunc3.insert({ "add","000" });
		rfunc3.insert({ "sub","000" });
		rfunc3.insert({ "sll","001" });
		rfunc3.insert({ "slt","010" });
		rfunc3.insert({ "and","111" });
		rfunc3.insert({ "or","110" });
		rfunc3.insert({ "xor","100" });
		rfunc3.insert({ "sra","101" });
		rfunc3.insert({ "srl","101" });
		rfunc3.insert({ "mul","000" });
		rfunc3.insert({ "div","100" });
		rfunc3.insert({ "rem","110" });


		rfunc7.insert({ "add","0000000" });
		rfunc7.insert({ "sll","0000000" });
		rfunc7.insert({ "slt","0000000" });
		rfunc7.insert({ "and","0000000" });
		rfunc7.insert({ "or","0000000" });
		rfunc7.insert({ "xor","0000000" });
		rfunc7.insert({ "srl","0000000" });
		rfunc7.insert({ "sub","0100000" });
		rfunc7.insert({ "sra","0100000" });
		rfunc7.insert({ "mul","0000001" });
		rfunc7.insert({ "div","0000001" });
		rfunc7.insert({ "rem","0000001" });
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

	string mc(string s) {
		int i = 0;
		while (s[i] == ' ') {
			i++;
		}
		string temp = "";
		while (s[i] != ' ') {
			temp = temp + s[i];
			i++;
		}

		this->func3 = rfunc3[temp];
		this->func7 = rfunc7[temp];

		while (s[i] == ' ') {
			i++;
		}
		temp = "";
		while (s[i] != ' ' && s[i] != ',') {
			temp = temp + s[i];
			i++;
		}
		this->rd = strToBinary(temp, 5);
		while (s[i] == ' ' || s[i] == ',') {
			i++;
		}
		temp = "";
		while (s[i] != ' ' && s[i] != ',') {
			temp = temp + s[i];
			i++;
		}
		rs1 = strToBinary(temp, 5);
		while (s[i] == ' ' || s[i] == ',') {
			i++;
		}
		temp = "";
		while (s[i] != ' ' && s[i] != '\n' && s[i] != '\0') {
			temp = temp + s[i];
			i++;
		}
		rs2 = strToBinary(temp, 5);

		string ans = func7 + rs2 + rs1 + func3 + rd + opcode;
		return ans;
	}

};

