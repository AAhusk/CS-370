Questions:
1) Semaphore
2) wait() and notify()
3) In the run() function
4) t.join(), where t is a thread

File Descriptions:
Producer.java - The producer produces random numbers which it adds to the buffer. 
    If the buffer is full, the producer waits for the consumer to remove elements from
    the buffer. The producer's method to add elements to the buffer is synchronized,
    so the either the producer or the consumer can access the buffer at any given time.
    There is only one producer.

Consumer.java - There are multiple consumers; they each remove an equal amount of elements
    from the buffer. If the buffer is empty, any current consumer will wait, until the
    producer has added elements to the buffer.

Coordinator.java - Coordinator creates a buffer, creates a producer, and then creates each
    consumer(the number of which is quantified by a command line argument). The consumers are
    created on separate threads, in an iterative loop. By using thread.join(), the producer and
    the consumers wait for all the threads to finish before Coordinator.java continues to run.

Buffer.java - Buffer is a circular, fixed data structure. It holds the integers and has methods
    that determine if the buffer is full, if it's empty, and methods to append and remove elements
    from itself.