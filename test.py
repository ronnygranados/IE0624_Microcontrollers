import time

h = 5
Cps = 0.89
area = 1e-4
masa = 10
Tamb = 25
T = Tamb
last = time.time()
interval = 0.1  # intervalo en segundos, equivalente a 100 milisegundos

def simPlanta(Q):
    global T, last

    if time.time() - last >= interval:
        last += interval

        T = T + Q * interval / 1000 / masa / Cps - (T - Tamb) * area * h

    return T

print(simPlanta(1000))