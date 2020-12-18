//#define PARTONE
#include "template.hpp"
#include <limits>

constexpr int PREFIX = 25;

using Vector = std::vector<I64>;

static bool is_in_prefix(const Vector &num, int index)
{
	FOR2(i, 1, PREFIX + 1) {
		FOR2(j, i + 1, PREFIX + 1) {
			if (num[index - i] + num[index - j] == num[index]) {
				return true;
			}
		}
	}
	return false;
}

static I64 find_target(const Vector &num)
{
	FOR2(i, PREFIX, num.size()) {
		if (! is_in_prefix(num, i)) {
			return num[i];
		}
	}
	TRACE << "no number found" << std::endl;
	die();
	return -1;
}

int main(int argc, char **argv)
{
	const char *const fname = (argc >= 2) ? argv[1] : "in.txt";
	std::ifstream f(fname);
	if (! f.good()) {
		TRACE << "file cannot be opened" << std::endl;
		die();
	}
	Vector num;
	{
		I64 n;
		while (f >> n) {
			num.push_back(n);
		}
	}
	const I64 target = find_target(num);
	trace(target);
#ifndef PARTONE
#ifdef BAD_TIME_COMPLEXITY
	FOR(i, num.size()) {
		FOR2(j, i + 1, num.size()) {
			I64 sum = 0;
			I64 biggest = std::numeric_limits<I64>::min();
			I64 smallest = std::numeric_limits<I64>::max();
			FOR2(k, i, j + 1) {
				sum += num[k];
				biggest = std::max(biggest, num[k]);
				smallest = std::min(smallest, num[k]);
			}
			if (sum == target) {
				trace(i);
				trace(j);
				trace(biggest);
				trace(smallest);
				trace(biggest + smallest);
			}
		}
	}
#else
	Vector prefix_sink(num.size() + 1);
	I64 *const prefix = &prefix_sink[1];
	prefix[-1] = 0;
	FOR(i, num.size()) {
		prefix[i] = prefix[i - 1] + num[i];
	}
	int start_index = 0;
	int end_index = 0;
	FOR(i, num.size()) {
		FOR2(j, i + 1, num.size()) {
			if (prefix[j] - prefix[i - 1] == target) {
				start_index = i;
				end_index = j;
			}
		}
	}
	I64 biggest = std::numeric_limits<I64>::min();
	I64 smallest = std::numeric_limits<I64>::max();
	FOR2(i, start_index, end_index) {
		biggest = std::max(biggest, num[i]);
		smallest = std::min(smallest, num[i]);
	}
	trace(start_index);
	trace(end_index);
	trace(biggest);
	trace(smallest);
	trace(biggest + smallest);
#endif
#endif
}
