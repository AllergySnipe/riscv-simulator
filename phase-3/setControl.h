using namespace std;
#define max_size 10000000000

class InsRecog
{
public:
    bitset<7> opcode;
    bitset<3> func3;
    bitset<7> func7;
    bitset<7> rtypeop = bitset<7>(string("0110011"));
    bitset<7> ujtypeop = bitset<7>(string("1101111"));
    bitset<7> auipcop = bitset<7>(string("0010111"));
    bitset<7> luiop = bitset<7>(string("0110111"));
    bitset<7> jalrop = bitset<7>(string("1100111"));
    bitset<7> rtype3func7 = bitset<7>(string("0100000"));
    bitset<7> rtype2func7 = bitset<7>(string("0000001"));
    int RF_write, cMuxB, cMuxY, cMuxMA=1, deMuxPMI, cMuxPC=1, cMuxINC=0, ALU_op;
    bool condn;
    //bitset<7> loadtypeop(string("0000011"));
    //bitset<7> ityperop(string("0010011"));
    //bitset<7> stypeop(string("0100011"));
    //bitset<7> sbtypeop(string("1100011"));

    unordered_map<bitset<3>, string> rtype1func3 = {
        // Map for R type functions with 0000000 func7
        {bitset<3>(string("000")), "add"},
        {bitset<3>(string("001")), "sll"},
        {bitset<3>(string("010")), "slt"},
        {bitset<3>(string("100")), "xor"},
        {bitset<3>(string("101")), "srl"},
        {bitset<3>(string("110")), "or"},
        {bitset<3>(string("111")), "and"},

    };
    unordered_map<bitset<3>, string> rtype2func3 = {
        // Map for R type functions with 0000001 func7
        {bitset<3>(string("000")), "mul"},
        {bitset<3>(string("100")), "div"},
        {bitset<3>(string("110")), "rem"},
    };

    unordered_map<bitset<3>, string> rtype3func3 = {
        // Map for R type functions with 0100000 func7
        {bitset<3>(string("000")), "sub"},
        {bitset<3>(string("101")), "sra"},
    };

    /*map < bitset<3>, string > jalrfunc3 = {
        { bitset<3>(string("000")),"jalr"},
    };
    map < bitset<3>, string > auipcfunc3 = {
        { bitset<3>(string("000")),"jalr"},
    };*/

    unordered_map<bitset<3>, string> sbtypefunc3 = {
        // Map for SB type func3 to name
        {bitset<3>(string("000")), "beq"},
        {bitset<3>(string("001")), "bne"},
        {bitset<3>(string("100")), "blt"},
        {bitset<3>(string("101")), "bge"},
    };

    unordered_map<bitset<3>, string> stypefunc3 = {
        // Map for S type func3 to name
        {bitset<3>(string("000")), "sb"},
        {bitset<3>(string("001")), "sh"},
        {bitset<3>(string("010")), "sw"},
        {bitset<3>(string("011")), "sd"},
    };

    unordered_map<bitset<3>, string> loadfunc3 = {
        // Map for load type func3 to name ( seperate than Itype coz differnet opcode )
        {bitset<3>(string("000")), "lb"},
        {bitset<3>(string("001")), "lh"},
        {bitset<3>(string("010")), "lw"},
        {bitset<3>(string("011")), "ld"},
    };

    unordered_map<bitset<3>, string> ityperfunc3 = {
        // Map for i type func3 to name
        {bitset<3>(string("000")), "addi"},
        {bitset<3>(string("110")), "ori"},
        {bitset<3>(string("111")), "andi"},
    };

    unordered_map<bitset<7>, unordered_map<bitset<3>, string>> op_map = {
        // Map of OPCODES
        {bitset<7>(string("1100011")), sbtypefunc3}, // key = opcode   element = maps declared above
        {bitset<7>(string("0100011")), stypefunc3},
        {bitset<7>(string("0000011")), loadfunc3},
        {bitset<7>(string("0010011")), ityperfunc3},
    };

    unordered_map<string, int> fin = { // Map for defining control/command
        {"add", 0},
        {"sub", 1},
        {"sll", 2},
        {"slt", 3},
        {"and", 4},
        {"or", 5},
        {"xor", 6},
        {"sra", 7},
        {"srl", 8},
        {"mul", 9},
        {"div", 10},
        {"rem", 11},
        {"addi", 12},
        {"ori", 13},
        {"andi", 14},
        {"lb", 15},
        {"lh", 16},
        {"lw", 17},
        {"ld", 18},
        {"jalr", 19},
        {"lui", 20},
        {"auipc", 21},
        {"sb", 22},
        {"sh", 23},
        {"sw", 24},
        {"sd", 25},
        {"beq", 26},
        {"blt", 27},
        {"bne", 28},
        {"bge", 29},
        {"jal", 30}

    };

    void set(bitset<7> _opcode, bitset<3> _func3)
    { // constructor
        this->opcode = _opcode;
        this->func3 = _func3;
        this->func7 = 0;
        //this->condn = _condn;
    }
    void set(bitset<7> _opcode, bitset<3> _func3, bitset<7> _func7)
    { // overloaded constructor for R type
        this->opcode = _opcode;
        this->func3 = _func3;
        this->func7 = _func7;
        //this->condn = _condn;
    }

    void setCon(bool _condn){
        this->condn=_condn;
    }

    string get_con()
    { // Function which returns the control number
        string control;
        int control_1;
        if (this->opcode == this->rtypeop)
        {
            if (this->func7 == this->rtype3func7)
            {
                control = rtype3func3[this->func3];
            }
            else if (this->func7 == this->rtype2func7)
            {
                control = rtype2func3[this->func3];
            }
            else
            {
                control = rtype1func3[this->func3];
            }
        }
        else if (this->opcode == this->ujtypeop)
        {
            control = "jal";
        }
        else if (this->opcode == this->jalrop)
        {
            control = "jalr";
        }
        else if (this->opcode == this->auipcop)
        {
            control = "auipc";
        }
        else if (this->opcode == this->luiop)
        {
            control = "lui";
        }
        else
        {
            unordered_map<bitset<3>, string> temp = op_map[this->opcode];
            control = temp[this->func3];
        }

        control_1 = fin[control];

        if (control_1 <= 11)
        {
            RF_write = 1; //
            cMuxB = 0;
            cMuxY = 00;
            cMuxMA = 1;
            deMuxPMI = 1;
            cMuxPC = 1;
            cMuxINC = 0;
            ALU_op = control_1;
        }
        else if (control_1 == 12 || control_1 == 13 || control_1 == 14)
        {
            RF_write = 1;
            cMuxB = 1;
            cMuxY = 00;
            cMuxMA = 1;
            deMuxPMI = 1;
            cMuxPC = 1;
            cMuxINC = 0;
            ALU_op = control_1;
        }
        else if (control_1 == 15 || control_1 == 16 || control_1 == 17 || control_1 == 18)
        {
            RF_write = 1;
            cMuxB = 1;
            cMuxY = 01;
            cMuxMA = 0;
            deMuxPMI = 0;
            cMuxPC = 1;
            cMuxINC = 0;
            ALU_op = control_1;
        }
        else if (control_1 == 19)
        {
            RF_write = 1;
            cMuxB = 1;
            cMuxY = 00;
            cMuxMA = 1;
            deMuxPMI = 1;
            cMuxPC = 0;
            cMuxINC = 1;
            ALU_op = control_1;
        }
        else if (control_1 == 20 || control_1 == 21)
        {
            RF_write = 1;
            cMuxB = 1;
            cMuxY = 00;
            cMuxMA = 1;
            deMuxPMI = 1;
            cMuxPC = 1;
            cMuxINC = 0;
            ALU_op = control_1;
        }
        else if (control_1 == 22 || control_1 == 23 || control_1 == 24 || control_1 == 25)
        {
            RF_write = 0;
            cMuxB = 1;
            cMuxY = 00;
            cMuxMA = 0;
            deMuxPMI = 0;
            cMuxPC = 1;
            cMuxINC = 0;
            ALU_op = control_1;
        }
        else if (control_1 == 26 || control_1 == 27 || control_1 == 28 || control_1 == 29)
        {
            RF_write = 0;
            cMuxB = 0;
            ALU_op = control_1;
            if (this->condn == 1)
            {
                cMuxY = 0;
                cMuxMA = 1;
                deMuxPMI = 1;
                cMuxPC = 1;
                cMuxINC = 1;
            }
            else
            {
                cMuxY = 0;
                cMuxMA = 1;
                deMuxPMI = 1;
                cMuxPC = 1;
                cMuxINC = 0;
            }
        }
        else if (control_1 == 30)
        {
            RF_write = 1;
            cMuxB = 0;
            cMuxY = 00;
            cMuxMA = 1;
            deMuxPMI = 1;
            cMuxPC = 1;
            cMuxINC = 1;
            ALU_op = control_1;
        }
        return control;
    }
};