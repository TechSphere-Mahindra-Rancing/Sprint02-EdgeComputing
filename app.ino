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
char EstadoSaida = '0'; // Variável para armazenar o estado do LED (ligado/desligado)

void initSerial() {
    Serial.begin(115200); // Inicia a comunicação serial
}

void initWiFi() {
    delay(10); // Espera 10ms
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
    reconectWiFi(); // Chama função para reconectar o Wi-Fi
}

void initMQTT() {
    MQTT.setServer(BROKER_MQTT, BROKER_PORT); // Define o broker MQTT e a porta
    MQTT.setCallback(mqtt_callback); // Define a função de callback para as mensagens MQTT
}

void setup() {
    InitOutput(); // Inicializa o LED onboard
    initSerial(); // Inicia a comunicação serial
    initWiFi(); // Conecta ao Wi-Fi
    initMQTT(); // Configura o MQTT
    delay(5000); // Aguarda 5 segundos
    MQTT.publish(TOPICO_PUBLISH_1, "s|on"); // Publica a mensagem "s|on" no tópico
}

void loop() {
    VerificaConexoesWiFIEMQTT(); // Verifica as conexões Wi-Fi e MQTT
    EnviaEstadoOutputMQTT(); // Envia o estado atual do LED para o Broker MQTT
    handleLuminosity(); // Lê e envia os dados do sensor de luminosidade
    handleTemperature(); // Lê e envia os dados do sensor de temperatura
    handleUmidade(); // Lê e envia os dados do sensor de umidade
    handleChuva(); // Lê e envia os dados do sensor de chuva
    MQTT.loop(); // Mantém a conexão MQTT ativa
}

void reconectWiFi() {
    if (WiFi.status() == WL_CONNECTED)
        return; // Se o Wi-Fi já está conectado, retorna
    WiFi.begin(SSID, PASSWORD); // Inicia a conexão Wi-Fi
    while (WiFi.status() != WL_CONNECTED) { // Aguarda a conexão
        delay(100);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("Conectado com sucesso na rede ");
    Serial.print(SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP()); // Imprime o IP local

    digitalWrite(D4, LOW); // Apaga o LED onboard
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    String msg;
    for (int i = 0; i < length; i++) {
        char c = (char)payload[i]; // Converte o payload para caractere
        msg += c;
    }
    Serial.print("- Mensagem recebida: ");
    Serial.println(msg);

    String onTopic = String(topicPrefix) + "@on|"; // Monta a mensagem de ligar o LED
    String offTopic = String(topicPrefix) + "@off|"; // Monta a mensagem de desligar o LED

    if (msg.equals(onTopic)) { // Se a mensagem é para ligar o LED
        digitalWrite(D4, HIGH);
        EstadoSaida = '1'; // Atualiza o estado do LED
    }

    if (msg.equals(offTopic)) { // Se a mensagem é para desligar o LED
        digitalWrite(D4, LOW);
        EstadoSaida = '0'; // Atualiza o estado do LED
    }
}

void VerificaConexoesWiFIEMQTT() {
    if (!MQTT.connected())
        reconnectMQTT(); // Se o MQTT não está conectado, tenta reconectar
    reconectWiFi(); // Verifica a conexão Wi-Fi
}

void EnviaEstadoOutputMQTT() {
    if (EstadoSaida == '1') {
        MQTT.publish(TOPICO_PUBLISH_1, "s|on"); // Publica o estado "ligado" do LED
        Serial.println("- Led Ligado");
    }

    if (EstadoSaida == '0') {
        MQTT.publish(TOPICO_PUBLISH_1, "s|off"); // Publica o estado "desligado" do LED
        Serial.println("- Led Desligado");
    }
    Serial.println("- Estado do LED onboard enviado ao broker!");
    delay(1000); // Atraso de 1 segundo
}

void InitOutput() {
    pinMode(D4, OUTPUT); // Configura o pino do LED como saída
    digitalWrite(D4, HIGH); // Liga o LED
    boolean toggle = false; // Variável para alternar o estado do LED

    for (int i = 0; i <= 10; i++) { // Pisca o LED 10 vezes
        toggle = !toggle;
        digitalWrite(D4, toggle);
        delay(200);
    }
}

void reconnectMQTT() {
    while (!MQTT.connected()) { // Tenta reconectar ao broker MQTT
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE); // Inscreve-se no tópico
        } else {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Haverá nova tentativa de conexão em 2s");
            delay(2000); // Aguarda 2 segundos antes de tentar reconectar
        }
    }
}

void handleLuminosity() {
    const int potPin = 34;
    int sensorValue = analogRead(potPin); // Lê o valor do sensor de luminosidade
    int luminosity = map(sensorValue, 0, 4095, 100, 0); // Mapeia o valor para porcentagem
    String mensagem = String(luminosity);

    Serial.print("Valor da luminosidade: ");
    Serial.println(mensagem.c_str());

    MQTT.publish(TOPICO_PUBLISH_2, mensagem.c_str()); // Publica a luminosidade no tópico
}

void handleTemperature() {
    float tempAmb = dht.readTemperature(); // Lê a temperatura ambiente do sensor DHT22
    String temperatura = String(tempAmb);

    Serial.print("Temperatura lida: ");
    Serial.println(temperatura.c_str());

    MQTT.publish(TOPICO_PUBLISH_3, temperatura.c_str()); // Publica a temperatura no tópico
}

void handleUmidade() {
    float hum = dht.readHumidity(); // Lê a umidade ambiente do sensor DHT22
    String umidade = String(hum);

    Serial.print("Umidade lida: ");
    Serial.println(umidade.c_str());

    MQTT.publish(TOPICO_PUBLISH_4, umidade.c_str()); // Publica a umidade no tópico
}

void handleChuva() {
    const int potPin = 35;
    int sensorValue = analogRead(potPin); // Lê o valor do sensor de chuva
    int chuva = map(sensorValue, 0, 4095, 0, 100); // Mapeia o valor para porcentagem
    String mensagem = String(chuva);

    Serial.print("Presenca de chuva: ");
    Serial.println(mensagem.c_str());

    MQTT.publish(TOPICO_PUBLISH_5, mensagem.c_str()); // Publica a presença de chuva no tópico
}
