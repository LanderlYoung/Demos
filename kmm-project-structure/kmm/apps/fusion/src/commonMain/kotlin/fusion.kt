package io.github.kotlin.fusion

import io.github.kotlin.fibonacci.Fibonacci
import io.github.kotlin.network.Network

class Fusion {
    fun hello() =
        "${Network().projectName()}:${Fibonacci().projectName()}:${projectName()}"

    fun projectName(): String =
        "project-fusion"
}