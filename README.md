# Sprint02 - Weather Sensor for Formula E

![Project Circuit](img/circuito.png)

## Project Description

This project consists of a weather sensor designed to capture information about the climatic conditions of Formula E racing tracks. The sensor collects data on luminosity, temperature, humidity, and precipitation, enabling the evaluation of atmospheric conditions in real-time. This information is essential for the TechSphere team, as it allows climatic data to be displayed directly on the team's website, assisting in strategic decisions during races.

## Objective

The main objective of this project is to provide accurate, real-time climatic data that can be utilized by the TechSphere team to:

- Monitor climatic conditions on the racing tracks.
- Inform decisions during races based on meteorological data.
- Improve communication of weather conditions to fans and race participants.

Additionally, the project aims to implement FIWARE configurations by following the instructions from [FIWARE Descomplicado](https://github.com/fabiocabrini/fiware) by professor [Cabrini](https://www.linkedin.com/in/fabio-cabrini/), ensuring that the collected data is efficiently managed and available for use in IoT applications.

### Access to the Simulated Prototype
<a href="https://wokwi.com/projects/410460243920643073" target="_blank" style="text-align: center; margin-right: 10px;">
  <img loading="lazy" src="/img/wokwi.png" width="150px">
</a>

### Explanatory Video about the Project

<a href="" target="_blank" style="text-align: center; margin-right: 10px;">
  <img loading="lazy" src="/img/youtube.png" width="150px">
</a>

## IoT Solution Architecture

![IoT Solution Architecture](img/arquitetura-iot..png)

## Required Resources

### Components Used

- **ESP32 DevKit v1**
- **Photoresistor (LDR) Sensor Module**
- **Connection Wires**
- Temperature and Humidity Sensor (DHT22)
- Rain Sensor
- ESP32 Module for Wi-Fi connectivity

### Back-end

- MQTT Broker for message management
- Server for data processing (can be configured with FIWARE)

### Front-end

- TechSphere team's website for displaying climatic data
- User interface for real-time visualization of meteorological conditions

### Integration with FIWARE

This project utilizes the **FIWARE Platform** as the solution's back-end, where:
- **Postman** is used to test and consume the API.
- A **Virtual Machine on Azure** is used to host the FIWARE server, managing the sending and receiving of data on luminosity, temperature, humidity, and precipitation.

### Features

- **Weather Condition Reading**: The sensors collect data on luminosity, temperature, humidity, and rain, sending the readings to the MQTT broker.
- **MQTT Publishing**: The collected data is published to an MQTT broker, enabling integration with FIWARE Helix.
- **Control via MQTT**: The system has control over devices (such as LEDs) through commands received via MQTT.

### Step-by-Step to Reproduce the Project

To reproduce this project, follow the detailed instructions in professor [Fábio Cabrini's](https://github.com/fabiocabrini/fiware) repository, which describes how to set up the FIWARE environment and integrate IoT devices with the MQTT broker and Helix.

### Libraries Used

The following libraries are required for the code to function:

- **WiFi.h**: For connecting to Wi-Fi networks.
- **PubSubClient.h**: For communication with the MQTT broker.
- **DHT.h**: For reading temperature and humidity (DHT22).
- **Adafruit_Sensor.h**: For sensor integration.

### Platform

Development was carried out on the Wokwi platform, which allows simulating the operation of the ESP32 with the weather sensors.

### How to Use

Clone the repository:

1. ```bash
   git clone https://github.com/Cps-Tech-Sphere/CP4-EdgeComputing.git

2. Configure the FIWARE environment according to the instructions in professor Cabrini's repository.
3. Configure the code with the correct Wi-Fi and MQTT credentials.
4. Upload the code to the ESP32 using the Wokwi platform or Arduino IDE.
5. Monitor the climatic data via Serial Monitor or MQTT broker.


### Autores

<div style="display: flex; justify-content: space-between; align-items: center;">
<a href="https://github.com/jaoAprendiz" target="_blank" style="text-align: center; margin-right: 10px;">
<img loading="lazy" src="https://avatars.githubusercontent.com/jaoAprendiz" width=120>
<p style="font-size:min(2vh, 36px); margin-top: 10px;">João Victor - RM 557595</p>
</a>

<a href="https://github.com/K1rit03" target="_blank" style="text-align: center; margin-right: 10px;">
<img loading="lazy" src="https://avatars.githubusercontent.com/K1rit03" width=120>
<p style="font-size:min(2vh, 36px); margin-top: 10px;">Thiago Oliveira - RM 555485</p>
</a>
</div>

<a href="https://github.com/JeannMatheuss" target="_blank" style="text-align: center; margin-right: 10px;">
<img loading="lazy" src="https://avatars.githubusercontent.com/JeannMatheuss" width=120>
<p style="font-size:min(2vh, 36px); margin-top: 10px;">Jean Matheus - RM 555519</p>
</a>


<a href="https://github.com/Malice112" target="_blank" style="text-align: center; margin-right: 10px;">
<img loading="lazy" src="https://github.com/Alicee112" width=120>
<p style="font-size:min(2vh, 36px); margin-top: 10px;">Maria Alice - 557516</p>
</a>


<a href="https://github.com/iannyrfs" target="_blank" style="text-align: center; margin-right: 10px;">
<img loading="lazy" src="https://github.com/iannyrfs" width=120>
<p style="font-size:min(2vh, 36px); margin-top: 10px;">Ianny Raquel - 559096</p>
</a>
</div>

### Licença

Este projeto está licenciado sob a licença MIT.
