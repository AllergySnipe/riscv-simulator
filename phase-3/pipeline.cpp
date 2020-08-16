#include <bits/stdc++.h>
#include "GlobalStates.h"
#include "Clock.h"
using namespace std;
class currstage
{
public:
    int val = 0;
    void inc()
    {
        if (val == 4)
            val = -1;
        else
            val++;
        return;
    }
};
class pipe : public GlobalStates
{
public:
    GlobalStates i0, i1, i2, i3, i4;
    int loadcount = 0, alucount = 0, controlcount = 0;
    bool endReached = 0;
    bool i0end = 0, i1end = 0, i2end = 0, i3end = 0, i4end = 0;
    currstage i0appStage, i1appStage, i2appStage, i3appStage, i4appStage, endStage;
    bool i0rs1i4 = 0, i0rs2i4 = 0, i0rs1i3 = 0, i0rs2i3 = 0, i1rs1i4 = 0, i1rs2i4 = 0;
    Clock clk;
    bool df_k;
    int dhazcount=0;
    int chazcount=0;
    int scount=0;
    int scountoof=0;
    pipe(bool df)
    {
        df_k = df;
        i0.myPmi.MAR = 0;
        i1.myPmi.MAR = 32;
        i2.myPmi.MAR = 32 * 2;
        i3.myPmi.MAR = 32 * 3;
        i4.myPmi.MAR = 32 * 4;
        i0.nextMA = 0;
        i1.nextMA = 32;
        i2.nextMA = 32 * 2;
        i3.nextMA = 32 * 3;
        i4.nextMA = 32 * 4;
        i0appStage.val = 0;
        i1appStage.val = 0;
        i2appStage.val = 0;
        i3appStage.val = 0;
        i4appStage.val = 0;
    }
    void initAppStages(int i, int i0, int i1, int i2, int i3, int i4)
    {
        endStage.val = i;
        i0appStage.val = i0;
        i1appStage.val = i1;
        i2appStage.val = i2;
        i3appStage.val = i3;
        i4appStage.val = i4;
        return;
    }
    void pipeflow(bitset<100000> &mem1)
    {
        if ((i0.IR != NULL) || (i0.IR == 0 && i0end == 1))
        {
            if (!(endReached == 1 && ((i0.stage <= endStage.val) || i0.stage == 4)))
                i0.next(i0.nextMA, mem1);
            if (i0.stage = 1)
            {
                if ((i0.myIndices.get() >= 15 && i0.myIndices.get() <= 18) || (i0.myIndices.get() >= 22 && i0.myIndices.get() <= 25))
                    loadcount++;
                else if ((i0.myIndices.get() >= 19 && i0.myIndices.get() <= 21) || (i0.myIndices.get() >= 26 && i0.myIndices.get() <= 30))
                    controlcount++;
                else
                    alucount++;
            }
            if (i0.IR == 0 && i0.stage == 0 && !i0appStage.val)
            {
                endReached = 1;
                i0end = 1;
                initAppStages(-1, 1, 0, 0, 0, 0);
            }
            if (i0end == 1)
                endStage.inc();
            if (endStage.val == 4 && i0.writeDone == 1)
                return;
            if (endReached == 1 && i0appStage.val == 0)
                i0appStage.inc();
            cout << "i0" << endl;
            /////////data forwarding for i0
            if ((((i0.rs1 == i4.rd || i0.rs2 == i4.rd) && i4.rd != 0) || ((i0.rs1 == i3.rd || i0.rs2 == i3.rd) && i3.rd != 0)) && i0.stage == 1 /* && !(i3.IR.to_ulong() == 19 && i4.IR.to_ulong() == 19) && i4.IR.to_ulong() != 19*/ && i0.IR.to_ulong() != 19)
            {
                cout << "i0 dependent on i4/i3" << endl;
                dhazcount++;
                if (df_k)
                {
                    if (i4.myIndices.get() == 15 || i4.myIndices.get() == 16 || i4.myIndices.get() == 17)
                    {
                        i0.myPmi.insertnop(mem1, 1);
                        scount++;
                        i0.myPmi.set(0, i0.tempMAR.to_ulong(), 1);
                        i0.Fetch(i0.nextMA, mem1);
                        i0.writeDone = 0;
                        i0.Decode();
                    }
                    else
                    {
                        //df i4->i0
                        if (i0.rs1 == i4.rd)
                            i0rs1i4 = 1;
                        if (i0.rs2 == i4.rd)
                            i0rs2i4 = 1;
                        //df i3->i0
                        if (i0.rs1 == i3.rd)
                            i0rs1i3 = 1;
                        if (i0.rs2 == i3.rd)
                            i0rs2i3 = 1;
                        if ((i0.rs1 == i3.rd && i0.rs1 == i4.rd) || (i0.rs2 == i3.rd && i0.rs2 == i4.rd))
                            i0.samedes = 1;
                    }
                }
                else
                {
                    if (i0.rs1 == i4.rd || i0.rs2 == i4.rd)
                        {i0.myPmi.insertnop(mem1, 0);scount++;scount++;}
                    else if (i0.rs1 == i3.rd || i0.rs2 == i3.rd)
                        {i0.myPmi.insertnop(mem1, 1);scount++;}
                    i0.myPmi.set(0, i0.tempMAR.to_ulong(), 1);
                    //myPC.set(myPC.get().to_ulong() - 32);
                    i0.Fetch(i0.myPmi.MAR, mem1);
                    i0.writeDone = 0;
                    i0.Decode();
                }
            }
            //pipeline flush for i0
            if (i0.stage == 2 && i0.myIndices.get() >= 26 && i0.myIndices.get() <= 29 && i0.myALU.getCon())
            {
                i1.B1.IR = 19;
                i1.B1.pcTemp = i1.myPC.get();
                //i1.Decode();
                i2.myPmi.set(0, i1.tempMAR.to_ulong(), 1);
                i2.nextMA = i0.SXT_imm.to_ulong() + 32 * 3;
                i3.myPmi.set(0, i0.SXT_imm, 1);
                i4.myPmi.set(0, i0.SXT_imm.to_ulong() + 32 * 1, 1);
                i0.myPmi.set(0, i0.SXT_imm.to_ulong() + 32 * 2, 1);
                i1.myPmi.set(0, i0.SXT_imm.to_ulong() + 32 * 3, 1);
                chazcount++;
            }
            ////////////I0 DONE
            if (i1.IR != NULL || (i1.IR == 0 && i1end == 1))
            {
                if (!(endReached == 1 && (i1.stage <= endStage.val || i1.stage == 4)))
                    i1.next(i1.nextMA, mem1);
                if (i1.stage = 1)
                {
                    if ((i1.myIndices.get() >= 15 && i1.myIndices.get() <= 18) || (i1.myIndices.get() >= 22 && i1.myIndices.get() <= 25))
                        loadcount++;
                    else if ((i1.myIndices.get() >= 19 && i1.myIndices.get() <= 21) || (i1.myIndices.get() >= 26 && i1.myIndices.get() <= 30))
                        controlcount++;
                    else
                        alucount++;
                }
                if (i1.IR == 0 && i1.stage == 0 && !i1appStage.val)
                {
                    endReached = 1;
                    i1end = 1;
                    initAppStages(-1, 0, 1, 0, 0, 0);
                }
                if (i1end == 1)
                    endStage.inc();
                if (endStage.val == 4)
                    return;
                if (endReached == 1 && i1appStage.val == 0)
                    i1appStage.inc();
                cout << "i1" << endl;
                //////data forwarding for i1
                if ((((i1.rs1 == i0.rd || i1.rs2 == i0.rd) && i0.rd != 0) || ((i1.rs1 == i4.rd || i1.rs2 == i4.rd) && i4.rd != 0)) && i1.stage == 1 /* && !(i4.IR.to_ulong() == 19 && i0.IR.to_ulong() == 19) && i0.IR.to_ulong() != 19*/ && i1.IR.to_ulong() != 19)
                {
                    cout << "i1 dependent on i0" << endl;
                    dhazcount++;
                    if (df_k)
                    {
                        if (i0.myIndices.get() == 15 || i0.myIndices.get() == 16 || i0.myIndices.get() == 17)
                        {
                            i1.myPmi.insertnop(mem1, 1);
                            scount++;
                            i1.myPmi.set(0, i1.tempMAR.to_ulong(), 1);
                            i1.Fetch(i1.nextMA, mem1);
                            i1.writeDone = 0;
                            i1.Decode();
                        }
                        else
                        { //df i0->i1
                            if (i1.rs1 == i0.rd)
                                regFile.writeData(i1.rs1, i0.B3.alu_out, 1);
                            if (i1.rs2 == i0.rd)
                                regFile.writeData(i1.rs2, i0.B3.alu_out, 1);
                            //df i4->i1
                            if (i1.rs1 == i4.rd)
                                i1rs1i4 = 1;
                            if (i1.rs2 == i4.rd)
                                i1rs2i4 = 1;
                            if ((i1.rs1 == i4.rd && i1.rs1 == i0.rd) || (i1.rs2 == i4.rd && i1.rs2 == i0.rd))
                            {
                                i1.samedes = 1;
                                i1.manualForward1 = regFile.readData(i1.rs1);
                                i1.manualForward2 = regFile.readData(i1.rs2);
                            }
                        }
                    }
                    else
                    {
                        if (i1.rs1 == i0.rd || i1.rs2 == i0.rd)
                            {i1.myPmi.insertnop(mem1, 0);scount++;scount++;}
                        else if (i1.rs1 == i4.rd || i1.rs2 == i4.rd)
                            {i1.myPmi.insertnop(mem1, 1);scount++;}
                        i1.myPmi.set(0, i1.tempMAR.to_ulong(), 1);
                        //myPC.set(myPC.get().to_ulong() - 32);
                        i1.Fetch(i1.myPmi.MAR, mem1);
                        i1.writeDone = 0;
                        i1.Decode();
                        if (i1.rs1 == i0.rd || i1.rs2 == i0.rd)
                        {
                            //i2.myPmi.set(0, i2.tempMAR.to_ulong(), 1);
                            //i2.Fetch(i2.nextMA, mem1);
                            //i2.writeDone = 0;
                        }
                    }
                }
                //pipeline flush for i1
                if (i1.stage == 2 && i1.myIndices.get() >= 26 && i1.myIndices.get() <= 29 && i1.myALU.getCon())
                {
                    i2.B1.IR = 19;
                    i2.B1.pcTemp = i2.myPC.get();
                    //i2.Decode();
                    i3.myPmi.set(0, i2.tempMAR.to_ulong(), 1);
                    i3.nextMA = i1.SXT_imm.to_ulong() + 32 * 3;
                    i4.myPmi.set(0, i1.SXT_imm, 1);
                    i0.myPmi.set(0, i1.SXT_imm.to_ulong() + 32 * 1, 1);
                    i1.myPmi.set(0, i1.SXT_imm.to_ulong() + 32 * 2, 1);
                    i2.myPmi.set(0, i1.SXT_imm.to_ulong() + 32 * 3, 1);
                    chazcount++;
                }
                /////////////I1 DONE
                if (i2.IR != NULL || (i2.IR == 0 && i2end == 1))
                {
                    if (!(endReached == 1 && (i2.stage <= endStage.val || i2.stage == 4)))
                        i2.next(i2.nextMA, mem1);
                    if (i2.stage = 1)
                    {
                        if ((i2.myIndices.get() >= 15 && i2.myIndices.get() <= 18) || (i2.myIndices.get() >= 22 && i2.myIndices.get() <= 25))
                            loadcount++;
                        else if ((i2.myIndices.get() >= 19 && i2.myIndices.get() <= 21) || (i2.myIndices.get() >= 26 && i2.myIndices.get() <= 30))
                            controlcount++;
                        else
                            alucount++;
                    }
                    if (i2.IR == 0 && i2.stage == 0 && !i2appStage.val)
                    {
                        endReached = 1;
                        i2end = 1;
                        initAppStages(-1, 0, 0, 1, 0, 0);
                    }
                    if (i2end == 1)
                        endStage.inc();
                    if (endStage.val == 4)
                        return;
                    if (endReached == 1 && i2appStage.val == 0)
                        i2appStage.inc();
                    cout << "i2" << endl;
                    /////////data forwarding for i2
                    if ((((i2.rs1 == i0.rd || i2.rs2 == i0.rd) && i0.rd != 0) || ((i2.rs1 == i1.rd || i2.rs2 == i1.rd) && i1.rd != 0)) && i2.stage == 1 /*&& !(i0.IR.to_ulong() == 19 && i1.IR.to_ulong() == 19) && i1.IR.to_ulong() != 19*/ && i2.IR.to_ulong() != 19)
                    {
                        cout << "i2 dependent on i0/i1" << endl;
                        dhazcount++;
                        if (df_k)
                        {
                            if (i1.myIndices.get() == 15 || i1.myIndices.get() == 16 || i1.myIndices.get() == 17)
                            {
                                i1.myPmi.insertnop(mem1, 1);
                                scount++;
                                i2.myPmi.set(0, i2.tempMAR.to_ulong(), 1);
                                i2.Fetch(i2.nextMA, mem1);
                                i2.writeDone = 0;
                                i2.Decode();
                            }
                            else
                            {
                                //df i0->i2
                                if (i2.rs1 == i0.rd)
                                    regFile.writeData(i2.rs1, i0.B4.output, 1);
                                if (i2.rs2 == i0.rd)
                                    regFile.writeData(i2.rs2, i0.B4.output, 1);
                                //df i1->i2
                                if (i2.rs1 == i1.rd)
                                    regFile.writeData(i2.rs1, i1.B3.alu_out, 1);
                                if (i2.rs2 == i1.rd)
                                    regFile.writeData(i2.rs2, i1.B3.alu_out, 1);
                                if ((i2.rs1 == i0.rd && i2.rs1 == i1.rd) || (i2.rs2 == i0.rd && i2.rs2 == i1.rd))
                                {
                                    i2.samedes = 1;
                                    i2.manualForward1 = regFile.readData(i2.rs1);
                                    i2.manualForward2 = regFile.readData(i2.rs2);
                                }
                            }
                        }
                        else
                        {
                            if (i2.rs1 == i1.rd || i2.rs2 == i1.rd)
                                {i2.myPmi.insertnop(mem1, 0);scount++;scount++;}
                            else if (i2.rs1 == i0.rd || i2.rs2 == i0.rd)
                                {i2.myPmi.insertnop(mem1, 1);scount++;}

                            i2.myPmi.set(0, i2.tempMAR.to_ulong(), 1);
                            //myPC.set(myPC.get().to_ulong() - 32);
                            i2.Fetch(i2.myPmi.MAR, mem1);
                            i2.writeDone = 0;
                            i2.Decode();
                            if (i2.rs1 == i1.rd || i2.rs2 == i1.rd)
                            {
                                //i3.myPmi.set(0, i3.tempMAR.to_ulong(), 1);
                                //i3.Fetch(i3.nextMA, mem1);
                                //i3.writeDone = 0;
                            }
                        }
                    }
                    //pipeline flush for i2
                    if (i2.stage == 2 && i2.myIndices.get() >= 26 && i2.myIndices.get() <= 29 && i2.myALU.getCon())
                    {
                        i3.B1.IR = 19;
                        i3.B1.pcTemp = i3.myPC.get();
                        //i3.Decode();
                        i4.myPmi.set(0, i3.tempMAR.to_ulong(), 1);
                        i4.nextMA = i2.SXT_imm.to_ulong() + 32 * 3;
                        i0.myPmi.set(0, i2.SXT_imm, 1);
                        i1.myPmi.set(0, i2.SXT_imm.to_ulong() + 32 * 1, 1);
                        i2.myPmi.set(0, i2.SXT_imm.to_ulong() + 32 * 2, 1);
                        i3.myPmi.set(0, i2.SXT_imm.to_ulong() + 32 * 3, 1);
                        chazcount++;
                    }
                    //////////I2 DONE
                    if (i3.IR != NULL || (i3.IR == 0 && i3end == 1))
                    {
                        if (!(endReached == 1 && (i3.stage <= endStage.val || i3.stage == 4)))
                            i3.next(i3.nextMA, mem1);
                        if (i3.stage = 1)
                        {
                            if ((i3.myIndices.get() >= 15 && i3.myIndices.get() <= 18) || (i3.myIndices.get() >= 22 && i3.myIndices.get() <= 25))
                                loadcount++;
                            else if ((i3.myIndices.get() >= 19 && i3.myIndices.get() <= 21) || (i3.myIndices.get() >= 26 && i3.myIndices.get() <= 30))
                                controlcount++;
                            else
                                alucount++;
                        }
                        if (i3.IR == 0 && i3.stage == 0 && !i3appStage.val)
                        {
                            endReached = 1;
                            i3end = 1;
                            initAppStages(-1, 0, 0, 0, 1, 0);
                        }
                        if (i3end == 1)
                            endStage.inc();
                        if (endStage.val == 4)
                            return;
                        if (endReached == 1 && i3appStage.val == 0)
                            i3appStage.inc();
                        cout << "i3" << endl;
                        if (i0rs1i3 && i3.stage == 3)
                            regFile.writeData(i0.rs1, i3.B4.output, 1);
                        if (i0rs2i3 && i3.stage == 3)
                            regFile.writeData(i0.rs2, i3.B4.output, 1);

                        i0rs1i3 = 0;
                        i0rs2i3 = 0;
                        /////////data forwarding for i3
                        if ((((i3.rs1 == i1.rd || i3.rs2 == i1.rd) && i1.rd != 0) || ((i3.rs1 == i2.rd || i3.rs2 == i2.rd) && i2.rd != 0)) && i3.stage == 1 /*&& !(i1.IR.to_ulong() == 19 && i2.IR.to_ulong() == 19) && i2.IR.to_ulong() != 19*/ && i3.IR.to_ulong() != 19)
                        {
                            cout << "i3 dependent on i1/i2" << endl;
                            dhazcount++;
                            if (df_k)
                            {
                                if (i2.myIndices.get() == 15 || i2.myIndices.get() == 16 || i2.myIndices.get() == 17)
                                {
                                    i2.myPmi.insertnop(mem1, 1);
                                    scount++;
                                    i3.myPmi.set(0, i3.tempMAR.to_ulong(), 1);
                                    i3.Fetch(i2.nextMA, mem1);
                                    i3.writeDone = 0;
                                    i3.Decode();
                                }
                                else
                                { //df i1->i3
                                    if (i3.rs1 == i1.rd)
                                        regFile.writeData(i3.rs1, i1.B4.output, 1);
                                    if (i3.rs2 == i1.rd)
                                        regFile.writeData(i3.rs2, i1.B4.output, 1);
                                    //df i2->i3
                                    if (i3.rs1 == i2.rd)
                                        regFile.writeData(i3.rs1, i2.B3.alu_out, 1);
                                    if (i3.rs2 == i2.rd)
                                        regFile.writeData(i3.rs2, i2.B3.alu_out, 1);
                                    if ((i3.rs1 == i1.rd && i3.rs1 == i2.rd) || (i3.rs2 == i1.rd && i3.rs2 == i2.rd))
                                    {
                                        i3.samedes = 1;
                                        i3.manualForward1 = regFile.readData(i3.rs1);
                                        i3.manualForward2 = regFile.readData(i3.rs2);
                                    }
                                }
                            }
                            else
                            {
                                if (i3.rs1 == i2.rd || i3.rs2 == i2.rd)
                                    {i3.myPmi.insertnop(mem1, 0);scount++;scount++;}
                                else if (i3.rs1 == i1.rd || i3.rs2 == i1.rd)
                                    {i3.myPmi.insertnop(mem1, 1);scount++;}
                                i3.myPmi.set(0, i3.tempMAR.to_ulong(), 1);
                                //myPC.set(myPC.get().to_ulong() - 32);
                                i3.Fetch(i3.myPmi.MAR, mem1);
                                i3.writeDone = 0;
                                i3.Decode();
                                if (i3.rs1 == i2.rd || i3.rs2 == i2.rd)
                                {
                                    //i4.myPmi.set(0, i4.tempMAR.to_ulong(), 1);
                                    //i4.Fetch(i4.nextMA, mem1);
                                    //i4.writeDone = 0;
                                }
                            }
                        }
                        //pipeline flush for i3
                        if (i3.stage == 2 && i3.myIndices.get() >= 26 && i3.myIndices.get() <= 29 && i3.myALU.getCon())
                        {
                            i4.B1.IR = 19;
                            i4.B1.pcTemp = i4.myPC.get();
                            //i4.Decode();
                            i0.myPmi.set(0, i4.tempMAR.to_ulong(), 1);
                            i0.nextMA = i3.SXT_imm.to_ulong() + 32 * 3;
                            i1.myPmi.set(0, i3.SXT_imm, 1);
                            i2.myPmi.set(0, i3.SXT_imm.to_ulong() + 32 * 1, 1);
                            i3.myPmi.set(0, i3.SXT_imm.to_ulong() + 32 * 2, 1);
                            i4.myPmi.set(0, i3.SXT_imm.to_ulong() + 32 * 3, 1);
                            chazcount++;
                        }
                        ///////////I3 DONE
                        if (i4.IR != NULL || (i4.IR == 0 && i4end == 1))
                        {
                            if (!(endReached == 1 && (i4.stage < endStage.val || i4.stage == 4)))
                                i4.next(i4.nextMA, mem1);
                            if (i4.stage = 1)
                            {
                                if ((i4.myIndices.get() >= 15 && i4.myIndices.get() <= 18) || (i4.myIndices.get() >= 22 && i4.myIndices.get() <= 25))
                                    loadcount++;
                                else if ((i4.myIndices.get() >= 19 && i4.myIndices.get() <= 21) || (i4.myIndices.get() >= 26 && i4.myIndices.get() <= 30))
                                    controlcount++;
                                else
                                    alucount++;
                            }
                            if (i4.IR == 0 && i4.stage == 0 && !i4appStage.val)
                            {
                                endReached = 1;
                                i4end = 1;
                                initAppStages(-1, 0, 0, 0, 0, 1);
                            }
                            if (i4end == 1)
                                endStage.inc();
                            if (endStage.val == 4)
                                return;
                            if (endReached == 1 && i4appStage.val == 0)
                                i4appStage.inc();
                            cout << "i4" << endl;
                            if (i0rs1i4 && i4.stage == 2)
                                regFile.writeData(i0.rs1, i4.B3.alu_out, 1);
                            if (i0rs2i4 && i4.stage == 2)
                                regFile.writeData(i0.rs2, i4.B3.alu_out, 1);
                            if (i1rs1i4 && i4.stage == 3)
                                regFile.writeData(i1.rs1, i4.B4.output, 1);
                            if (i1rs2i4 && i4.stage == 3)
                                regFile.writeData(i1.rs1, i4.B4.output, 1);
                            if (i1.samedes)
                            {
                                i1.manualForward1 = regFile.readData(i1.rs1);
                                i1.manualForward2 = regFile.readData(i1.rs2);
                            }
                            i0rs1i4 = 0;
                            i0rs2i4 = 0;
                            i1rs1i4 = 0;
                            i1rs2i4 = 0;
                            /////////data forwarding for i4
                            if ((((i4.rs1 == i2.rd || i4.rs2 == i2.rd) && i2.rd != 0) || ((i4.rs1 == i3.rd || i4.rs2 == i3.rd) && i3.rd != 0)) && i4.stage == 1 /*&& !(i2.IR.to_ulong() == 19 && i3.IR.to_ulong() == 19) && i3.IR.to_ulong() != 19 */ && i4.IR.to_ulong() != 19)
                            {
                                cout << "i4 dependent on i2/i3" << endl;
                                dhazcount++;
                                if (df_k)
                                {
                                    if (i3.myIndices.get() == 15 || i3.myIndices.get() == 16 || i3.myIndices.get() == 17)
                                    {
                                        i3.myPmi.insertnop(mem1, 1);
                                        scount++;
                                        i4.myPmi.set(0, i4.tempMAR.to_ulong(), 1);
                                        i4.Fetch(i4.nextMA, mem1);
                                        i4.writeDone = 0;
                                        i4.Decode();
                                    }
                                    else
                                    {
                                        //df i2->i4
                                        if (i4.rs1 == i2.rd)
                                            regFile.writeData(i4.rs1, i2.B4.output, 1);
                                        if (i4.rs2 == i2.rd)
                                            regFile.writeData(i4.rs2, i2.B4.output, 1);
                                        //df i3->i4
                                        if (i4.rs1 == i3.rd)
                                            regFile.writeData(i4.rs1, i3.B3.alu_out, 1);
                                        if (i4.rs2 == i3.rd)
                                            regFile.writeData(i4.rs2, i3.B3.alu_out, 1);
                                        if ((i4.rs1 == i2.rd && i4.rs1 == i3.rd) || (i4.rs2 == i2.rd && i4.rs2 == i3.rd))
                                        {
                                            i4.samedes = 1;
                                            i4.manualForward1 = regFile.readData(i4.rs1);
                                            i4.manualForward2 = regFile.readData(i4.rs2);
                                        }
                                    }
                                }
                                else
                                {
                                    if (i4.rs1 == i3.rd || i4.rs2 == i3.rd)
                                        {i4.myPmi.insertnop(mem1, 0);scount++;scount++;}
                                    else if (i4.rs1 == i2.rd || i4.rs2 == i2.rd)
                                        {i4.myPmi.insertnop(mem1, 1);scount++;}
                                    i4.myPmi.set(0, i4.tempMAR.to_ulong(), 1);
                                    //myPC.set(myPC.get().to_ulong() - 32 - 32);
                                    i4.Fetch(i4.myPmi.MAR, mem1);
                                    i4.writeDone = 0;
                                    //myPC.set(myPC.get().to_ulong()-32 );
                                    i0.myPmi.set(0, i0.tempMAR.to_ulong(), 1);
                                    i0.Fetch(i0.myPmi.MAR, mem1);
                                    i0.writeDone = 0;
                                    i4.Decode();
                                }
                            }
                            //pipeline flush for i4
                            if (i4.stage == 2 && i4.myIndices.get() >= 26 && i4.myIndices.get() <= 29 && i4.myALU.getCon())
                            {
                                i0.B1.IR = 19;
                                i0.B1.pcTemp = i0.myPC.get();
                                //i0.Decode();
                                i1.myPmi.set(0, i0.tempMAR.to_ulong(), 1);
                                i1.nextMA = i4.SXT_imm.to_ulong() + 32 * 3;
                                i2.myPmi.set(0, i4.SXT_imm, 1);
                                i3.myPmi.set(0, i4.SXT_imm.to_ulong() + 32 * 1, 1);
                                i4.myPmi.set(0, i4.SXT_imm.to_ulong() + 32 * 2, 1);
                                i0.myPmi.set(0, i4.SXT_imm.to_ulong() + 32 * 3, 1);
                                chazcount++;
                            }
                            ////////////I4 DONE
                        }
                        else if (i3.stage > 0 && endReached == 0)
                        { /////////I4 FETCH
                            i4.next(i4.nextMA, mem1);
                            cout << "i4" << endl;
                            cout << "i4 fetched^" << endl;
                            if (i4.IR == 0)
                            {
                                endReached = 1;
                                i4end = 1;
                                initAppStages(0, 0, 0, 0, 0, 1);
                            }
                        }
                    }
                    else if (i2.stage > 0 && endReached == 0)
                    { //////////////I3 FETCH
                        i3.next(i3.nextMA, mem1);
                        cout << "i3" << endl;
                        cout << "i3 fetched^" << endl;
                        if (i3.IR == 0)
                        {
                            endReached = 1;
                            i3end = 1;
                            initAppStages(0, 0, 0, 0, 1, 0);
                        }
                    }
                }
                else if (i1.stage > 0 && endReached == 0)
                { //////////I2 FETCH
                    i2.next(i2.nextMA, mem1);
                    cout << "i2" << endl;
                    cout << "i2 fetched^" << endl;
                    if (i2.IR == 0)
                    {
                        endReached = 1;
                        i2end = 1;
                        initAppStages(0, 0, 0, 1, 0, 0);
                    }
                }
            }
            else if (i0.stage > 0 && endReached == 0)
            { /////////////I1 FETCH
                i1.next(i1.nextMA, mem1);
                cout << "i1" << endl;
                cout << "i1 fetched^" << endl;
                if (i1.IR == 0)
                {
                    endReached = 1;
                    i1end = 1;
                    initAppStages(0, 0, 1, 0, 0, 0);
                }
            }
        }
        else if (endReached == 0)
        { /////////////I0 FETCH
            i0.next(i0.nextMA, mem1);
            cout << "i0" << endl;
            cout << "i0 fetched" << endl;
            if (i0.IR == 0)
            {
                endReached = 1;
                i0end = 1;
                initAppStages(0, 1, 0, 0, 0, 0);
            }
        }
        clk.plus();
        if (knob_rf)
        {
            cout << "cycles: " << p.clk.get() << endl;
            cout << "registers: " << endl;
            for (int i = 0; i < 32; i++)
            {
                cout << "x" << i << "=" << regFile.readData(i).to_ulong() << ",  ";
                if ((i + 1) % 8 == 0)
                    cout << endl;
            }
        }
        pipeflow(mem1); //ez recursive call
    }
};
bool knob_df = 0;
bool knob_p = 1;
bool knob_rf = 0;
pipe p(knob_df);
GlobalStates gs;
int main()
{
    ifstream input;
    ofstream fout;
    input.open("output.mc");
    fout.open("stats.txt");
    bitset<100000> mem1;
    long long int k = 0;
    string curr;
    int inscount = 0;

    vector<string> pcs, inss;
    while (getline(input, curr))
    {
        string temp = "";
        int i = 0;
        while (curr[i] != ' ')
        {
            temp = temp + curr[i];
            i++;
        }
        string pc = HexToBin(temp);
        while (curr[i] == ' ')
        {
            i++;
        }
        temp = "";
        while (curr[i] != ' ' && curr[i] != '\n' && curr[i] != '\0')
        {
            temp = temp + curr[i];
            i++;
        }
        string ins;
        ins = HexToBin(temp);
        int x = 0;
        while (x < 32)
        {
            int b = int(ins[31 - x] - '0');
            mem1[k + x] = b;
            x++;
        }
        k = k + 32;
        inscount++;
    }

    if (knob_p)
    {
        p.pipeflow(mem1);
        fout << "Stat1: Total number of cycles: " << p.clk.get() << endl;
        fout << "Stat2: Total instructions: " << inscount << endl;
        fout << "Stat3: CPI: " << ((float)p.clk.get()) / ((float)inscount) << endl;
        fout << "Stat4: Number of Data-transfer (load and store) instructions executed: "<<p.loadcount<< endl;
        fout << "Stat5: Number of ALU instructions executed: "<<p.alucount << endl;
        fout << "Stat6: Number of Control instructions executed: "<<p.controlcount << endl;
        fout << "Stat7: Number of stalls/bubbles in the pipeline: "<<p.scount << endl;
        fout << "Stat8: Number of data hazards: " <<p.dhazcount<< endl;
        fout << "Stat9: Number of control hazards: " <<p.chazcount<< endl;
        fout << "Stat10: Number of branch mispredictions: "<<" - " << endl;
        fout << "Stat11: Number of stalls due to data hazards: " << endl;
        fout << "Stat12: Number of stalls due to control hazards: " << endl;
    }
    else
    {
        while (1)
        {
            gs.Fetch(gs.nextMA, mem1);
            cout << "FETCH: Fetch instruction" << endl;
            if (gs.IR == 0)
                break;
            gs.Decode();
            cout << "DECODE: operation is " << gs.recog.get_con() << ", first operand is r" << (gs.rs1).to_ulong() << ", second operand is r" << (gs.rs2).to_ulong() << " and destination is r" << (gs.rd).to_ulong() << endl;
            cout << "DECODE: read rs1=" << regFile.readData(gs.rs1).to_ulong() << " , rs2=" << regFile.readData(gs.rs2).to_ulong() << endl;
            gs.ALU();
            cout << "ALU: execute " << gs.recog.get_con() << " " << regFile.readData(gs.rs1).to_ulong() << " , " << regFile.readData(gs.rs2).to_ulong() << endl;
            gs.MemAccess();
            if (gs.myPmi.flag == 1)
                cout << "NO MEMORY ACCESS" << endl;
            else
                cout << "MEM ACCESS: get data from address " << gs.RZ.get() << " with offset " << gs.RM.get() << endl;

            gs.RegUpdate();
            cout << "WRITEBACK: write" << regFile.readData(gs.rd).to_ulong() << "to r" << (gs.rd).to_ulong() << endl;
        }
    }
}