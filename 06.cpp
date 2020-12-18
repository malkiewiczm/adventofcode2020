//#define PARTONE
#include "template.hpp"

#ifndef PARTONE
static bool char_is_in_all(char needle, const std::vector<std::string> &people)
{
	for (auto person : people) {
		bool found = false;
		for (auto c : person) {
			if (c == needle) {
				found = true;
			}
		}
		if (! found)
			return false;
	}
	return true;
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
	{
#ifdef PARTONE
		std::string line;
		std::string group;
		int sum = 0;
		while (std::getline(f, line)) {
			if (! line.empty()) {
				group += ' ';
				group += line;
			} else if (! group.empty()) {
				std::stringstream ss(group);
				group.clear();
				std::string chunk;
				std::set<char> answered;
				while (ss >> chunk) {
					for (auto c : chunk) {
						answered.insert(c);
					}
				}
				sum += answered.size();
			}
		}
		{
			std::stringstream ss(group);
			group.clear();
			std::string chunk;
			std::set<char> answered;
			while (ss >> chunk) {
				for (auto c : chunk) {
					answered.insert(c);
				}
			}
			sum += answered.size();
		}
		trace(sum);
#else
		std::string line;
		std::string group;
		int sum = 0;
		while (std::getline(f, line)) {
			if (! line.empty()) {
				group += ' ';
				group += line;
			} else if (! group.empty()) {
				std::vector<std::string> people;
				std::stringstream ss(group);
				group.clear();
				std::string chunk;
				while (ss >> chunk) {
					people.push_back(chunk);
				}
				if (people.size() == 1) {
					sum += people[0].size();
				} else {
					for (auto c : people[0]) {
						if (char_is_in_all(c, people))
							++sum;
					}
				}
			}
		}
		{
			std::vector<std::string> people;
			std::stringstream ss(group);
			group.clear();
			std::string chunk;
			while (ss >> chunk) {
				people.push_back(chunk);
			}
			if (people.size() == 1) {
				sum += people[0].size();
			} else {
				for (auto c : people[0]) {
					if (char_is_in_all(c, people))
						++sum;
				}
			}
		}
		trace(sum);
#endif
	}
}
