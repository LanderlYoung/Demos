package io.github.kotlin.fibonacci

import io.github.kotlin.network.Network

class Fibonacci {

    fun fibonacci(n: Int): Int {
        return generateFibi().take(n).last()
    }

    fun dependsOnNetwork() {
        Network().httpRequest("url")
    }

    fun projectName(): String =
        "project-fibonacci"
}

fun generateFibi() = sequence {
    // invoke method from network

    var a = firstElement
    yield(a)
    var b = secondElement
    yield(b)
    while (true) {
        val c = a + b
        yield(c)
        a = b
        b = c
    }
}


expect val firstElement: Int
expect val secondElement: Int
