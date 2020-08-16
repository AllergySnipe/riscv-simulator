using namespace std;
class Clock{
    int cycles=0;
    public:
    void plus(){
        cycles++;
        return;
    }
    int get(){
        return cycles;
    }
};