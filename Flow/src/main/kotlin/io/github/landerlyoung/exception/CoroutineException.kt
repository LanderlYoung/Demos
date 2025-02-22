package io.github.landerlyoung.demo.io.github.landerlyoung.exception

import kotlinx.coroutines.CancellationException
import kotlinx.coroutines.CoroutineExceptionHandler
import kotlinx.coroutines.CoroutineName
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.cancelAndJoin
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch
import kotlinx.coroutines.runBlocking
import kotlin.system.exitProcess

/*
 * ```
 * Author: taylorcyang@tencent.com
 * Date:   2024-12-27
 * Time:   17:09
 * Life with Passion, Code with Creativity.
 * ```
 */

fun main() {
    Thread.setDefaultUncaughtExceptionHandler { _, _ ->
        println("fuck")
        exitProcess(-1)
    }

    val scope =
        CoroutineScope(Dispatchers.Default + Job() + CoroutineExceptionHandler { context, throwable ->
            if (throwable is IllegalStateException && throwable !is CancellationException) {
                println("[${context[CoroutineName]}] caught my exception ${throwable.message}")
            } else {
                println("[${context[CoroutineName]}] caught unexpected exception ${throwable.message}")
                throw throwable
            }
        })

    val job1 = scope.launch(CoroutineName("Job1")) {
        throw IllegalStateException("Hahaha")
    }

    val job2 = scope.launch(CoroutineName("Job2")) {
        delay(Long.MAX_VALUE)
    }

    runBlocking {
        job1.join()
        job2.cancelAndJoin()
    }

}