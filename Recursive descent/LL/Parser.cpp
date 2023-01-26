#include "Parser.h"

using namespace std;

Parser::Parser(string str): progText(ProgText(str))
{
}

string Parser::CreateMsg(string expected, string found, size_t pos)
{
	return "Expected: " + expected + " found: '" + found + "' position: " + to_string(pos) + "\n";
}

void Parser::OpenBracket(Token token)
{
	if (token.type != Type::OpenBracket)
	{
		throw CreateMsg("'('", token.value, progText.pos);
	}
}

void Parser::ClosedBracket(Token token)
{
	if (token.type != Type::ClosedBracket)
	{
		throw CreateMsg("')'", token.value, progText.pos);
	}
}

void Parser::Semicolon(Token token)
{
	if (token.type != Type::Semicolon)
	{
		throw CreateMsg("';'", token.value, progText.pos);
	}
}

void Parser::Colon(Token token)
{
	if (token.type != Type::Colon)
	{
		throw CreateMsg("':'", token.value, progText.pos);
	}
}

void Parser::Equal(Token token)
{
	if (token.type != Type::Equal)
	{
		throw CreateMsg("'='", token.value, progText.pos);
	}
}

void Parser::Id(Token token, string name)
{
	if (name != "")
	{
		if (token.type != Type::Identifier && token.value != name)
		{
			throw CreateMsg(name, token.value, progText.pos);
		}
	}
	else
	{
		if (token.type != Type::Identifier)
		{
			throw CreateMsg("Identifier", token.value, progText.pos);
		}
	}
}

void Parser::Begin(Token token)
{
	if (token.type != Type::Begin)
	{
		throw CreateMsg("'begin'", token.value, progText.pos);
	}
}

void Parser::End(Token token)
{
	if (token.type != Type::End)
	{
		throw CreateMsg("'end'", token.value, progText.pos);
	}

	token = progText.GetNextToken();
	if (token.type != Type::EndF)
	{
		throw CreateMsg("", token.value, progText.pos);
	}
}

void Parser::IDList(Token token)
{
	Id(token, "id");

	size_t pos = progText.pos;

	token = progText.GetNextToken();

	if (token.type == Type::Comma)
	{
		IDList(progText.GetNextToken());
	}
	else
	{
		progText.pos = pos;
	}
}

void Parser::DataType(Token token)
{
	if (token.type != Type::Type)
	{
		throw CreateMsg("Type", token.value, progText.pos);
	}
}

void Parser::Var(Token token)
{
	if (token.type != Type::Var)
	{
		throw CreateMsg("'var'", token.value, progText.pos);
	}

	IDList(progText.GetNextToken());

	Colon(progText.GetNextToken());

	DataType(progText.GetNextToken());

	Semicolon(progText.GetNextToken());
}

void Parser::Assign(Token token)
{
	Id(token, "id");
	Colon(progText.GetNextToken());
	if (isspace(progText.code[progText.pos]))
	{
		throw CreateMsg("':='", ":", progText.pos);
	}
	Equal(progText.GetNextToken());

	token = progText.GetNextToken();
	switch (token.type)
	{
	case Type::OpenBracket:
	case Type::Identifier:
	case Type::Minus:
		Exp(token);
		break;
	default:
		throw CreateMsg("Identifier or '(' or '-'", token.value, progText.pos);
	}
}

void Parser::Exp(Token token)
{
	switch (token.type)
	{
	case Type::OpenBracket:
	case Type::Identifier:
	case Type::Minus:
		T(token);
		token = progText.GetNextToken();
		switch (token.type)
		{
		case Type::Plus:
			Exp(progText.GetNextToken());
			break;
		case Type::ClosedBracket:
		case Type::Semicolon:
			progText.pos--;
			break;
		default:
			throw CreateMsg("'+' or ')' or ';'", token.value, progText.pos);
		}
		break;
	default:
		throw CreateMsg("Identifier or '(' or '-'", token.value, progText.pos);
	}
}

void Parser::T(Token token)
{
	switch (token.type)
	{
	case Type::OpenBracket:
	case Type::Identifier:
	case Type::Minus:
		F(token);
		token = progText.GetNextToken();
		switch (token.type)
		{
		case Type::Star:
			T(progText.GetNextToken());
			break;
		case Type::Plus:
		case Type::ClosedBracket:
		case Type::Semicolon:
			progText.pos--;
			break;
		default:
			throw CreateMsg("'*' or '+' or ')' or ';'", token.value, progText.pos);
		}
		break;
	default:
		throw CreateMsg("Identifier or '(' or '-'", token.value, progText.pos);
	}
}

void Parser::F(Token token)
{
	switch (token.type)
	{
	case Type::Identifier:
		return;
	case Type::OpenBracket:
		Exp(progText.GetNextToken());
		ClosedBracket(progText.GetNextToken());
		break;
	case Type::Minus:
		F(progText.GetNextToken());
		break;
	default:
		throw CreateMsg("Identifier or '(' or '-'", token.value, progText.pos);
	}
}

void Parser::IDListInBrackets(Token token)
{
	OpenBracket(token);

	IDList(progText.GetNextToken());

	ClosedBracket(progText.GetNextToken());
}

void Parser::St(Token token)
{
	if (token.type == Type::Read)
	{
		IDListInBrackets(progText.GetNextToken());
	}
	else if (token.type == Type::Write)
	{
		IDListInBrackets(progText.GetNextToken());
	}
	else if (token.type == Type::Identifier)
	{
		Assign(token);
	}
	else
	{
		throw CreateMsg("Statement", token.value, progText.pos);
	}
}

void Parser::ListSt(Token token)
{
	St(token);

	Semicolon(progText.GetNextToken());

	size_t pos = progText.pos;
	token = progText.GetNextToken();

	if (token.type != Type::End && token.type != Type::EndF)
	{
		ListSt(token);
	}
	else
	{
		progText.pos = pos;
	}
}

void Parser::Prog(Token token)
{
	if (token.type != Type::Prog)
	{
		throw CreateMsg("'prog'", token.value, progText.pos);
	}

	Id(progText.GetNextToken(), "id");

	Var(progText.GetNextToken());

	Begin(progText.GetNextToken());

	ListSt(progText.GetNextToken());

	End(progText.GetNextToken());
}