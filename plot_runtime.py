import matplotlib.pyplot as plt 
import math

f = open("run_times.out", "r")

data = f.read().split('\n')

results = []
operation_cost = 1.0/4200000
for i in range(1,len(data)-1):
  elem = data[i].split(',')
  result = {}
  result['M'] = int(elem[0])
  result['N'] = int(elem[1])
  result['compression_fact0'] = float(elem[2])
  result['compression_fact1'] = float(elem[3])
  result['naive_runtime'] = float(elem[4])
  result['rle_runtime'] = float(elem[5])
  result['m'] = result['M']/result['compression_fact0']
  result['n'] = result['N']/result['compression_fact1']
  result['compressed_size'] = result['m']
  results.append(result)

# results.sort(key = lambda x : x['compressed_size'])

naive_runtimes = [result['naive_runtime'] for result in results]
naive_runtimes_penalised = [4 * result['naive_runtime'] * math.log(result['compressed_size']**2,2) for result in results]
rle_runtimes = [result['rle_runtime'] for result in results]
sizes = [result['M'] for result in results]
compressed_sizes = [result['compressed_size'] for result in results]
N2logN2 = [size**2 * math.log(size**2, 2) * operation_cost for size in compressed_sizes]
NlogN = [size * math.log(size, 2) * operation_cost for size in compressed_sizes]
plt.plot(sizes, naive_runtimes, label='Naive')
plt.plot(sizes, naive_runtimes_penalised, label='Naive multiplied by $log(n^2)$')
plt.plot(sizes, rle_runtimes, label='Compressed')
# plt.plot(compressed_sizes, rle_runtimes, label='Compressed')
# plt.plot(compressed_sizes, N2logN2, label='Complexity - $N^2log(N^2)$')
# plt.plot(compressed_sizes, NlogN, label='Complexity - $Nlog(N)$')
# plt.plot(xs, aux, label='4x naive')
# plt.plot(xs, compression_factor, label='Compression factor')
plt.xlabel('N')
plt.legend()
# plt.title('Runtimes for strings comressable by 10x')
# # plt.plot(times_naive_adjusted, c='y')
# # plt.plot(aux, c='g')
plt.show()