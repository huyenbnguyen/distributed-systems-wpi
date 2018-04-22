package utils;
/**
 * 
 */

/**
 * @author hadoop
 *
 */
public class Main {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		DataSetCreator.writeCustomerFile("smallCustomers.txt", 10);
		DataSetCreator.writeTransactionFile("smallTransactions.txt", 10, 10);
		DataSetCreator.writeCustomerFile("bigCustomers.txt", 50000);
		DataSetCreator.writeTransactionFile("bigTransactions.txt", 5000000, 50000);
	}

}
