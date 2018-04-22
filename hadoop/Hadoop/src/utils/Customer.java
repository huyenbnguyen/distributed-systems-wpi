package utils;

/**
 * 
 */

/**
 * @author Huyen Nguyen
 *
 */
public class Customer {
	private int id; 
	private String name;
	private int age;
	private int countryCode;
	private float salary;
	
	// constants
	private static final int NAME_MIN_LEN = 10;
	private static final int NAME_MAX_LEN = 20;
	private static final int AGE_MIN = 10;
	private static final int AGE_MAX = 70;
	private static final int COUNTRYCODE_MIN = 1;
	private static final int COUNTRYCODE_MAX = 10;
	private static final int SALARY_MIN = 100;
	private static final int SALARY_MAX = 10000;
	
	public Customer(int id) {
		this.id = id;
		this.name = RandomGenerator.getRandomString(NAME_MIN_LEN, NAME_MAX_LEN+1);
		this.age = RandomGenerator.getRandomNumInt(AGE_MIN, AGE_MAX+1);
		this.countryCode = RandomGenerator.getRandomNumInt(COUNTRYCODE_MIN, COUNTRYCODE_MAX+1);
		this.salary = RandomGenerator.getRandomFloat(SALARY_MIN, SALARY_MAX);
	}

	/**
	 * @return the id
	 */
	public int getId() {
		return id;
	}

	/**
	 * @return the name
	 */
	public String getName() {
		return name;
	}

	/**
	 * @return the age
	 */
	public int getAge() {
		return age;
	}

	/**
	 * @return the countryCode
	 */
	public int getCountryCode() {
		return countryCode;
	}

	/**
	 * @return the salary
	 */
	public float getSalary() {
		return salary;
	}
}
