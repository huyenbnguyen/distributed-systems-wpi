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
public class TransactionMapper 
	extends Mapper<LongWritable, Text, IntWritable, Text>{
	
	@Override
	public void map(LongWritable key, Text value, Context context)
		throws IOException, InterruptedException {
		
		String line = value.toString();
		String[] tokens = line.split(Constants.COMMA_DELIMITER);
		int custID = Integer.parseInt(tokens[1]);
		String transValue = tokens[2];
		String numItem = tokens[3];
		String valueStr = 1 + "," + transValue + "," + numItem;
		Text valueText = new Text(valueStr);
		context.write(new IntWritable(custID), valueText);
	}

}
