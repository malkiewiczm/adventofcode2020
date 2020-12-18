//#define PARTONE
#include "template.hpp"

#define SET_HAS(set, key) ((set).find(key) != (set).end())

#ifdef PARTONE
static bool dfs(const std::set<int> &nums, std::map<int, int> &prev, int n, int target)
{
	if (n == target)
		return true;
	for (int i = 1; i <= 3; ++i) {
		const int m = n + i;
		if (SET_HAS(nums, m)) {
			prev[m] = n;
			if (dfs(nums, prev, m, target))
				return true;
		}
	}
	return false;
}

static std::vector<int> get_reverse_path(const std::map<int, int> &prev, int start_at)
{
	std::vector<int> ret;
	ret.push_back(start_at);
	for ( ; ; ) {
		auto iter = prev.find(start_at);
		if (iter == prev.end())
			break;
		ret.push_back(iter->second);
		start_at = iter->second;
	}
	return ret;
}
#else

static I64 dfs(const std::set<int> &nums, std::map<int, I64> &memo, int n, int target)
{
	{
		auto iter = memo.find(n);
		if (iter != memo.end())
			return iter->second;
	}
	if (n == target) {
		memo.insert({n, 1});
		return 1;
	}
	I64 ret = 0;
	for (int i = 1; i <= 3; ++i) {
		const int m = n + i;
		if (SET_HAS(nums, m)) {
			ret += dfs(nums, memo, m, target);
		}
	}
	memo.insert({n, ret});
	return ret;
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
	std::set<int> nums;
	int target = 0;
	{
		int line;
		int biggest = -10;
		while (f >> line) {
			if (line > biggest)
				biggest = line;
			nums.insert(line);
		}
		target = biggest + 3;
		nums.insert(target);
	}
#ifdef PARTONE
	std::map<int, int> prev;
	dfs(nums, prev, 0, target);
	const std::vector<int> path = get_reverse_path(prev, target);
	int number_ones = 0;
	int number_threes = 0;
	FOR2(i, 1, path.size()) {
		const int diff = path[i - 1] - path[i];
		if (diff == 1)
			++number_ones;
		else if (diff == 3)
			++number_threes;
	}
	trace(number_ones);
	trace(number_threes);
	trace(number_ones * number_threes);
#else
	std::map<int, I64> memo;
	const I64 res = dfs(nums, memo, 0, target);
	trace(res);
#endif
}
