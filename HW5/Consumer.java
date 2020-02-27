public class Consumer extends java.lang.Thread {

    Buffer buff;
    int consumerID;
    int checkSum;
    int numToConsume;
    int count;

    public Consumer(Buffer buff, int id, int numToConsume) {
        this.buff = buff;
        this.consumerID = id;
        this.numToConsume = numToConsume;
        this.count = 0;
    }
    @Override
    public void run() {
        
        while(!this.isDone()) {
            try {
                synchronized (this.buff) {              //synchronized so only one consumer or producer can access the buffer at a time
                    while (this.buff.isEmpty()) {
                        this.buff.wait();               //wait if the buffer is empty so the producer can add more elements to the buffer
                    }
                    this.remove();
                    this.buff.notifyAll();              //notify all other threads that this thread is done accessing the critical section, allowing another thread to do so
                }
            }
            catch(InterruptedException e) {
                System.out.println("Consumer was interrupted!");     //catch an exception in which another thread interrupts this thread in the critical section
            }    
        }
        return;
    }

    private void remove() {                         //remove an element from the buffer
        int removedElem = this.buff.remove();
        String time = Coordinator.getTime();
        System.out.flush();                         //make sure the consumer prints in the correct order
        System.out.printf("\033[0;4mConsumer %3d consumed %3d from index %d at time\033[0;0m %s\n", this.consumerID, removedElem, buff.getLastRemoval(), time);
        this.checkSum += removedElem;               //add the value of the element removed to the sum of values removed
        ++this.count;                               //increase the count of elements removed from the buffer by 1
    }

    public int getCheckSum() {               //return the sum of the values consumed
        return this.checkSum;
    }
    private boolean isDone() {                  //return if this consumer has consumed as many elements as it was told to
        if (this.count == this.numToConsume) {
            return true;
        }
        return false;
    }
}