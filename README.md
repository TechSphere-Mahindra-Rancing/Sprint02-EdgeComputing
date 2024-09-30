# Weather Sensor Dashboard

## Descrição do Projeto

Este projeto é um sensor meteorológico feito especialmente para as corridas da Fórmula E e tem como objetivo monitorar dados ambientais (luminosidade, temperatura, umidade e chuva) utilizando dispositivos IoT (ESP32 e sensores) e exibi-los em um dashboard web interativo desenvolvido com Dash. Os dados coletados pelos sensores são enviados via MQTT para um broker, onde o back-end processa e armazena as informações. O front-end exibe os dados em gráficos para facilitar a análise em tempo real.

## Arquitetura da Solução

A arquitetura proposta envolve três componentes principais:

1. **Dispositivos IoT**: Sensores de luminosidade, temperatura, umidade e chuva conectados a um ESP32.
2. **Back-End**: Responsável por coletar, processar e armazenar os dados dos sensores em um broker MQTT. A solução usa APIs para expor os dados coletados.
3. **Front-End**: Dashboard desenvolvido em Dash (Python), que acessa os dados do back-end e exibe gráficos interativos para análise dos dados ambientais.

![Draft da Arquitetura](./img/arquitetura-iot.png)  
*(Incluir o diagrama gerado no draw.io aqui)*

## Recursos Necessários

### Dispositivos IoT
- **ESP32**: Microcontrolador para realizar a leitura dos sensores e enviar os dados via protocolo MQTT.
- **Sensores**:
  - Sensor de Luminosidade (LDR)
  - Sensor de Temperatura e Umidade (DHT22 ou BMP280)
  - Sensor de Chuva
- **Módulo MQTT**: Para comunicação entre os dispositivos IoT e o back-end.

### Back-End
- **Broker MQTT**: Responsável por gerenciar a comunicação entre os dispositivos e o servidor.
- **API FIWARE**: Utilizada para expor os dados do broker MQTT em formato JSON.

### Front-End
- **Dash (Python)**: Utilizado para desenvolver o dashboard web, onde os dados coletados são apresentados em gráficos.

## Instruções de Uso

### Clonando o Repositório
Para clonar o projeto, execute o seguinte comando no terminal:
```bash
git clone https://github.com/seu-usuario/seu-repositorio.git
