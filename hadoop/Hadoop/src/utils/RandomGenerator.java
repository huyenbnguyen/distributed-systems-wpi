package utils;
import java.util.Random;
import java.util.concurrent.ThreadLocalRandom;

/**
 * 
 */

/**
 * @author hadoop
 *
 */
public class RandomGenerator {
	
	/**
	 * Generate a random string of random length between minLen (inclusive) and maxLen (exclusive)
	 * @param minLen minimum length
	 * @param maxLen maximum length
	 * @return String a random string of length within the specified range
	 */
	public static String getRandomString(int minLen, int maxLen) {
		String chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
		int randomLength = getRandomNumInt(minLen, maxLen);
		StringBuilder sb = new StringBuilder(randomLength);
		for (int i = 0; i < randomLength; i++)
			sb.append(chars.charAt(getRandomNumInt(0, chars.length())));
		return sb.toString();
	}
	
	/**
	 * Generate random int between minValue (inclusive) and maxValue (exclusive)
	 * @param minValue minimum value 
	 * @param maxValue maximum value
	 * @return int a random integer within specified range
	 */
	public static int getRandomNumInt(int minValue, int maxValue) {
		return ThreadLocalRandom.current().nextInt(minValue, maxValue);
 	}
	
	/**
	 * Generate random float between minValue (inclusive) and maxValue (exclusive)
	 * @param minValue minimum value 
	 * @param maxValue maximum value
	 * @return float a random float within specified range
	 */
	public static float getRandomFloat(int minValue, int maxValue) {
		Random rand = new Random();
		return getRandomNumInt(minValue, maxValue) + rand.nextFloat();
	}
}
