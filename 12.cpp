//#define PARTONE
#include "template.hpp"

using Coord = std::pair<int, int>;

#ifdef PARTONE
static int deg_to_amt(const int deg)
{
	switch (deg) {
	case 0:
		return 0;
	case 90:
		return 1;
	case 180:
		return 2;
	case 270:
		return 3;
	default:
		TRACE << "bad deg" << std::endl;
		trace(deg);
		die();
		return 0;
	}
}

#else

static Coord rotate(const Coord origin, const Coord p, int deg)
{
	while (deg < 0)
		deg += 360;
	const Coord v { p.first - origin.first, p.second - origin.second };
	Coord ret { 0, 0 };
	switch (deg) {
	case 0:
		break;
	case 90:
		ret.first = v.second;
		ret.second = -v.first;
		break;
	case 180:
		ret.first = -v.first;
		ret.second = -v.second;
		break;
	case 270:
		ret.first = -v.second;
		ret.second = v.first;
		break;
	default:
		TRACE << "bad deg" << std::endl;
		trace(deg);
		die();
		break;
	}
	ret.first += origin.first;
	ret.second += origin.second;
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
#ifdef PARTONE
	std::string line;
	Coord pos { 0, 0 };
	int dir = 0;
	const Coord dir_table[4] { { 0, 1 }, { -1, 0 }, { 0, -1 }, { 1, 0 } };
	while (std::getline(f, line)) {
		if (line.empty())
			continue;
		char dir_c = line[0];
		int amt = std::atoi(&line[0] + 1);
		switch (dir_c) {
		case 'N':
			pos.first += amt;
			break;
		case 'S':
			pos.first -= amt;
			break;
		case 'E':
			pos.second += amt;
			break;
		case 'W':
			pos.second -= amt;
			break;
		case 'F':
			pos.first += dir_table[dir].first * amt;
			pos.second += dir_table[dir].second * amt;
			break;
		case 'R':
			dir = (dir + deg_to_amt(amt)) % 4;
			break;
		case 'L':
			dir -= deg_to_amt(amt);
			while (dir < 0)
				dir += 4;
			break;
		default:
			TRACE << "unknown dir" << std::endl;
			trace(dir_c);
			die();
		}
	}
#else
	std::string line;
	Coord pos { 0, 0 };
	Coord waypoint { 1, 10 };
	while (std::getline(f, line)) {
		if (line.empty())
			continue;
		char dir_c = line[0];
		int amt = std::atoi(&line[0] + 1);
		switch (dir_c) {
		case 'N':
			waypoint.first += amt;
			break;
		case 'S':
			waypoint.first -= amt;
			break;
		case 'E':
			waypoint.second += amt;
			break;
		case 'W':
			waypoint.second -= amt;
			break;
		case 'F': {
			const Coord dist { (waypoint.first - pos.first) * amt, (waypoint.second - pos.second) * amt };
			pos.first += dist.first;
			pos.second += dist.second;
			waypoint.first += dist.first;
			waypoint.second += dist.second;
			break;
		}
		case 'R':
			waypoint = rotate(pos, waypoint, -amt);
			break;
		case 'L':
			waypoint = rotate(pos, waypoint, amt);
			break;
		default:
			TRACE << "unknown dir" << std::endl;
			trace(dir_c);
			die();
		}
	}
#endif
	trace(pos);
	trace(std::abs(pos.first) + std::abs(pos.second));
}
