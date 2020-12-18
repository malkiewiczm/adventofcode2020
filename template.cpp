#include "template.hpp"

int main(int argc, char **argv)
{
	const char *const fname = (argc >= 2) ? argv[1] : "in.txt";
#ifdef A
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
	trace(ins.size());
#endif
#ifdef B
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
	trace(ins.size());
#endif
#ifdef C
	FILE *const f = fopen(fname, "r");
	if (f == nullptr) {
		TRACE << "file cannot be opened" << std::endl;
		die();
	}
	std::vector<int> ins;
	{
		int line;
		while (fscanf(f, "%d", &line) != EOF) {
			ins.push_back(line);
		}
		fclose(f);
	}
	trace(ins.size());
#endif
}
