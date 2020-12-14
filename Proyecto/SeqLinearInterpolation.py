from interpo_trozos import trozos
from sys import stdin
import numpy as np
import matplotlib.pyplot as plt
import statistics
from time import time

def read_data():
    n = 8023
    data = [None for _ in range(n)]
    for i in range(n):
        line = stdin.readline().split()
        data[i] = (float(line[0]), float(line[1]))
    return data

def ran_bd():
   n = 1000000
   data = [None for _ in range(n)]
   for i in range(n):
     data[i] = (int(i+1), random.randint(900,4000))
   return data

def main():
    #data = ran_bd()
    data = read_data()
    data_en = []
    data_val = []
    #Division de los datos en entrenamiento y validacion (50% - 50%)
    jump = 2
    for i in range(len(data)):
        if i % jump == 0:
            data_en.append(data[i])
        else:
            data_val.append(data[i])

    #Calculo de interpolacion
    t1 = time()
    A = trozos(data_en)
    t2 = time()
    print("Execution time: {0}".format(t2-t1))
    #Graficar resultados
    plt.plot(0,0,'m', label="Linear Interpolation")
    for i in range(len(data_en)-1):
        punto1 = data_en[i]
        punto2 = data_en[i+1]
        x = np.linspace(punto1[0], punto2[0], 2)
        y = A[0][i] * x + A[1][i] #Polinomio de grado 1
        plt.plot(x,y,'m')
    plt.title("Linear Interpolation by Parts")
    plt.legend()
    plt.grid()
    plt.xlabel('Days')
    plt.ylabel('Dolar Price in COP')
    plt.show()

    #Calculo de error medio para la interpolacion
    x_val = [int(x[0]) for x in data_val]
    y_val = [x[1] for x in data_val]
    last = int(data_en[-1][0])
    yp3 = []
    for t in x_val:
        if t < last: #para evitar extrapolacion
            i = (t // jump) - 1
            yp3.append(A[0][i] * t + A[1][i])
    error = []
    for i in range(len(yp3)):
        error.append(abs(yp3[i] - y_val[i]))
    error_medio = statistics.mean(error)
    print("El error promedio es {0}".format(error_medio))

main()
