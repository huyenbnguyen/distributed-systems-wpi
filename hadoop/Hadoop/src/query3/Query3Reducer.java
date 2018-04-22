package query3;

import java.io.IOException;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

import utils.Constants;

public class Query3Reducer 
	extends Reducer<IntWritable, Text, IntWritable, Text>{

	@Override
	public void reduce(IntWritable key, Iterable<Text> values, Context context) {
		int transNum = 0;
		float transValue = 0;
		int minItem = Integer.MAX_VALUE;
		String name = "", salary = "";
		for (Text value : values) {
			String line = value.toString();
			String[] tokens = line.split(Constants.COMMA_DELIMITER);
			if (tokens.length == 3) {
				transNum += Integer.parseInt(tokens[0]);
				transValue += Float.parseFloat(tokens[1]);
				minItem = Math.min(minItem, Integer.parseInt(tokens[2]));
			} else if (tokens.length == 2) {
				name = tokens[0];
				salary = tokens[1];
			}
		}
		
		if (transNum == 0) minItem = 0; // handle customers with 0 transaction
		String valueStr = name + "," + 
				salary + "," + 
				transNum + "," + 
				transValue + "," + 
				minItem;
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
