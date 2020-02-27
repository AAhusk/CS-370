import java.util.Random;
import java.time.Clock;
import java.time.Instant;
import java.time.Duration;

public class Producer extends java.lang.Thread  
{
	//Necessary variables and object declaration
	Random randomWithSeed;
	Buffer buff;
	private int checkSum;
	private int number;
	private int count;

	public Producer(Buffer buff, int count, int number, int seed) 
	{
		//Assign values to the varaibles
		this.randomWithSeed = new Random(seed);
		this.buff = buff;
		this.count = 0;
		this.number = number;
	}

	@Override
	public void run() 
	{
		while(!this.isDone()) {								//while the producer hasn't produced as many elements as it was told to
			try {
				synchronized (this.buff) {					//synchronized so only one thread can access the buffer at a time
					while (this.buff.isFull()) {			
						this.buff.wait();					//if the buffer is full, wait for a consumer to remove at least one element
					}
					int variable = randomWithSeed.nextInt(99) + 1;	//get the random number to insert into the buffer
					this.appendElement(variable);
					this.buff.notifyAll();					//notify all other threads that this thread is done accessing the critical section
				}
			}
			catch(InterruptedException e) {
				System.out.println("Interrupted!");
			}		
		}
		return;
	}

	private void appendElement(int element) {
		this.buff.append(element);						//add the random number to the buffer
		int writeIndex = this.buff.getLastWrtten();		//get the index that the number was written into
		String time = Coordinator.getTime();
		System.out.printf("Producer     inserted %3d  at  index %d at time %s\n", element, writeIndex, time);
		this.count += 1;								//increase the count of the number of elements the producer has produced
		this.checkSum += element;						//add the value of the element added to the sum of values added to the buffer
	}

	public int getCheckSum() {							//return the sum of the values of elements added
		return this.checkSum;
	}
	public boolean isDone() {							//if the producer has produced as many items as it was told to, it is done
		if (this.count == this.number) {
			return true;
		}
		return false;
	}
}
