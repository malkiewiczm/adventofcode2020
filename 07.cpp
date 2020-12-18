//#define PARTONE
#include "template.hpp"

struct WhichBag {
	int amt;
	std::string key;
};

using Graph_t = std::map<std::string, std::vector<WhichBag>>;

static std::string read_key(std::stringstream &ss)
{
	std::string key;
	std::string adjective;
	std::string color;
	ss >> adjective >> color;
	key += adjective;
	key += ' ';
	key += color;
	return key;
}

#ifdef PARTONE
static bool dfs(const Graph_t &graph, const std::string &key)
{
	auto iter = graph.find(key);
	if (iter == graph.end()) {
		return false;
	}
	for (auto bag : iter->second) {
		if (bag.key == "shiny gold")
			return true;
		else if (dfs(graph, bag.key))
			return true;
	}
	return false;
}
#else
static int dfs(const Graph_t &graph, const std::string &key)
{
	auto iter = graph.find(key);
	if (iter == graph.end()) {
		return 0;
	}
	int count = 0;
	for (auto bag : iter->second) {
		count += bag.amt * (1 + dfs(graph, bag.key));
	}
	return count;
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
	Graph_t graph;
	{
		std::string line;
		while (std::getline(f, line)) {
			if (line.size() <= 2)
				continue;
			std::string dummy;
			std::stringstream ss(line);
			const std::string key = read_key(ss);
			// bags
			ss >> dummy;
			// contain
			ss >> dummy;
			std::string amt_or_string;
			ss >> amt_or_string;
			if (amt_or_string == "no") {
				continue;
			}
			{
				const int amt = std::atoi(amt_or_string.c_str());
				const std::string second_key = read_key(ss);
				graph[key].push_back({ amt, second_key });
				ss >> dummy;
			}
			while (dummy[dummy.size() - 1] == ',') {
				int amt;
				ss >> amt;
				const std::string second_key = read_key(ss);
				graph[key].push_back({ amt, second_key });
				ss >> dummy;
			}
		}
	}
#ifdef PARTONE
	int amt = 0;
	for (auto key_value : graph) {
		if (dfs(graph, key_value.first)) {
			++amt;
		}
	}
	trace(amt);
#else
	const int amt = dfs(graph, "shiny gold");
	trace(amt);
#endif
}
