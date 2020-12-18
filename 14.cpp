//#define PARTONE
#include "template.hpp"

using Map = std::map<I64, I64>;

#ifdef PARTONE
static I64 mask_digits(const std::string &mask_string, I64 n)
{
	FOR(i, 36)  {
		switch (mask_string[i]) {
		case '0':
			n &= ~(1LL << (35 - i));
			break;
		case '1':
			n |= (1LL << (35 - i));
			break;
		}
	}
	return n;
}


int main(int argc, char **argv)
{
	const char *const fname = (argc >= 2) ? argv[1] : "in.txt";
	std::ifstream f(fname);
	if (! f.good()) {
		TRACE << "file cannot be opened" << std::endl;
		die();
	}
	Map mem;
	{
		std::string line;
		std::string mask;
		while (std::getline(f, line)) {
			if (line.empty())
				continue;
			if (line[1] == 'a') {
				// mask
				mask = &line[7];
			} else {
				// command
				const size_t end = line.find(']');
				if (end == std::string::npos) {
					TRACE << "bad line" << std::endl;
					trace(line);
					die();
				}
				const std::string index_s = line.substr(4, end  - 4);
				const I64 index = std::stoll(index_s.c_str(), nullptr, 10);
				const std::string value_s = line.substr(end + 4);
				const I64 value_no_mask = std::stoll(value_s.c_str(), nullptr, 10);
				const I64 value = mask_digits(mask, value_no_mask);
				mem[index] = value;
			}
		}
	}
	I64 sum = 0;
	for (auto kv : mem) {
		sum += kv.second;
	}
	trace(sum);
}

#else

int main(int argc, char **argv)
{
	const char *const fname = (argc >= 2) ? argv[1] : "in.txt";
	std::ifstream f(fname);
	if (! f.good()) {
		TRACE << "file cannot be opened" << std::endl;
		die();
	}
	Map mem;
	{
		std::string line;
		I64 mask = 0;
		std::vector<int> wild;
		while (std::getline(f, line)) {
			if (line.empty())
				continue;
			if (line[1] == 'a') {
				// mask
				wild.clear();
				mask = 0;
				const char *m = &line[7];
				FOR(i, 36) {
					switch (m[i]) {
					case 'X':
						wild.push_back(i);
						break;
					case '1':
						mask |= (1LL << (35 - i));
						break;
					}
				}

			} else {
				// command
				const size_t end = line.find(']');
				if (end == std::string::npos) {
					TRACE << "bad line" << std::endl;
					trace(line);
					die();
				}
				const std::string index_s = line.substr(4, end  - 4);
				const I64 index_no_mask = std::stoll(index_s.c_str(), nullptr, 10);
				const std::string value_s = line.substr(end + 4);
				const I64 value = std::stoll(value_s.c_str(), nullptr, 10);
				const int to = 1 << static_cast<int>(wild.size());
				FOR(i, to) {
					I64 index = index_no_mask | mask;
					FOR(j, wild.size()) {
						if (i & (1 << j)) {
							index |= 1LL << (35 - wild[j]);
						} else {
							index &= ~(1LL << (35 - wild[j]));
						}
					}
					mem[index] = value;
				}
			}
		}
	}
	I64 sum = 0;
	for (auto kv : mem) {
		sum += kv.second;
	}
	trace(sum);
}

#endif
