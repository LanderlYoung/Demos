package io.github.landerlyoung.kmm

class Greeting {
  private val platform: Platform = getPlatform()

  fun greet(): String {
    return """
      Hello, ${platform.name}!
      There are only ${daysUntilNewYear()} days left until New Year! 🎆
            """.trimIndent()
  }
}