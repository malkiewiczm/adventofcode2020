//#define PARTONE
#include "template.hpp"

#define substring(a, b) substr((a), (b) - (a))

using Vector = std::vector<int>;

struct Rule {
	const std::string name;
	const std::pair<int, int> range[2];
	Rule(const std::string &l_name, int low0, int high0, int low1, int high1)
		: name(l_name), range{ {low0, high0}, {low1, high1} } {}
	bool is_in_range(const int n) const {
		return (n >= range[0].first && n <= range[0].second) || (n >= range[1].first && n <= range[1].second);
	}
};

static inline std::ostream &operator<<(std::ostream &o, const Rule &rhs)
{
	o << "(" << rhs.name << ": " << rhs.range[0] << "; " << rhs.range[1] << ")";
	return o;
}

static Vector parse_list(const std::string &s)
{
	Vector ret;
	size_t start, end;
	for (start = 0; (end = s.find(',', start)) != std::string::npos; start = end + 1) {
		ret.push_back(std::atoi(s.substring(start, end).c_str()));
	}
	ret.push_back(std::atoi(s.substr(start).c_str()));
	return ret;
}

int main(int argc, char **argv)
{
	const char *const fname = (argc >= 2) ? argv[1] : "in.txt";
	std::ifstream f(fname);
	if (! f.good()) {
		TRACE << "file cannot be opened" << std::endl;
		die();
	}
	std::vector<Rule> rules;
	Vector your_ticket;
	std::vector<Vector> nearby_tickets;
	{
		std::string line;
		// rule parsing
		while (std::getline(f, line)) {
			if (line.empty())
				continue;
			if (line == "your ticket:")
				break;
			auto colon = line.find(':');
			const std::string rule_name = line.substr(0, colon);
			auto hyphen0 = line.find('-', colon);
			const int low0 = std::atoi(line.substring(colon + 1, hyphen0).c_str());
			auto keyword_or = line.find('o', hyphen0);
			const int high0 = std::atoi(line.substring(hyphen0 + 1, keyword_or).c_str());
			auto hyphen1 = line.find('-', keyword_or);
			const int low1 = std::atoi(line.substring(keyword_or + 3, hyphen1).c_str());
			const int high1 = std::atoi(line.substr(hyphen1 + 1).c_str());
			rules.emplace_back(rule_name, low0, high0, low1, high1);
		}
		if (! std::getline(f, line)) {
			TRACE << "out of input" << std::endl;
			die();
		}
		your_ticket = parse_list(line);
		while (std::getline(f, line)) {
			if (line.empty())
				continue;
			// "nearby ticket"
			if (line.size() == 15 && line[0] == 'n')
				continue;
			nearby_tickets.emplace_back(parse_list(line));
		}
	}
	std::vector<std::set<Rule*>> marker(1000);
	FOR(i, rules.size()) {
		Rule *const rule = &rules[i];
		for (int k = rule->range[0].first; k <= rule->range[0].second; ++k) {
			marker[k].insert(rule);
		}
		for (int k = rule->range[1].first; k <= rule->range[1].second; ++k) {
			marker[k].insert(rule);
		}
	}
#ifdef PARTONE
	int sum = 0;
	for (auto ticket : nearby_tickets) {
		for (auto num : ticket) {
			if (marker[num].empty()) {
				sum += num;
			}
		}
	}
	trace(sum);
#else
	std::vector<Vector> valid_tickets;
	valid_tickets.emplace_back(your_ticket);
	for (auto ticket : nearby_tickets) {
		bool is_invalid = false;
		for (auto num : ticket) {
			if (marker[num].empty()) {
				is_invalid = true;;
				break;
			}
		}
		if (! is_invalid) {
			valid_tickets.emplace_back(ticket);
		}
	}
	std::vector<std::set<Rule*>> rules_per_index(rules.size());
	for (auto ticket : valid_tickets) {
		FOR(i, ticket.size()) {
			std::set<Rule*> &r = marker[ticket[i]];
			rules_per_index[i].insert(r.begin(), r.end());
		}
	}
	for (auto ticket : valid_tickets) {
		FOR(i, ticket.size()) {
			std::vector<Rule*> rules_to_delete;
			const int num = ticket[i];
			for (auto rule : rules_per_index[i]) {
				if (! rule->is_in_range(num)) {
					rules_to_delete.push_back(rule);
				}
			}
			for (auto rule : rules_to_delete) {
				rules_per_index[i].erase(rule);
			}
		}
	}
	std::vector<Rule*> final_rule_per_index(rules.size());
	FOR(counter, rules_per_index.size() + 1) {
		size_t smallest = std::numeric_limits<size_t>::max();
		std::set<Rule*>* smallest_rule_set = nullptr;
		FOR(i, rules_per_index.size()) {
			std::set<Rule*>* rule_set = &rules_per_index[i];
			const size_t n = rule_set->size();
			if (n > 0 && n < smallest) {
				smallest = n;
				smallest_rule_set = rule_set;
			}
		}
		if (smallest_rule_set == nullptr)
			break;
		if (smallest != 1) {
			TRACE << "problem: " << std::endl;
			trace(smallest);
			die();
		}
		Rule *const rule = *smallest_rule_set->begin();
		FOR(i, rules_per_index.size()) {
			std::set<Rule*> &rule_set = rules_per_index[i];
			auto iter = rule_set.find(rule);
			if (iter == rule_set.end())
				continue;
			if (&rule_set == smallest_rule_set) {
				final_rule_per_index[i] = rule;
			}
			rule_set.erase(iter);
		}
	}
	I64 product = 1;
	FOR(i, final_rule_per_index.size()) {
		Rule *const rule = final_rule_per_index[i];
		const std::string &rule_name = rule ? rule->name : "(null)";
		TRACE << "[" << i << "] " << rule_name << ": " << your_ticket[i] << std::endl;
		size_t space_index = rule_name.find(' ');
		if (space_index == std::string::npos)
			continue;
		if (rule_name.substring(0, space_index) == "departure")
			product *= your_ticket[i];
	}
	trace(product);
#endif
}
