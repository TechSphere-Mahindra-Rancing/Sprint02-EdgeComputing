// Autor: João Victor Soave
// Resumo: Esse programa possibilita ligar e desligar o LED onboard, além de enviar o status para o Broker MQTT, permitindo que o Helix saiba
// se o LED está ligado ou desligado, além de enviar leituras de sensores de luminosidade, temperatura, umidade e chuva.

#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h> // Carrega a biblioteca DHT

// Define a ligação ao pino de dados do sensor
#define DHTPIN 4
// Define o tipo de sensor DHT utilizado
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE); // Instância do sensor DHT22
// Instância do sensor de pressão BMP280 (utilizando SPI)

// Configurações - variáveis editáveis
const char* default_SSID = "Wokwi-GUEST"; // Nome da rede Wi-Fi
const char* default_PASSWORD = ""; // Senha da rede Wi-Fi
const char* default_BROKER_MQTT = "4.201.196.181"; // IP do Broker MQTT
const int default_BROKER_PORT = 1883; // Porta do Broker MQTT
const char* default_TOPICO_SUBSCRIBE = "/TEF/sensor001/cmd"; // Tópico MQTT de escuta
const char* default_TOPICO_PUBLISH_1 = "/TEF/sensor001/attrs"; // Tópico MQTT de envio de informações para Broker
const char* default_TOPICO_PUBLISH_2 = "/TEF/sensor001/attrs/l"; // Tópico MQTT para luminosidade
const char* default_TOPICO_PUBLISH_3 = "/TEF/sensor001/attrs/t"; // Tópico MQTT para temperatura
const char* default_TOPICO_PUBLISH_4 = "/TEF/sensor001/attrs/h"; // Tópico MQTT para umidade
const char* default_TOPICO_PUBLISH_5 = "/TEF/sensor001/attrs/r"; // Tópico MQTT para pressão
const char* default_ID_MQTT = "fiware_001"; // ID MQTT
const int default_D4 = 2; // Pino do LED onboard
const char* topicPrefix = "sensor001"; // Prefixo do tópico

// Variáveis para configurações editáveis
char* SSID = const_cast<char*>(default_SSID);
char* PASSWORD = const_cast<char*>(default_PASSWORD);
char* BROKER_MQTT = const_cast<char*>(default_BROKER_MQTT);
int BROKER_PORT = default_BROKER_PORT;
char* TOPICO_SUBSCRIBE = const_cast<char*>(default_TOPICO_SUBSCRIBE);
char* TOPICO_PUBLISH_1 = const_cast<char*>(default_TOPICO_PUBLISH_1);
char* TOPICO_PUBLISH_2 = const_cast<char*>(default_TOPICO_PUBLISH_2);
char* TOPICO_PUBLISH_3 = const_cast<char*>(default_TOPICO_PUBLISH_3);
char* TOPICO_PUBLISH_4 = const_cast<char*>(default_TOPICO_PUBLISH_4);
char* TOPICO_PUBLISH_5 = const_cast<char*>(default_TOPICO_PUBLISH_5);
char* ID_MQTT = const_cast<char*>(default_ID_MQTT);
int D4 = default_D4;

WiFiClient espClient;
PubSubClient MQTT(espClient);
char EstadoSaida = '0';

void initSerial() {
    Serial.begin(115200);
}

void initWiFi() {
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
    reconectWiFi();
}

void initMQTT() {
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);
    MQTT.setCallback(mqtt_callback);
}

void setup() {
    InitOutput();
    initSerial();
    initWiFi();
    initMQTT();
    delay(5000);
    MQTT.publish(TOPICO_PUBLISH_1, "s|on");
}

void loop() {
    VerificaConexoesWiFIEMQTT();
    EnviaEstadoOutputMQTT();
    handleLuminosity();
    handleTemperature();
    handleUmidade();
    handleChuva();
    MQTT.loop();
}

void reconectWiFi() {
    if (WiFi.status() == WL_CONNECTED)
        return;
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("Conectado com sucesso na rede ");
    Serial.print(SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());

    digitalWrite(D4, LOW);
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    String msg;
    for (int i = 0; i < length; i++) {
        char c = (char)payload[i];
        msg += c;
    }
    Serial.print("- Mensagem recebida: ");
    Serial.println(msg);

    String onTopic = String(topicPrefix) + "@on|";
    String offTopic = String(topicPrefix) + "@off|";

    if (msg.equals(onTopic)) {
        digitalWrite(D4, HIGH);
        EstadoSaida = '1';
    }

    if (msg.equals(offTopic)) {
        digitalWrite(D4, LOW);
        EstadoSaida = '0';
    }
}

void VerificaConexoesWiFIEMQTT() {
    if (!MQTT.connected())
        reconnectMQTT();
    reconectWiFi();
}

void EnviaEstadoOutputMQTT() {
    if (EstadoSaida == '1') {
        MQTT.publish(TOPICO_PUBLISH_1, "s|on");
        Serial.println("- Led Ligado");
    }

    if (EstadoSaida == '0') {
        MQTT.publish(TOPICO_PUBLISH_1, "s|off");
        Serial.println("- Led Desligado");
    }
    Serial.println("- Estado do LED onboard enviado ao broker!");
    delay(1000);
}

void InitOutput() {
    pinMode(D4, OUTPUT);
    digitalWrite(D4, HIGH);
    boolean toggle = false;

    for (int i = 0; i <= 10; i++) {
        toggle = !toggle;
        digitalWrite(D4, toggle);
        delay(200);
    }
}

void reconnectMQTT() {
    while (!MQTT.connected()) {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE);
        } else {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Haverá nova tentativa de conexão em 2s");
            delay(2000);
        }
    }
}

void handleLuminosity() {
    const int potPin = 34;
    int sensorValue = analogRead(potPin);
    int luminosity = map(sensorValue, 0, 4095, 100, 0);
    String mensagem = String(luminosity);

    Serial.print("Valor da luminosidade: ");
    Serial.println(mensagem.c_str());

    MQTT.publish(TOPICO_PUBLISH_2, mensagem.c_str());
}

void handleTemperature() {
    float tempAmb = dht.readTemperature();
    if (isnan(tempAmb)) {
        Serial.println("Falha ao ler a temperatura!");
    } else {
        Serial.print("Temperatura ambiente: ");
        Serial.println(tempAmb);
        String mensagemTemp = String(tempAmb);
        MQTT.publish(TOPICO_PUBLISH_3, mensagemTemp.c_str()); // Publica no tópico de temperatura
    }
}

void handleUmidade() {
    float umidade = dht.readHumidity(); // Lê a umidade do sensor

    if (isnan(umidade)) {
        Serial.println("Falha ao ler a umidade!");
    } else {
        Serial.print("Umidade: ");
        Serial.print(umidade);
        Serial.println("%");
        String mensagem = String(umidade);
        MQTT.publish(TOPICO_PUBLISH_4, mensagem.c_str()); // Publica no tópico de umidade
    }
}

void handleChuva() {
  const int rainAnalog = 35;  // Pino analógico
  const int rainDigital = 16; // Pino digital

  // Leitura do sensor
  int chuvaAnalogSensorValue = analogRead(rainAnalog);
  
  // Mapeamento do valor analógico para porcentagem
  int chuva = map(chuvaAnalogSensorValue, 0, 4095, 0, 100);

  // Impressão do valor da chuva
  Serial.print("Quantidade de chuva (mapeada): ");
  Serial.println(chuva);

  // Verifica se está chovendo
  if (chuva >= 10) { // Aqui você deve verificar se está chovendo
    Serial.println("Está chovendo!");
    
    // Se está chovendo, captura a leitura analógica (quantidade de chuva)
    String rainMessage = String(chuva); // Use o valor analógico lido
    Serial.print("Quantidade de chuva (analógica): ");
    Serial.println(chuva);

    // Publica os dados analógicos no tópico de chuva
    MQTT.publish(TOPICO_PUBLISH_5, rainMessage.c_str());
  } else {
    Serial.println("Não está chovendo!");

    String rainMessage = "0"; // Use o valor analógico lido
    Serial.print("Quantidade de chuva (analógica): ");
    Serial.println(chuva);

    MQTT.publish(TOPICO_PUBLISH_5, rainMessage.c_str());
  }
}