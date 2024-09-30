import dash
from dash import dcc, html
from dash.dependencies import Input, Output, State
import plotly.graph_objs as go
import requests
from datetime import datetime
import pytz

# Constants for IP and port
IP_ADDRESS = "4.201.196.181"
PORT_STH = 8666
DASH_HOST = "0.0.0.0"  # Set this to "0.0.0.0" to allow access from any IP

# Function to get data from the API
def get_sensor_data(attribute, lastN):
    url = f"http://{IP_ADDRESS}:{PORT_STH}/STH/v1/contextEntities/type/Sensor/id/urn:ngsi-ld:Sensor:001/attributes/{attribute}?lastN={lastN}"
    headers = {
        'fiware-service': 'smart',
        'fiware-servicepath': '/'
    }
    response = requests.get(url, headers=headers)
    if response.status_code == 200:
        data = response.json()
        try:
            values = data['contextResponses'][0]['contextElement']['attributes'][0]['values']
            return values
        except KeyError as e:
            print(f"Key error: {e}")
            return []
    else:
        print(f"Error accessing {url}: {response.status_code}")
        return []

# Function to convert UTC timestamps to São Paulo time
def convert_to_sao_paulo_time(timestamps):
    utc = pytz.utc
    sao_paulo = pytz.timezone('America/Sao_Paulo')
    converted_timestamps = []
    for timestamp in timestamps:
        try:
            timestamp = timestamp.replace('T', ' ').replace('Z', '')
            converted_time = utc.localize(datetime.strptime(timestamp, '%Y-%m-%d %H:%M:%S.%f')).astimezone(sao_paulo)
        except ValueError:
            converted_time = utc.localize(datetime.strptime(timestamp, '%Y-%m-%d %H:%M:%S')).astimezone(sao_paulo)
        converted_timestamps.append(converted_time)
    return converted_timestamps

# Set lastN value
lastN = 10  # Get 10 most recent points at each interval

app = dash.Dash(__name__)

app.layout = html.Div([
    html.H1('TechSphere - Weather Sensor Data Viewer'),
    dcc.Graph(id='sensor-graph'),
    dcc.Store(id='sensor-data-store', data={'timestamps': [], 'luminosity_values': [], 'humidity_values': [], 'temperature_values': [], 'rain_values': []}),
    dcc.Interval(
        id='interval-component',
        interval=10*1000,  # in milliseconds (10 seconds)
        n_intervals=0
    )
])

@app.callback(
    Output('sensor-data-store', 'data'),
    Input('interval-component', 'n_intervals'),
    State('sensor-data-store', 'data')
)
def update_data_store(n, stored_data):
    # Get data for each attribute
    data_luminosity = get_sensor_data('luminosity', lastN)
    data_humidity = get_sensor_data('humidity', lastN)
    data_temperature = get_sensor_data('temperature', lastN)
    data_rain = get_sensor_data('rain', lastN)  # Get data for the rain sensor

    # Process luminosity data
    if data_luminosity:
        luminosity_values = [float(entry['attrValue']) for entry in data_luminosity]
        timestamps = [entry['recvTime'] for entry in data_luminosity]
        timestamps = convert_to_sao_paulo_time(timestamps)
        stored_data['timestamps'].extend(timestamps)
        stored_data['luminosity_values'].extend(luminosity_values)

    # Process humidity data
    if data_humidity:
        humidity_values = [float(entry['attrValue']) for entry in data_humidity]
        stored_data['humidity_values'].extend(humidity_values)

    # Process temperature data
    if data_temperature:
        temperature_values = [float(entry['attrValue']) for entry in data_temperature]
        stored_data['temperature_values'].extend(temperature_values)

    # Process rain data
    if data_rain:
        rain_values = [float(entry['attrValue']) for entry in data_rain]
        stored_data['rain_values'].extend(rain_values)

    return stored_data

@app.callback(
    Output('sensor-graph', 'figure'),
    Input('sensor-data-store', 'data')
)
def update_graph(stored_data):
    if stored_data['timestamps']:
        # Create traces for the plot
        trace_luminosity = go.Scatter(
            x=stored_data['timestamps'],
            y=stored_data['luminosity_values'],
            mode='lines+markers',
            name='Luminosity (Lux)',
            line=dict(color='orange')
        )
        trace_humidity = go.Scatter(
            x=stored_data['timestamps'],
            y=stored_data['humidity_values'],
            mode='lines+markers',
            name='Humidity (%)',
            line=dict(color='blue')
        )
        trace_temperature = go.Scatter(
            x=stored_data['timestamps'],
            y=stored_data['temperature_values'],
            mode='lines+markers',
            name='Temperature (°C)',
            line=dict(color='red')
        )
        trace_rain = go.Scatter(
            x=stored_data['timestamps'],
            y=stored_data['rain_values'],
            mode='lines+markers',
            name='Rain (mm)',
            line=dict(color='lightblue')
        )

        # Create figure
        fig = go.Figure(data=[trace_luminosity, trace_humidity, trace_temperature, trace_rain])

        # Update layout
        fig.update_layout(
            title='Weather Sensor Dashboard',  # Updated graph title
            xaxis_title='Timestamp',
            yaxis_title='Sensor Values',
            hovermode='closest',
            legend_title='Sensors'  # Updated legend title
        )

        return fig

    return {}

if __name__ == '__main__':
    app.run_server(debug=True, host=DASH_HOST, port=8050)
