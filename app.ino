// Autor: João Victor Soave
// Resumo: Este programa permite ligar e desligar o LED onboard, além de enviar o status para o Broker MQTT, 
// permitindo que o Helix saiba se o LED está ligado ou desligado, além de enviar leituras de sensores de 
// luminosidade, temperatura, umidade e chuva.

#include <WiFi.h>             // Inclui a biblioteca para conexão Wi-Fi
#include <PubSubClient.h>     // Inclui a biblioteca para MQTT
#include <DHT.h>             // Carrega a biblioteca DHT para leitura de temperatura e umidade

// Define a ligação ao pino de dados do sensor
#define DHTPIN 4              // Pino de dados do sensor DHT
// Define o tipo de sensor DHT utilizado
#define DHTTYPE DHT22         // Define que o tipo do sensor é DHT22

DHT dht(DHTPIN, DHTTYPE);      // Instância do sensor DHT22

// Configurações - variáveis editáveis
const char* default_SSID = "Wokwi-GUEST"; // Nome da rede Wi-Fi
const char* default_PASSWORD = "";          // Senha da rede Wi-Fi
const char* default_BROKER_MQTT = "4.201.196.181"; // IP do Broker MQTT
const int default_BROKER_PORT = 1883;       // Porta do Broker MQTT
const char* default_TOPICO_SUBSCRIBE = "/TEF/sensor001/cmd"; // Tópico MQTT para comandos
const char* default_TOPICO_PUBLISH_1 = "/TEF/sensor001/attrs"; // Tópico para envio de informações
const char* default_TOPICO_PUBLISH_2 = "/TEF/sensor001/attrs/l"; // Tópico para luminosidade
const char* default_TOPICO_PUBLISH_3 = "/TEF/sensor001/attrs/t"; // Tópico para temperatura
const char* default_TOPICO_PUBLISH_4 = "/TEF/sensor001/attrs/h"; // Tópico para umidade
const char* default_TOPICO_PUBLISH_5 = "/TEF/sensor001/attrs/r"; // Tópico para pressão
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
int D4 = default_D4; // Pino para LED

WiFiClient espClient; // Cliente Wi-Fi
PubSubClient MQTT(espClient); // Cliente MQTT
char EstadoSaida = '0'; // Estado do LED (ligado/desligado)

// Função para inicializar a comunicação serial
void initSerial() {
    Serial.begin(115200); // Inicia a comunicação serial a 115200 bps
}

// Função para conectar ao Wi-Fi
void initWiFi() {
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
    reconectWiFi(); // Chama a função para reconectar ao Wi-Fi
}

// Função para configurar o MQTT
void initMQTT() {
    MQTT.setServer(BROKER_MQTT, BROKER_PORT); // Configura o broker MQTT
    MQTT.setCallback(mqtt_callback); // Define a função de callback para mensagens recebidas
}

// Função de configuração
void setup() {
    InitOutput(); // Inicializa a saída (LED)
    initSerial(); // Inicializa a comunicação serial
    initWiFi(); // Conecta ao Wi-Fi
    initMQTT(); // Configura o MQTT
    delay(5000); // Atraso para estabilizar a conexão
    MQTT.publish(TOPICO_PUBLISH_1, "s|on"); // Publica que o LED está ligado no broker
}

// Loop principal
void loop() {
    VerificaConexoesWiFIEMQTT(); // Verifica a conexão com Wi-Fi e MQTT
    EnviaEstadoOutputMQTT(); // Envia o estado do LED ao broker
    handleLuminosity(); // Lida com a leitura de luminosidade
    handleTemperature(); // Lida com a leitura de temperatura
    handleUmidade(); // Lida com a leitura de umidade
    handleChuva(); // Lida com a leitura de chuva
    MQTT.loop(); // Mantém a conexão MQTT ativa
}

// Função para reconectar ao Wi-Fi
void reconectWiFi() {
    if (WiFi.status() == WL_CONNECTED) // Se já está conectado, retorna
        return;
    WiFi.begin(SSID, PASSWORD); // Inicia a conexão Wi-Fi
    while (WiFi.status() != WL_CONNECTED) { // Aguarda até estar conectado
        delay(100);
        Serial.print("."); // Exibe ponto enquanto aguarda
    }
    Serial.println();
    Serial.println("Conectado com sucesso na rede ");
    Serial.print(SSID); // Exibe o nome da rede
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP()); // Exibe o IP atribuído

    digitalWrite(D4, LOW); // Desliga o LED
}

// Função callback para mensagens MQTT recebidas
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    String msg; // Mensagem recebida
    for (int i = 0; i < length; i++) { // Converte o payload em string
        char c = (char)payload[i];
        msg += c;
    }
    Serial.print("- Mensagem recebida: ");
    Serial.println(msg); // Exibe a mensagem recebida

    String onTopic = String(topicPrefix) + "@on|"; // Tópico para ligar o LED
    String offTopic = String(topicPrefix) + "@off|"; // Tópico para desligar o LED

    if (msg.equals(onTopic)) { // Se a mensagem é para ligar o LED
        digitalWrite(D4, HIGH); // Liga o LED
        EstadoSaida = '1'; // Atualiza estado do LED
    }

    if (msg.equals(offTopic)) { // Se a mensagem é para desligar o LED
        digitalWrite(D4, LOW); // Desliga o LED
        EstadoSaida = '0'; // Atualiza estado do LED
    }
}

// Função para verificar conexões Wi-Fi e MQTT
void VerificaConexoesWiFIEMQTT() {
    if (!MQTT.connected()) // Se não está conectado ao MQTT
        reconnectMQTT(); // Tenta reconectar ao MQTT
    reconectWiFi(); // Tenta reconectar ao Wi-Fi
}

// Função para enviar o estado do LED ao broker MQTT
void EnviaEstadoOutputMQTT() {
    if (EstadoSaida == '1') { // Se o LED está ligado
        MQTT.publish(TOPICO_PUBLISH_1, "s|on"); // Publica estado 'on'
        Serial.println("- Led Ligado"); // Log
    }

    if (EstadoSaida == '0') { // Se o LED está desligado
        MQTT.publish(TOPICO_PUBLISH_1, "s|off"); // Publica estado 'off'
        Serial.println("- Led Desligado"); // Log
    }
    Serial.println("- Estado do LED onboard enviado ao broker!"); // Log
    delay(1000); // Atraso para evitar múltiplas publicações
}

// Função para inicializar o pino do LED
void InitOutput() {
    pinMode(D4, OUTPUT); // Define o pino do LED como saída
    digitalWrite(D4, HIGH); // Inicialmente desliga o LED
    boolean toggle = false; // Estado de alternância

    for (int i = 0; i <= 10; i++) { // Pisca o LED 10 vezes
        toggle = !toggle; // Alterna o estado
        digitalWrite(D4, toggle); // Aplica o estado ao LED
        delay(200); // Atraso de 200ms
    }
}

// Função para reconectar ao Broker MQTT
void reconnectMQTT() {
    while (!MQTT.connected()) { // Enquanto não estiver conectado
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        // Tenta se conectar com o ID MQTT
        if (MQTT.connect(ID_MQTT)) { 
            Serial.println("Conectado com sucesso ao broker");
            MQTT.subscribe(TOPICO_SUBSCRIBE); // Assina o tópico de comandos
        } else {
            Serial.print("Falha na conexão com o broker. Código de erro: ");
            Serial.println(MQTT.state()); // Exibe código de erro
            delay(2000); // Atraso antes da próxima tentativa
        }
    }
}

// Função para lidar com a leitura de luminosidade
void handleLuminosity() {
    int sensorValue = analogRead(34); // Lê valor do sensor LDR no pino 34
    String luminosity = String(sensorValue); // Converte para string
    MQTT.publish(TOPICO_PUBLISH_2, luminosity.c_str()); // Publica o valor da luminosidade
    delay(1000); // Atraso para evitar múltiplas leituras
}

// Função para lidar com a leitura de temperatura
void handleTemperature() {
    float h = dht.readHumidity(); // Lê umidade
    float t = dht.readTemperature(); // Lê temperatura
    String temperature = String(t); // Converte para string
    String humidity = String(h); // Converte para string
    MQTT.publish(TOPICO_PUBLISH_3, temperature.c_str()); // Publica a temperatura
    MQTT.publish(TOPICO_PUBLISH_4, humidity.c_str()); // Publica a umidade
    delay(1000); // Atraso para evitar múltiplas leituras
}

// Função para lidar com a leitura de umidade
void handleUmidade() {
    float h = dht.readHumidity(); // Lê umidade
    String humidity = String(h); // Converte para string
    MQTT.publish(TOPICO_PUBLISH_4, humidity.c_str()); // Publica a umidade
    delay(1000); // Atraso para evitar múltiplas leituras
}

// Função para lidar com a leitura de chuva
void handleChuva() {
    int chuva = digitalRead(35); // Lê estado do sensor de chuva no pino 35
    String chuvaState = (chuva == HIGH) ? "Sem chuva" : "Chovendo"; // Verifica estado
    MQTT.publish(TOPICO_PUBLISH_5, chuvaState.c_str()); // Publica o estado da chuva
    delay(1000); // Atraso para evitar múltiplas leituras
}
