#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <set>

using namespace std;
const string filefileInName = "input3.txt";
const string filefileOutName = "output3.txt";
const char СharY = 'Y';
const char CharQ = 'q';
const char Space = ' ';
const string CharNoTransition = "-";

struct MoorAutomatonColumn
{
	string nameGroup = "";
	vector<int> vectorSymbolY = {};
	int stateNumber = 0;
};

typedef vector<MoorAutomatonColumn> MooryAutomaton;
int GroupingTableByName(MooryAutomaton& actualTable);
MoorAutomatonColumn& GetColumnByGroup(MooryAutomaton& table, int group);

int main()
{
	ifstream fileIn(filefileInName);
	ofstream fileOut(filefileOutName);
	string symbolY;
	string inStateQ;
	int CountY = 0;

	int stateCount;
	fileIn >> stateCount;

	int inputSymbolCount;
	fileIn >> inputSymbolCount;

	int tableSize = stateCount + 1;

	MooryAutomaton mainTable;
	mainTable = vector<MoorAutomatonColumn>(tableSize);

	MooryAutomaton actualTable;
	actualTable = vector<MoorAutomatonColumn>(tableSize);

	MooryAutomaton tmpTable;

	for (int i = 0; i < stateCount; i++)
	{
		fileIn >> symbolY;
		actualTable[i].nameGroup = symbolY;
		mainTable[i].nameGroup = symbolY;

		int IntSymbolY = stoi(symbolY);
		if (IntSymbolY > CountY)
			CountY = stoi(symbolY);


		for (size_t j = 0; j < inputSymbolCount; j++)
		{
			fileIn >> inStateQ;

			if (inStateQ == CharNoTransition)
			{
				mainTable[i].vectorSymbolY.push_back(stateCount);
				actualTable[i].vectorSymbolY.push_back(stateCount);
			}

			else if (inStateQ != CharNoTransition)
			{
				mainTable[i].vectorSymbolY.push_back(stoi(inStateQ));
				actualTable[i].vectorSymbolY.push_back(stoi(inStateQ));
			}
		}

		actualTable[i].stateNumber = i;
	}

	actualTable.back().stateNumber = stateCount;
	string lastGroupY = to_string(CountY + 1);
	actualTable.back().nameGroup = lastGroupY;

	int countActualGroup = CountY + 1;
	int newTableGroup = 0;

	do
	{
		newTableGroup = countActualGroup;
		tmpTable.clear();

		for (int k = 0; k <= stateCount; k++)
		{
			tmpTable.push_back(MoorAutomatonColumn());

			if (k != stateCount)
			{
				for (size_t j = 0; j < inputSymbolCount; j++)
				{
					string groupName;
					groupName = actualTable[mainTable[k].vectorSymbolY[j]].nameGroup;

					int groupNameInt = stoi(groupName);
					tmpTable[k].vectorSymbolY.push_back(groupNameInt);
					tmpTable[k].nameGroup += groupName;
				}
			}
			tmpTable[k].stateNumber = k;
			tmpTable[k].nameGroup += actualTable[k].nameGroup;
		}

		countActualGroup = GroupingTableByName(tmpTable);
		actualTable = tmpTable;
	} while (countActualGroup != newTableGroup);

	for (int i = 0; i < countActualGroup; i++)
	{
		MoorAutomatonColumn column = GetColumnByGroup(actualTable, i);

		fileOut << mainTable[column.stateNumber].nameGroup << Space;

		for (int j = 0; j < inputSymbolCount; j++)
		{
			if (mainTable[column.stateNumber].vectorSymbolY.size() <= j)
			{
				return 1;
			}

			MoorAutomatonColumn currentColumn = actualTable[mainTable[column.stateNumber].vectorSymbolY[j]];
			int realMainTableSize = mainTable.size() - 1;

			if (currentColumn.stateNumber == realMainTableSize)
				fileOut << CharNoTransition;

			else
				fileOut << currentColumn.nameGroup;

			int compareInputSymbolCount = inputSymbolCount - 1;
			if (j != compareInputSymbolCount)
				fileOut << Space;
		}
		fileOut << "\n";
	}
}

int GroupingTableByName(MooryAutomaton& actualTable)
{
	set<int> visited;
	int stateNumber = 0;
	for (MoorAutomatonColumn column1 : actualTable)
	{
		if (!visited.count(stoi(column1.nameGroup)))
		{
			for (MoorAutomatonColumn& column2 : actualTable)
			{
				if ((column1.nameGroup == column2.nameGroup) && !visited.count(stoi(column2.nameGroup)))
				{
					column2.nameGroup = to_string(stateNumber);
				}
			}
			visited.insert(stateNumber);
			stateNumber++;
		}
	}
	return stateNumber;

}

MoorAutomatonColumn& GetColumnByGroup(MooryAutomaton& table, int group)
{
	for (MoorAutomatonColumn& column : table)
	{
		if (column.nameGroup == to_string(group))
			return column;
	}
}
