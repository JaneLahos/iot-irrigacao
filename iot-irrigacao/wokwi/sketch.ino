#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ===== OLED =====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ===== POTENCIÔMETROS =====
int potUmidade = 4;
int potNivel   = 5;

// ===== LEDS + BUZZER =====
int ledVerde     = 6;
int ledVermelho  = 7;
int ledIrrigacao = 16;
int buzzer       = 15;

// ===== VARIÁVEIS =====
int umidade = 0;
int nivel = 0;
bool irrigando = false;

void setup() {
  Serial.begin(115200);

  Wire.begin(8, 9);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED NAO ENCONTRADO");
    while (true);
  }

  // ===== PINOS =====
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledIrrigacao, OUTPUT);
  pinMode(buzzer, OUTPUT);

  analogReadResolution(12);

  Serial.println("Sistema iniciado");
}

void loop() {

  // ===== LEITURA =====
  int leituraUmidade = analogRead(potUmidade);
  int leituraNivel   = analogRead(potNivel);

  umidade = map(leituraUmidade, 0, 4095, 0, 100);
  nivel   = map(leituraNivel,   0, 4095, 0, 100);

  // ===== LÓGICA =====
  irrigando = false;

  // Nível baixo (alerta)
  if (nivel < 30) {
    digitalWrite(ledVermelho, HIGH);
    digitalWrite(ledVerde, LOW);
  } else {
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledVermelho, LOW);
  }

  // Umidade baixa (irrigação)
  if (umidade < 25) {
    digitalWrite(ledIrrigacao, HIGH);
    digitalWrite(buzzer, HIGH);
    irrigando = true;
  } else {
    digitalWrite(ledIrrigacao, LOW);
    digitalWrite(buzzer, LOW);
  }

  // ===== SERIAL =====
  Serial.print("Umidade: ");
  Serial.print(umidade);
  Serial.print("% | Nivel: ");
  Serial.print(nivel);
  Serial.print("% | Irrigacao: ");
  Serial.println(irrigando ? "ON" : "OFF");

  // ===== OLED =====
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

  delay(500);
}
