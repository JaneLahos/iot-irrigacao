# 🌱 Sistema de Irrigação Inteligente IoT

## 📌 Descrição
Sistema desenvolvido com ESP32-S3 para monitoramento de umidade do solo e nível de água, com acionamento automático de irrigação.

## 🧠 Funcionamento
O ESP32 coleta dados e envia:
- via MQTT para o Node-RED (dashboard)
- via HTTP para API PHP (armazenamento no MySQL)

## 🛠️ Tecnologias Utilizadas
- ESP32
- MQTT (Mosquitto)
- Node-RED
- Apache
- PHP
- MySQL

## ⚙️ Funcionalidades
- Leitura de sensores (simulados com potenciômetros)
- Irrigação automática
- Alarme com buzzer
- Dashboard web
- Armazenamento em banco de dados

## 🌐 Arquitetura

ESP32 → MQTT/HTTP → Servidor → Node-RED + MySQL

## 📂 Estrutura

- esp32 → código do dispositivo
- server → API PHP
- database → script do banco

## 👨‍💻 Autor
Jane de Godoi Lahos