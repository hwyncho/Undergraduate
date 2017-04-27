import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;

public class Test {
	public static void main(String[] args) {
		ArrayList<Data> dataArrayList = new ArrayList<>();

		long start = System.currentTimeMillis();

		try {
			dataArrayList = DataParser.parse("input_A");
		} catch (IOException e) {
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		}

		long end = System.currentTimeMillis();

		System.out.println(String.format("Execution time : %d ms", (end - start)));

		for (Data data : dataArrayList) {
			System.out.println(String.format("nodeId:%d degree:%d otherNodeId:%s", data.getNodeId(), data.getDegree(), Arrays.toString(data.getOtherNodeId())));
		}

		return;
	}
}
