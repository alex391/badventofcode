package org.example

import java.io.FileNotFoundException

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

fun pointDifference(a: Pair<Int, Int>, b: Pair<Int, Int>): Pair<Int, Int> {
    return Pair(a.first - b.first, a.second - b.second)
}

fun pointSum(a: Pair<Int, Int>, b: Pair<Int, Int>): Pair<Int, Int> {
    return Pair(a.first + b.first, a.second + b.second)
}

fun isPointInBounds(point: Pair<Int, Int>, width: Int, height: Int): Boolean {
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

    val nodeFrequencies: MutableMap<Char, MutableSet<Pair<Int, Int>>> = HashMap()
    for ((y, line) in roof.withIndex()) {
        for ((x, c) in line.withIndex()) {
            if (c == '.') {
                continue
            }
            if (nodeFrequencies[c] == null) {
                nodeFrequencies[c] = HashSet()
            }
            nodeFrequencies[c]!!.add(Pair(x, y))
        }
    }

    val antiNodes: MutableSet<Pair<Int, Int>> = HashSet()
    for (frequency in nodeFrequencies) {
        // https://stackoverflow.com/q/62593231
        for (a in frequency.component2()) {
            for (b in frequency.component2()) {
                if (a == b) {
                    continue
                }

                val antiNodeOne = pointSum(pointDifference(a, b), a)
                if (isPointInBounds(antiNodeOne, roof[0].length, roof.size)) {
                    antiNodes.add(antiNodeOne)
                }
                val antiNodeTwo = pointSum(pointDifference(b, a), b)
                if (isPointInBounds(antiNodeTwo, roof[0].length, roof.size)) {
                    antiNodes.add(antiNodeTwo)
                }
            }
        }
    }
    println(antiNodes.size)

}