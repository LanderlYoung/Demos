package io.github.landerlyoung.demo

import io.github.landerlyoung.demo.io.github.landerlyoung.Repo
import io.github.landerlyoung.demo.io.github.landerlyoung.Todo
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Job
import kotlinx.coroutines.coroutineScope
import kotlinx.coroutines.flow.MutableSharedFlow
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.SharedFlow
import kotlinx.coroutines.flow.SharingStarted
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.shareIn
import kotlinx.coroutines.flow.stateIn
import kotlinx.coroutines.launch
import kotlinx.coroutines.runBlocking

fun main() {
  val scope = CoroutineScope(Job())
  runBlocking {
    val todos = Repo.requestTodo2()
    todos.collect {
      println(it)
      println()
    }

    todos.stateIn(scope)

    todos.shareIn(scope, SharingStarted.WhileSubscribed(), 1)

    val sf = MutableStateFlow(0)

  }
}

suspend fun flowTypes() {
  coroutineScope {
    val _likeCount: MutableStateFlow<Int> = MutableStateFlow(0)
    val likeCount: StateFlow<Int> = _likeCount

    _likeCount.value++
    likeCount.value
    launch {
      likeCount.collect {
        // setText(it)
      }
    }

    val _followState: MutableSharedFlow<Boolean> = MutableSharedFlow<Boolean>(1)
    val followState: SharedFlow<Boolean> = _followState
  }

  coroutineScope {
    val todos: SharedFlow<Todo> = Repo.requestTodo().shareIn(this, SharingStarted.Lazily)

    val todo = Repo.requestTodo().stateIn(this)
    val todo2 = Repo.requestTodo().stateIn(this, SharingStarted.Lazily, Todo(0, "", "", 0))
  }
}

