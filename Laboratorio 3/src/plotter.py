import pandas as pd
import matplotlib.pyplot as plt

# Lee el archivo CSV
data = pd.read_csv("dataCollecter.csv")

# Extrae las columnas
x_values = data.iloc[:, 0]  # Valores de la primera columna

# Obten las columnas del 1 al 3
y_columns = data.iloc[:, 1]

# Grafica los datos de cada columna individualmente
# for i, column in enumerate(y_columns, start=1):
plt.figure(figsize=(7, 6))  # Tamaño de la figura
plt.plot(x_values, y_columns)
# Añade etiquetas y título
plt.xlabel("Tiempo (s)")
plt.ylabel(f"°C")
plt.title(f"Setpoint")
plt.grid(True)  # Muestra la cuadrícula
plt.show()  # Muestra la gráfica
# plt.savefig("temperatura.png")