import tkinter as tk
from tkinter import ttk
import serial
import threading
import spotify

class SerialMonitorApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Serial Monitor")
        
        # Set fixed window size
        self.root.geometry('600x200')

        # Create labels to display state and LED value
        self.state_label = ttk.Label(self.root, text="Estado: ", font=('Helvetica', 14))
        self.state_label.pack(padx=20, pady=20)
        self.led_label = ttk.Label(self.root, text="LED: ", font=('Helvetica', 14))
        self.led_label.pack(padx=20, pady=20)
        self.music = ttk.Label(self.root, font=('Helvetica', 14))
        self.music.pack(padx=20, pady=20)

        # Set up the serial connection (adjust COM port and baud rate as needed)
        self.serial_port = 'COM6'
        self.baud_rate = 9600
        self.ser = serial.Serial(self.serial_port, self.baud_rate, timeout=1)
        
        # Start a thread to read from serial port
        self.read_serial_thread = threading.Thread(target=self.read_serial)
        self.read_serial_thread.daemon = True  # Daemonize thread
        self.read_serial_thread.start()
        
        self.state_var = ""
        
        self.past_state = None
        self.current_state = None 

    def read_serial(self):
        while True:
            if self.ser.in_waiting > 0:
                value = self.ser.readline()
                valueString = value.decode('utf-8').strip()
                print(valueString)

                # Check for specific lines
                if valueString.startswith('Estado:'):
                    state = valueString.split(': ')[1]
                    self.update_state_label(state)
                elif valueString.startswith('LED:'):
                    led_value = valueString.split(': ')[1]
                    self.update_led_label(led_value)

    def update_state_label(self, state):
        self.state_label.config(text=f"Estado: {state}")
        
        self.update_state(state)
        
        if state == "PLAY" and self.past_state != "PLAY":
            spotify.play_music()
            track_info = spotify.get_current_playing()
            if track_info:
                self.music.config(text=f"Now playing: {track_info['name']} by {', '.join(track_info['artists'])}")
        elif state == "PLAY" and self.past_state == "PLAY":
            pass
        elif state == "STOP" and self.past_state != "STOP":
            spotify.pause_music()
            self.music.config(text=f"")
        elif state == "STOP" and self.past_state == "STOP":
            pass
        else:
            pass
        
    def update_led_label(self, led_value):
        if led_value == "1":
            self.led_label.config(text=f"LED: ON", foreground="green")
        elif led_value == "0":
            self.led_label.config(text=f"LED: OFF", foreground="red")
            
    def update_state(self, new_state):
        self.past_state = self.current_state
        self.current_state = new_state

if __name__ == "__main__":
    root = tk.Tk()
    app = SerialMonitorApp(root)
    root.mainloop()
