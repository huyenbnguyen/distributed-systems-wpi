/**
 * 
 */
package query2WithCombiner;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

import query2NoCombiner.TransactionMapper;
import query2NoCombiner.TransactionReducer;
import utils.Constants;

/**
 * @author hadoop
 *
 */
public class Query2WithCombinerMain {

	/**
	 * @param args
	 */
	public static void main(String[] args) throws Exception {
		if (args.length != 2) {
			System.err.println("Usage: Query2WithCombiner <transaction file path> <output path>");
			System.exit(-1);
		}
		
		Job job = new Job();
		final Configuration conf = job.getConfiguration();
		conf.set("mapred.textoutputformat.separator", Constants.COMMA_DELIMITER);
		job.setJarByClass(Query2WithCombinerMain.class);
		job.setJobName("Query 2 With Combiner");
		FileInputFormat.addInputPath(job, new Path(args[0]));
		FileOutputFormat.setOutputPath(job, new Path(args[1]));
		
		job.setMapperClass(TransactionMapper.class);
		job.setCombinerClass(TransactionReducer.class);
		job.setReducerClass(TransactionReducer.class);
		job.setOutputKeyClass(IntWritable.class);
		job.setOutputValueClass(Text.class);
		
		System.exit(job.waitForCompletion(true) ? 0 : 1);

	}

}
