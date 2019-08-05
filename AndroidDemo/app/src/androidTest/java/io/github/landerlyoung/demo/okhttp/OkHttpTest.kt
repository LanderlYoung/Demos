package io.github.landerlyoung.demo.okhttp

import okhttp3.*
import okhttp3.EventListener
import org.junit.Test
import java.io.IOException
import java.net.InetSocketAddress
import java.net.Proxy
import java.util.*
import java.util.concurrent.CountDownLatch
import java.util.concurrent.TimeUnit
import kotlin.system.measureTimeMillis


/*
 * ```
 * Author: taylorcyang@tencent.com
 * Date:   2019-07-23
 * Time:   10:50
 * Life with Passion, Code with Creativity.
 * ```
 */


class OkHttpTest {

    @Test
    fun test() {
        val client = OkHttpClient.Builder()
            .connectionPool(ConnectionPool(24, 5, TimeUnit.MINUTES))
            .dispatcher(Dispatcher().also {
                it.maxRequests = 64
                it.maxRequestsPerHost = 8
            })
            .addNetworkInterceptor(object : Interceptor {
                private val connectionMap = WeakHashMap<Connection, Int>()
                private var id = 0
                override fun intercept(chain: Interceptor.Chain): Response {
                    val i = chain.request().tag() as Int?
                    val index = synchronized(connectionMap) {
                        connectionMap.getOrPut(chain.connection()!!) { id++ }
                    }
                    println("request $i in connection $index")
                    return chain.proceed(chain.request())
                }
            })
            .eventListener(object : EventListener() {
                override fun connectStart(call: Call, inetSocketAddress: InetSocketAddress, proxy: Proxy) {
                    super.connectStart(call, inetSocketAddress, proxy)
                    println("connect start")
                }

                override fun connectEnd(
                    call: Call,
                    inetSocketAddress: InetSocketAddress,
                    proxy: Proxy,
                    protocol: Protocol?
                ) {
                    super.connectEnd(call, inetSocketAddress, proxy, protocol)
                }

                override fun requestHeadersStart(call: Call) {
                    super.requestHeadersStart(call)
                }
            })
            .build()

        val HOST = "https://txdgame.baiyoukeji168.cn"

        fun url(x: Int, y: Int) =
            "$HOST/xzwd/Preloading/wx/server_4/maps/10006/10006_${x}_$y.jpg"

        val downloadList = (0 until 70).map { url(it / 10, it % 7) }

        val CONCURRENCY = 16


        fun ResponseBody.drain(): Int {
            var bytes = 0
            try {
                while (true) {
                    this.source().readByte()
                    bytes++
                }
            } catch (e: IOException) {
            }
            return bytes
        }

        fun warmUp() {
            val c = client.dispatcher.maxRequestsPerHost
            val latch = CountDownLatch(c)

            repeat(c) {
                client.newCall(
                    Request.Builder()
                        .head()
                        .url(HOST)
                        .build()
                ).enqueue(object : Callback {
                    override fun onFailure(call: Call, e: IOException) {
                        latch.countDown()
                    }

                    override fun onResponse(call: Call, response: Response) {
                        latch.countDown()
                        response.body?.drain()
                    }
                })
            }
        }

        fun runOnce(startIndex: Int) {
            val end = (startIndex + CONCURRENCY).coerceAtMost(downloadList.size)
            val latch = CountDownLatch(end - startIndex)

            fun done(reason: String) {
                if (reason.isNotEmpty()) println(reason)
                latch.countDown()
            }

            for (i in startIndex until end) {
                client.newCall(
                    Request.Builder()
                        .url(downloadList[i])
                        .tag(i)
                        .build()
                ).enqueue(object : Callback {
                    override fun onFailure(call: Call, e: IOException) {
                        done("$i failure")
                    }

                    override fun onResponse(call: Call, response: Response) {
                        val rspLength = response.body?.drain()
                        done("")
                    }
                })
            }
            latch.await()
        }

warmUp()

        fun test() = measureTimeMillis {
            var i = 0
            while (i < downloadList.size) {
                runOnce(i)
                i += CONCURRENCY
            }
        }

        test()

        println("${test()} ms")

    }
}