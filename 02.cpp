//#define PARTONE
#include "template.hpp"

#ifdef PARTONE
static bool is_valid(int low, int high, char letter, const char *buf)
{
	int count = 0;
	for (const char *c = buf; *c; ++c) {
		if (*c == letter) {
			++count;
			if (count > high)
				return false;
		}
	}
	if (count < low)
		return false;
	return true;
}
#else
static bool is_valid(int low, int high, char letter, const char *buf)
{
	return (buf[low - 1] == letter) != (buf[high - 1] == letter);
}
#endif

int main(int argc, char **argv)
{
	const char *const fname = (argc >= 2) ? argv[1] : "in.txt";
	FILE *const f = fopen(fname, "r");
	if (f == nullptr) {
		TRACE << "file cannot be opened" << std::endl;
		die();
	}
	{
		int low, high;
		char letter;
		char buf[500];
		int num_valid = 0;
		while (fscanf(f, "%d-%d %c: %s", &low, &high, &letter, buf) == 4) {
			if (is_valid(low, high, letter, buf)) {
				++num_valid;
			}
		}
		fclose(f);
		trace(num_valid);
	}
}
