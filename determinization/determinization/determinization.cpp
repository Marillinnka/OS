#include <vector>
#include <sstream>
#include <algorithm>
#include <set>
#include <map>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

typedef vector<int> Cell;
string input = "input.txt";
string output = "output.txt";
string noTransition = "-";

struct Position
{
	int x;
	int y;
};

struct Row
{
	Cell shortStateName;
	Cell fullStateName;
	vector<Cell> row;
};

string СonvertVector(const vector<int>& initialVector)
{
	ostringstream vectorStr;
	if (!initialVector.empty())
	{
		copy(initialVector.begin(), initialVector.end() - 1, ostream_iterator<int>(vectorStr));
		vectorStr << initialVector.back();
	}
	return vectorStr.str();
}

typedef vector<Row> Table;
set<int> visitedElements;

vector<int> Split(const string& convertibleString, char divider)
{
	stringstream stream(convertibleString);
	string ch;
	vector<int> vectorInt;

	while (getline(stream, ch, divider))
	{
		vectorInt.push_back(stoi(ch));
	}
	return vectorInt;
}

vector<int> CreateClosure(const Table& mainTable, int state)
{
	visitedElements.insert(state);
	vector<int> ecloseVector = { state };

	for (int i = 0; i < mainTable[state].row.back().size(); i++)
	{
		if (!visitedElements.count(mainTable[state].row.back()[i]))
		{
			vector<int> resultEcloseVector = CreateClosure(mainTable, mainTable[state].row.back()[i]);
			ecloseVector.insert(ecloseVector.end(), resultEcloseVector.begin(), resultEcloseVector.end());
		}
	}
	return ecloseVector;
}

vector<vector<int>> CreateClosures(const Table& mainTable)
{
	vector<vector<int>> ecloseVector;
	for (int i = 0; i < mainTable.size(); i++)
	{
		ecloseVector.push_back(CreateClosure(mainTable, i));
		visitedElements.clear();
	}
	return ecloseVector;
}

vector<int> UseECloseName(const vector<vector<int>>& ecloseVector, const vector<int>& states)
{
	vector<int> resultName;
	for (int state : states)
	{
		resultName.insert(resultName.end(), ecloseVector[state].begin(), ecloseVector[state].end());
	}
	return resultName;
}

int main()
{
	ifstream fileIn(input);
	ofstream fileOut(output);

	int stateCount;
	int symbolCount;
	fileIn >> stateCount;
	fileIn >> symbolCount;
	Table mainTable = vector<Row>(stateCount);
	string q;

	for (int i = 0; i < stateCount; i++)
	{
		mainTable[i].shortStateName.push_back(i);
		for (size_t j = 0; j < symbolCount + 1; j++)
		{
			fileIn >> q;
			if (q != noTransition)
				mainTable[i].row.push_back({ Split(q, ',') });

			else
				mainTable[i].row.push_back({});
		}
	}

	vector<vector<int>> ecloseVector = CreateClosures(mainTable);

	Row row;
	int currRow = 0;
	row.row = vector<Cell>(symbolCount);
	row.shortStateName = mainTable[currRow].shortStateName;

	bool newRow = true;
	set<vector<int>> visitedVector;
	Table curentTable = { row };

	map<string, string> statesMap = { {"", noTransition} };
	int stateMax = 0;

	while (newRow)
	{
		newRow = false;
		curentTable[currRow].fullStateName = UseECloseName(ecloseVector, curentTable[currRow].shortStateName);

		for (int state : curentTable[currRow].fullStateName)
		{
			for (int i = 0; i < symbolCount; i++)
			{
				if (mainTable[state].row[i].size() > 0)
				{
					curentTable[currRow].row[i].insert(curentTable[currRow].row[i].end(), mainTable[state].row[i].begin(), mainTable[state].row[i].end());
					sort(curentTable[currRow].row[i].begin(), curentTable[currRow].row[i].end());
					auto last = unique(curentTable[currRow].row[i].begin(), curentTable[currRow].row[i].end());
					curentTable[currRow].row[i].erase(last, curentTable[currRow].row[i].end());
				}
			}
		}

		string name = СonvertVector(curentTable[currRow].shortStateName);
		statesMap[name] = to_string(stateMax);
		stateMax++;

		int curentTableSize = curentTable.size();
		for (int i = 0; i < curentTableSize; i++)
		{
			for (int j = 0; j < curentTable[i].row.size(); j++)
			{
				if (curentTable[i].row[j].size() > 0 && !visitedVector.count(curentTable[i].row[j]))
				{
					currRow++;
					Row row;
					row.row = vector<Cell>(symbolCount);
					row.shortStateName = curentTable[i].row[j];

					visitedVector.insert(curentTable[i].row[j]);
					curentTable.push_back(row);
					newRow = true;
					i = 999;
					break;
				}
			}
		}
	}

	for (int i = 0; i < curentTable.size(); i++)
	{
		for (int j = 0; j < curentTable[i].row.size(); j++)
		{
			string name = СonvertVector(curentTable[i].row[j]);
			fileOut << statesMap[name];

			if (j != (curentTable[i].row.size() - 1))
				fileOut << " ";
		}
		fileOut << endl;
	}
}

