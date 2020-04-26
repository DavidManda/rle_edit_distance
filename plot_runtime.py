import matplotlib.pyplot as plt 
import math

f = open("run_times.out", "r")

data = f.read().split('\n')

results = []
operation_cost = 1.0/4500
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

results.sort(key = lambda x : x['compressed_size'])

compressed_sizes = [result['compressed_size'] for result in results]
rle_runtimes = [result['rle_runtime'] for result in results]
mlogm = [operation_cost * m * math.log(m, 2) for m in compressed_sizes]
plt.plot(compressed_sizes, rle_runtimes, label='Compressed')
plt.plot(compressed_sizes, mlogm, label='$mlog(m)$')

# sizes = [result['M'] for result in results]
# naive_runtimes = [result['naive_runtime'] for result in results]
# plt.plot(sizes, naive_runtimes, label='Naive')
# plt.plot(sizes, naive_runtimes_penalised, label='Naive multiplied by $log(n^2)$')
# plt.plot(sizes, rle_runtimes, label='Compressed')
# plt.plot(compressed_sizes, NlogN, label='Complexity - $Nlog(N)$')
# plt.plot(xs, aux, label='4x naive')
# plt.plot(xs, compression_factor, label='Compression factor')
plt.xlabel('N')
plt.ylabel('seconds')
plt.legend()
plt.title('Runtimes with one string fixed with compressed length 735')
plt.show()