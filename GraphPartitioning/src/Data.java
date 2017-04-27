import java.util.ArrayList;

/**
 * @author hwyn.cho
 */
public class Data {
	private int nodeId;							// node의 id
	private int degree;							// 연결된 node의 갯수
	private int[] otherNodeId;					// 연결된 node의 id

	public Data() {
		this.nodeId = 0;
		this.degree = 0;
		this.otherNodeId = null;
	}

	public int getNodeId() { return this.nodeId; }

	public int getDegree() { return this.degree; }

	public int[] getOtherNodeId() { return this.otherNodeId; }

	public void setNodeId(int nodeId) { this.nodeId = nodeId; }

	public void setDegree(int degree) { this.degree = degree; }

	public void setOtherNodeId(String[] otherNodeId) {
		this.otherNodeId = new int[this.degree];

		for (int i = 0; i < this.otherNodeId.length; i++) {
			this.otherNodeId[i] = Integer.parseInt(otherNodeId[i]);
		}
	}
}
