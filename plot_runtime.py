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
    results.append(result)
  return results

file_clifford = open("measurements/clifford_rle_times.txt", "r")
file_chen = open("measurements/chen_rle_times.txt", "r")

f = open("run_times.out")
operation_cost = 1.0/200000000
results = get_results(f)

# clifford_results = get_results(file_clifford)
# results = get_results(file_chen)
results.sort(key = lambda x : x['m'])
# chen_results.sort(key = lambda x : x['compressed_size'])

compressed_sizes = [result['m'] for result in results]
rle_runtimes = [result['rle_runtime'] for result in results]
# chen_rle_runtimes = [result['rle_runtime'] for result in chen_results]

# mnlogmn = [operation_cost * m**2 * math.log(m**2, 2) for m in compressed_sizes]
mn2 = [operation_cost * result['m'] * (result['n']**2) for result in results]
# plt.plot(compressed_sizes, rle_runtimes, label='mnlog(mn) algorithm', linewidth=2)
plt.plot(compressed_sizes, rle_runtimes, label='$mn^2$ algorithm')
plt.plot(compressed_sizes, mn2, label='$mn^2$')
# sizes = [result['M'] * result['N'] for result in results]
# naive_runtimes = [result['naive_runtime'] for result in results]
# plt.plot(sizes, naive_runtimes, label='Naive')
# plt.plot(sizes, rle_runtimes, label='Compressed')
# plt.plot(compressed_sizes, NlogN, label='Complexity - $Nlog(N)$')
# plt.plot(xs, aux, label='4x naive')
# plt.plot(xs, compression_factor, label='Compression factor')
plt.xlabel('Size of input ($MxN$)')
plt.ylabel('seconds')
plt.legend()
# plt.title('Runtimes with one string fixed')
plt.show()