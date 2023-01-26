#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include <map>
#include <fstream>

using namespace std;
string input = "input.txt";
string output = "output.txt";
char latticeChar = '#';
char starChar = '*';
char openBracket = '{';
char closeBracket = '}';
char stateS = 'S';
char stateH = 'H';
char stateF = 'F';
char rightGrammar = 'R';
char leftGrammar = 'L';
char separator = '|';

typedef map<size_t, map<char, vector<char>>> Table;
vector<string> SplitLine(string line);
void AddNewStateInTable(Table& table, map<char, size_t>& regulations, char regulation);

int main()
{
	ifstream fileIn(input);
	ofstream fileOut(output);
	Table table;
	vector<string> linesVector;
	string singleLine;
	map<char, size_t> regulations;
	map<char, size_t> inputSymbols;

	while (getline(fileIn, singleLine))
	{
		erase(singleLine, ' ');
		erase(singleLine, '-');

		replace(singleLine.begin(), singleLine.end(), latticeChar, openBracket);
		replace(singleLine.begin(), singleLine.end(), starChar, closeBracket);

		linesVector.push_back(singleLine);
	}

	char grammarType = linesVector[0][0];

	linesVector.erase(linesVector.begin());

	if (grammarType == rightGrammar)
	{
		AddNewStateInTable(table, regulations, stateS);
	}

	if (grammarType == leftGrammar)
	{
		AddNewStateInTable(table, regulations, stateH);
	}

	for (size_t i = 1; i < linesVector.size(); i++)
	{
		table.insert({ regulations.size(), {} });

		regulations.insert({ linesVector[i][0], regulations.size() });
	}

	if (grammarType == rightGrammar)
	{
		AddNewStateInTable(table, regulations, stateF);
	}

	if (grammarType == leftGrammar)
	{
		AddNewStateInTable(table, regulations, stateS);
	}

	for (string line : linesVector)
	{
		vector<string> conversionVector = SplitLine(line);
		if (grammarType == rightGrammar)
		{
			for (string conversion : conversionVector)
			{
				inputSymbols[conversion[0]];
				if (conversion.length() == 1)
				{
					table[regulations[line[0]]][conversion[0]].push_back(stateF);
				}
				if (conversion.length() == 2)
				{
					table[regulations[line[0]]][conversion[0]].push_back(conversion[1]);
				}
			}
		}
		else if (grammarType == leftGrammar)
		{
			for (string conversion : conversionVector)
			{
				if (conversion.length() == 1)
				{
					inputSymbols[conversion[0]];
					table[0][conversion[0]].push_back(line[0]);
				}

				if (conversion.length() == 2)
				{
					inputSymbols[conversion[1]];
					table[regulations[conversion[0]]][conversion[1]].push_back(line[0]);
				}
			}
		}
	}

	for (auto& [lineTableKey, mapTableValue] : table)
	{
		for (auto& [lineSymbolKey, mapSymbolValue] : inputSymbols)
			mapTableValue[lineSymbolKey];
	}

	for (auto& [lineTableKey, mapTableValue] : table)
	{
		for (auto& [rowTableKey, rowTableValue] : mapTableValue)
		{
			if (rowTableValue.size() == 0)
				fileOut << "-";

			else
			{
				for (size_t i = 0; i < rowTableValue.size(); i++)
				{
					fileOut << regulations[rowTableValue[i]];

					if (i < rowTableValue.size() - 1)
					{
						fileOut << ",";
					}

					else
					{
						fileOut << "";
					}
				}
			}

			if (mapTableValue.rbegin()->first != rowTableKey)
			{
				fileOut << " ";
			}

			else
			{
				fileOut << "";
			}
		}
		fileOut << endl;
	}
}

void AddNewStateInTable(Table& table, map<char, size_t>& regulations, char regulation)
{
	table.insert({ regulations.size(), {} });

	regulations.insert({ regulation, regulations.size() });
}

vector<string> SplitLine(string line)
{
	vector<string> linesVector;
	bool separatorFlag = true;

	for (int i = 1; i < line.length(); i++)
	{
		while (line[i] == separator)
		{
			separatorFlag = true;
			i++;
		}

		if (separatorFlag)
		{
			separatorFlag = false;
			linesVector.push_back(string({ line[i] }));
		}

		else
			linesVector.back() += line[i];
	}

	return linesVector;
}