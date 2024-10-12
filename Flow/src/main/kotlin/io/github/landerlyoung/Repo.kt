package io.github.landerlyoung.demo.io.github.landerlyoung

import io.ktor.client.*
import io.ktor.client.call.*
import io.ktor.client.engine.java.*
import io.ktor.client.plugins.contentnegotiation.*
import io.ktor.client.request.*
import io.ktor.serialization.kotlinx.json.*
import kotlinx.coroutines.channels.awaitClose
import kotlinx.coroutines.delay
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.asFlow
import kotlinx.coroutines.flow.callbackFlow
import kotlinx.coroutines.flow.flow
import kotlinx.coroutines.flow.map
import kotlinx.coroutines.flow.onEach
import kotlinx.serialization.Serializable
import kotlinx.serialization.json.Json

object Repo {

  private val client = HttpClient(Java) {
    expectSuccess = true

    install(ContentNegotiation) {
      json(Json {
        prettyPrint = true
        isLenient = true
      })
    }
  }

  fun requestTodo(): Flow<Todo> {
    return flow {
      (0..100).forEach { index ->
        val res = client.get("https://jsonplaceholder.typicode.com/posts/${index}").body<Todo>()
        emit(res)
        delay(1_000)
      }
    }
  }

  fun requestTodo2(): Flow<Todo> {
    return (1..100).asFlow()
      .onEach { delay(1_000) }
      .map { index ->
        val res = client.get("https://jsonplaceholder.typicode.com/posts/${index}").body<Todo>()
        res
      }
  }

  // fun requestTodo3(): Flow<Todo> {
  //   return callbackFlow<Todo> {
  //     var index = 1
  //
  //     val res = client.get("https://jsonplaceholder.typicode.com/posts/${index}")
  //     res.body()
  //
  //     trySend(TODO())
  //
  //     awaitClose {  }
  //   }
  // }
}

/**
 * ```json
 * {
 *   id: 1,
 *   title: '...',
 *   body: '...',
 *   userId: 1
 * }
 * ```
 */
@Serializable
data class Todo(
  val id: Int,
  val title: String,
  val body: String,
  val userId: Int,
)
