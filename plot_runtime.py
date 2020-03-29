import matplotlib.pyplot as plt 
import math

f_rle = open("run_times_rle.out", "r")
f_naive = open("run_times_naive.out", "r")
times_rle = f_rle.read()
times_naive = f_naive.read()
times_rle = [float(i) for i in times_rle.split(",")]
times_naive = [float(i) for i in times_naive.split(",")]
# times_naive_adjusted = [i*(1+math.log((times_naive.index(i)+1)**2, 2)) for i in times_naive]
# times_rle = [times_rle[i] for i in range(10)]
# times_naive = [times_naive[i] for i in range(10)]
# times_naive_adjusted = [times_naive_adjusted[i] for i in range(10)]
# aux = [i/14 for i in times_rle]
# mnlogmn = [i*(math.log(i,2))/10000.0 for i in range(1,len(times))]
# mnlogmn = [i*i/10000.0 for i in range(1,len(times))]

plt.plot(times_rle, label='Compressed')
plt.plot(times_naive, label='Naive')
plt.legend()
plt.title('Runtimes for strings comressable by 10x')
# plt.plot(times_naive_adjusted, c='y')
# plt.plot(aux, c='g')
plt.show()