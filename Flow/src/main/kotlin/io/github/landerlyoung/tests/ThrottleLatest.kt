package io.github.landerlyoung.demo.io.github.landerlyoung.tests

import kotlinx.coroutines.delay
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.conflate
import kotlinx.coroutines.flow.flow
import kotlinx.coroutines.flow.transform
import kotlinx.coroutines.runBlocking

/*
 * ```
 * Author: taylorcyang@tencent.com
 * Date:   2024-12-12
 * Time:   15:14
 * Life with Passion, Code with Creativity.
 * ```
 */

fun main() {
    runBlocking {
        flow {
            for (num in 1..15) {
                emit(num)
                delay(25)
            }
        }.throttleLatest(100)
            .collect(::println)
    }


}

private fun <T> Flow<T>.throttleLatest(delayMillis: Long): Flow<T> = this
    .conflate()
    .transform {
        emit(it)
        delay(delayMillis)
    }