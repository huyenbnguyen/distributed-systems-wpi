/**
 * 
 */
package query4;

import java.net.URI;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.filecache.DistributedCache;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import utils.Constants;

/**
 * @author hadoop
 *
 */
public class Query4Driver {

	/**
	 * @param args
	 */
	public static void main(String[] args) throws Exception {
		if (args.length != 3) {
			System.err.println("Usage: Query4 <customer file path> <transaction file path> <output path>");
			System.exit(-1);
		}
		
		Job job = new Job();
		final Configuration conf = job.getConfiguration();
		conf.set("mapred.textoutputformat.separator", Constants.COMMA_DELIMITER);
		job.setJarByClass(Query4Driver.class);
		job.setJobName("Query 4");
		DistributedCache.addCacheFile(new URI(args[0] + "#customerData"), conf);
		FileInputFormat.addInputPath(job, new Path(args[1]));
		FileOutputFormat.setOutputPath(job, new Path(args[2]));
		job.setMapperClass(Query4Mapper.class);
		job.setReducerClass(Query4Reducer.class);
		job.setOutputKeyClass(IntWritable.class);
		job.setOutputValueClass(Text.class);
	
		System.exit(job.waitForCompletion(true) ? 0 : 1);
	}

}
