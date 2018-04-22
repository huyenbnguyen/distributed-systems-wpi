/**
 * This is the mapper with customer ID as key, and 
 * the transactions 
 */
package query2NoCombiner;

import java.io.IOException;

//import org.apache.hadoop.io.FloatWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

import utils.Constants;

/**
 * @author hadoop
 *
 */
public class TransactionMapper 
	extends Mapper<LongWritable, Text, IntWritable, Text>{
	
	@Override
	public void map(LongWritable key, Text value, Context context)
		throws IOException, InterruptedException {
		
		String line = value.toString();
		String[] tokens = line.split(Constants.COMMA_DELIMITER);
		int custID = Integer.parseInt(tokens[1]);
		String transValue = tokens[2];
		String valuePair = 1 + "," + transValue;
		Text valueText = new Text(valuePair);
		context.write(new IntWritable(custID), valueText);
	}
}
