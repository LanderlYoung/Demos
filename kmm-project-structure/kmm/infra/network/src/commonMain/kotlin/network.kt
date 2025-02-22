package io.github.kotlin.network

class Network {
    fun httpRequest(url: String): String {
        return url + "body"
    }

    fun projectName(): String =
        "project-network"
}
