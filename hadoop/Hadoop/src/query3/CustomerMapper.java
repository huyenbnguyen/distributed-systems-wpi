/**
 * 
 */
package query3;

import java.io.IOException;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

import utils.Constants;

/**
 * @author hadoop
 *
 */
public class CustomerMapper 
	extends Mapper<LongWritable, Text, IntWritable, Text> {
	
	@Override
	public void map(LongWritable key, Text value, Context context)
		throws IOException, InterruptedException {
		
		String line = value.toString();
		String[] tokens = line.split(Constants.COMMA_DELIMITER);
		int custID = Integer.parseInt(tokens[0]);
		String name = tokens[1];
		String salary = tokens[4];
		String valuePair = name + "," + salary;
		Text valueText = new Text(valuePair);
		context.write(new IntWritable(custID), valueText);
	}

}
