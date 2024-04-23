
import statistics

def parse_times(path):
    runtimes = []
    with open(path, 'r') as file:
        for line in file:
            runtimes.append(float(line.strip()))
    return runtimes


runtimes = parse_times('exec_times.txt')
single_thread_avg = 1

avg = statistics.mean(runtimes)
std_dev = statistics.stdev(runtimes)
speed_up = single_thread_avg / avg

print("Average:", avg)
print("Standard Deviation:", std_dev)
print("Speed Up:", speed_up)

