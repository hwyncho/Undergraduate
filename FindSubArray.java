/**
 * Created by hwync on 2016-11-02.
 */

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.concurrent.*;

/**
 * @author hwyn.cho
 * @version 0.0.0.1
 */
public class FindSubArray {
	/**
	 * 메인 함수
	 * @param args
	 */
	public static void main(String... args) {
		System.out.println(">>> Hello World!");

		/* 파일을 읽고, 변수에 저장한다. */
		double[][] data1 = inputData();
		double[][] data2 = reversedData(data1);

		System.out.println(">>> n = " + data1.length);

		/* 최대·최소합을 갖는 부분 배열을 구한다. */
		ExecutorService executorService = Executors.newFixedThreadPool(2);

		Callable<double[]> callable1 = () -> subArrayMax(data1);
		Callable<double[]> callable2 = () -> subArrayMax(data2);

		Future<double[]> future1 = executorService.submit(callable1);
		Future<double[]> future2 = executorService.submit(callable2);

		double[] maxResult = new double[] {Double.MIN_VALUE, 0, 0, 0, 0};
		double[] minResult = new double[] {Double.MAX_VALUE, 0, 0, 0, 0};

		System.out.print(">>> Start... ");
		long start = System.currentTimeMillis();

		try {
			maxResult = future1.get();
			minResult = future2.get();
		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (ExecutionException e) {
			e.printStackTrace();
		}

		long end = System.currentTimeMillis();
		System.out.println("Complete!");

		executorService.shutdown();

		System.out.print(">>> Max = " + maxResult[0]);
		System.out.print(" ⇒ ");
		System.out.println("[(" + (int)maxResult[1] + ", " + (int)maxResult[2] + "), "
				+ "(" + (int)maxResult[3] + ", " + (int)maxResult[4] + ")]");

		System.out.print(">>> Min = " + (-1.0) * minResult[0]);
		System.out.print(" ⇒ ");
		System.out.println("[(" + (int)minResult[1] + ", " + (int)minResult[2] + "), "
				+ "(" + (int)minResult[3] + ", " + (int)minResult[4] + ")]");

		System.out.println(">>> " + (end - start) + "ms");
	}

	/**
	 * 파일에 입력되어 있는 실수를 배열에 저장한다.
	 * @exception FileNotFoundException
	 * @exception IOException
	 * @exception Exception
	 * @return
	 */
	public static double[][] inputData() {
		double[][] data = null;

		try {
			BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(System.in));

			/* 파일에 입력되어 있는 크기로 배열을 생성한다 */
			String line = bufferedReader.readLine();
			int n = Integer.parseInt(line);
			data = new double[n][n];

			int i = 0;
			int j = 0;

			/* 파일에 입력되어 있는 실수를 배열에 저장한다 */
			while ((line = bufferedReader.readLine()) != null) {
				String[] values = line.split(" ");

				j = 0;
				while (j < values.length) {
					double value = Double.parseDouble(values[j]);
					data[i][j] = value;
					j++;
				}
				i++;
			}

			bufferedReader.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		}

		return data;
	}

	/**
	 * 배열의 값을 모두 음수로 변경한다.
	 * @param data
	 * @return
	 */
	public static double[][] reversedData(double[][] data) {
		int rows = data.length;
		int cols = data[0].length;

		double[][] reversed = new double[rows][cols];

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				reversed[i][j] = (-1.0) * data[i][j];
			}
		}

		return reversed;
	}

	/**
	 * 1차원배열을 입력받고, { 최대합, 시작 인덱스, 끝 인덱스 }를 반환한다.
	 * @param arr
	 * @return
	 */
	private static double[] findMax(double[] arr) {
		double[] result = new double[] {Double.MIN_VALUE, 0, -1};		// { max, startIndex, endIndex }
		double currentSum = 0;
		int startIndex = 0;

		for (int i = 0; i < arr.length; i++) {
			currentSum += arr[i];

			if (currentSum < 0) {
				currentSum = 0.0;
				startIndex = i + 1;
			} else if (currentSum > result[0]) {
				result[0] = currentSum;
				result[1] = startIndex;
				result[2] = i;
			}
		}

		if (result[2] == -1) {
			result[0] = 0.0;
			result[1] = 0;
			result[2] = 0;

			for (int i = 0; i < arr.length; i++) {
				if (arr[i] > result[0]) {
					result[0] = arr[i];
					result[1] = i;
					result[2] = i;
				}
			}
		}

		return result;
	}

	/**
	 * 최대합을 갖는 부분 배열을 구한다.
	 * { 최대합, 시작 행 인덱스, 시작 열 인덱스, 끝 행 인덱스, 끝 열 인덱스 }를 반환한다.
	 */
	public static double[] subArrayMax(double[][] arr) {
		int rows = arr.length;
		int cols = arr[0].length;

		int startRow = 0;
		int startCol = 0;
		int endRow = 0;
		int endCol = 0;

		double currentResult[];
		double max = Double.MIN_VALUE;

		double result[] = new double[] {Double.MIN_VALUE, 0, 0, 0, 0};

		for (int leftCol = 0; leftCol < cols; leftCol++) {
			double[] temp = new double[rows];

			for (int rightCol = leftCol; rightCol < cols; rightCol++) {
				for (int i = 0; i < rows; i++) {
					temp[i] += arr[i][rightCol];
				}

				currentResult = findMax(temp);

				if (currentResult[0] > max) {
					max = currentResult[0];
					startRow = (int)currentResult[1];
					startCol = leftCol;
					endRow = (int)currentResult[2];
					endCol = rightCol;
				}
			}
		}

		result[0] = max;
		result[1] = startRow + 1;
		result[2] = startCol + 1;
		result[3] = endRow + 1;
		result[4] = endCol + 1;

		return	result;
	}
}
