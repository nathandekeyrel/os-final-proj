import csv
import random

'''
pid,        arrival_time,       priority,       burst_time,     io_frequency
1,                  0,              100,             50,             0      # CPU-bound
2,                  5,            110,               5,             5       # IO-bound
'''

#FUNCTION TO CREAT WORKLOAD SIMULATIONS FROM ABOVE EXAMPLE :)
def create_workload(num):
    num_tasks = 10  
    results = []

    for i in range(num_tasks):
        arrival_time = i * 5  
        priority = random.randint(-20, 20)  
        burst_time = random.randint(5, 15)  
        io_frequency = random.randint(0, 50) 
        pidID = i+1
        results.append([pidID, arrival_time, priority, burst_time, io_frequency])

    # WRITE RESULTS TO A AWESOME CSV
    with open(f'task_schedule{num}.csv', mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(['pid', 'arrival_time', 'priority', 'burst_time', 'io_frequency'])

        for result in results:
            writer.writerow(result)

# Running the simulation
if __name__ == "__main__":
    for i in range(5):
        create_workload(i)
