import com.sun.istack.internal.Nullable;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;

/**
 * @author hwyn.cho
 */
public class DataParser {
	/**
	 * 데이터를 파싱 후에 ArrayList로 반환
	 * @param path 데이터 파일 경로
	 * @return 파싱한 데이터 ArrayList
	 * @throws IOException
	 */
	@Nullable
	public static ArrayList<Data> parse(String path) throws IOException {
		ArrayList<Data> dataArrayList = new ArrayList<>(10);

		try {
			BufferedReader bufferedReader = new BufferedReader(new FileReader(path));

			while (true) {
				String line = bufferedReader.readLine();

				if (line == null) { break; }

				String[] splitedLine = line.replaceAll("^\\s+|\\(.*\\)|\n$", "").split("\\s+");

				Data data = new Data();
				data.setNodeId(Integer.parseInt(splitedLine[0]));
				data.setDegree(Integer.parseInt(splitedLine[1]));
				if (splitedLine.length > 2) {
					data.setOtherNodeId(Arrays.copyOfRange(splitedLine, 2, splitedLine.length));
				}

				dataArrayList.add(data);
			}

			return dataArrayList;
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		}

		return null;
	}
}
