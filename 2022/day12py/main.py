# Also just doing day 12 in Python.
# Following along with:
# https://stackabuse.com/courses/graphs-in-python-theory-and-implementation/lessons/dijkstras-algorithm/
from queue import PriorityQueue


class Graph:
    def __init__(self, num_of_vertices):
        self.v = num_of_vertices
        self.edges = [[-1 for _ in range(num_of_vertices)] for _ in range(num_of_vertices)]
        self.visited = []

    def add_edge(self, vertex_1, vertex_2, w):
        self.edges[vertex_1][vertex_2] = w
        # This graph has different weights one way than the other.

    def get_edges(self):
        return self.edges


def dijkstra(graph: Graph, start_vertex: int):
    D = [float('inf') for _ in range(graph.v)]
    D[start_vertex] = 0

    pq = PriorityQueue()
    pq.put((0, start_vertex))

    while not pq.empty():
        (dist, current_vertex) = pq.get()
        graph.visited.append(current_vertex)

        for neighbor in range(graph.v):
            if graph.edges[current_vertex][neighbor] != -1:
                distance = graph.edges[current_vertex][neighbor]
                if neighbor not in graph.visited:
                    old_cost = D[neighbor]
                    new_cost = D[current_vertex] + distance
                    if new_cost < old_cost:
                        pq.put((new_cost, neighbor))
                        D[neighbor] = new_cost
    return D


def vertex_to_height(vertex: int) -> int:
    """
    From a vertex number, get a height
    :param vertex: the vertex number
    :return: the y-coordinate of the vertex (the x-coordinate is just vertex % height)
    """
    y = 0
    while vertex >= width:
        vertex = vertex - width
        y += 1
    return y


def weight(vertex_1: int, vertex_2: int) -> float:
    """
    Test if you can get from vertex 1 to 2
    :param vertex_1: The first vertex - the one we're "standing" on
    :param vertex_2: The second vertex - the one we want to get to without taking out our climbing gear
    :return: inf if you can't go that way, or 1 if you can
    """  #
    vertex_1_height = heightmap[vertex_to_height(vertex_1)][vertex_1 % width]
    vertex_2_height = heightmap[vertex_to_height(vertex_2)][vertex_2 % width]
    return float("inf") if vertex_2_height + 1 < vertex_1_height  else 1.0


f = open("input.txt")
# Generate the graph:
contents = list(map(lambda l: l.strip(), f.readlines()))  # Cut off the \n
width = len(contents[0])
height = len(contents)
heightmap = [[0 for _ in range(width)] for _ in range(height)]
start = 0  # The vertex number for the S and the E
end = 0

vertex_number = 0
a_list = []
for line in enumerate(contents):
    for character in enumerate(line[1]):
        if character[1] == 'S':
            heightmap[line[0]][character[0]] = 0
            start = vertex_number
            a_list.append(vertex_number)
        elif character[1] == 'E':
            heightmap[line[0]][character[0]] = 25
            end = vertex_number
        else:  # Unicode hack:
            elevation = (ord(character[1]) - ord('a'))
            heightmap[line[0]][character[0]] = elevation
            if elevation == 0:
                a_list.append(vertex_number)
        vertex_number += 1


def main():
    g = Graph(width * height)

    vertex = 0
    for y in heightmap:
        for _ in y:
            if vertex % width != 0:  # Left:
                g.add_edge(vertex, vertex - 1, weight(vertex, vertex - 1))
            if (vertex % width) != (width - 1):  # Right:
                g.add_edge(vertex, vertex + 1, weight(vertex, vertex + 1))
            if vertex_to_height(vertex) != 0:  # Up:
                g.add_edge(vertex, vertex - width, weight(vertex, vertex - width))
            if vertex_to_height(vertex) != (height - 1):  # Down:
                g.add_edge(vertex, vertex + width, weight(vertex, vertex + width))
            vertex += 1
    D = dijkstra(g, end)

    a_distances = D
    for i in range(0, len(a_distances)):
        if i not in a_list:
            a_distances[i] = float("inf")
            # Just make that distance huge, rather than making mutating the list we're iterating on
    a_distances.sort()

    print(a_distances[0])


if __name__ == "__main__":
    main()
