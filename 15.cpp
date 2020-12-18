//#define PARTONE
//#define TEST_CASE
#include "template.hpp"

int main()
{
#ifdef PARTONE
	constexpr int ENDING_TURN = 2020;
#else
	constexpr int ENDING_TURN = 30000000;
#endif
#ifdef TEST_CASE
	const std::vector<int> starting_nums { 0, 3, 6 };
#else
	const std::vector<int> starting_nums { 6, 3, 15, 13, 1, 0 };
#endif
	std::map<int, int> seen;
	int turn_number = 1;
	for (auto num : starting_nums) {
		seen[num] = turn_number;
		++turn_number;
	}
	int last_num = starting_nums.back();
	while (turn_number <= ENDING_TURN) {
		int num;
		auto iter = seen.find(last_num);
		if (iter == seen.end()) {
			num = 0;
		} else {
			num = (turn_number - 1) - iter->second;
		}
		seen[last_num] = turn_number - 1;
		last_num = num;
		++turn_number;
	}
	trace(last_num);
}
