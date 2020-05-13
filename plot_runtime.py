import matplotlib.pyplot as plt 
import math
import numpy as np

def get_results(file):
  data = file.read().split('\n')

  results = []
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
    results.append(result)
  return results

file_clifford = open("measurements/clifford_rle_times.txt", "r")
file_chen = open("measurements/chen_rle_times.txt", "r")

f = open("run_times.out")
operation_cost = 1.0/860000000
results = get_results(f)
results.sort(key = lambda x : x['m'])

clifford_results = get_results(file_clifford)
clifford_results.sort(key = lambda x : x['m'])
chen_results = get_results(file_chen)
chen_results.sort(key = lambda x : x['m'])

compressed_sizes = [result['m'] for result in clifford_results]
# C = [r['compression_fact0'] * r['compression_fact1'] for r in results]
# print(np.mean(C), np.std(C))
# rle_runtimes = [result['rle_runtime'] for result in results]
clifford_rle_runtimes = [result['rle_runtime'] for result in clifford_results]
chen_rle_runtimes = [result['rle_runtime'] for result in chen_results]
ratio = [chen_rle_runtimes[i]/clifford_rle_runtimes[i] for i in range(len(clifford_results))]

mnlogmn = [operation_cost * m**2 * math.log(m**2, 2) for m in compressed_sizes]
mn2 = [operation_cost * result['m'] * (result['n']**2) for result in results]
# plt.plot(compressed_sizes, clifford_rle_runtimes, label='Clifford algorithm', linewidth=2)
plt.plot(compressed_sizes, chen_rle_runtimes, label='Chen algorithm')
plt.plot(compressed_sizes, mn2, label='$c \star mn^2$')
# sizes = [result['M'] for result in results]
# naive_runtimes = [result['naive_runtime'] for result in results]
# plt.plot(sizes, naive_runtimes, label='Naive')
# plt.plot(sizes, rle_runtimes, label='Compressed')
# plt.plot(compressed_sizes, NlogN, label='Complexity - $Nlog(N)$')
# plt.plot(xs, aux, label='4x naive')
# plt.plot(xs, compression_factor, label='Compression factor')
plt.xlabel('Length of compressed strings ($m=n$)')
plt.ylabel('seconds')
plt.legend()
# plt.title('Runtimes with one string fixed')
plt.show()