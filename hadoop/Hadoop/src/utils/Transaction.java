package utils;
/**
 * 
 */

/**
 * @author hadoop
 *
 */
public class Transaction {
	int transID;
	int custID; 
	float value;
	int numItems;
	String desc;
	
	// constants
	private static final int TRANS_VALUE_MIN = 10;
	private static final int TRANS_VALUE_MAX = 1000;
	private static final int TRANS_NUM_ITEMS_MIN = 1;
	private static final int TRANS_NUM_ITEMS_MAX = 10;
	private static final int DESC_MIN_LENGTH = 20;
	private static final int DESC_MAX_LENGTH = 50;
	
	public Transaction(int transID, int custID) {
		this.transID = transID;
		this.custID = custID;
		this.value = RandomGenerator.getRandomFloat(TRANS_VALUE_MIN, TRANS_VALUE_MAX);
		this.numItems = RandomGenerator.getRandomNumInt(TRANS_NUM_ITEMS_MIN, TRANS_NUM_ITEMS_MAX)+1;
		this.desc = RandomGenerator.getRandomString(DESC_MIN_LENGTH, DESC_MAX_LENGTH+1);
	}

	/**
	 * @return the transID
	 */
	public int getTransID() {
		return transID;
	}

	/**
	 * @return the custID
	 */
	public int getCustID() {
		return custID;
	}

	/**
	 * @return the value
	 */
	public float getValue() {
		return value;
	}

	/**
	 * @return the numItems
	 */
	public int getNumItems() {
		return numItems;
	}

	/**
	 * @return the desc
	 */
	public String getDesc() {
		return desc;
	}
}
