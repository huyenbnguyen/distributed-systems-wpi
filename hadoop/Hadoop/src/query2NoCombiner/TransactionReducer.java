/**
 * 
 */
package query2NoCombiner;

import java.io.IOException;

//import org.apache.hadoop.io.FloatWritable;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

import utils.Constants;

/**
 * @author hadoop
 *
 */
public class TransactionReducer 
	extends Reducer<IntWritable, Text, IntWritable, Text>{

	@Override
	public void reduce(IntWritable key, Iterable<Text> values, Context context) {
		int transNum = 0;
		float transValue = 0;
		for (Text value : values) {
			String line = value.toString();
			String[] tokens = line.split(Constants.COMMA_DELIMITER);
			transNum += Integer.parseInt(tokens[0]);
			transValue += Float.parseFloat(tokens[1]);
		}
		String valuePair = transNum + "," + transValue;
		Text value = new Text(valuePair);
		try {
			context.write(key, value);
		} catch (IOException e) {
			e.printStackTrace();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
}
