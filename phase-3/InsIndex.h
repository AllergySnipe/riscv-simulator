using namespace std;
class InsIndex
{
    int currInsIndex;
    std::map<string, int> indices =
        {
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
            //
            {"addi", 12},
            {"ori", 13},
            {"andi", 14},
            {"lb", 15},
            {"lh", 16},
            {"lw", 17},
            {"ld", 18},
            {"jalr", 19},
            //
            {"lui", 20},
            {"auipc", 21},
            //
            {"sb", 22},
            {"sh", 23},
            {"sw", 24},
            {"sd", 25},
            //
            {"beq", 26},
            {"blt", 27},
            {"bne", 28},
            {"bge", 29},
            //
            {"jal", 30}};

public:
    void set(string s)
    {
        map<string, int>::iterator it = indices.begin();
        while (it != indices.end())
        {
            if (it->first == s)
            {
                currInsIndex = it->second;
                return;
            }
            it++;
        }
    }
    int get()
    {
        return currInsIndex;
    }
};