#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <stdexcept>
#include <format>
#include <map>
#include <set>
#include <deque>
#include <algorithm>

// Doing this in two steps:
// Flood fill to find all the connected paths through the maze and to build a
// more usable map out of the input
//
// Djikstra's algroithm to find the shortest path from there

using Map = std::vector<std::vector<char>>;

// The offsets for all four neighbors of a grid position
const std::pair<int, int> neighbors[] {
	// these are in the same order as Facing
	{1, 0},
	{0, 1},
	{-1, 0},
	{0, -1},
};

enum class Facing {
	east, // this is frist because it's the starting direction
	south,
	west,
	north,
};

class Node {
	// https://excalidraw.com/#json=kthX3FK3IfsO3rlKIo93k,tjk9H4IR5C0mb0cAHQJkUg
	public:
		// indexes into the vector that stores all of the nodes:
		int straight = -1;
		int left = -1;
		int right = -1;
		// It's coordinate and facing uniquely identify this node
		Facing facing; 
		std::pair<int, int> coordinate;

		Node(Facing facing, std::pair<int, int> coordinate): 
			facing(facing), coordinate(coordinate)
			{ };
};

const bool operator<(const Node &lhs, const Node &rhs)
{
       int rhs_facing = static_cast<int>(rhs.facing);
       int lhs_facing = static_cast<int>(lhs.facing);
       if (lhs_facing != rhs_facing) {
	       return lhs_facing < rhs_facing;
       }

       return lhs.coordinate < rhs.coordinate;
}

const bool operator==(const Node &lhs, const Node& rhs)
{
	return lhs.facing == rhs.facing && lhs.coordinate == rhs.coordinate;
}

void visualize_map(Map &map)
{
	for (auto row: map) {
		for (auto tile: row) {
			std::cout << tile;
		}
		std::cout << '\n';
	}
}

std::pair<int, int> find_char(const Map &map, char c)
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

bool in_bounds(std::pair<int, int> coordinate, const Map &map)
{
	return coordinate.first >= 0
		&& coordinate.second >= 0
		&& static_cast<Map::size_type>(coordinate.second) < map.size() // Casting explicitly to get rid of warning: we know here that it's positive anyway
		&& static_cast<Map::size_type>(coordinate.first) < map[0].size();
}

// returns a vector of all the nodes, the start node is the first element
// https://en.wikipedia.org/wiki/Flood_fill
std::vector<Node> flood_fill(const Map &map)
{
	auto start = find_char(map, 'S');

	std::vector<Node> nodes;
	std::map<Node, int> node_indices; // For searching later
	int cursor = 0;

	std::deque<std::pair<int, int>> q;
	q.push_back(start);
	while (!q.empty()) {
		std::pair<int, int> n = q.front();
		q.pop_front();
		char tile = map[n.second][n.first];
		if (tile == '#') {
			continue;
		}

		// Add the four nodes to the vector
		for(int i = 0; i < 4; i++) {
			nodes.push_back(Node(static_cast<Facing>(i), n));
			node_indices[nodes.back()] = nodes.size() - 1;
		}

		// Set it's left and right neihghbors
		for (int i = 0; i < 4; i++) {
			nodes[cursor + i].left = cursor + ((i + 3) % 4);
			nodes[cursor + i].right = cursor + ((i + 1) % 4);
		}
		
		cursor += 4;
		
		for (auto neighbor_offset: neighbors) {
			std::pair<int, int> neighbor = {
				n.first + neighbor_offset.first, 
				n.second + neighbor_offset.second
			};
			// Add to q, only if it's in bounds, and only if we havn't added it already
			if (in_bounds(neighbor, map) && (!node_indices.count(Node(Facing::east, neighbor)))) {
				q.push_back(neighbor);
			}
		}
	}
	// Find the indexes for straight, if they exist
	for (std::vector<Node>::size_type i = 0, size = nodes.size(); i < size; i++) {
		std::pair<int, int> coordinate_in_front_of = { 
			nodes[i].coordinate.first + neighbors[static_cast<int>(nodes[i].facing)].first,
			nodes[i].coordinate.second + neighbors[static_cast<int>(nodes[i].facing)].second,
		};
		int straight = node_indices[Node(nodes[i].facing, coordinate_in_front_of)];
		nodes[i].straight = straight;
	}
	return nodes;
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
		std::cerr << "Can't open input.txt!\n";
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
	auto graph = flood_fill(map);
	std::cout << graph.size() << '\n';
}
