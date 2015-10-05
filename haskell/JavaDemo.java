/*************************************************************************
  > File Name:     JavaDemo.java
  > Author:        Landerl Young
  > Mail:          LanderlYoung@gmail.com
  > Created Time:  Tue 06 Oct 2015 01:28:30 AM CST
 ************************************************************************/

public class JavaDemo {
    public static int binSearch(int key, int[] arr) {
        int left = 0, right = arr.length - 1, mid;
        while (left <= right) {
            mid = (left + right) / 2;
            if (arr[mid] == key) return mid;
            if (arr[mid] < key) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        return -1;
    }

    public static void main(String[] args) {
        final int ARR_LEN = 128;
        int[] ordArr = new int[ARR_LEN];
        for (int i = 0; i < ARR_LEN; i++) ordArr[i] = i;

        System.out.println(binSearch(0, ordArr));
        System.out.println(binSearch(-1, ordArr));
        System.out.println(binSearch(100, ordArr));
        System.out.println(binSearch(256, ordArr));
    }
}
