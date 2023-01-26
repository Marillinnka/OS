#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <fstream>

using namespace std;

set<string> singleCharacters = { "(", ")", "{", "}", "+", "-", "*", "/", "=", ">", "<", ";" };
set<string> doubleCharacters = { "<<", ">>", "==", "!=", "||", "&&", "<=", ">=" };
string input = "input.txt";
string output = "output.txt";

map<string, string> setOfTokens =
{
	{ "(", "Left parenthesis" },
	{ ")", "Right parenthesis" },
	{ "{", "Left brace" },
	{ "}", "Right brace" },
	{ "+", "Addition sign" },
	{ "-", "Subtraction sign" },
	{ "*", "Asterisk sign" },
	{ "/", "Slash" },
	{ "=", "Appropriation" },
	{ "!=", "Not equal" },
	{ "==", "Equality" },
	{ ">", "More sign" },
	{ "<", "Less sign" },
	{ "<=", "Less or equality sign" },
	{ ">=", "More or equality sign" },
	{ ">>", "Right bit shift" },
	{ "<<", "Left bit shift" },
	{ "||", "Logical OR" },
	{ "&&", "Logical AND" },
	{ ";", "Semicolon" },
	{ "return", "Return" },
	{ "true", "True" },
	{ "false", "False" },
	{ "bool", "Type" },
	{ "string", "Type" },
	{ "int", "Type" },
	{ "char", "Type" },
	{ "if", "If" },
	{ "while", "While" },
	{ "for", "For" },
	{ "break", "Break" },
	{ "auto", "Auto" },
	{ "void", "Void" },
	{ "const", "Const" },
};

void AddWordToTocenVector(string& word, vector<string>& tokensVector);

bool IntToken(const string& line);

bool FloatToken(const string& line);

bool StringToken(const string& line);

bool IdentifierToken(const string& line);

int main()
{
	setlocale(LC_ALL, "Russian");

	ifstream fileIn(input);
	ofstream fileOut(output);
	string word = "";
	vector<string> tokensVector;

	string programText{ istreambuf_iterator<char>(fileIn), istreambuf_iterator<char>() };
	programText += '\n';
	transform(programText.begin(), programText.end(), programText.begin(), tolower);

	size_t programLenght = programText.length();
	for (size_t m = 0; m < programLenght; m++)
	{
		if (isspace(programText[m]))
		{
			AddWordToTocenVector(word, tokensVector);
			continue;
		}

		if (programText[m] == '"')
		{
			AddWordToTocenVector(word, tokensVector);
			for (size_t k = m + 1; k < programLenght; k++)
			{
				if (programText[k] == '\n' || programText[k] == '"')
				{
					m = k;
					break;
				}
				word = word + programText[k];
			}
			tokensVector.push_back("'" + word + "'");
			word = "";
			continue;
		}

		if (m + 1 < programLenght)
		{
			if (doubleCharacters.count(string({ programText[m], programText[m + 1] })))
			{
				AddWordToTocenVector(word, tokensVector);
				tokensVector.push_back(string({ programText[m], programText[m + 1] }));
				m++;
				continue;
			}
		}

		if (singleCharacters.count(string({ programText[m] })))
		{
			AddWordToTocenVector(word, tokensVector);
			tokensVector.push_back(string({ programText[m] }));
			continue;
		}

		word = word + programText[m];
	}

	for (string token : tokensVector)
	{
		if (setOfTokens.count(token))
		{
			fileOut << token  << " - " << setOfTokens[token] << endl;
			continue;
		}

		if (IdentifierToken(token))
		{
			fileOut << token << " - "  <<  "Identifier"  << endl;
			continue;
		}

		if (StringToken(token))
		{
			fileOut << token  << " - " << "String" << endl;
			continue;
		}

		if (FloatToken(token))
		{
			fileOut << token << " - " << "Float"  << endl;
			continue;
		}

		if (IntToken(token))
		{
			fileOut << token << " - " << "Integer" << endl;
			continue;
		}

		fileOut << token  << " - " << "Error" << endl;
	}

}

bool StringToken(const string& line)
{
	bool result = (line.front() == '\'') && (line.back() == '\'');
	return result;
}

bool FloatToken(const string& line)
{
	int separator = 0;
	int lineLength = line.length();

	for (int i = 1; i < lineLength; i++)
	{
		if (!isdigit(line[i]))
		{
			if (line[i] == '.')
			{
				separator++;
				if (separator > 1)
					return false;
			}

			else
			{
				return false;
			}
		}
	}
	return isdigit(line.front());
}

bool IdentifierToken(const string& line)
{
	if (!isalpha(line.front()) && line.front() != '_')
		return false;

	size_t lineLength = line.length();
	for (size_t i = 1; i < lineLength; i++)
	{
		if (!isdigit(line[i]) && !isalpha(line[i]) && line.front() != '_')
			return false;
	}

	return true;
}

bool IntToken(const string& line)
{
	for (size_t i = 0; i < line.length(); i++)
	{
		if (!isdigit(line[i]))
			return false;
	}

	return true;
}

void AddWordToTocenVector(string& word, vector<string>& tokensVector)
{
	if (word != "")
	{
		tokensVector.push_back(word);
		word = "";
	}
}