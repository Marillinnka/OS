#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
const char СharY = 'Y';
const char СharS = 'S';
const char CharQ = 'q';
const char CharNoTransition = '-';
const string fileInputName = "input2.txt";
const string fileOutputName = "output2.txt";

struct State
{
	int stateQ = 0;
	bool isEmpty = false;

	State()
	{
		isEmpty = true;
	}

	State(string inputStateQ)
	{
		auto it = find_if(inputStateQ.begin(), inputStateQ.end(), isdigit);
		if (it != inputStateQ.end()) stateQ = stoi(inputStateQ.c_str() + (it - inputStateQ.begin()));
	}

};

struct MooreAutomatonColumn
{
	int outSymbolY = 0;
	bool isEmpty = false;
	vector<State*> states = {};

	MooreAutomatonColumn()
	{
		isEmpty = true;
	}

	MooreAutomatonColumn(string outputSymbolY)
	{
		auto it = find_if(outputSymbolY.begin(), outputSymbolY.end(), isdigit);
		if (it != outputSymbolY.end()) outSymbolY = stoi(outputSymbolY.c_str() + (it - outputSymbolY.begin()));
	}
};

typedef vector<MooreAutomatonColumn*> MooreAutomaton;

int main()
{
	ifstream fileIn(fileInputName);
	ofstream fileOut(fileOutputName);
	MooreAutomaton inputAutomaton;

	int stateCount;
	fileIn >> stateCount;

	int inputSymbolCount;
	fileIn >> inputSymbolCount;

	for (size_t i = 0; i < stateCount; i++)
	{
		string outputSymbolY;
		fileIn >> outputSymbolY;

		if (outputSymbolY.front() != CharNoTransition)
			inputAutomaton.push_back(new MooreAutomatonColumn(outputSymbolY));

		else 
			inputAutomaton.push_back(new MooreAutomatonColumn());

		for (size_t j = 0; j < inputSymbolCount; j++)
		{
			string currentStateQ;
			fileIn >> currentStateQ;
			State* state;
			
			if (currentStateQ.front() != CharNoTransition)
				state = new State(currentStateQ);

			else
				state = new State();
			
			inputAutomaton[i]->states.push_back(state);

		}
	}

	size_t inputAutomatonSize = inputAutomaton.size();
	for (size_t i = 0; i < inputAutomatonSize; i++)
	{
		for (State* state : inputAutomaton[i]->states)
		{
			bool emptyState = state->isEmpty;
			if (emptyState)
			{
				fileOut << CharNoTransition << ' ';
				continue;
			}

			else
			{
				int numberS = state->stateQ;
				fileOut << СharS << numberS;
			}

			bool emptyStateQ = inputAutomaton[state->stateQ]->isEmpty;
			if (emptyStateQ)
				fileOut << ' ' << CharNoTransition << ' ';
			
			else
			{
				int numberY = inputAutomaton[state->stateQ]->outSymbolY;
				fileOut << ' ' << СharY << numberY << ' ';
			}
		}

		fileOut << "\n";
	}
}

