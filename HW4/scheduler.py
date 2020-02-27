import sys
import csv
#import pdb; pdb.set_trace()

def open_file(filename_arg):                                    #open the CSVfile
    with open(filename) as csvfile:
        readCSV = csv.reader(csvfile, delimiter=',')
        readCSV.next()                                          #skip the first row(the header)
        CSVdata = []                                            #2 dimensional array to hold the CSV data
        for row in readCSV:
            row = [int(element) for element in row]
            CSVdata.append(row)

    return CSVdata

class scheduling:

    Gantt_chart = []                                            #2 dimensional array to hold the Gantt chart. Each entry is an array with 3 values: initial time, process ID, time after executing
    Table = []                                                  #2 dimensional array to hold the table with each entry containing 3 values: process ID, waiting time, and turnaround time
    current_time = 0

    def __init__(self, CSVdata_arg):                            #constructor for the scheduling class
        self.CSVdata = CSVdata_arg
        self.Table = []
        self.Gantt_chart = []
        self.current_time = 0
    
    def schedule_RR(self, time_quantum):
        print("\n----------------- Round-Robin Scheduling -----------------")
        print("    Process ID  | Waiting Time  | Turnaround Time")
        self.CSVdata.sort(key = lambda x: (x[0]))                  #sort the CSVdata by process ID
        arrivals_by_PID = self.get_arrival_time()                  #get the arrival time of each process in order of the process ID
        burst_times = self.get_burst_time()                        #burst times to use for calculating waiting time for each process after running the algorithm
        time_remaining = self.get_burst_time()                     #keeps track of time remaining for each process as they are run for the time quantum, in order of process ID

        self.CSVdata.sort(key = lambda x: (x[1], x[0]))
        ready_queue = []                                            #will keep track of which processes have arrived but not finished
        next_arrival_index = 0
        finish_times = [None]*len(CSVdata)
        idle_time = 0
        process_to_arrive = -1                                      #holds the value of a process if it hasn't finished after it is given a time quantum. This process ID is saved so it can be added to the ready queue after any new processes that arrived while it was running its time quantum
        
        while True:
            next_arrival_index = self.find_ready_processes(ready_queue, next_arrival_index, process_to_arrive)       #add other processes that have arrived to the ready queue
            process_to_arrive = -1
            if (not ready_queue):                                                                          #if ready queue is empty(but not every process has yet been in the ready queue)
                idle_time += 1
                self.current_time += 1                                                                          #then processor is Idle for at least one unit of time
                continue                                                                                   #go to the next iteration to see if a new process has arrived in the ready queue yet
            
            if (idle_time > 0):                                                                            #this will only be reached once the queue has a task to run
                self.create_Gantt_entry(self.current_time-idle_time, "IDLE")                           #create entry for how long the processor was idle
                idle_time = 0

            current_process = ready_queue.pop(0)
            process_time_left = time_remaining[current_process-1]
            if (process_time_left <= time_quantum):                                                        #if process will finish in this time quantum
                self.current_time += process_time_left
                self.create_Gantt_entry(self.current_time - process_time_left, current_process)            
                finish_times[current_process-1] = self.current_time                                        #record the finish time for this process
            else:                                                                                          #else time remaining on current process is reduced by time quantum
                time_remaining[current_process-1] -= time_quantum
                self.current_time += time_quantum
                self.create_Gantt_entry(self.current_time - time_quantum, current_process)                 #gantt entry is created for the current process
                process_to_arrive = current_process
            if (next_arrival_index >= len(self.CSVdata) and (not ready_queue) and process_to_arrive == -1):               #end loop when every entry has arrived, and there are no processes left in the ready queue
                break

        self.create_RR_table(arrivals_by_PID, finish_times, burst_times)                                    #will calculate waiting times and turnaround times of each process
        self.print_table()
        print("\nGantt Chart is:")
        self.print_Gantt_chart()
        wait = self.calc_avg_wait_time(); turnaround = self.calc_avg_turnaround(); throughput = self.calc_throughput()
        self.print_averages(wait, turnaround, throughput)
        return
    '''    
    def run_time_quantum(self, process_time_left, time_quantum, current_process)
        if (process_time_left < time_quantum):
            create_Gantt_entry(current_time, current_process+1, current_time + process_time_left)
            current_time += process_time_left
        else:
            create_Gantt_entry(current_time, current_process+1, current_time+time_quantum)
            time_remaining[current_process] -= time_quantum
            current_time += time_quantum'''

    def find_ready_processes(self, ready_queue, next_arrival_index, process_to_arrive):                     #this method finds any processes that have arrived during the last time quantum that ran
        while (next_arrival_index < len(self.CSVdata) and self.CSVdata[next_arrival_index][1] <= self.current_time):
            ready_queue.append(self.CSVdata[next_arrival_index][0])
            next_arrival_index += 1
        if (process_to_arrive != -1):                                                                       #if there was a process that didn't finish after the last time quantum, it is added back into the ready queue(after any new processes that arrived)
            ready_queue.append(process_to_arrive)
        return next_arrival_index

    def schedule_FCFS(self):
        print("----------------- FCFS -----------------")
        print("    Process ID  | Waiting Time  | Turnaround Time")

        self.CSVdata.sort(key = lambda x:(x[0]))                    #sort processes by process ID
        arrivals_by_PID = self.get_arrival_time()                         #get arrival times in an array, in order of process IDs

        self.CSVdata.sort(key = lambda x: (x[1], x[0]))                     #sort data by arrival times first, then by process ID if arrival times are equal

        arrival_times = self.get_arrival_time()
        burst_times = self.get_burst_time()
        PIDs = self.get_PID()

        self.current_time = 0                        
        for i in range(0, len(PIDs)):                                        #for each process
            if (arrival_times[i] <= self.current_time):                      #if the next process to arrive has arrived
                self.current_time += burst_times[i]                         #the process runs
                self.create_Gantt_entry(self.current_time-burst_times[i], PIDs[i])
                
        
            else:                                                            #there is no process to run yet
                idle_time = arrival_times[i] - self.current_time            #time until the next process to arrive arrives
                self.current_time += idle_time
                self.create_Gantt_entry(self.current_time-idle_time, "IDLE")
                
                self.current_time += burst_times[i]
                self.create_Gantt_entry(self.current_time-burst_times[i], PIDs[i])
                
                
        self.create_table(arrivals_by_PID)                                      #create entries for process ID, waiting time, and turnaround time
        self.print_table()
        print("\nGantt Chart is:")
        self.print_Gantt_chart()
        wait = self.calc_avg_wait_time(); turnaround = self.calc_avg_turnaround(); throughput = self.calc_throughput()  #calculate average waiting time, avg turnaround time, and throughput
        self.print_averages(wait, turnaround, throughput)
        return


    def schedule_PBS(self):
        print("\n----------------- Priority Scheduling -----------------")
        print("    Process ID  | Waiting Time  | Turnaround Time")
        CSVdata.sort(key = lambda x: (x[0]))                                #sort data by process ID
        arrival_times = self.get_arrival_time()                             #get arrival times in an array, in order of process ID

        CSVdata.sort(key = lambda x: (x[3], x[1]))                          #sort CSV data by priority
        self.run_processes(CSVdata)                                         #run_processes contains the actual pbs algorithm

        self.create_table(arrival_times)
        self.print_table()
        print("\nGantt Chart is:")
        self.print_Gantt_chart()
        wait = self.calc_avg_wait_time(); turnaround = self.calc_avg_turnaround(); throughput = self.calc_throughput()
        self.print_averages(wait, turnaround, throughput)
        return
        
    def run_processes(self, process_data):                                                          #process_data originally contains just the CSV data sorted by priority. It is modified by each recursive call to this function
        if len(process_data) == 0:
            return
        earliest_arrival = 0                                                                        #earliest arrival holds the index of the process in process_data that arrives first(since process_data is sorted by priority, not arrival time)
        num_processes = len(process_data)                                                           #num_processes holds the total number of processes to be ran
        for i in range(0, num_processes):                                                           #iterate through processes, starting with the highest priority process
            if (process_data[i][1] > self.current_time):                                                #see if current process(num_processes[i]) hasn't arrived yet
                if (i == num_processes-1):                                                              #see if we have checked every process for arrival
                    idle_time = process_data[earliest_arrival][1] - self.current_time                     #processor is idle until first process arrives
                    self.current_time += idle_time
                    self.create_Gantt_entry(self.current_time-idle_time, "IDLE")
                    
                    burst_time = process_data[earliest_arrival][2]
                    PID = process_data[earliest_arrival][0]
                    del process_data[earliest_arrival]
                    self.current_time+=burst_time
                    self.create_Gantt_entry(self.current_time-burst_time, PID)                            #earliest_arrival process runs
                    self.run_processes(process_data)
                    return
                else:                                                                                  #else there are more processes left
                    if process_data[i][1] < process_data[earliest_arrival][1]:                        #if this process arrives before the earliest arrival time seen thus far
                        earliest_arrival = i                                                            #set the process with the earliest arrival time seen thus far
                    continue
            else:                                                                                       #else current process has arrived
                burst_time = process_data[i][2]
                PID = process_data[i][0]
                del process_data[i] 
                self.current_time += burst_time
                self.create_Gantt_entry(self.current_time-burst_time, PID)                                                 #current process runs
                                                                 
                self.run_processes(process_data)                                                        #recursively call this function with modified process_data
                return

    def create_table(self, arrival_times):
        self.Table = [None]*(len(arrival_times))
        for Gantt_entry in self.Gantt_chart:
            process = Gantt_entry[1]
            if process == "IDLE":
                continue
            else:
                process = int(process)
            arrival_time = arrival_times[process-1]
            execution_time = int(Gantt_entry[0]); finish_time = int(Gantt_entry[2])
            wait_time = execution_time - arrival_time; turnaround_time = finish_time - arrival_time
            
            self.Table[process-1] = [process, wait_time, turnaround_time]
        return

    def create_RR_table(self, arrival_times, finish_times, burst_times):
        process = 1
        for entry in finish_times:
            table_entry = [process, entry - (arrival_times[process-1]+burst_times[process-1]), entry-arrival_times[process-1]]      #calculate wait time and turnaround time for processes
            self.Table.append(table_entry) 
            process += 1
        return

    def get_PID(self):
        PIDs = [row[0] for row in self.CSVdata]
        return PIDs

    def get_arrival_time(self):
        arrival_times = [row[1] for row in self.CSVdata]
        return arrival_times

    def get_burst_time(self):
        burst_times = [row[2] for row in self.CSVdata]
        return burst_times

    def get_priority(self):
        priorities = [row[3] for row in self.CSVdata]
        return priorities

    def create_Gantt_entry(self,time_before, PID):
        self.Gantt_chart.append([str(time_before), str(PID), str(self.current_time)])

    def print_table(self):
        for entry in self.Table:
            print("\t%d\t|\t%d\t|\t%d" % (entry[0], entry[1], entry[2]))

    def print_Gantt_chart(self):
        for entry in self.Gantt_chart:
            print("[\t%s\t]--\t%s\t--[\t%s\t]" % (entry[0], entry[1], entry[2]))

    def calc_avg_wait_time(self):
        total_waiting = 0
        for entry in self.Table:
           total_waiting += entry[1] 
        avg_wait = float(total_waiting)/(len(self.Table))
        return avg_wait
        
    def calc_avg_turnaround(self):
        total_turnaround = 0
        for entry in self.Table:
           total_turnaround += entry[2] 
        avg_turnaround = float(total_turnaround)/(len(self.Table))
        return avg_turnaround

    def calc_throughput(self):
        total_time = self.Gantt_chart[len(self.Gantt_chart)-1][2]
        num_processes = len(self.Table)
        return num_processes/(float(total_time))

    def print_averages(self, wait, turnaround, throughput):
        print("Average Waiting Time: %f" % (wait))
        print("Average Turnaround Time: %f" % (turnaround))
        print("Throughput: %f" % (throughput)) 

if __name__ == "__main__":
    filename = sys.argv[1]
    time_quantum = int(sys.argv[2])

    CSVdata = open_file(filename)

    FCFS_schedule = scheduling(CSVdata)                                     #create object to run FCFS scheduling
    FCFS_schedule.schedule_FCFS()                                           #call FCFS scheduling method
   
    priority_schedule = scheduling(CSVdata)                                 #create object to run Priority scheduling
    priority_schedule.schedule_PBS()

    CSVdata = open_file(filename)                                           #re-open the file, to clean the CSV data for Round Robin
    
    RR_schedule = scheduling(CSVdata)                                       #create object to run Round Robin scheduling
    RR_schedule.schedule_RR(time_quantum)   


