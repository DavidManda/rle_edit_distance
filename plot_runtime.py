import matplotlib.pyplot as plt 
import math


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
    result['compressed_size'] = result['m']
    results.append(result)
  return results

file_clifford = open("measurements/clifford_rle_times.txt", "r")
file_chen = open("measurements/chen_rle_times.txt", "r")

f = open("run_times.out")
operation_cost = 1.0/5500000
clifford_results = get_results(f)
# chen_results = get_results(file_chen)
clifford_results.sort(key = lambda x : x['compressed_size'])
# chen_results.sort(key = lambda x : x['compressed_size'])

compressed_sizes = [result['compressed_size'] for result in clifford_results]
clifford_rle_runtimes = [result['rle_runtime'] for result in clifford_results]
# chen_rle_runtimes = [result['rle_runtime'] for result in chen_results]

# mnlogmn = [operation_cost * m**2 * math.log(m**2, 2) for m in compressed_sizes]

plt.plot(compressed_sizes, clifford_rle_runtimes, label='mnlog(mn) algorithm', linewidth=2)
# plt.plot(compressed_sizes, chen_rle_runtimes, label='$mn^2 algorithm$', linewidth=1.3)
# sizes = [result['M'] for result in results]
# naive_runtimes = [result['naive_runtime'] for result in results]
# plt.plot(sizes, naive_runtimes, label='Naive')
# plt.plot(sizes, rle_runtimes, label='Compressed')
# plt.plot(compressed_sizes, NlogN, label='Complexity - $Nlog(N)$')
# plt.plot(xs, aux, label='4x naive')
# plt.plot(xs, compression_factor, label='Compression factor')
plt.xlabel('compressed size of strings')
plt.ylabel('seconds')
plt.legend()
# plt.title('Runtimes with one string fixed')
plt.show()