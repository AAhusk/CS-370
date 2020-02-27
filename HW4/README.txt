File description:

scheduler.py - I used only one file with one class because all three scheduling algorithms need the same class variables, e.g. Gantt chart, current time, wait time and 
turnaround time(contained in a table); multiple classes/files would've used redundant variables and methods. Therefore, I created one class for scheduling, with three 
different functions to incorporate the three different scheduling algorithms. Each scheduling algorithm is called by first creating one object for each algorithm. Then,
the respective scheduling algorithm method is called on each of these three objects.

Questions:
1) Shortest-remaining-time-first
2) The five states are: new, ready, waiting, running, terminated
3) When priority is based on the burst time of the process(highest priority for shortest burst time)
4) Convoy effect
5) Using aging: the longer a process waits, the higher its priority becomes.    