# Sensor Meteorológico para Formula E

![Circuito do Projeto](img/circuito.png)

## Descrição do Projeto

Este projeto consiste em um sensor meteorológico projetado para captar informações sobre as condições climáticas das pistas de corrida da Formula E. O sensor coleta dados sobre luminosidade, temperatura, umidade e precipitação, possibilitando a avaliação das condições atmosféricas em tempo real. Essas informações são essenciais para a equipe TechSphere, pois permitem que os dados climáticos sejam informados diretamente no site da equipe, auxiliando em decisões estratégicas durante as corridas.

## Objetivo

O principal objetivo deste projeto é fornecer dados climáticos precisos e em tempo real que podem ser utilizados pela equipe TechSphere para:

- Monitorar as condições climáticas nas pistas de corrida.
- Informar as decisões durante as corridas com base em dados meteorológicos.
- Melhorar a comunicação das condições do tempo para os fãs e participantes da corrida.

Além disso, o projeto visa implementar as configurações do FIWARE, seguindo as instruções do [FIWARE Descomplicado](https://github.com/fabiocabrini/fiware) do professor [Cabrini](https://www.linkedin.com/in/fabio-cabrini/), para garantir que os dados coletados sejam gerenciados de forma eficiente e estejam disponíveis para utilização em aplicações IoT.

### Acesso ao prototipo simulado
<a href="https://wokwi.com/projects/410460243920643073" target="_blank" style="text-align: center; margin-right: 10px;">
  <img loading="lazy" src="/img/wokwi.png" width="150px">
</a>

### Vídeo explicativo sobre o prejeto

<a href="" target="_blank" style="text-align: center; margin-right: 10px;">
  <img loading="lazy" src="/img/youtube.png" width="150px">
</a>

## Arquitetura da Solução IoT

![Arquitetura da Solução IoT](img/arquitetura-iot..png)

## Recursos Necessários

### Componentes Utilizados

- **ESP32 DevKit v1**
- **Módulo Sensor Fotoresistor (LDR)**
- **Fios de Conexão**
- Sensor de temperatura e umidade (DHT22)
- Sensor de chuva
- Módulo ESP32 para conectividade Wi-Fi

### Back-end

- Broker MQTT para gerenciamento de mensagens
- Servidor para processamento de dados (pode ser configurado com FIWARE)

### Front-end

- Site da equipe TechSphere para apresentação dos dados climáticos
- Interface de usuário para visualização em tempo real das condições meteorológicas

### Integração com FIWARE

Este projeto utiliza a **Plataforma FIWARE** como back-end da solução, onde:
- O **Postman** é utilizado para testar e consumir a API.
- Uma **Máquina Virtual no Azure** é utilizada para hospedar o servidor FIWARE, que gerencia o envio e recebimento de dados de luminosidade, temperatura, umidade e precipitação.

### Funcionalidades

- **Leitura de Condições Meteorológicas**: Os sensores coletam dados de luminosidade, temperatura, umidade e chuva, enviando as leituras para o broker MQTT.
- **Publicação MQTT**: Os dados coletados são publicados em um broker MQTT, permitindo a integração com o FIWARE Helix.
- **Controle via MQTT**: O sistema possui controle de dispositivos (como LEDs) através de comandos recebidos no MQTT.

### Passo a Passo para Reproduzir o Projeto

Para reproduzir este projeto, siga as instruções detalhadas no repositório do professor [Fábio Cabrini](https://github.com/fabiocabrini/fiware), que descreve como configurar o ambiente FIWARE e integrar dispositivos IoT com o broker MQTT e o Helix.

### Bibliotecas Utilizadas

As seguintes bibliotecas são necessárias para o funcionamento do código:

- **WiFi.h**: Para conexão com redes Wi-Fi.
- **PubSubClient.h**: Para comunicação com o broker MQTT.
- **DHT.h**: Para leitura de temperatura e umidade (DHT22).

### Plataforma

O desenvolvimento foi realizado na plataforma Wokwi, que permite simular a operação do ESP32 com os sensores meteorológicos.

### Integração com FIWARE

Este projeto utiliza a **Plataforma FIWARE** como back-end da solução, onde:
- O **Postman** é utilizado para testar e consumir a API.
- Uma **Máquina Virtual no Azure** é utilizada para hospedar o servidor FIWARE, que gerencia o envio e recebimento de dados de luminosidade, temperatura, umidade e precipitação.

### Funcionalidades

- **Leitura de Condições Meteorológicas**: Os sensores coletam dados de luminosidade, temperatura, umidade e chuva, enviando as leituras para o broker MQTT.
- **Publicação MQTT**: Os dados coletados são publicados em um broker MQTT, permitindo a integração com o FIWARE Helix.
- **Controle via MQTT**: O sistema possui controle de dispositivos (como LEDs) através de comandos recebidos no MQTT.

### Passo a Passo para Reproduzir o Projeto

Para reproduzir este projeto, siga as instruções detalhadas no repositório do professor [Fábio Cabrini](https://github.com/fabiocabrini/fiware), que descreve como configurar o ambiente FIWARE e integrar dispositivos IoT com o broker MQTT e o Helix.

### Bibliotecas Utilizadas

As seguintes bibliotecas são necessárias para o funcionamento do código:

- **WiFi.h**: Para conexão com redes Wi-Fi.
- **PubSubClient.h**: Para comunicação com o broker MQTT.
- **DHT.h**: Para leitura de temperatura e umidade (DHT22).
- **Adafruit_Sensor.h**: Para integração de sensores.

### Plataforma

O desenvolvimento foi realizado na plataforma Wokwi, que permite simular a operação do ESP32 com os sensores meteorológicos.

### Como Usar

Clone o repositório:

1. ```bash
   git clone https://github.com/Cps-Tech-Sphere/CP4-EdgeComputing.git

2. Configure o ambiente FIWARE conforme as instruções no repositório do professor Cabrini.
3. Configure o código com as credenciais corretas de Wi-Fi e MQTT.
4. Faça o upload do código no ESP32 através da plataforma Wokwi ou Arduino IDE.
5. Monitore os dados climáticos pelo Serial Monitor ou broker MQTT

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
