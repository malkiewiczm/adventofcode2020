//#define PARTONE
#include "template.hpp"

#ifdef PARTONE
using Tuple = std::tuple<int, int, int>;
#else
using Tuple = std::tuple<int, int, int, int>;
#endif

class CubeIterator {
private:
	int state;
public:
	CubeIterator(const int l_state)
		: state(l_state) {}
	void operator++() {
#ifdef PARTONE
		constexpr int SKIP_POINT = 13;
#else
		constexpr int SKIP_POINT = 40;
#endif
		++state;
		if (state == SKIP_POINT)
			++state;
	}
	bool operator!=(const CubeIterator &rhs) const {
		return state != rhs.state;
	}
	Tuple operator*() const {
		int s = state;
		const int x = (s % 3) - 1;
		s /= 3;
		const int y = (s % 3) - 1;
		s /= 3;
		const int z = (s % 3) - 1;
#ifdef PARTONE
		return std::make_tuple(z, y, x);
#else
		s /= 3;
		const int w = (s % 3) - 1;
		return std::make_tuple(w, z, y, x);
#endif
	}
};

static inline Tuple operator+ (const Tuple &lhs, const Tuple &rhs)
{
#ifdef PARTONE
	return std::make_tuple(std::get<0>(lhs) + std::get<0>(rhs), std::get<1>(lhs) + std::get<1>(rhs), std::get<2>(lhs) + std::get<2>(rhs));
#else
	return std::make_tuple(std::get<0>(lhs) + std::get<0>(rhs), std::get<1>(lhs) + std::get<1>(rhs), std::get<2>(lhs) + std::get<2>(rhs), std::get<3>(lhs) + std::get<3>(rhs));
#endif
}

static inline std::ostream &operator<< (std::ostream &o, const Tuple &rhs)
{
#ifdef PARTONE
	o << "{" << std::get<0>(rhs) << ", " << std::get<1>(rhs) << ", " << std::get<2>(rhs) << "}";
#else
	o << "{" << std::get<0>(rhs) << ", " << std::get<1>(rhs) << ", " << std::get<2>(rhs) << ", " << std::get<3>(rhs) << "}";
#endif
	return o;
}

struct CubeIteratorFactory {
	CubeIterator begin() const {
		return CubeIterator(0);
	}
	CubeIterator end() const {
#ifdef PARTONE
		return CubeIterator(27);
#else
		return CubeIterator(81);
#endif
	}
};

#define CUBE_FOR(i) for (auto i : CubeIteratorFactory())

class Field {
private:
	struct CellData {
		int neighbor_count = 0;
		bool is_active = false;
	};
	std::map<Tuple, CellData> data;
public:
	Field() = default;
	void activate_cell(const Tuple &loc);
	void kill_cell(const Tuple &loc);
	int get_num_active() const;
	void step();
};

int Field::get_num_active() const
{
	int ret = 0;
	for (auto kv : data) {
		if (kv.second.is_active)
			++ret;
	}
	return ret;
}

void Field::activate_cell(const Tuple &loc)
{
	data[loc].is_active = true;
	CUBE_FOR(d) {
		++data[loc + d].neighbor_count;
	}
}

void Field::kill_cell(const Tuple &loc)
{
	{
		auto iter = data.find(loc);
		if (iter == data.end())
			return;
		CellData &cell = iter->second;
		cell.is_active = false;
		if (cell.neighbor_count <= 0)
			data.erase(iter);
	}
	CUBE_FOR(d) {
		auto iter = data.find(loc + d);
		if (iter == data.end()) {
			TRACE << "problem: inconsistant neighbor count" << std::endl;
			trace(loc);
			trace(loc + d);
			die();
		}
		CellData &cell = iter->second;
		if (cell.neighbor_count > 0)
			--cell.neighbor_count;
		if (! cell.is_active && cell.neighbor_count == 0)
			data.erase(iter);
	}
}

void Field::step()
{
	std::vector<Tuple> to_kill;
	std::vector<Tuple> to_activate;
	for (auto kv : data) {
		const Tuple &loc = kv.first;
		const CellData &cell = kv.second;
		if (cell.is_active) {
			if (cell.neighbor_count != 2 && cell.neighbor_count != 3) {
				to_kill.push_back(loc);
			}
		} else {
			if (cell.neighbor_count == 3) {
				to_activate.push_back(loc);
			}
		}
	}
	for (auto loc : to_activate) {
		activate_cell(loc);
	}
	for (auto loc : to_kill) {
		kill_cell(loc);
	}
}

int main(int argc, char **argv)
{
	const char *const fname = (argc >= 2) ? argv[1] : "in.txt";
	std::ifstream f(fname);
	if (! f.good()) {
		TRACE << "file cannot be opened" << std::endl;
		die();
	}
	Field field;
	{
		std::string line;
		int y = 0;
		while (std::getline(f, line)) {
			if (line.empty())
				continue;
			FOR(x, line.size()) {
				if (line[x] == '#') {
#ifdef PARTONE
					field.activate_cell(std::make_tuple(0, y, x));
#else
					field.activate_cell(std::make_tuple(0, 0, y, x));
#endif
				}
			}
			++y;
		}
	}
	FOR(i, 6) {
		field.step();
	}
	trace(field.get_num_active());
}
