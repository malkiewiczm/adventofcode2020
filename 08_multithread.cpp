#include "template.hpp"
#include <thread>

enum OpKind {
	NOP, ACC, JMP
};

struct Op {
	OpKind op;
	int arg;
};

using Program = std::vector<Op>;

static std::optional<int> program_exits(const Program &program)
{
	const int len = static_cast<int>(program.size());
	int acc = 0;
	int pc = 0;
	std::set<int> seen;
	for ( ; ; ) {
		if (pc == len) {
			return acc;
		}
		if (pc < 0 || pc >= len) {
			return {};
		}
		if (seen.find(pc) != seen.end()) {
			return {};
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
}

static void thread_main(const Program &program_readonly, int from, int to)
{
	Program program(program_readonly);
	FOR2(i, from, to) {
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
}

#define NEW_THREAD(n) std::thread(thread_main, program, step*(n), step*((n) + 1))
#define NEW_THREAD_LAST(n) std::thread(thread_main, program, step*(n), static_cast<int>(program.size()))

int main(int argc, char **argv)
{
	const char *const fname = (argc >= 2) ? argv[1] : "in.txt";
	std::ifstream f(fname);
	Program program;
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
	constexpr int num_threads = 8;
	const int step = program.size() / num_threads;
	std::thread t[num_threads];
	FOR(i, num_threads - 1) {
		t[i] = NEW_THREAD(i);
	}
	t[num_threads - 1] = NEW_THREAD_LAST(num_threads - 1);
	FOR(i, num_threads) {
		t[i].join();
	}
}
