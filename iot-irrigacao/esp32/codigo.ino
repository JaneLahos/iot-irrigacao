#include WiFi.h
#include HTTPClient.h
#include PubSubClient.h

 ===== WIFI =====
const char ssid = SEU_WIFI;
const char password = SUA_SENHA;

 ===== SERVIDOR =====
const char serverName = http54.232.15.194dados.php;

 ===== MQTT =====
const char mqtt_server = 54.232.15.194;

WiFiClient espClient;
PubSubClient client(espClient);

 ===== PINOS (ESP32 S3 AJUSTADO) =====
#define POT_UMIDADE 1
#define POT_NIVEL 2

#define LED_VERDE 10
#define LED_VERMELHO 11
#define BUZZER 12

 ===== VARIÁVEIS =====
int umidade = 0;
int nivel = 0;

unsigned long lastMsg = 0;

 ===== WIFI =====
void setup_wifi() {
  delay(10);
  Serial.println(Conectando WiFi...);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(.);
  }

  Serial.println(nWiFi conectado!);
}

 ===== MQTT =====
void reconnect() {
  while (!client.connected()) {
    Serial.print(Conectando MQTT...);
    if (client.connect(ESP32Client)) {
      Serial.println(conectado!);
    } else {
      Serial.print(erro, rc=);
      Serial.print(client.state());
      delay(2000);
    }
  }
}

 ===== SETUP =====
void setup() {
  Serial.begin(115200);

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  setup_wifi();

  client.setServer(mqtt_server, 1883);
}

 ===== LOOP =====
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

   ===== LEITURA =====
  umidade = analogRead(POT_UMIDADE);
  nivel = analogRead(POT_NIVEL);

   ===== CONVERSÃO =====
  umidade = map(umidade, 0, 4095, 0, 100);
  nivel = map(nivel, 0, 4095, 0, 100);

  Serial.print(Umidade );
  Serial.print(umidade);
  Serial.print( %  Nivel );
  Serial.println(nivel);

   ===== LÓGICA =====

  if (nivel  30) {
    digitalWrite(LED_VERMELHO, HIGH);
    digitalWrite(LED_VERDE, LOW);
  } else {
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_VERMELHO, LOW);
  }

  if (umidade  25) {
    digitalWrite(BUZZER, HIGH);
  } else {
    digitalWrite(BUZZER, LOW);
  }

   ===== MQTT =====
  if (millis() - lastMsg  5000) {
    lastMsg = millis();

    String payload = {;
    payload += umidade + String(umidade) + ,;
    payload += nivel + String(nivel);
    payload += };

    client.publish(irrigacaodados, payload.c_str());

    Serial.println(MQTT enviado);
  }

   ===== HTTP =====
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = String(serverName) + umidade= + umidade + &nivel= + nivel;

    http.begin(url);
    int httpResponseCode = http.GET();

    Serial.print(HTTP );
    Serial.println(httpResponseCode);

    http.end();
  }

  delay(3000);
}