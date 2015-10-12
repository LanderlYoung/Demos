/*************************************************************************
  > File Name:     JavaDemo.java
  > Author:        Landerl Young
  > Mail:          LanderlYoung@gmail.com
  > Created Time:  Tue 06 Oct 2015 01:28:30 AM CST
 ************************************************************************/

import java.util.*;

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

    public static void insertSort(int[] arr) {
        for (int i = 1; i <= arr.length; i++) {
            insert(arr[i-1], arr,  i);
        }
    }

    public static void insert(int num, int [] arr, int right) {
        for (int i = 0; i < right; i++) {
            if (arr[i] > num) {
                for (int j = right - 1; j > i; j--) {
                    arr[j] = arr[j - 1];
                }
                arr[i] = num;
                break;
            }
        }

    }

    public static void bubbleSort(int[] arr) {
        boolean changed = true;
        while (changed) {
            changed = false;
            for (int i = 0; i < arr.length; i++) {
                if (arr[i-1] > arr[i]) {
                    swap(arr, i-1, i);
                    changed = true;
                }
            }
        }
    }

    private static void swap(int[] arr, int l,  int r) {
        int tmp = arr[r];
        arr[r] = arr[l];
        arr[l] = tmp;
    }

    public static void main(String[] args) {
        final int ARR_LEN = 128;
        int[] ordArr = new int[ARR_LEN];
        for (int i = 0; i < ARR_LEN; i++) ordArr[i] = i;

        System.out.println(binSearch(0, ordArr));
        System.out.println(binSearch(-1, ordArr));
        System.out.println(binSearch(100, ordArr));
        System.out.println(binSearch(256, ordArr));

        System.out.println("==== insertSort");
        Random r = new Random();
        for (int i = 0; i < ARR_LEN; i++) ordArr[i] = r.nextInt(100);
        System.out.println(Arrays.toString(ordArr));
        insertSort(ordArr);
        System.out.println(Arrays.toString(ordArr));

        System.out.println("==== bubbleSort");
        for (int i = 0; i < ARR_LEN; i++) ordArr[i] = r.nextInt(100);
        System.out.println(Arrays.toString(ordArr));
        bubbleSort(ordArr);
        System.out.println(Arrays.toString(ordArr));

    }
}
