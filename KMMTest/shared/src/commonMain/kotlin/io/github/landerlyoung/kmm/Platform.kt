package io.github.landerlyoung.kmm

interface Platform {
    val name: String
}

expect fun getPlatform(): Platform