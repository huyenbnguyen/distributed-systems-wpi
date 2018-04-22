/**
 * 
 */
package query3;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.MultipleInputs;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

import utils.Constants;

/**
 * @author hadoop
 *
 */
public class Query3Driver {

	/**
	 * @param args
	 */
	public static void main(String[] args) throws Exception {
		if (args.length != 3) {
			System.err.println("Usage: Query3 <customer file path> <transaction file path> <output path>");
			System.exit(-1);
		}
		
		Job job = new Job();
		final Configuration conf = job.getConfiguration();
		conf.set("mapred.textoutputformat.separator", Constants.COMMA_DELIMITER);
		job.setJarByClass(Query3Driver.class);
		job.setJobName("Query 3");
		MultipleInputs.addInputPath(job, new Path(args[0]), TextInputFormat.class, CustomerMapper.class);
		MultipleInputs.addInputPath(job, new Path(args[1]), TextInputFormat.class, TransactionMapper.class);
		FileOutputFormat.setOutputPath(job, new Path(args[2]));
		job.setReducerClass(Query3Reducer.class);
		job.setOutputKeyClass(IntWritable.class);
		job.setOutputValueClass(Text.class);
	
		System.exit(job.waitForCompletion(true) ? 0 : 1);
	}

}
