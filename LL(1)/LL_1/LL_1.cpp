//Программа LL(1). Выполнили студентки ПС-33: Макарова Марина и Седёлкина Анастасия.

#include <stack>
#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

struct Rule 
{
	char terminalCharacter;
	vector<char> guideCharacter;
	bool shift;
	int pointer;
	bool stack;
	bool error;
	bool end;
};

vector<Rule> rules = {
		//направляющие символы				сдвиг указ. стек   ошибка	конец
	{'E', { '8', '3', 'a', 'b', '-', '(' }, false,	2,	true,	true,	false }, //0
	{'\0', { '\0' },						true,	-1,	false,	true,	true },  //1
	{'T', { '8', '3', 'a', 'b', '-', '(' },	false,	5,	true,	true,	false }, //2
	{'A', { ')', '\0'},						false,	-1,	false,	false,	false},	 //3
	{'A', { '+' },							true,	2,	false,	true,	false }, //4
	{'F', { '8', '3', 'a', 'b', '-', '(' },	false,	8,	true,	true,	false }, //5
	{'B', { '+', ')', '\0'},				false,	-1,	false,	false,	false},  //6
	{'B', { '*' },							true,	5,	false,	true,	false }, //7
	{'-', { '-' },							true,	5,	false,	false,	false }, //8
	{'a', { 'a' },							true,	-1,	false,	false,	false }, //9
	{'b', { 'b' },							true,	-1,	false,	false,	false }, //10
	{'3', { '3' },							true,	-1,	false,	false,	false }, //11
	{'8', { '8' },							true,	-1,	false,	false,	false }, //12
	{'(', { '(' },							true,	2,	true,	true,	false }, //13
	{')', { ')' },							true,	-1,	false,	true,	false }, //14
};

int main()
{
	ifstream input("input.txt");
	string line;

	getline(input, line);

	stack<int> checkStack;
	int index = 0;
	int position = 0;

	while (true)
	{
		char symbol;

		if (position >= line.size())
		{
			symbol = '\0';
		}
		else
		{
			symbol = line[position];
		}

		if (symbol != ' ')
		{

			auto found = find(rules[index].guideCharacter.begin(), rules[index].guideCharacter.end(), symbol);

			if (rules[index].error && found == rules[index].guideCharacter.end())
			{
				if (index == 4 || index == 7)
				{
					cout << "Position: " << (position + 1) << " Found '" << symbol << "' Expected  '*', '+', ')', '\\0' \n";
					return 0;
				}

				string listChar = "";
				for (char ch : rules[index].guideCharacter)
				{
					if (ch == '\0')
					{
						listChar += "\\0, ";
					}
					else
					{
						listChar += string({ ch }) + ", ";
					}

				}
				cout << "Position: " << (position + 1) << " Found '" << symbol << "' Expected " << listChar << "\n";
				return 0;
			}

			if (!rules[index].error && found == rules[index].guideCharacter.end())
			{
				index++;
				continue;
			}

			if (rules[index].stack)
			{
				checkStack.push(index + 1);
			}

			if (rules[index].shift)
			{
				position++;
			}

			if (rules[index].end)
			{
				break;
			}

			if (rules[index].pointer != -1)
			{
				index = rules[index].pointer;
			}
			else
			{
				index = checkStack.top();
				checkStack.pop();
			}
		}
		else
		{
			position++;
		}
	}
	cout << "No errors\n";
}
