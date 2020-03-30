import matplotlib.pyplot as plt 
import math

f = open("run_times.out", "r")

data = f.read().split('\n')

xs = []
naive_runtimes = []
rle_runtimes = []
compression_factor = []
for i in range(1,len(data)-1):
  elem = data[i].split(',')
  M = int(elem[0])
  N = int(elem[1])
  compression_fact0 = float(elem[2])
  compression_fact1 = float(elem[3])
  naive_runtime = float(elem[4])
  rle_runtime = float(elem[5])
  xs.append(M/compression_fact0)
  compression_factor.append(compression_fact0 * compression_fact1)
  naive_runtimes.append(naive_runtime)
  rle_runtimes.append(rle_runtime)

aux = [4*i for i in naive_runtimes]
plt.plot(xs, rle_runtimes, label='Compressed')
# plt.plot(xs, naive_runtimes, label='Naive')
# plt.plot(xs, aux, label='4x naive')
# plt.plot(xs, compression_factor, label='Compression factor')
plt.legend()
# plt.title('Runtimes for strings comressable by 10x')
# # plt.plot(times_naive_adjusted, c='y')
# # plt.plot(aux, c='g')
plt.show()