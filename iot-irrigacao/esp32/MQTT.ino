#include <WiFi.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ===== WIFI =====
const char* ssid = "teste123";
const char* password = "12345678";

// ===== MQTT =====
const char* mqtt_server = "18.231.109.143";
WiFiClient espClient;
PubSubClient client(espClient);

// ===== OLED =====
Adafruit_SSD1306 display(128, 64, &Wire, -1);
bool oledOK = false;

// ===== PINOS =====
int potUmidade = 1;
int potNivel   = 2;

int ledVerde     = 3;
int ledVermelho  = 10;
int ledIrrigacao = 11;
int buzzer       = 12;

// ===== VARIÁVEIS =====
int umidade = 0;
int nivel = 0;
bool irrigando = false;

unsigned long ultimoEnvio = 0;

// ===== WIFI =====
void conectarWiFi() {
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);

  Serial.print("Conectando WiFi");

  int tentativas = 0;

  while (WiFi.status() != WL_CONNECTED && tentativas < 20) {
    delay(500);
    Serial.print(".");
    tentativas++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi conectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFalha ao conectar WiFi!");
  }
}

// ===== MQTT =====
void conectarMQTT() {
  client.setServer(mqtt_server, 1883);

  while (!client.connected()) {
    Serial.println("Conectando MQTT...");

    if (client.connect("ESP32Client")) {
      Serial.println("MQTT conectado");
    } else {
      Serial.print("Falha MQTT, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente...");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  Serial.println("INICIO SETUP");

  delay(1000);
  Wire.begin(8, 9);

  // OLED
  if (display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    oledOK = true;
    display.clearDisplay();
    display.display();
    Serial.println("OLED OK");
  } else {
    Serial.println("OLED NAO DETECTADO");
  }

  // PINOS
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledIrrigacao, OUTPUT);
  pinMode(buzzer, OUTPUT);

  analogReadResolution(12);

  conectarWiFi();
  conectarMQTT();
}

void loop() {

  // ===== GARANTE MQTT =====
  if (!client.connected()) {
    conectarMQTT();
  }
  client.loop();

  // ===== LEITURA =====
  int leituraUmidade = analogRead(potUmidade);
  int leituraNivel   = analogRead(potNivel);

  umidade = map(leituraUmidade, 0, 4095, 0, 100);
  nivel   = map(leituraNivel,   0, 4095, 0, 100);

  // ===== LÓGICA =====
  if (nivel < 30) {
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(ledVerde, LOW);
  } else {
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledVermelho, LOW);
  }

  if (umidade < 25) {
    digitalWrite(ledIrrigacao, HIGH);
    digitalWrite(buzzer, HIGH);
    irrigando = true;
  } else {
    digitalWrite(ledIrrigacao, LOW);
    digitalWrite(buzzer, LOW);
    irrigando = false;
  }

  // ===== ENVIO (HTTP + MQTT) =====
  if (millis() - ultimoEnvio > 5000) {

    // --- HTTP ---
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;

      String url = "http://18.231.109.143/dados.php?umidade=" 
                   + String(umidade) + "&nivel=" + String(nivel);

      http.begin(url);
      int httpCode = http.GET();

      Serial.print("HTTP Code: ");
      Serial.println(httpCode);

      http.end();
    } else {
      Serial.println("WiFi desconectado");
    }

    // --- MQTT ---
    String payload = "{";
    payload += "\"umidade\":" + String(umidade) + ",";
    payload += "\"nivel\":" + String(nivel);
    payload += "}";

    client.publish("irrigacao/dados", payload.c_str());

    Serial.print("MQTT enviado: ");
    Serial.println(payload);

    ultimoEnvio = millis();
  }

  // ===== OLED =====
  if (oledOK) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 0);
    display.print("Umidade: ");
    display.print(umidade);
    display.println("%");

    display.setCursor(0, 20);
    display.print("Nivel: ");
    display.print(nivel);
    display.println("%");

    display.setCursor(0, 40);
    display.println(irrigando ? "IRRIGANDO..." : "NORMAL");

    display.display();
  }

  delay(200);
}