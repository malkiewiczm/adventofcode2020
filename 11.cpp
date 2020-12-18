//#define PARTONE
#include "template.hpp"

using Grid = std::vector<std::string>;
constexpr char EMPTY = 'L';
constexpr char OCC = '#';

#ifdef PARTONE
static int step(const Grid &read_grid, Grid &write_grid, const int width, const int height)
{
	int number_changed = 0;
	const int dx[8] { -1, 0, 1, 1, 1, 0, -1, -1 };
	const int dy[8] { 1, 1, 1, 0, -1, -1, -1, 0 };
	FOR(y, height) {
		FOR(x, width) {
			int occ_adj = 0;
			FOR(i, 8) {
				const int tx = x + dx[i];
				const int ty = y + dy[i];
				if (tx < 0 || tx >= width || ty < 0 || ty >= height)
					continue;
				if (read_grid[ty][tx] == OCC) {
					++occ_adj;
				}
			}
			write_grid[y][x] = read_grid[y][x];
			switch (read_grid[y][x]) {
			case EMPTY:
				if (occ_adj == 0) {
					write_grid[y][x] = OCC;
					++number_changed;
				}
				break;
			case OCC:
				if (occ_adj >= 4) {
					write_grid[y][x] = EMPTY;
					++number_changed;
				}
				break;
			}
		}
	}
	return number_changed;
}
#else

static int beam(const Grid &grid, const int width, const int height, int x, int y, const int dx, const int dy)
{
	for ( ; ; ) {
		x += dx;
		y += dy;
		if (x < 0 || x >= width || y < 0 || y >= height)
			return 0;
		if (grid[y][x] == OCC)
			return 1;
		else if(grid[y][x] == EMPTY)
			return 0;
	}
}

static int step(const Grid &read_grid, Grid &write_grid, const int width, const int height)
{
	int number_changed = 0;
	const int dx[8] { -1, 0, 1, 1, 1, 0, -1, -1 };
	const int dy[8] { 1, 1, 1, 0, -1, -1, -1, 0 };
	constexpr int THRESH = 5;
	FOR(y, height) {
		FOR(x, width) {
			int occ_adj = 0;
			FOR(i, 8) {
				occ_adj += beam(read_grid, width, height, x, y, dx[i], dy[i]);
				if (occ_adj >= THRESH)
					break;
			}
			write_grid[y][x] = read_grid[y][x];
			switch (read_grid[y][x]) {
			case EMPTY:
				if (occ_adj == 0) {
					write_grid[y][x] = OCC;
					++number_changed;
				}
				break;
			case OCC:
				if (occ_adj >= THRESH) {
					write_grid[y][x] = EMPTY;
					++number_changed;
				}
				break;
			}
		}
	}
	return number_changed;
}
#endif

#ifdef PRINT_ME
static void print_grid(const Grid &grid)
{
	FOR(i, grid.size()) {
		std::cout << i << ": " << grid[i] << std::endl;
	}
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
	Grid grid0;
	{
		std::string line;
		while (std::getline(f, line)) {
			if (line.size() >= 2)
				grid0.push_back(line);
		}
	}
	Grid grid1(grid0);
	const int height = grid0.size();
	if (height == 0) {
		TRACE << "height is zero" << std::endl;
		die();
	}
	const int width = grid0[0].size();
	if (width == 0) {
		TRACE << "width is zero" << std::endl;
		die();
	}
	for (int which_grid = 0; ; which_grid ^= 1) {
		const Grid &read_grid = which_grid ? grid0 : grid1;
		Grid &write_grid = which_grid ? grid1 : grid0;
		const int number_changed = step(read_grid, write_grid, width, height);
#ifdef PRINT_ME
		trace(number_changed);
		print_grid(write_grid);
#endif
		if (number_changed == 0) {
			break;
		}
	}
	int final_num_occupied = 0;
	FOR(y, height) {
		FOR(x, width) {
			if (grid0[y][x] == OCC)
				++final_num_occupied;
		}
	}
	trace(final_num_occupied);
}
