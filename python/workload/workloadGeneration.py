import csv
import random

'''
pid,        arrival_time,       priority,       burst_time,     io_frequency
1,                  0,              100,             50,             0      # CPU-bound
2,                  5,            110,               5,             5       # IO-bound
'''


# FUNCTION TO CREAT WORKLOAD SIMULATIONS FROM ABOVE EXAMPLE :)
def create_workload(num):
    num_tasks = 10
    results = []
    current_time = 0

    for i in range(num_tasks):
        pid = i + 1
        arrival_time = current_time
        current_time += random.randint(0, 10)
        priority = random.randint(100, 139)
        burst_time = random.randint(5, 50)
        io_frequency = random.randint(0, 10)
        results.append([pid, arrival_time, priority, burst_time, io_frequency])

    # WRITE RESULTS TO A AWESOME CSV
    filepath = f'../../data/outputs/task_schedule{num}.csv'
    with open(filepath, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(['pid', 'arrival_time', 'priority', 'burst_time', 'io_frequency'])

        for result in results:
            writer.writerow(result)


# Running the simulation
if __name__ == "__main__":
    for i in range(5):
        create_workload(i)
