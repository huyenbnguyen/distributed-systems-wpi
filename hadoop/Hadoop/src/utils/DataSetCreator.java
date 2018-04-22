package utils;
import java.io.FileWriter;
import java.io.IOException;
/**
 * 
 */

/**
 * @author hadoop
 *
 */
public class DataSetCreator {
	
	public static void writeCustomerFile(String fileName, int recordSize) {
		FileWriter fileWriter = null;
		
		try {
			fileWriter = new FileWriter(fileName);
			
			for (int i = 0; i < recordSize; i++) {
				Customer customer = new Customer(i);
				fileWriter.append(String.valueOf(customer.getId()));
				fileWriter.append(Constants.COMMA_DELIMITER);
				fileWriter.append(customer.getName());
				fileWriter.append(Constants.COMMA_DELIMITER);
				fileWriter.append(String.valueOf(customer.getAge()));
				fileWriter.append(Constants.COMMA_DELIMITER);
				fileWriter.append(String.valueOf(customer.getCountryCode()));
				fileWriter.append(Constants.COMMA_DELIMITER);
				fileWriter.append(String.valueOf(customer.getSalary()));
				fileWriter.append(Constants.NEWLINE_SEPARATOR);
			}
		} catch (Exception e) {
			System.out.println("Error in writeCustomerFile()");
			e.printStackTrace();
		} finally {
			try {
				fileWriter.flush();
				fileWriter.close();
			} catch (IOException e) {
				System.out.println("Error flushing/closing file writer");
				e.printStackTrace();
			}
		}
	}
	
	public static void writeTransactionFile(String fileName, int recordSize, int customerNum) {
		FileWriter fileWriter = null;
		
		try {
			fileWriter = new FileWriter(fileName);
			for (int i = 0; i < recordSize; i++) {
				int randomCustID = RandomGenerator.getRandomNumInt(0, customerNum);
				Transaction transaction = new Transaction(i, randomCustID);
				fileWriter.append(String.valueOf(transaction.getTransID()));
				fileWriter.append(Constants.COMMA_DELIMITER);
				fileWriter.append(String.valueOf(transaction.getCustID()));
				fileWriter.append(Constants.COMMA_DELIMITER);
				fileWriter.append(String.valueOf(transaction.getValue()));
				fileWriter.append(Constants.COMMA_DELIMITER);
				fileWriter.append(String.valueOf(transaction.getNumItems()));
				fileWriter.append(Constants.COMMA_DELIMITER);
				fileWriter.append(transaction.getDesc());
				fileWriter.append(Constants.NEWLINE_SEPARATOR);
			}
		} catch (Exception e) {
			System.out.println("Error in writeCustomerFile()");
			e.printStackTrace();
		} finally {
			try {
				fileWriter.flush();
				fileWriter.close();
			} catch (IOException e) {
				System.out.println("Error flushing/closing file writer");
				e.printStackTrace();
			}
		}
	}
}
