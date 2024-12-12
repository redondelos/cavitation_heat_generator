import plotly.express as px
import make_dataframe as mdata

fig = px.line(mdata.df, x="Time [s]", y=["T in [ºC]", "T out [ºC]"], line_shape="spline",
            labels={'value': 'Temperature [ºC]', 'variable': 'Sensor'},
            title='Inlet and Outlet Temperatures Over Time')

fig.show()

fig2 = px.line(mdata.df, x="Time [s]", y=["T out [ºC]"], line_shape="spline",
            labels={'value': 'Temperature [ºC]', 'variable': 'Sensor'},
            title='Outlet Temperature Over Time')

fig2.show()

fig3 = px.line(
    mdata.df2,  # Ensure this is the correct DataFrame for flowrate data
    x="Time [s]",
    y=["Flowrate Out [L/min]"],  # Ensure this column exists in mdata.df2
    line_shape="spline",
    labels={'value': 'Flowrate [L/min]', 'variable': 'Flow Sensor'},
    title='Flowrate Over Time'  # Correct title for this plot
)

fig3.show()
