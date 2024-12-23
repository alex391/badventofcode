package org.example

import java.io.FileNotFoundException

typealias Point = Pair<Int, Int>

class InputReader {
    /**
     * Read from the file
     */
    fun readInput(): List<String> {
        // https://stackoverflow.com/a/42740416
        val fileContent =
            InputReader::class.java.classLoader.getResource("input.txt")?.readText() ?: throw FileNotFoundException()
        return fileContent.lines()
    }

}

class Visualization(roof: List<String>) {
    private val grid: MutableList<MutableList<Char>> = ArrayList()

    init {
        for (item in roof) {
            grid.add(item.toMutableList())
        }
    }

    fun addAntiNode(position: Point) {
        if (grid[position.second][position.first] == '.') {
            grid[position.second][position.first] = '#'
        }
    }

    override fun toString(): String {
        val stringBuilder = StringBuilder()
        for (line in grid) {
            for (c in line) {
                stringBuilder.append(c)
            }
            stringBuilder.append('\n')
        }
        return stringBuilder.toString()
    }
}

fun pointDifference(a: Point, b: Point): Point {
    return Point(a.first - b.first, a.second - b.second)
}

fun pointSum(a: Point, b: Point): Point {
    return Point(a.first + b.first, a.second + b.second)
}

fun pointScale(point: Point, scale: Int): Point {
    return Point(point.first * scale, point.second * scale)
}

fun isPointInBounds(point: Point, width: Int, height: Int): Boolean {
    return when {
        point.first < 0 -> false
        point.second < 0 -> false
        point.first >= width -> false
        point.second >= height -> false
        else -> true
    }
}

fun main() {
    val roof: List<String> = InputReader().readInput()

    val visualization = Visualization(roof)

    val nodeFrequencies: MutableMap<Char, MutableSet<Point>> = HashMap()
    for ((y, line) in roof.withIndex()) {
        for ((x, c) in line.withIndex()) {
            if (c == '.') {
                continue
            }
            if (nodeFrequencies[c] == null) {
                nodeFrequencies[c] = HashSet()
            }
            nodeFrequencies[c]!!.add(Point(x, y))
        }
    }

    val antiNodes: MutableSet<Point> = HashSet()
    for (frequency in nodeFrequencies) {
        // https://stackoverflow.com/q/62593231
        for (a in frequency.component2()) {
            for (b in frequency.component2()) {
                if (a == b) {
                    continue
                }

                val aDifference = pointDifference(a, b)
                var i = 0
                while (true) {
                    val antiNode = pointSum(pointScale(aDifference, i), a)
                    if (isPointInBounds(antiNode, roof[0].length, roof.size)) {
                        antiNodes.add(antiNode)
                        visualization.addAntiNode(antiNode)
                    } else {
                        break
                    }
                    i++

                }
                i = 0
                val bDifference = pointDifference(b, a)
                while (true) {
                    val antiNode = pointSum(pointScale(bDifference, i), b)
                    if (isPointInBounds(antiNode, roof[0].length, roof.size)) {
                        antiNodes.add(antiNode)
                        visualization.addAntiNode(antiNode)
                    } else {
                        break
                    }
                    i++
                }
            }
        }
    }
    println(visualization)
    println(antiNodes.size)

}