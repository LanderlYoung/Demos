import kotlinx.coroutines.*
import kotlinx.coroutines.channels.*

object Main1 {
  fun main() = runBlocking {
    val numbers = produceNumbers() // produces integers from 1 and on
    val squares = square(numbers) // squares integers
    repeat(5) {
      println(squares.receive()) // print first five
    }
    println("Done!") // we are done
    coroutineContext.cancelChildren() // cancel children coroutines
  }
}

fun main() = runBlocking {
  coroutineScope {
    val numbers = produceNumbers() // produces integers from 1 and on
    val squares = square(numbers) // squares integers
    repeat(5) {
      println(squares.receive()) // print first five
    }
    println("Done!") // we are done

    // managed by explicit coroutine scope
    // coroutineContext.cancelChildren() // cancel children coroutines
  }
}

fun CoroutineScope.produceNumbers() = produce<Int> {
  var x = 1
  while (true) send(x++) // infinite stream of integers starting from 1
}

fun CoroutineScope.square(numbers: ReceiveChannel<Int>): ReceiveChannel<Int> = produce {
  for (x in numbers) send(x * x)
}