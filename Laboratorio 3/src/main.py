import serial
import time
import csv

# Configurar el puerto serial
ser = serial.Serial('COM2', 9600)  # Cambiar 'COM2' al puerto COM al que está conectado tu Arduino
counter = 0

with open("dataCollecter.csv", "w", newline="") as csvfile:
    csv_writer = csv.writer(csvfile)

    try:
        while True:
            counter += 0.5 # Se fija un contador de 0.5s por que es el que se fijó en Arduino
            # Hora
            current_time = time.strftime("%S", time.localtime())
            # Leer línea desde el puerto serial
            line = ser.readline().decode().strip()
            setpoint, input_val, output = map(float, line.split(","))
            csv_writer.writerow([counter, setpoint, input_val, output])
            # print("Datos recibidos:", line)
            print(f"[{counter}] {setpoint}, {input_val}, {output}")
    except KeyboardInterrupt:
        ser.close()
        print("Conexion cerrada.")
