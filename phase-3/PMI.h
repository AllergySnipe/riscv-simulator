using namespace std;
bitset<10000000000> data_MEM = 0;
class PMI
{
	public:
	Mux MuxMA;
	int MIN_MOD = 268437504;	
	bitset<32> RZ, RM, MAR, MDR = 0;


	bool flag=0;

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

	void store_byte()
	{
		bitset<32> final_data(0);
		for (int i = 0; i < 8; i++)
		{
			final_data[24 + i] = RM[i];
		}
		int final_address = bitset_to_int(RZ);

		for (int i = 0; i < 8; i++)
		{
			data_MEM[max_size - final_address - MIN_MOD - i - 1] = final_data[31 - i];
		}
		cout << "MEMORY-";
		for (int i = 0; i < 32; i++)
		{
			cout << data_MEM[max_size - final_address - MIN_MOD - i - 1];
		}
		cout << endl;
	}

	void store_half()
	{
		bitset<32> final_data(0);
		for (int i = 0; i < 8; i++)
		{
			final_data[24 + i] = RM[i];
		}
		for (int i = 8; i < 16; i++)
		{
			final_data[8 + i] = RM[i];
		}

		int final_address = bitset_to_int(RZ);

		for (int i = 0; i < 16; i++)
		{
			data_MEM[max_size - final_address - MIN_MOD - i - 1] = final_data[31 - i];
		}

		cout << "MEMORY-";
		for (int i = 0; i < 32; i++)
		{
			cout << data_MEM[max_size - final_address - MIN_MOD - i - 1];
		}
		cout << endl;
	}

	void store_word()
	{
		bitset<32> final_data(0);
		for (int i = 0; i < 8; i++)
		{
			final_data[24 + i] = RM[i];
		}
		for (int i = 8; i < 16; i++)
		{
			final_data[8 + i] = RM[i];
		}
		for (int i = 16; i < 24; i++)
		{
			final_data[i - 8] = RM[i];
		}
		for (int i = 24; i < 32; i++)
		{
			final_data[i - 24] = RM[i];
		}

		int final_address = bitset_to_int(RZ);

		for (int i = 0; i < 32; i++)
		{
			data_MEM[max_size - final_address - MIN_MOD - i - 1] = final_data[31 - i];
		}
		cout << "MEMORY-";
		for (int i = 0; i < 32; i++)
		{
			cout << data_MEM[max_size - final_address - MIN_MOD - i - 1];
		}
		cout << endl;
	}

	bitset<32> load_byte()
	{
		bitset<32> final_data(0);
		int final_address = bitset_to_int(RZ);

		for (int i = 0; i < 8; i++)
		{
			final_data[i] = data_MEM[max_size - final_address - MIN_MOD - 1 - 7 + i];
		}

		for (int i = 8; i < 32; i++)
		{
			if (final_data[7])
				final_data[i] = 1;
			else
			{
				final_data[i] = 0;
			}
		}
		return final_data;
	}

	bitset<32> load_half()
	{
		bitset<32> final_data(0);
		int final_address = bitset_to_int(RZ);

		for (int i = 0; i < 8; i++)
		{
			final_data[i] = data_MEM[max_size - final_address - MIN_MOD - 1 - 7 + i];
		}

		for (int i = 8; i < 16; i++)
		{
			final_data[i] = data_MEM[max_size - final_address - MIN_MOD - 1 - 23 + i];
		}

		for (int i = 16; i < 32; i++)
		{
			if (final_data[15])
				final_data[i] = 1;
			else
			{
				final_data[i] = 0;
			}
		}
		return final_data;
	}

	bitset<32> load_word()
	{
		bitset<32> final_data(0);
		int final_address = bitset_to_int(RZ);
		//cout<<"finaladdress-"<<final_address;
		for (int i = 0; i < 8; i++)
		{
			final_data[i] = data_MEM[max_size - final_address - MIN_MOD - 1 - 7 + i];
		}

		for (int i = 8; i < 16; i++)
		{
			final_data[i] = data_MEM[max_size - final_address - MIN_MOD - 1 - 23 + i];
		}
		for (int i = 16; i < 24; i++)
		{
			final_data[i] = data_MEM[max_size - final_address - MIN_MOD - 1 - 39 + i];
		}

		for (int i = 24; i < 32; i++)
		{
			final_data[i] = data_MEM[max_size - final_address - MIN_MOD - 1 - 55 + i];
		}

		return final_data;
	}
	bitset<32> reverse(bitset<32> b)
	{
		for (int i = 0; i < 16; ++i)
		{
			bool t = b[i];
			b[i] = b[32 - i - 1];
			b[32 - i - 1] = t;
		}
		return b;
	}
	bitset<32> get_ins(bitset<100000> &mem1, int index)
	{
		bitset<32> temp;
		int i = 0;
		while (i < 32)
		{
			temp[i] = mem1[index + i];
			i++;
		}
		return (temp);
	}
	bitset<32> dMEM_access(bitset<100000> &mem1)
	{
		/////////////////////////////////////////			READ INS MEMORY
		bitset<32> final_data(0); ////////////DEKHNA HAI ABHI
		int final_address = bitset_to_int(MAR);
		final_data = get_ins(mem1, final_address);
		MDR = final_data; //////////////MEMACCESS DAALO
		return MDR;
	}
	void insertnop(bitset<100000> &mem1, bool df){
		int i = 0;
        string ins = "00000000000000000000000000010011" ;
        string a = "" ;
        string b = "" ;
		string mem = mem1.to_string();
        while (i < 100000-MAR.to_ulong()+32*5) {
            a = a + mem[i];
            i++;
        }
        while (i < 100000 ) {
            b = b + mem[i];
            i++;
        }
        string ans;
        if (df) {
            ans = a + ins + b;
            ans.erase(0, 32);
        }
        else {
            ans = a + ins + ins + b;
            ans.erase(0, 32*2);
        }
        mem1=bitset<100000>(ans);
	}
	bitset<32> MEM_access(bitset<32> _RZ, bitset<32> _RM, int control)
	{
		this->RZ = _RZ;
		this->RM = _RM;
		bitset<32> load_data(0);

		if (control == 22)
		{
			store_byte();
			flag=0;
		}
		else if (control == 23)
		{
			store_half();
			flag=0;
		}
		else if (control == 24)
		{
			store_word();
			flag=0;
		}
		else if (control == 25)
		{
			cout << "error";
		}
		else if (control == 15)
		{
			load_data = load_byte();
			flag=0;
		}
		else if (control == 16)
		{
			load_data = load_half();
			flag=0;
		}
		else if (control == 17)
		{
			load_data = load_word();
			flag=0;
		}
		else if (control == 18)
		{
			cout << "error";
		}
		else
		{
			flag=1;
			return 0;
		}
		return load_data;
	}
	void set(bitset<32> RZ, bitset<32> IAG, int cMuxMA)
	{
		bitset<32> MuxMA_result = MuxMA.select(RZ, IAG, cMuxMA); //mux control
		this->MAR = MuxMA_result;
		return;
	}
	bitset<32> getData(bitset<32> RM, int control)
	{
		this->MDR = MEM_access(this->MAR, RM, control); //////////////MEMACCESS DAALO
		return this->MDR;
	}
};