#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <stdexcept>
#include <format>
#include <map>
#include <limits>
#include <set>

using Map = std::vector<std::vector<char>>;

// The actions you can take at any node
enum class Action {
	straight,
	left,
	right
};

// Doing this in two steps:
// Flood fill to find all the connected paths through the maze and to build a
// more usable map out of the input
//
// Djikstra's algroithm to find the shortest path from

void visualize_map(Map map)
{
	for (auto row: map) {
		for (auto tile: row) {
			std::cout << tile;
		}
		std::cout << '\n';
	}
}

std::pair<int, int> find_char(Map map, char c)
{
	for (Map::size_type height = map.size(), y = 0; y < height; y++) {
		for (Map::size_type width = map[y].size(), x = 0; x < width; x++) {
			if (map[y][x] == c) {
				return {x, y};
			}
		}
	}
	throw std::runtime_error(std::format("c ({}) not found in map", c));
}

// find the node in q with the smallest distance
// (or just any of the tied options)
std::pair<int, int>
minimum_dist(std::set<std::pair<int, int>> q, std::map<std::pair<int, int>, double> dist)
{
	std::pair<int, int> smallest = *q.begin();
	double smallest_distance = std::numeric_limits<double>::infinity();
	for(auto node: q) {
		double distance = dist[node];
		if (distance < smallest_distance) {
			smallest_distance = distance;
			smallest = node;
		}
	}
	return smallest;
}

// Not using a* here because I couldn't think of a good h
// https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
// The pseudocode in that article is difficult to follow... I got some better
// hints from the Algorithm section
std::pair<std::map<std::pair<int, int>, double>, std::map<std::pair<int, int>, std::pair<int, int>>>
djikstra(Map map, std::pair<int, int> start)
{
	std::map<std::pair<int, int>, double> dist;
	std::map<std::pair<int, int>, std::pair<int, int>> prev;
	std::set<std::pair<int, int>> unviseted;

	for (Map::size_type height = map.size(), y = 0; y < height; y++) {
		for (Map::size_type width = map[y].size(), x = 0; x < width; x++) {
			dist[{x, y}] = std::numeric_limits<double>::infinity();
			unviseted.insert({x, y});
		}
	}
	dist[start] = 0.0;
	while(!unviseted.empty()) {
		auto current_node = minimum_dist(unviseted, dist);
		unviseted.erase(current_node);
		// TODO

	}
}

int main()
{
	std::ifstream file("input.txt");
	if (!file.is_open()) {
		std::cerr << "Can't open input.txt!";
	}

	Map map;
	{
		std::string line;
		while(getline(file, line)) {
			std::vector<char> row(line.begin(), line.end());
			map.push_back(row);
		}
	}
	file.close();
	visualize_map(map);
	auto start = find_char(map, 'S');
	auto end = find_char(map, 'E');
}
