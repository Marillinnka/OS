#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <set>

using namespace std;
const char СharY = 'Y';
const char CharQ = 'q';
const char Space = ' ';
const string CharNoTransition = "-";
const string fileInputName = "input1.txt";
const string fileOutputName = "output1.txt";

struct State 
{
	int stateS = 0;
	int outSymbolY = 0;

	State(int inputStateS, int outputSymbolY)
	{
		stateS = inputStateS;
		outSymbolY = outputSymbolY;
	}
};

struct MealyAutomatonColumn
{
	string nameGroup = "";
	vector<int> vectorSymbolY = {};
	int stateNumber = 0;
};

typedef vector<vector<State>> MealyAutomaton;
typedef vector<MealyAutomatonColumn> Table;
int GroupingTableByName(Table& actualTable);
MealyAutomatonColumn& GetColumnByGroup(Table& table, int group);

int main()
{
	ifstream fileIn(fileInputName);
	ofstream fileOut(fileOutputName);

	int stateCount;
	fileIn >> stateCount;

	int inputSymbolCount;
	fileIn >> inputSymbolCount;

	int tableSize = stateCount + 1;

	MealyAutomaton mainTable;
	mainTable = vector<vector<State>>(tableSize);

	Table actualTable;
	actualTable = vector<MealyAutomatonColumn>(tableSize);

	Table tmpTable;
	string inStateS;
	int symbolY;
	bool isEmpty = false;

	for (int a = 0; a < stateCount; a++)
	{
		for (size_t m = 0; m < inputSymbolCount; m++)
		{
			fileIn >> inStateS;

			if (inStateS == CharNoTransition)
			{
				isEmpty = true;
				State nullState = State(stateCount, -1);
				mainTable[a].push_back(nullState);

				actualTable[a].nameGroup += CharNoTransition;
				actualTable[a].vectorSymbolY.push_back(stateCount);
			}

			else if (inStateS != CharNoTransition)
			{
				fileIn >> symbolY;

				State newState = State(stoi(inStateS), symbolY);
				mainTable[a].push_back(newState);

				string symbolStrY = to_string(symbolY);
				actualTable[a].nameGroup += symbolStrY;
				actualTable[a].vectorSymbolY.push_back(symbolY);
			}
		}
		actualTable[a].stateNumber = a;
	}
	actualTable.back().stateNumber = stateCount;
    string lastGroupY = to_string(stateCount);
	actualTable.back().nameGroup = lastGroupY;
	int countActualGroup = GroupingTableByName(actualTable);
	int newTableGroup = 0; 

	do
	{
		newTableGroup = countActualGroup;
		tmpTable.clear();

		for (int k = 0; k < stateCount + 1; k++)
		{
			tmpTable.push_back(MealyAutomatonColumn());

			if (k != stateCount)
			{
				for (size_t l = 0; l < inputSymbolCount; l++)
				{
					string groupName;
					groupName = actualTable[mainTable[k][l].stateS].nameGroup;
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

	if (isEmpty)
	{
		countActualGroup--;
	}

	for (int i = 0; i < countActualGroup; i++)
	{
		MealyAutomatonColumn column = GetColumnByGroup(actualTable, i);

		for (int j = 0; j < inputSymbolCount; j++)
		{
			MealyAutomatonColumn currentColumn = actualTable[mainTable[column.stateNumber][j].stateS];
			int realMainTableSize = mainTable.size() - 1;

			if (currentColumn.stateNumber != realMainTableSize)
				fileOut << currentColumn.nameGroup << Space << mainTable[column.stateNumber][j].outSymbolY;
				
			else if (currentColumn.stateNumber == realMainTableSize)
				fileOut << CharNoTransition;

			int compareInputSymbolCount = inputSymbolCount - 1;
			if (j != compareInputSymbolCount)
				fileOut << Space;
		}

		fileOut << "\n";
	}
}

MealyAutomatonColumn& GetColumnByGroup(Table& table, int group)
{
	for (MealyAutomatonColumn& column : table)
	{
		if (column.nameGroup == to_string(group))
			return column;
	}
}

int GroupingTableByName(Table& actualTable)
{
	set<string> visited;
	int stateNumber = 0;
	for (MealyAutomatonColumn column1 : actualTable)
	{
		if (!visited.count(column1.nameGroup))
		{
			for (MealyAutomatonColumn& column2 : actualTable)
			{
				if ((column1.nameGroup == column2.nameGroup) && !visited.count(column2.nameGroup))
				{
					column2.nameGroup = to_string(stateNumber);
				}
			}
			visited.insert(to_string(stateNumber));
			stateNumber++;
		}
	}
	return stateNumber;
}