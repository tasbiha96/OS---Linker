#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <unordered_map>
#include <map>
#include <cctype>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

ifstream infile;
int Final_InstructCount = 0, p = 0, q = 0, m = 0, instruct_count = 0;
int Erroroffset = 1, _index_Seven = 0;
char Nmode = NULL;
char* nmode;
string UndefinedSymbol, warning_Seven;
char* previous_line;
int _index = 0, map_index = 0;

unordered_map <int, string> warning_seven;

unordered_map <string, int> SymbolTable;
unordered_map <string, int>::const_iterator it;

unordered_map <int, string> Uselist;
unordered_map <int, string>::const_iterator useit;

unordered_map <int, int> MemoryMap;
unordered_map <int, int>::const_iterator mmit;

unordered_map <string, string > ErrorSymbolTable;
unordered_map <string, string>::const_iterator errsymit;

unordered_map <int, string> ErrorMemoryMap;
unordered_map <int, string>::const_iterator errmmpit;

vector <string>::const_iterator searchit;
vector <string> Defined;
vector <string> Used;

vector <string> WarningList;
vector <string> ::const_iterator warnit;

vector <string> Deflist_Offsetsize_check;
vector <int> ::const_iterator offsetit;

vector <string> Deflist_symbol_order;
vector <string> ::const_iterator symorderit;

vector <string> Repeatedsymbol;
vector <string> ::const_iterator Repeatedsymbolit;

vector <int> MemoryMap_Indexcheck;
vector <int> ::const_iterator MemoryMapcheckit;

vector <int> MemoryMap_errorcheck;
vector <int> ::const_iterator MemoryMap_errorcheckit;

void MemoryMapCalc(char* mode, int address, int map_index);
void Error_SymbolTable(int rule, string Sym);
void Error_MemoryMap(int address, int rule, int map_index);
void CreateSymbolTable(vector<string> SymbolTable_final, int defcount);
void PrintSymbolTable();
void CreateMemoryMap(int address);
void PrintMemoryMap();
void ExtAddressCalc(int address, int map_index);
void parseerror(int Erroroffset, int line_num, int errcode);
void Warnings(string sym, string mod, int rule);
void PrintWarning_four();
void Warning_Seven(int module_count, string sym, int _index);
void pass1();
void pass2();

void CreateSymbolTable(vector<string> SymbolTable_final, int defcount)
{
	unordered_map <string, int>::const_iterator errit;
	string symbol;

	while (m < SymbolTable_final.size())
	{

		symbol = SymbolTable_final.at(m);

		int offset = stoi(SymbolTable_final.at(m + 1));

		errit = SymbolTable.find(symbol);
		if (errit != SymbolTable.end())
		{
			offset = errit->second;
			Error_SymbolTable(2, symbol);
		}

		SymbolTable[symbol] = offset;
		m = m + 2;

	}
}

void PrintSymbolTable()
{
	string CorrectSym;
	cout << "Symbol Table" << endl;
	for (int n = 0; n < Deflist_symbol_order.size(); n++)
	{
		CorrectSym = Deflist_symbol_order.at(n);
		Repeatedsymbolit = find(Repeatedsymbol.begin(), Repeatedsymbol.end(), CorrectSym);
		if (Repeatedsymbolit == Repeatedsymbol.end())
		{
			errsymit = ErrorSymbolTable.find(CorrectSym);
			it = SymbolTable.find(CorrectSym);

			if (errsymit != ErrorSymbolTable.end())
			{
				cout << it->first << "=" << it->second << " " << errsymit->second << endl;
				Repeatedsymbol.push_back(CorrectSym);
			}
			else

				cout << it->first << "=" << it->second << endl;
		}
	}
}

void CreateMemoryMap(int address)
{
	{
		
		MemoryMap[_index] = address;
	}
}

void PrintMemoryMap()
{
	
	int print_index = 0;
	cout << endl;
	cout << "Memory Map" << endl;
	int q = 0;
	while (q < MemoryMap_Indexcheck.size())
	{
		print_index = MemoryMap_Indexcheck.at(q);
	
		MemoryMap_errorcheckit = find(MemoryMap_errorcheck.begin(), MemoryMap_errorcheck.end(), print_index);
		errmmpit = ErrorMemoryMap.find(print_index);
		mmit = MemoryMap.find(print_index);
		if (mmit->first == (_index_Seven))
		{
			cout << warning_Seven << endl;
		}
		if (MemoryMap_errorcheckit != MemoryMap_errorcheck.end())
		{
		
			cout << setw(3) << setfill('0') << mmit->first << ": " << setw(4) << setfill('0') << *(MemoryMap_errorcheckit + 1) << " " << errmmpit->second << endl;
		}
		else
		{
		
			cout << setw(3) << setfill('0') << mmit->first << ": " << setw(4) << setfill('0') << mmit->second << endl;
		}

		q=q + 2;

	}
	
}

void ExtAddressCalc(int address, int map_index)
{
	unordered_map <int, string>::const_iterator ext_it;
	string sym;
	int digit[4];
	int opcode = 0; int operand = 0, Naddress = 0, Iaddress = 0;
	Iaddress = address;
	opcode = address / 1000;

	if (opcode >= 10)
	{
		address = 9999;
	
		Error_MemoryMap(address, 11, map_index);												
	}
	
	else
	{
		
		operand = address % 1000;

		for (int i = 3; i >= 1; i--)
		{
			digit[i] = address % 10;
			address = address / 10;

		}
		digit[0] = address;

		if (Uselist.size() < operand + 1)
		{
			Nmode = 'I';
			nmode = &Nmode;
		
			MemoryMapCalc(nmode, Iaddress, map_index);

			Error_MemoryMap(Iaddress, 6, map_index);
		}
		else
		{
			useit = Uselist.find(operand);
		
			if (useit != Uselist.end()) 
			{
				sym = useit->second;
				it = SymbolTable.find(sym);
				if (it != SymbolTable.end())
				{
					Naddress = it->second;
					
					address = address * 1000 + Naddress;
			
					CreateMemoryMap(address);
				}
				else
				{
					UndefinedSymbol = useit->second;
					Nmode = 'A';
					nmode = &Nmode;
				
					MemoryMapCalc(nmode, Iaddress, map_index);
					Error_MemoryMap(Iaddress, 3, map_index);
				}

			}
			
		}

	}
}


void MemoryMapCalc(char* mode, int address, int map_index)
{

	int absolute_add = 0;
	int relative_add = 0;
	int opcode = 0;
	switch (*mode)
	{
	case 'I':
		if (address >= 10000)
		{
			address = 9999;
			Error_MemoryMap(address, 10, map_index);
		}
		else
			address = address + 0;

		CreateMemoryMap(address);
		break;
	case 'A':
		absolute_add = address % 1000;
		if (absolute_add > 512)
		{
			address = address / 1000;
			address = address * 1000 + 0;
			Error_MemoryMap(address, 8, map_index);
		}
		else
			address = address + 0;

		CreateMemoryMap(address);
		break;
	case 'R':
		opcode = address / 1000;
		relative_add = address % 1000;
		if (opcode >= 10)
		{
			address = 9999;
			Error_MemoryMap(address, 11, map_index);												
			CreateMemoryMap(address);
		}
		else if (relative_add > instruct_count)
		{
			address = address / 1000;
			address = address * 1000;
			address = address + Final_InstructCount;
			Error_MemoryMap(address, 9, map_index);
			CreateMemoryMap(address);

		}
		else
		{
			address = address + Final_InstructCount;		
			CreateMemoryMap(address);
		}
		break;
	case 'E':

		ExtAddressCalc(address, map_index);
		break;

	}
}
void parseerror(int Erroroffset, int line_num, int errcode)
{
	const char* errstr[] = { "NUM_EXPECTED", "SYM_EXPECTED", "ADDR_EXPECTED", "SYM_TOO_LONG", "TOO_MANY_DEF_IN_MODULE", "TOO_MANY_USE_IN_MODULE", "TOO_MANY_INSTR" };
	cout << "Parse Error line " << line_num << " offset " << Erroroffset << ": " << errstr[errcode] << endl;
	exit(0);
}


void Error_SymbolTable(int rule, string Sym)
{

	string Error, err_one, err_two, err_three;

	if (rule == 2)
	{
		Error = "Error: This variable is multiple times defined; first value used";
		ErrorSymbolTable[Sym] = Error;
	}

}
void Error_MemoryMap(int address, int rule, int map_index)
{
	string Error, err_one, err_two, err_three;
	if (rule == 3)
	{
		err_one = "Error: ";
		err_two = " is not defined; zero used";
		Error = err_one + UndefinedSymbol + err_two;
		ErrorMemoryMap[map_index] = Error;
		MemoryMap_errorcheck.push_back(map_index);
		MemoryMap_errorcheck.push_back(address);

	}
	if (rule == 6)
	{
		Error = "Error: External address exceeds length of uselist; treated as immediate";


		ErrorMemoryMap[map_index] = Error;
		MemoryMap_errorcheck.push_back(map_index);
		MemoryMap_errorcheck.push_back(address);

	}
	if (rule == 8)
	{
		Error = "Error: Absolute address exceeds machine size; zero used";
		ErrorMemoryMap[map_index] = Error;
		MemoryMap_errorcheck.push_back(map_index);
		MemoryMap_errorcheck.push_back(address);

	}
	if (rule == 9)
	{
		Error = "Error: Relative address exceeds module size; zero used";
		ErrorMemoryMap[map_index] = Error;
		MemoryMap_errorcheck.push_back(map_index);
		MemoryMap_errorcheck.push_back(address);
	}
	if (rule == 10)

	{
		Error = "Error: Illegal immediate value; treated as 9999";
		ErrorMemoryMap[map_index] = Error;
		MemoryMap_errorcheck.push_back(map_index);
		MemoryMap_errorcheck.push_back(address);
		
	}
	if (rule == 11)
	{
		Error = "Error: Illegal opcode; treated as 9999";
		ErrorMemoryMap[map_index] = Error;
		MemoryMap_errorcheck.push_back(map_index);
		MemoryMap_errorcheck.push_back(address);
		
	}


}
void Warnings(string sym, string mod, int rule)
{

	string Error, err_one, err_two, err_three;
	if (rule == 4)
	{
		err_one = "Warning: Module ";
		err_two = ": ";
		err_three = " was defined but never used";
		Error = err_one + mod +err_two + sym + err_three;
	
		WarningList.push_back(Error);
	}

}

void PrintWarning_four()
{
	cout << endl;
	for (warnit = WarningList.begin(); warnit != WarningList.end(); warnit++)
		cout << *warnit << endl;
}

void Warning_Seven(int module_count, string sym, int _index)
{
	_index_Seven = _index;
	string err_one, err_two, err_three, err_four;
	err_one = "Warning: Module ";
	err_two = ": ";
	err_three = " appeared in the uselist but was not actually used";
	warning_Seven = err_one + to_string(module_count + 1) + err_two + sym + err_three;
	warning_seven[_index_Seven] = warning_Seven;
	


}



class Tokenizer
{
public:
	char* token;
	string line, next_line;

	char* str;
	
	int line_num = 0, previous_length = 0;
	int	module_count = 0;
	char* GetToken() //For creating tokens
	{
		if (token != NULL and !infile.eof())
		{
			token = strtok(NULL, " \t\n");

			if (token != NULL)
			{
				Erroroffset = token - str + 1;
				return token;
			}
		}

		if (token == NULL)
		{
			Erroroffset = 1;
			getline(infile, line);

			if (line.length() == 0)
			{

				while (line.length() == 0 && !infile.eof())
				{
					getline(infile, line);
					previous_length = line.length();
					line_num++;
				}
			}
			if (!infile.eof())
			{
				
				line_num++;
				previous_length = line.length();
				str = new char[line.length() + 1];
				strcpy(str, line.c_str());
				token = strtok(str, " \t\n");
				Erroroffset = str - token + 1;

				if (token == NULL)
				{
					Erroroffset = Erroroffset + 0;
					return NULL;
				}
				else
				{
					return token;
				}
			}
			else
			{
				
				if (previous_length == 0)
				{
					Erroroffset = 1;
				}
				return NULL;
			}
		}
	}



	int ReadInt()
	{
			char* token = GetToken();
	
			if (token == NULL || infile.eof())
			{
				return -1;
			}
			else
			{
			
				for (int j = 0; j < strlen(token); j++)
				{

					if (!(isdigit(token[j])))
					{
						parseerror(Erroroffset, line_num, 0);
					}
					else
					{
						Erroroffset++;
					}
				}

				return stoi(token);

			
		}
	}

	char* ReadSym()
	{

		char* token = GetToken();
	
		if (token == NULL && previous_length > 0)
		{
			Erroroffset = Erroroffset + previous_length;
			parseerror(Erroroffset, line_num, 1);
		}
		if (token == NULL && previous_length == 0)
		{
			parseerror(Erroroffset, line_num, 1);
		}

		if (!isalpha(token[0]))
		{
			
			parseerror(Erroroffset, line_num, 1);
		}
		else
		{
			Erroroffset = Erroroffset - strlen(token);
			if (isalpha(token[0]))
			{
				Erroroffset++;
				for (int k = 1; k < strlen(token); k++)
				{
					Erroroffset++;
					if (!isalnum(token[k]))
					{
						parseerror(Erroroffset, line_num, 1);
					}
				}
			}
		}

		if (strlen(token) > 16)
		{
			parseerror(Erroroffset, line_num, 3);
		}
		return token;
	}


	char* ReadIEAR()
	{
		char* token = GetToken();
		
		if (token == NULL)
		{
			parseerror(Erroroffset, line_num, 2);
		}
		else if (((*token == 'R') || (*token == 'I') || (*token == 'A') || (*token == 'E')))
		{
			return token;
		}
		else
		{
			parseerror(Erroroffset, line_num, 2);
		}

	}

};

void pass1()
{
	int defcount = 0, usecount = 0, instr_count = 0, j = 0, base_address = 0, initial_instruct_count = 0, val = 0, initial_val = 0;
	vector <string> ::const_iterator symbolorder;
	Tokenizer tok1;

	if (infile.fail())
	{
		cout << "Error opening file" << endl;
	}

	else
	{

		while (!infile.eof())

		{
			defcount = tok1.ReadInt();

			if (defcount < 0)   //Checking for EOF
			{
				
			break;
		}
			else if (defcount > 16)
				parseerror(Erroroffset, tok1.line_num, 4);
			else
			{
				
				for (j = 0; j < defcount; j++)
				{
					char* Sym = tok1.ReadSym();
					Deflist_Offsetsize_check.push_back(Sym);

					Deflist_symbol_order.push_back(Sym);
					initial_val = tok1.ReadInt();

					val = base_address + initial_val;

					Deflist_Offsetsize_check.push_back(to_string(val));

				}
				usecount = tok1.ReadInt();
				if (usecount > 16)
				{
					parseerror(Erroroffset, tok1.line_num, 5);
				}


				for (int i = 0; i < usecount; i++)
				{

					tok1.ReadSym();
				}

				instr_count = tok1.ReadInt();
				
				base_address = base_address + instr_count;

				if (base_address > 512)
				{
					parseerror(Erroroffset, tok1.line_num, 6);
				}

				for (int i = 0; i < instr_count; i++)
				{
					
					tok1.ReadIEAR();
					int test = tok1.ReadInt();

				}
				
				tok1.module_count++;
			}



			while (q < Deflist_Offsetsize_check.size())
			{
				if (initial_val > instr_count)
				{
					string initial_offset = Deflist_Offsetsize_check.at(q + 1);


					Deflist_Offsetsize_check.at(q + 1) = to_string(initial_instruct_count);

					cout << "Warning: Module " << tok1.module_count << ": " << Deflist_Offsetsize_check.at(q) << " too big " << initial_val << " (max=" << instr_count - 1 << ")" << " assume zero relative" << endl;
				}
				q = q + 2;
			}
			initial_instruct_count = initial_instruct_count + instr_count;
		
			CreateSymbolTable(Deflist_Offsetsize_check, defcount);

		}

	}


	infile.close();
}

void pass2()
{
	
	Tokenizer tok2;
	int defcount = 0, i = 0, usecount = 0, initial_count = 0;
	string sym, mod, used_sym;
	vector <string> mode_check;
	vector <string> ::const_iterator mode_checkit;


	char* symbol;

	while (!infile.eof())
	{
		
		defcount = tok2.ReadInt();
		if (defcount < 0)   //Checking for EOF
		{
			
			break;
		}
		for (i = 0; i < defcount; i++)
		{
			
			Defined.push_back(tok2.ReadSym());
			Defined.push_back(to_string(tok2.module_count + 1));

			tok2.ReadInt();
		}

		usecount = tok2.ReadInt();
		
		for (i = 0; i < usecount; i++)
		{
			symbol = tok2.ReadSym();
			Used.push_back(symbol);
			Uselist[i] = symbol;
		}

		instruct_count = tok2.ReadInt();
		
		for (int i = 0; i < instruct_count; i++)
		{
			
			char* mode = tok2.ReadIEAR();
		
			mode_check.push_back(mode);

			int address = tok2.ReadInt();
			
			MemoryMap_Indexcheck.push_back(map_index);
		
			MemoryMap_Indexcheck.push_back(address);
				
			MemoryMapCalc(mode, address, map_index);
			map_index++;
			_index++;
		}


		Final_InstructCount = Final_InstructCount + instruct_count;

		string Emode = "E";
		if (usecount > 0)
		{	
			mode_checkit = find(mode_check.begin(), mode_check.end(), Emode);
			for (int t = 0; t < usecount; t++)
				used_sym = Used[t];
			if (mode_checkit == mode_check.end())
				Warning_Seven(tok2.module_count, used_sym, _index);
		}
		/*if (Emode == "E")
		{
			for (int r = 0; r < Used.size(); r++)
			{
				if (Uselist.find(r) == Uselist.end())
				{
					for (i = 0; i < usecount; i++)

						used_sym = Used[i];
					Warning_Seven(tok2.module_count, used_sym, _index +1);

				}
			}

		}*/

		tok2.module_count++;
		
	}

	p = 0;
	while (p < (Defined.size()))
	{

		searchit = find(Used.begin(), Used.end(), Defined.at(p));

		if (searchit == Used.end())
		{
			sym = Defined.at(p);

			mod = Defined.at(p + 1);

			Warnings(sym, mod, 4);
		}
		p = p + 2;

	}


	infile.close();
}





int main(int argc, char* argv[])
{

	infile.open(argv[1]);
	pass1();

	PrintSymbolTable();

	infile.open(argv[1]);
	pass2();

	PrintMemoryMap();

	PrintWarning_four();

	return 0;

}








