class Buffer {

    private int capacity;
    private int[] bufArray;
    private int readIndex;
    private int writeIndex;
    private int length;


    public Buffer(int bufSize) {
        this.capacity = bufSize;
        readIndex = 0; length = 0; writeIndex = 0;
        bufArray = new int[bufSize];
    }

    public boolean isFull() {                                      //return if the buffer is full(the number of elements is equal to the capacity)
        if (this.length == this.capacity){
            return true;
        }
        return false;
    }

    public boolean isEmpty() {                                      //return if the buffer is empty/has no more elements to be consumed
        if (this.length == 0) {
            return true;
        }
        return false;
    }

    public int getLastRemoval() {
        int lastRemoval = this.readIndex - 1;                      //retreive the index of the element most recently removed, the element before the oldest element
        if(lastRemoval < 0) {                                       //if readIndex(the index of the oldest element) is 0, the index of the most recently removed element must be the end of the buffer
            lastRemoval = this.capacity - 1;
        }
        return lastRemoval;
    }
    public int getLastWrtten() {                                //retrieve the index of the element last inserted
        int writtenInto = this.writeIndex - 1;
        if(writtenInto < 0) {                                   //if writeIndex(the index where the next element is to be inserted) is 0, the last index written into was the end of the buffer
            writtenInto = this.capacity -1;
        }
        if (this.isEmpty()) {
            System.out.println("The buffer is empty");
            return -1;
        }
        return writtenInto;
    }

    public int append(int element) {                    //Insert an element into the buffer, into the index held by variable "writeIndex"
        this.length += 1;                               //The amount of elements is increased by 1
        this.bufArray[writeIndex] = element;            
        this.incWriteIndex();                           //The index for where the next element is to be inserted is increased by 1
        return element;
    }

    public int remove() {                               //Remove an element from the buffer, from the index held by variable "readIndex"
        int element = this.bufArray[readIndex];
        this.bufArray[readIndex] = 0;                   //Set the value of this buffer slot to 0(signifying empty)
        this.length -= 1;                               //The amount of elements in the buffer is decreased by 1
        this.incReadIndex();
        return element;
    }
    private void incReadIndex() {                       //Increases the index for where the next element is to be removed
        if(++this.readIndex == this.capacity) {
            this.readIndex = 0;                         //If we are at the end of the buffer, the next index to be inserted is index 0(thus, the buffer is circular)
        }
        return;
    }
    private void incWriteIndex() {                      //Increases the index for where the next element is to be inserted
        if(++this.writeIndex == this.capacity) {
            this.writeIndex = 0;                        //If we are at the end of the buffer, the next index to be inserted is index 0(thus, the buffer is circular)
        }  
        return;
    }
}