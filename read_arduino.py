import serial
import time

# Set up the serial connection
arduino_port = '/dev/cu.usbmodem142301'  # Update this with your Arduino's port
baud_rate = 9600  # Match this with the baud rate in your Arduino sketch

# Store sensor data
t_inlet = []
t_outlet = []
flow_outlet = []

# Store time
timestamps = []

try:
    ser = serial.Serial(arduino_port, baud_rate, timeout=1)
    time.sleep(2)  # Wait for the connection to establish

    # Establish a number of readings
    num_readings = 1800

    # Record the start time
    start_time = time.time()

    while len(t_inlet) < num_readings:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            data = line.split(",")
            if len(data) == 4:
                # Read 
                t_in = data[0]
                t_inlet.append(str(t_in))
        
                t_out = data[1]
                t_outlet.append((t_out))

                f_out = data[2]
                flow_outlet.append(str(f_out))

                # Calculate elapsed time in seconds
                elapsed_time = time.time() - start_time
                timestamps.append(round(elapsed_time, 2))
        
                print(f"Temperature in: {t_in} °C\n")
                print(f"Temperature out: {t_out} °C\n")
                print(f"Flowrate Out: {f_out} [L/min]\n")
except serial.SerialException as e:
    print(f"Error: {e}")
finally:
    ser.close()
