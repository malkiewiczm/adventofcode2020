//#define PARTONE
#include "template.hpp"

#define INF(T) std::numeric_limits<T>::max()

#ifndef PARTONE
struct BusPair {
	I64 line;
	I64 order;
};

static inline std::ostream &operator<< (std::ostream &o, const BusPair &pair)
{
	o << "{ line: " << pair.line << ", order: " << pair.order << " }";
	return o;
}

#endif

int main(int argc, char **argv)
{
	const char *const fname = (argc >= 2) ? argv[1] : "in.txt";
	std::ifstream f(fname);
	if (! f.good()) {
		TRACE << "file cannot be opened" << std::endl;
		die();
	}
	int start_time;
	f >> start_time;
#ifdef PARTONE
	std::vector<int> bus_lines;
	{
		std::string line;
		std::getline(f, line);
		std::getline(f, line);
		size_t start = 0;
		size_t end;
		for (end = 0; (end = line.find(',', end)) != std::string::npos; ++end) {
			const std::string token = line.substr(start, end - start);
			start = end + 1;
			if (token[0] == 'x')
				continue;
			bus_lines.push_back(atoi(token.c_str()));
		}
		{
			const std::string token = line.substr(start, end - start);
			bus_lines.push_back(atoi(token.c_str()));
		}
	}
	int smallest = INF(int);
	int which_line = 0;
	for (auto bus : bus_lines) {
		const int amt = bus - (start_time % bus);
		if (amt < smallest) {
			smallest = amt;
			which_line = bus;
		}
	}
	trace(smallest * which_line);
#else
	std::vector<BusPair> bus_lines;
	{
		std::string line;
		std::getline(f, line);
		std::getline(f, line);
		size_t start = 0;
		size_t end;
		int i = -1;
		for (end = 0; (end = line.find(',', end)) != std::string::npos; ++end) {
			const std::string token = line.substr(start, end - start);
			start = end + 1;
			++i;
			if (token[0] == 'x')
				continue;
			bus_lines.push_back({ atoi(token.c_str()), i});
		}
		{
			const std::string token = line.substr(start, end - start);
			++i;
			bus_lines.push_back({ atoi(token.c_str()), i});
		}
	}
	I64 t = 0;
	int solved = 0;
	I64 step = 1;
	for ( ; ; ) {
		const BusPair &bus = bus_lines[solved];
		const I64 level = (t + bus.order) % bus.line;
		if (level == 0) {
			// we have solved this line
			++solved;
			if (solved == static_cast<int>(bus_lines.size()))
				break;
			step = std::lcm(step, bus.line);
		}
		t += step;
	}
	trace(t);
#endif
}
