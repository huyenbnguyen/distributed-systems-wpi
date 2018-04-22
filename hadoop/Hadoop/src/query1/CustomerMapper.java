/**
 * 
 */
package query1;
import java.io.IOException;

import org.apache.hadoop.io.IntWritable; 
import org.apache.hadoop.io.LongWritable; 
import org.apache.hadoop.io.Text; 
import org.apache.hadoop.mapreduce.Mapper;

import utils.Constants;

import org.apache.hadoop.io.NullWritable; 
/**
 * @author hadoop
 *
 */
public class CustomerMapper 
	extends Mapper<LongWritable, Text, NullWritable, IntWritable>{
	
	private static final int MIN_COUNTRYCODE = 2;
	private static final int MAX_COUNTRYCODE = 6;
	
	@Override
	public void map(LongWritable key, Text value, Context context)
		throws IOException, InterruptedException {
		
		String line = value.toString();
		String[] tokens = line.split(Constants.COMMA_DELIMITER);
		int countryCode = Integer.parseInt(tokens[3]);
		if (countryCode >= MIN_COUNTRYCODE && countryCode <= MAX_COUNTRYCODE) {
			int custID = Integer.parseInt(tokens[0]);
			context.write(NullWritable.get(), new IntWritable(custID));
		}
	}
}
