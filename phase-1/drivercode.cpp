#include <bits/stdc++.h>
#include "itype.h"
#include "rtype.h"
#include "sbtype.h"
#include "stype.h"
#include "ujtype.h"
#include "utype.h"
#include <fstream>

using namespace std;

void chng_ins(map<string, int>var, int k, string input[])
{
	int i = k;
	int j = 0;
	string ins = "";
	string reg = "";
	string variable = "";
	ostringstream str1;
	while (input[i][j] == ' ') {
		j++;
	}
	while (input[i][j] != ' ') {
		ins = ins + input[i][j];
		j++;
	}
	while (input[i][j] == ' ') {
		j++;
	}
	while (input[i][j] != ' ' && input[i][j]!=',') {
		reg = reg + input[i][j];
		j++;
	}
	while (input[i][j] == ' ' || input[i][j] == ',') {
		j++;
	}
	while (input[i][j] != ' ' && input[i][j] != '\0' && input[i][j] != '\n') {
		variable = variable + input[i][j];
		j++;
	}

	int off = var[variable];
	str1 << off;
	string offset = str1.str();
	input[i] = "auipc " + reg + ", 65536";
	input[i + 1] = ins + " " + reg + ", " + offset + "(" + reg + ")";
}

string inttostrbin(int n, int p)
{
    int val=n;
    int binaryNum[32];
	    int i = 0;
	    while (i<p) {
    	    binaryNum[i] = val % 2;
    	    val = val / 2;
    	    i++;
	    }
        char out[p+1];
	    for(int j=0; j<p; j++)
	    {
    	    out[j]= binaryNum[p-1-j] + '0';
	    }
	    out[p]='\0';
	    return out;
}

string strtobin(string s, int p)
{
	    int n = s.length();
	    int val=0;
	    int pow=1;
	    int min;
	    if(s[0] == 'x'){min=1;}
	    else{min=0;}
	    for (int i = n-1; i>=min; i--)
	    {
  	    val = val+(s[i]-'0')*pow;
  	    pow=pow*10;
        }
	    int binaryNum[32];
	    int i = 0;
	    while (i<p) {
    	    binaryNum[i] = val % 2;
    	    val = val / 2;
    	    i++;
	    }
        char out[p+1];
	    for(int j=0; j<p; j++)
	    {
    	    out[j]= binaryNum[p-1-j] + '0';
	    }
	    out[p]='\0';
	    return out;
}

void createMap(map<string, char> *um)
{
    (*um)["0000"] = '0';
    (*um)["0001"] = '1';
    (*um)["0010"] = '2';
    (*um)["0011"] = '3';
    (*um)["0100"] = '4';
    (*um)["0101"] = '5';
    (*um)["0110"] = '6';
    (*um)["0111"] = '7';
    (*um)["1000"] = '8';
    (*um)["1001"] = '9';
    (*um)["1010"] = 'A';
    (*um)["1011"] = 'B';
    (*um)["1100"] = 'C';
    (*um)["1101"] = 'D';
    (*um)["1110"] = 'E';
    (*um)["1111"] = 'F';
}

string bintohex(string bin)
{
    int l = bin.size();
    int t = bin.find_first_of('.');

    int len_left = t != -1 ? t : l;

    for (int i = 1; i <= (4 - len_left % 4) % 4; i++)
        bin = '0' + bin;

    if (t != -1)
    {
        int len_right = l - len_left - 1;
        for (int i = 1; i <= (4 - len_right % 4) % 4; i++)
            bin = bin + '0';
    }

    map<string, char> bin_hex_map;
    createMap(&bin_hex_map);

    int i = 0;
    string hex = "";

    while (1)
    {
        hex += bin_hex_map[bin.substr(i, 4)];
        i += 4;
        if (i == bin.size())
            break;
    }
    return hex;
}

string forlabel(int i)
{
    if(i==0){return "0";}
    i=4*i;
    string ans="";
    char c;
    while(i>0)
    {
        c=48+i%10;
        ans+=c;
        i=i/10;
    }
    reverse(ans.begin(),ans.end());
    return ans;
}

string getword(string s, int i)
{
    string ans = "";
    while (s[i] == ' ' || s[i] == ',')
    {
        i++;
    }
    while ((s[i] != ' ' && s[i] != ',') && i < s.size())
    {
        ans += s[i];
        if(s[i]== ':'){break;}
        i++;
    }
    return ans;
}

string getlastword(string s)
{
    int i = s.size() - 1;
    string ans = "";
    while (s[i] != ',' && s[i] != ' ')
    {
        ans += s[i];
        i--;
    }
    reverse(ans.begin(), ans.end());
    return ans;
}

string getvar(string s)
{
    string ans = "";
    int i = 0;
    while (s[i] != ':')
    {
        ans += s[i];
        i++;
    }
    return ans;
}
int main()
{

    itype ity;
    rtype rty;
    sbtype sbty;
    stype sty;
    utype uty;
    ujtype ujty; //declared all the objects
    int text = 1;
    int data = 0;

    map<string, int> varmap; //varmap
    string datamem[1000];

    map<string, int> type;
    type["add"] = 1;
    type["and"] = 1;
    type["or"] = 1;
    type["sll"] = 1;
    type["slt"] = 1;
    type["sra"] = 1;
    type["srl"] = 1;
    type["sub"] = 1;
    type["xor"] = 1;
    type["mul"] = 1;
    type["div"] = 1;
    type["rem"] = 1; //rtype

    type["beq"]=6;
    type["bne"]=6;
    type["bge"]=6;
    type["blt"]=6; //sbtype

    type["addi"] = 2;
    type["andi"] = 2;
    type["ori"] = 2;
    type["lb"] = 2;
    type["ld"] = 2;
    type["lh"] = 2;
    type["lw"] = 2;
    type["jalr"] = 2; //itype

    type["sb"] = 3;
    type["sw"] = 3;
    type["sd"] = 3;
    type["sh"] = 3; //stype

    type["auipc"] = 4;
    type["lui"] = 4; //utype

    type["jal"] = 5; //ujtype

    ifstream input;
    ofstream fout;
    fout.open("output.mc");
    input.open("input.asm");
    string curr;
    int pc = 0;
    int cur=0;
    int k = 0;
    string inp[100];
    int off = 0;
    while (getline(input, curr)) //takes line by line input to update label/var map and store insts in array of strings...incomplete
    {
        if(curr==""){continue;}
        inp[k] = curr;
        k++;
        if (curr == ".data")
        {
            data = 1;
            text = 0;
            continue;
        }
        if (curr == ".text")
        {
            data = 0;
            text = 1;
            continue;
            pc = k;
        }
        if (data == 1)
        {
            string temp=getword(curr, getvar(curr).size()+2);
            //cout<<temp;
            varmap[getvar(curr)] = off;
           // cout<<getvar(curr)<<" "<<off<<endl;

            if(temp==".asciiz")
            {
                int j=0;
                while(curr[j]!='"'){j++;}
                string pop=curr.substr(j,curr.size()-1);
                for(int i=1; i < pop.size()-1; i++)
                {
                    int as=pop[i];
                    datamem[off]=inttostrbin(as,8);
                    off++;
                }
                datamem[off]="00000000";
                off++;
            }
            if(temp== ".word" )
            {
                //cout<<"word srt off is "<<off<<"last word is"<<getlastword(curr)<<endl;
                string t;
                t=strtobin(getlastword(curr),32);
                //cout<<t<<endl;
                for(int i=3; i>-1; i--)
                {
                    datamem[off]=t.substr(8*i, 8);
                    off++;
                }

            }
            if(temp == ".half")
            {

                //cout<<"half start off is "<<off<<"last word is"<<getlastword(curr)<<endl;
                string t;
                t=strtobin(getlastword(curr),16);
                for(int i=1; i>=0; i--)
                {
                    datamem[off]=t.substr(8*i, 8);
                    off++;
                }
            }
            if(temp == ".byte")
            {
               // cout<<"byte start off is "<<off<<"last word is"<<getlastword(curr)<<endl;
                string t;
                t=strtobin(getlastword(curr),8);
                for(int i=0; i<1; i++)
                {
                    for(int j=0; j<8; j++)
                    {
                        datamem[off]+=t[8*i + j];
                    }
                    off++;
                }
            }
            continue;
        }
        if (text == 1)
        {
            string temp;
            temp = getword(curr, 0);
            if (temp[temp.size() - 1] == ':')
            {
                int pop=temp.size()-1;
                int faltu=curr.size()-1;
                //while(curr[faltu] == " "){faltu--;}
                if(curr[faltu]==':')
                {
                    cout<<"yes"<<endl;
                    sbty.labToimm[temp.erase(pop,pop)] = k;
                    ujty.labToimm[temp.erase(pop,pop)] = k;
                }
                else
                {
                    sbty.labToimm[temp.erase(pop,pop)] = k - 1;
                    ujty.labToimm[temp.erase(pop,pop)] = k - 1;
                }
            }
            temp = getlastword(curr);
            if(varmap.count(temp)>0)
            {
                chng_ins(varmap, k-1, inp);
                k++;
            }
        }
    }
    cout<<"ys"<<endl;
    while (pc < k) //reads the pc instruction
    {
        //fout<<inp[pc]<<endl;
        string word = "";
        int j;
        for (j = 0; inp[pc][j] != ' '; j++)                 //checks the first word for type identification
        {
            word += inp[pc][j];
            if(inp[pc][j] == ':'){j++; break;}
        }
        if (word[j - 1] == ':')                             //check if its a label, then skip the word
        {
            while (inp[pc][j] == ' ')
            {
                j++;
            }
            inp[pc].erase(0,j); j=0;
            //cout<<inp[pc]<<endl;
            word = "";
            for (; inp[pc][j] != ' '; j++)
            {
                word += inp[pc][j];
            }
        }
        int t = type[word];                                 //implements from type class.
        if (t == 1)
        {
           fout <<"0x"<<bintohex(strtobin(forlabel(cur),16))<<" "<<"0x"<< bintohex(rty.mc(inp[pc])) << endl;
            cur++;
        }
        if (t == 2)
        {
            fout <<"0x"<<bintohex(strtobin(forlabel(cur),12))<<" "<<"0x"<< bintohex(ity.mc(inp[pc])) << endl;
            cur++;
        }
        if (t == 3)
        {
            fout <<"0x"<<bintohex(strtobin(forlabel(cur),12))<<" "<< "0x"<<bintohex(sty.mc(inp[pc])) << endl;
            cur++;
        }
        if (t == 4)
        {
            fout <<"0x"<<bintohex(strtobin(forlabel(cur),12))<<" "<<"0x"<< bintohex(uty.mc(inp[pc])) << endl;
            cur++;
        }
        if (t == 5)
        {
            fout << "0x"<<bintohex(strtobin(forlabel(cur),12))<<" "<< "0x"<<bintohex(ujty.mc(inp[pc],pc)) << endl;
            cur++;
        }
        if(t == 6)
        {
            fout <<"0x"<<bintohex(strtobin(forlabel(cur),12))<<" "<< "0x"<<bintohex(sbty.mc(inp[pc],pc)) << endl;
            cur++;
        }
        pc++;
    }
    //fout <<"0x"<<bintohex(strtobin(forlabel(cur),12))<<" "<<"end of assembly program"<<endl;
    fout<<"0x00000000\n";
    for(int i=0; i<off; i++)
    {
        fout <<"0x1"<<bintohex(inttostrbin(i,28))<<" "<<"0x"<<bintohex(datamem[i])<<endl;
    }
    cout<<"Task executed successfully! Please check the output file."<<endl;
    fout.close();
    input.close();
}
