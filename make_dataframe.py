import pandas as pd
import read_arduino as ra

# Make Dataframe
data = [ra.t_inlet, ra.t_outlet]
df = pd.DataFrame({'Time [s]': ra.timestamps, 'T in [ºC]': ra.t_inlet, 'T out [ºC]': ra.t_outlet})
print(df)

# Store data to csv file
csv_file = "temp_vs_time_2.csv"
df.to_csv(csv_file, index=True)

# Make Dataframe
data2 = [ra.flow_outlet]
df2 = pd.DataFrame({'Time [s]': ra.timestamps, 'Flowrate Out [L/min]': ra.flow_outlet})
print(df2)

# Store data to csv file
csv_file_2 = "flow_vs_time_2.csv"
df2.to_csv(csv_file_2, index=True)
