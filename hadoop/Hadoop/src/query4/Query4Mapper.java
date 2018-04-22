package query4;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;

import org.apache.hadoop.filecache.DistributedCache;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

import utils.Constants;

public class Query4Mapper 
	extends Mapper<LongWritable, Text, IntWritable, Text> {
	
	private HashMap<String, String> hm = new HashMap<>();
	
	@Override
	public void setup(Context context) throws IOException, InterruptedException {
		Path[] localFiles = DistributedCache.getLocalCacheFiles(context.getConfiguration());
		if (localFiles != null && localFiles.length > 0) {
			BufferedReader reader = new BufferedReader(new FileReader(localFiles[0].toString()));
			String line = "";
			while ((line = reader.readLine()) != null) {
				String[] tokens = line.split(Constants.COMMA_DELIMITER);
				String countryCode = tokens[3];
				String custID = tokens[0];
				hm.put(custID, countryCode);
			}
			reader.close();
		}
	}
	
	@Override
	public void map(LongWritable key, Text value, Context context)
		throws IOException, InterruptedException {
		
		String line = value.toString();
		String[] tokens = line.split(Constants.COMMA_DELIMITER);
		String transValue = tokens[2];
		String custID = tokens[1];
		int countryCode = Integer.parseInt(hm.get(custID));
		context.write(new IntWritable(countryCode), new Text(transValue));
	}
}
