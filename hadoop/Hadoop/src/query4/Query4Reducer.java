package query4;

import java.io.IOException;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

import utils.Constants;

public class Query4Reducer 
	extends Reducer<IntWritable, Text, IntWritable, Text>{

	@Override
	public void reduce(IntWritable key, Iterable<Text> values, Context context) {
		float minTransValue = Float.MAX_VALUE, maxTransvalue = -1;
		int custNum = 0;
		for (Text value : values) {
			String line = value.toString();
			String[] tokens = line.split(Constants.COMMA_DELIMITER);
			float transValue = Float.parseFloat(tokens[0]);
			custNum++;
			minTransValue = Math.min(minTransValue, transValue);
			maxTransvalue = Math.max(maxTransvalue, transValue);
		}
		
		String valueStr = custNum + "," + minTransValue + "," + maxTransvalue;
		Text value = new Text(valueStr);
		try {
			context.write(key, value);
		} catch (IOException e) {
			e.printStackTrace();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
}
