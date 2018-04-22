package query1;

import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class Query1Driver {

	public static void main(String[] args) throws Exception {
		if (args.length != 2) {
			System.err.println("Usage: Query1 <transaction file path> <output path>");
			System.exit(-1);
		}
		
		Job job = new Job();
		job.setJarByClass(Query1Driver.class);
		job.setJobName("Query 1");
		FileInputFormat.addInputPath(job, new Path(args[0]));
		FileOutputFormat.setOutputPath(job, new Path(args[1]));
		
		job.setMapperClass(CustomerMapper.class);
		job.setNumReduceTasks(0);
//		job.setInputFormatClass(TextInputFormat.class);
//		job.setOutputFormatClass(TextOutputFormat.class);
		job.setOutputKeyClass(NullWritable.class);
		job.setOutputValueClass(IntWritable.class);
		
		
		
		System.exit(job.waitForCompletion(true) ? 0 : 1);
	}
}
