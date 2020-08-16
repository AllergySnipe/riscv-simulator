using namespace std;
class ALU
{
    bitset<32> result;
    bool condn;

public:
    int bitset_to_int(bitset<32> RZ)
	{
		int value = 0;
		for (int i = 0; i < 31; i++)
		{
			value = value + RZ[i] * pow(2, i); // RZ[i] is ith bit that is 32-i index
		}
		value = value - RZ[31] * pow(2, 31);
		return value;
	}
    bool getCon()
    {
        return condn;
    }
    bitset<32> operate(bitset<32> ra, bitset<32> rb, int aluop, bitset<32> pcTemp)
    {
        int32_t _ra = (int32_t)ra.to_ulong();
        int32_t _rb = (int32_t)rb.to_ulong();
        switch (aluop)
        {
            //R-TYPE
        case 0: //add
        {
            result = _ra + _rb;
            break;
        }
        case 1: //sub
        {
            result = _ra - _rb;
            break;
        }
        case 2: //sll
        {
            result = ra <<= _rb;
            break;
        }
        case 3: //slt
        {
            result = (_ra < _rb) ? 1 : 0;
            break;
        }
        case 4: //and
        {
            result = ra & rb;
            break;
        }
        case 5: //or
        {
            result = ra | rb;
            break;
        }
        case 6: //xor
        {
            result = ra ^ rb;
            break;
        }
        case 7: //sra
        {
            bool temp = ra[31];
            result = ra >>= _rb;
            for(int i=0;i<_rb;i++){
            result[31-i] = temp;    
            }
            break;
        }

        case 8: //srl
        {
            result = ra >>= _rb;
            break;
        }
        case 9: //mul
        {
            result = _ra * _rb;
            break;
        }
        case 10: //div
        {
            result = _ra / _rb;
            break;
        }
        case 11: //rem
        {
            result = _ra % _rb;
            break;
        }
            //I-TYPE
        case 12: //addi
        {
            result = _ra + _rb;
            break;
        }
        case 13: //ori
        {
            result = ra | rb;
            break;
        }
        case 14: //andi
        {
            result = ra & rb;
            break;
        }
        case 15: //lb                       #########################
        {
            result = (_ra + _rb)*8;
            break;
        }
        case 16: //lh
        {
            result = (_ra + _rb)*8;
            break;
        }
        case 17: //lw
        {
            result = (_ra + _rb)*8;
            break;
        }
        /*case 18: //ld
            result= ra | rb;
            break; */
        case 19: //jalr
        {
            result = bitset<32>(bitset_to_int(pcTemp) + 32);
            break;
        }
        case 20: //lui
        {
            result = rb<<=12;
            break;
        }
        case 21: //auipc
        {
            result = bitset<32>(((bitset_to_int(pcTemp)/8) + bitset_to_int(rb<<=12))*8);
            break;
        }
        case 22: //sb
        {
            result = (_ra + _rb)*8;
            break;
        }
        case 23: //sh
        {
            result = (_ra + _rb)*8;
            break;
        }
        case 24: //sw
        {
            result = (_ra + _rb)*8;
            break;
        }
        /*case 25: //sd
            result= ra | rb;
            break; */
        case 26: //beq
        {
            if (ra == rb)
                condn = 1;
            else
                condn = 0;
            break;
        }
        case 27: //blt
        {
            if (_ra < _rb)
                condn = 1;
            else
                condn = 0;
            break;
        }
        case 28: //bne
        {
            if (ra != rb)
                condn = 1;
            else
                condn = 0;
            break;
        }
        case 29: //bge
        {
            if (_ra >= _rb)
                condn = 1;
            else
                condn = 0;
            break;
        }
        case 30: //jal
        {
            result = bitset<32>(bitset_to_int(pcTemp) + 32);
            break;
        }

        default:
            break;
        }

        return result;
    }
};