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
	std::vector<int> ins;
	{
		int line;
		while (f >> line) {
			ins.push_back(line);
		}
	}
#ifdef PARTONE
	FOR(i, ins.size()) {
		FOR2(j, i + 1, ins.size()) {
			if (ins[i] + ins[j] == 2020) {
				trace(ins[i] * ins[j]);
			}
		}
	}
#else
	FOR(i, ins.size()) {
		FOR2(j, i + 1, ins.size()) {
			FOR2(k, j + 1, ins.size()) {
				if (ins[i] + ins[j] + ins[k] == 2020) {
					trace(ins[i] * ins[j] * ins[k]);
				}
			}
		}
	}
#endif
}
