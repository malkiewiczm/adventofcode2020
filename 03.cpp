//#define PARTONE
#include "template.hpp"

int main(int argc, char **argv)
{
	const char *const fname = (argc >= 2) ? argv[1] : "in.txt";
	std::ifstream f(fname);
	if (! f.good()) {
		TRACE << "file cannot be opened" << std::endl;
		die();
	}
	std::vector<std::string> ins;
	{
		std::string line;
		while (std::getline(f, line)) {
			if (line.size() > 1) {
				ins.push_back(line);
			}
		}
	}
#ifdef PARTONE
	constexpr int RIGHT = 3;
	constexpr int DOWN = 1;
	int y = 0;
	int x = 0;
	int count = 0;
	while (y < static_cast<int>(ins.size())) {
		if (ins[y][x] == '#')
			++count;
		x = (x + RIGHT) % ins[y].size();
		y += DOWN;
	}
	trace(count);
#else
	const std::vector<std::pair<int, int>> PAIRS {
		{ 1, 1 }, { 3, 1 }, { 5, 1 }, { 7, 1 }, { 1, 2 }
	};
	int product = 1;
	for (auto pair : PAIRS) {
		const int RIGHT = pair.first;
		const int DOWN = pair.second;
		int y = 0;
		int x = 0;
		int count = 0;
		while (y < static_cast<int>(ins.size())) {
			if (ins[y][x] == '#')
				++count;
			x = (x + RIGHT) % ins[y].size();
			y += DOWN;
		}
		trace(count);
		product *= count;
	}
	trace(product);
#endif
}
