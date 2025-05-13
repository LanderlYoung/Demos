package io.github.landerlyoung.composex

interface Platform {
    val name: String
}

expect fun getPlatform(): Platform