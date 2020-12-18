//#define PARTONE
#include "template.hpp"

enum OpKind {
	NOP, ACC, JMP
};

struct Op_t {
	OpKind op;
	int arg;
};

using Program_t = std::vector<Op_t>;

#ifndef PARTONE
static std::optional<int> program_exits(const Program_t &program)
{
	const int len = static_cast<int>(program.size());
	int acc = 0;
	int pc = 0;
	std::vector<bool> seen(program.size());
	for ( ; ; ) {
		if (pc == len) {
			return acc;
		}
		if (pc < 0 || pc >= len) {
			return {};
		}
		if (seen[pc]) {
			return {};
		}
		seen[pc] = true;
		const int arg = program[pc].arg;
		switch (program[pc].op) {
		case NOP:
			break;
		case ACC:
			acc += arg;
			break;
		case JMP:
			pc += arg - 1;
			break;
		}
		++pc;
	}
}
#endif

int main(int argc, char **argv)
{
	const char *const fname = (argc >= 2) ? argv[1] : "in.txt";
	std::ifstream f(fname);
	Program_t program;
	{
		std::string op;
		int arg;
		while (f >> op >> arg) {
			if (op == "nop") {
				program.push_back({ NOP, arg });
			} else if (op == "acc") {
				program.push_back({ ACC, arg });
			} else if (op == "jmp") {
				program.push_back({ JMP, arg });
			} else {
				TRACE << "strange op detected: " << op << std::endl;
				die();
			}
		}
	}
#ifdef PARTONE
	int acc = 0;
	int pc = 0;
	std::set<int> seen;
	while (pc >= 0 && pc < static_cast<int>(program.size())) {
		if (seen.find(pc) != seen.end()) {
			TRACE << "found dup" << std::endl;
			trace(pc);
			trace(acc);
			break;
		}
		seen.insert(pc);
		const int arg = program[pc].arg;
		switch (program[pc].op) {
		case NOP:
			break;
		case ACC:
			acc += arg;
			break;
		case JMP:
			pc += arg - 1;
			break;
		}
		++pc;
	}
#else
	FOR(i, program.size()) {
		switch (program[i].op) {
		case NOP: {
			program[i].op = JMP;
			auto res = program_exits(program);
			if (res) {
				trace(*res);
			}
			program[i].op = NOP;
			break;
		}
		case JMP: {
			program[i].op = NOP;
			auto res = program_exits(program);
			if (res) {
				trace(*res);
			}
			program[i].op = JMP;
			break;
		}
		case ACC:
			break;
		}
	}
#endif
}
