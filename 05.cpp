//#define PARTONE
#include "template.hpp"

#define avg(a, b) ((a) + ((b) - (a)) / 2)

#define SET_HAS(set, what) ((set).find(what) != (set).end())

static int get_id(const std::string &code)
{
	std::pair<int, int> low { 0, 0 };
	std::pair<int, int> high { 127, 7 };
	for (auto c : code) {
		switch (c) {
		case 'F':
			high.first = avg(low.first, high.first);
			break;
		case 'B':
			low.first = avg(low.first, high.first);
			break;
		case 'R':
			low.second = avg(low.second, high.second);
			break;
		case 'L':
			high.second = avg(low.second, high.second);
			break;
		}
	}
	return high.first * 8 + high.second;
}

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
			ins.push_back(line);
		}
	}
#ifdef PARTONE
	int biggest = 0;
	for (auto code : ins) {
		biggest = std::max(biggest, get_id(code));
	}
	trace(biggest);
#else
	std::set<int> seats;
	for (auto code : ins) {
		seats.insert(get_id(code));
	}
	FOR(row, 128) {
		FOR(col, 8) {
			const int seat = row * 8 + col;
			if (SET_HAS(seats, seat)) {
				continue;
			}
			if (SET_HAS(seats, seat - 1) && SET_HAS(seats, seat + 1)) {
				trace(seat);
			}
		}
	}
#endif
}
