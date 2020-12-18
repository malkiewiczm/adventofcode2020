//#define PARTONE
//#define PRINT_ME
#include "template.hpp"

enum TokenType {
	LPAREN, RPAREN, PLUS, MUL, NUMBER
};

struct Token {
	TokenType type;
	I64 value;
	Token(TokenType token_type)
		: type(token_type), value(0) {}
	Token(I64 l_value)
		: type(NUMBER), value(l_value) {}
};

static inline std::ostream &operator<< (std::ostream &o, const TokenType &type)
{
	switch (type) {
	case LPAREN:
		o << '(';
		break;
	case RPAREN:
		o << ')';
		break;
	case PLUS:
		o << '+';
		break;
	case MUL:
		o << '*';
		break;
	case NUMBER:
		o << "NUMBER";
		break;
	default:
		o << "???";
	}
	return o;
}

static inline std::ostream &operator<< (std::ostream &o, const Token &token)
{
	if (token.type == NUMBER) {
		o << "Number(" << token.value << ")";
	} else {
		o << token.type;
	}
	return o;
}

#ifndef PARTONE
static int precedence(const Token &token)
{
	if (token.type == PLUS)
		return 1;
	else
		return 0;
}
#endif

static I64 eval(const std::vector<Token> &rpn)
{
	std::vector<I64> stack;
	for (auto token : rpn) {
		if (token.type == NUMBER) {
			stack.emplace_back(token.value);
		} else {
			if (stack.size() <= 1) {
				TRACE << "too many operators not enough numbers" << std::endl;
				trace(stack.size());
				trace(stack);
				trace(rpn);
				die();
			}
			const I64 rhs = stack.back();
			stack.pop_back();
			const I64 lhs = stack.back();
			stack.pop_back();
			switch (token.type) {
			case PLUS:
				stack.emplace_back(lhs + rhs);
				break;
			case MUL:
				stack.emplace_back(lhs * rhs);
				break;
			default:
				TRACE << "unexpected operator at this time" << std::endl;
				trace(token);
				die();
			}
		}
	}
	if (stack.size() != 1) {
		TRACE << "multiple numbers left on stack" << std::endl;
		trace(stack.size());
		trace(stack);
		trace(rpn);
	}
	return stack.back();
}

int main(int argc, char **argv)
{
	const char *const fname = (argc >= 2) ? argv[1] : "in.txt";
	std::ifstream f(fname);
	if (! f.good()) {
		TRACE << "file cannot be opened" << std::endl;
		die();
	}
	I64 sum = 0;
	std::string line;
	while (std::getline(f, line)) {
		std::vector<Token> tokens;
		I64 current_num = 0;
		bool is_parsing_num = false;
		for (auto c : line) {
			if (c <= ' ') {
				continue;
			}
			if (c >= '0' && c <= '9') {
				is_parsing_num = true;
				current_num *= 10;
				current_num += c - '0';
				continue;
			}
			if (is_parsing_num) {
				tokens.emplace_back(current_num);
				current_num = 0;
				is_parsing_num = false;
			}
			switch (c) {
			case '(':
				tokens.emplace_back(LPAREN);
				break;
			case ')':
				tokens.emplace_back(RPAREN);
				break;
			case '+':
				tokens.emplace_back(PLUS);
				break;
			case '*':
				tokens.emplace_back(MUL);
				break;
			default:
				TRACE << "bad token" << std::endl;
				trace(c);
				die();
			}
		}
		if (is_parsing_num) {
			tokens.emplace_back(current_num);
			current_num = 0;
			is_parsing_num = false;
		}
		std::vector<Token> stack;
		std::vector<Token> output;
		for (auto token : tokens) {
			switch (token.type) {
			case NUMBER:
				output.emplace_back(token);
				break;
			case LPAREN:
				stack.emplace_back(token);
				break;
			case RPAREN:
				while (! stack.empty() && stack.back().type != LPAREN) {
					output.emplace_back(stack.back());
					stack.pop_back();
				}
				if (stack.empty()) {
					TRACE << "parenthesis mismatch" << std::endl;
					die();
				}
				if (stack.back().type == LPAREN) {
					stack.pop_back();
				}
				break;
			default:
				// operators
#ifdef PARTONE
				while (! stack.empty() && stack.back().type != LPAREN)
#else
				while (! stack.empty() && stack.back().type != LPAREN && precedence(stack.back()) > precedence(token))
#endif
				{
					output.emplace_back(stack.back());
					stack.pop_back();
				}
				stack.emplace_back(token);
			}
		}
		while (! stack.empty()) {
			output.emplace_back(stack.back());
			stack.pop_back();
		}
		const I64 res = eval(output);
#ifdef PRINT_ME
		trace(res);
#endif
		sum += res;
	}
	trace(sum);
}
