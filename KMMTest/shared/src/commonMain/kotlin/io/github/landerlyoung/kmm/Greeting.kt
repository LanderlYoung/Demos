package io.github.landerlyoung.kmm

import io.github.landerlyoung.kmm.proto.RocketLaunch
import io.ktor.client.*
import io.ktor.client.call.*
import io.ktor.client.plugins.contentnegotiation.*
import io.ktor.client.request.*
import io.ktor.serialization.kotlinx.json.*
import io.ktor.utils.io.errors.*
import kotlinx.serialization.json.Json
import kotlin.coroutines.cancellation.CancellationException

class Greeting {
  private val platform: Platform = getPlatform()

  // used to track iOS dealloc calling stack
  var any: Any? = null

  @Throws(CancellationException::class)
  suspend fun greeting(): String {
    val rockets: List<RocketLaunch> =
      httpClient.get("https://api.spacexdata.com/v4/launches").body()
    val lastSuccessLaunch = rockets.last { it.launchSuccess == true }

    return """
      Hello, ${platform.name}!
      There are only ${daysUntilNewYear()} days left until New Year! 🎆
      The last successful launch was ${lastSuccessLaunch.launchDateUTC} 🚀
            """.trimIndent()
  }

  private val httpClient = HttpClient {
    install(ContentNegotiation) {
      json(Json {
        prettyPrint = true
        isLenient = true
        ignoreUnknownKeys = true
      })
    }
  }
}