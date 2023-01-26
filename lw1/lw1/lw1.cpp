#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

const char СharY = 'Y';
const char CharQ = 'q';
const char CharNoTransition = '-';
const string fileInputName = "input2.txt";
const string fileOutputName = "output2.txt";

struct State
{
	int stateS = 0;
	int stateQ = 0;
	int outSymbolY = 0;

	string StrStateS = "";
	string StrOutSymbolY = "";
	bool isEmpty = false;

	State()
	{
		isEmpty = true;
	}

	State(string inputStateS, string outputSymbolY)
	{
		auto it = find_if(inputStateS.begin(), inputStateS.end(), isdigit);
		if (it != inputStateS.end()) stateS = stoi(inputStateS.c_str() + (it - inputStateS.begin()));
		StrStateS = to_string(stateS);

		it = find_if(outputSymbolY.begin(), outputSymbolY.end(), isdigit);
		if (it != outputSymbolY.end()) outSymbolY = stoi(outputSymbolY.c_str() + (it - outputSymbolY.begin()));
		StrOutSymbolY = to_string(outSymbolY);

		stateQ = stoi(StrStateS + StrOutSymbolY);
	}

};

typedef vector<vector<State*>> MealyAutomaton;
bool SortStates(const State* firstState, const State* secondState);
State* GetState(vector<State*> states, int q);
vector<State*> GetStateName(vector<State*>& statesVector);

int main()
{
	ifstream fileIn(fileInputName);
	ofstream fileOut(fileOutputName);
	MealyAutomaton inputAutomaton;
	vector<State*> vectorStates;

	int stateCount;
	fileIn >> stateCount;

	int inputSymbolCount;
 	fileIn >> inputSymbolCount;

	for (size_t i = 0; i < stateCount; i++)
	{
		inputAutomaton.push_back(vector<State*>());
		for (size_t j = 0; j < inputSymbolCount; j++)
		{
			string currentStateS;
			string outputSymbolY;
			fileIn >> currentStateS;

			if (currentStateS[0] == CharNoTransition)
			{
				State* state = new State();
				inputAutomaton[i].push_back(state);
			}

			if (currentStateS[0] != CharNoTransition)
			{
				fileIn >> outputSymbolY;
				State* state = new State(currentStateS, outputSymbolY);

				inputAutomaton[i].push_back(state);
				vectorStates.push_back(state);
			}
			
		}
	}
	sort(vectorStates.begin(), vectorStates.end(), SortStates);
	vectorStates = GetStateName(vectorStates);

	auto lastState = vectorStates.back();

	for (int a = 0; a <= lastState->stateQ; a++)
	{
		State* state = GetState(vectorStates, a);
		int numberY = state->outSymbolY;

		fileOut << СharY << numberY << ' ';

		for (size_t m = 0; m < inputSymbolCount; m++)
		{
			int numberQ = inputAutomaton[state->stateS][m]->stateQ;
			bool isEmpty = inputAutomaton[state->stateS][m]->isEmpty;
			bool separator = (m + 1 == inputSymbolCount);

			if (isEmpty)
			{
				fileOut << CharNoTransition;
			}
				
			else
			{
				fileOut << CharQ << numberQ;
			}

			if (!separator)
			{
				fileOut << ' ';
			}
		}
		fileOut << "\n";
	}
}

bool SortStates(const State* firstState, const State* secondState)
{
	int FirstStateQ = firstState->stateQ;
	int SecondStateQ = secondState->stateQ;
	bool result = SecondStateQ > FirstStateQ;

	return result;
}

vector<State*> GetStateName(vector<State*>& statesVector)
{
	int getIndexQ = 0;
	size_t statesSize = statesVector.size();

	for (size_t a = 0; a < statesSize; a++)
	{
		if (((a + 1) < statesSize) && (statesVector[a]->stateQ == statesVector[a+1]->stateQ))
			statesVector[a]->stateQ = getIndexQ;

		else
		{
			statesVector[a]->stateQ = getIndexQ;
			getIndexQ++;
		}
	}

	return statesVector;
}

State* GetState(vector<State*> statesVector, int qState)
{
	size_t statesVectorSize = statesVector.size();

	for (size_t a = 0; a <= statesVectorSize; a++)
	{
		if (statesVector[a]->stateQ == qState)
			return statesVector[a];
	}

	return new State();
}