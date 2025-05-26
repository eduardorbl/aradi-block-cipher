import matplotlib.pyplot as plt
import csv
import os

trials = []
no_shuffle = []
shuffle_1 = []
shuffle_2 = []

csv_path = os.path.join(os.path.dirname(__file__), 'bench_results.csv')

with open(csv_path, 'r') as csvfile:
    reader = csv.DictReader(csvfile)
    for row in reader:
        trials.append(int(row['Trials']))
        no_shuffle.append(float(row['No_Shuffle(s)']))
        shuffle_1.append(float(row['Shuffle_1Block(s)']))
        shuffle_2.append(float(row['Shuffle_2Blocks(s)']))

plt.figure(figsize=(10, 6))

plt.plot(trials, no_shuffle, marker='o', label='No Shuffle')
plt.plot(trials, shuffle_1, marker='o', label='Shuffle 1 Block')
plt.plot(trials, shuffle_2, marker='o', label='Shuffle 2 Blocks')

plt.xlabel('Number of Trials')
plt.ylabel('Execution Time (s)')
plt.title('ARADI Implementations Performance Comparison')
plt.legend()
plt.grid(True)

plt.savefig(os.path.join(os.path.dirname(__file__), 'bench_plot.png'))
plt.show()
