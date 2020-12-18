//#define PARTONE
#include "template.hpp"

#define PREFIX(what) (std::memcmp(&chunk[0], what, 3) == 0)
#define MUST(what) if (!(what)) return false

#ifdef PARTONE
static bool is_valid(const std::string &s)
#else
static bool has_all_fields(const std::string &s)
#endif
{
	std::stringstream ss(s);
	std::string chunk;
	bool has_cid = false;
	int count = 0;
	while (ss >> chunk) {
		if (chunk.size() >= 3) {
			if (PREFIX("cid")) {
				has_cid = true;
			}
			++count;
		}
	}
	if (count == 7)
		return !has_cid;
	return count == 8;
}

#ifndef PARTONE

const std::set<std::string> VALID_EYECOLOR {
	"amb", "blu", "brn", "gry", "grn", "hzl", "oth",
};

static bool is_valid(const std::string &s)
{
	if (! has_all_fields(s))
		return false;
	std::stringstream ss(s);
	std::string chunk;
	while (ss >> chunk) {
		MUST(chunk.size() >= 4);
		MUST(chunk[3] == ':');
		if (PREFIX("byr")) {
			MUST(chunk.size() == 8);
			const int n = std::atoi(&chunk[4]);
			MUST(n >= 1920 && n <= 2002);
		} else if (PREFIX("iyr")) {
			MUST(chunk.size() == 8);
			const int n = std::atoi(&chunk[4]);
			MUST(n >= 2010 && n <= 2020);
		} else if (PREFIX("eyr")) {
			MUST(chunk.size() == 8);
			const int n = std::atoi(&chunk[4]);
			MUST(n >= 2020 && n <= 2030);
		} else if (PREFIX("hgt")) {
			if (chunk.size() == 8) {
				MUST(std::memcmp(&chunk[6], "in", 2) == 0);
				char buf[3] { 0 };
				std::memcpy(buf, &chunk[4], 2);
				const int n = std::atoi(buf);
				MUST(n >= 59 && n <= 76);
			} else if (chunk.size() == 9) {
				MUST(std::memcmp(&chunk[7], "cm", 2) == 0);
				char buf[4] { 0 };
				std::memcpy(buf, &chunk[4], 3);
				const int n = std::atoi(buf);
				MUST(n >= 150 && n <= 193);
			} else {
				return false;
			}
		} else if (PREFIX("hcl")) {
			MUST(chunk.size() == 11);
			MUST(chunk[4] == '#');
			FOR(i, 6) {
				const char c = chunk[5 + i];
				MUST((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'));
			}
		} else if (PREFIX("ecl")) {
			MUST(chunk.size() == 7);
			char buf[4] { 0 };
			std::memcpy(buf, &chunk[4], 3);
			auto iter = VALID_EYECOLOR.find(buf);
			MUST(iter != VALID_EYECOLOR.end());
		} else if (PREFIX("pid")) {
			MUST(chunk.size() == 13);
			FOR(i, 9) {
				const char c = chunk[4 + i];
				MUST(c >= '0' && c <= '9');
			}
		} else if (PREFIX("cid")) {
			// nothing
		} else {
			return false;
		}
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
	std::string line;
	std::string passport_str;
	int count = 0;
	while (std::getline(f, line)) {
		if (line.size() < 1) {
			if (is_valid(passport_str))
				++count;
			passport_str.clear();
		} else {
			passport_str += ' ';
			passport_str += line;
		}
	}
	if (passport_str.size() > 5 && is_valid(passport_str)) {
		++count;
	}
	trace(count);
}
