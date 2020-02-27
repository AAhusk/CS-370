import java.lang.Exception;
import java.util.Random;
import java.time.Instant;
import java.time.Clock;
import java.time.Duration;
import java.lang.Integer;

class Coordinator 
{

	public static void main(String[] args) 
	{
		int bufferSize = Integer.parseInt(args[0]);								//store the command line arguments in these variables
		int numItems = Integer.parseInt(args[1]);
		int numConsumers = Integer.parseInt(args[2]);
		int seedProvided = Integer.parseInt(args[3]);

		Buffer buffer = new Buffer(bufferSize);
		Producer producer = new Producer(buffer, 0, numItems, seedProvided);	//create a producer including access to the buffer, and including the number of items to produce
		
		producer.start();														//start the producer thread
		int sumConsumed = runConsumers(buffer, numConsumers, numItems);			//start all the consumers, passing in necessary info for creation of a consumer thread
		try {
			producer.join();													//wait for the producer to finish
		}
		catch(InterruptedException e) {
			System.out.println("Producer interrupted");
		}

		//print the finishing stats, after producer and consumers have finished runnning

		System.out.printf("Producer: finished producing %d items with checksum being %d\n", numItems, producer.getCheckSum());			
		System.out.printf("\033[0;4mConsumer: finished consuming %d items with checksum being %d\n\033[0;0m", numItems, sumConsumed);

	}

	private static int runConsumers(Buffer buffer, int numConsumers, int numItems) {
		int numToConsume = numItems/numConsumers;
		int sumConsumed = 0;
		Consumer[] consumerArray = new Consumer[numConsumers];
		for (int i=1; i<=numConsumers; ++i) {									//iterate through as many times as how many consumers should be created
			Consumer consumer = new Consumer(buffer, i, numToConsume);
			consumerArray[i-1] = consumer;
			consumer.start();													//start consumer thread
		}
		for (int j=0; j<consumerArray.length; ++j) {
			Consumer consumerToJoin = consumerArray[j];
			try {
				consumerToJoin.join();												//wait for current consumer thread to finish
				sumConsumed += consumerToJoin.getCheckSum();								//keep track of the sum of the values consumed from the buffer
			}
			catch(InterruptedException e) {
				System.out.println("Consumer interrupted");
			}
		}
		return sumConsumed;
	}
	//Call this function from your producer or your conusmer to get the time stamp to be displayed
	public static String getTime()
	{
		Clock offsetClock = Clock.offset(Clock.systemUTC(), Duration.ofHours(-9));
		Instant time = Instant.now(offsetClock);
		String timeString = time.toString();
		timeString = timeString.replace('T', ' ');
		timeString = timeString.replace('Z', ' ');
		return(timeString);
	}
}