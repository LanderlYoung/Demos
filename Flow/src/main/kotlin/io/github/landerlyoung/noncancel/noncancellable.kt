package io.github.landerlyoung.demo.io.github.landerlyoung.noncancel

import kotlinx.coroutines.CancellationException
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Job
import kotlinx.coroutines.NonCancellable
import kotlinx.coroutines.cancelAndJoin
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch
import kotlinx.coroutines.runBlocking
import kotlinx.coroutines.withContext

/*
 * ```
 * Author: taylorcyang@tencent.com
 * Date:   2024-12-27
 * Time:   17:58
 * Life with Passion, Code with Creativity.
 * ```
 */

fun main() {
    val job = CoroutineScope(Job()).launch {
        try {
            withContext(NonCancellable) {
                delay(3_000)
            }
        } catch (e: CancellationException) {
            println("still cancelable")
            throw e
        }
    }

    runBlocking {
        job.cancelAndJoin()
    }
}