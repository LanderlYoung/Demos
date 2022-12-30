import java.util.concurrent.atomic.AtomicInteger;

/**
 * <pre>
 * Author: yangchao12
 * Date:   ${YEAR}-${MONTH}-${DAY}
 * Time:   ${HOUR}:${MINUTE}
 * Life with Passion, Code with Creativity.
 * </pre>
 */
public class Main {
  public static void main(String[] args) throws InterruptedException {

    var count = new AtomicInteger();

    while (true) {
      Thread.ofVirtual().start(() -> {
        var c = count.getAndIncrement();
        System.out.println("Hello world!  -- " + Thread.currentThread() + " -- " + c);
        try {
          Thread.sleep(1_000_000_000L);
        } catch (InterruptedException e) {
          throw new RuntimeException(e);
        }
      });
    }

  }
}