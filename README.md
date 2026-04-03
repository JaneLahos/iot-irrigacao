🌱 Sistema de Irrigação Inteligente com ESP32
📌 Descrição
Sistema IoT para monitoramento e controle de irrigação, utilizando ESP32-S3, com envio de dados para nuvem (AWS) e visualização em tempo real via Node-RED.
________________________________________
🧱 Arquitetura
ESP32 → WiFi → AWS → (HTTP + MQTT) → Node-RED Dashboard
________________________________________
⚙️ Tecnologias
•	ESP32-S3
•	Arduino IDE
•	Node-RED
•	MQTT (Mosquitto)
•	HTTP (PHP)
•	MySQL
•	AWS EC2
________________________________________
🔌 Hardware
•	ESP32-S3 N16R8
•	OLED SSD1306 (I2C)
•	Potenciômetros (simulação sensores)
•	LEDs (status)
•	Buzzer
•	Resistores
________________________________________
🌐 Backend
📡 API HTTP
Arquivo: server/dados.php
Recebe dados via GET:
/dados.php?umidade=50&nivel=80
________________________________________
🗄️ Banco de Dados
Arquivo: database/banco.sql
Tabela:
•	leituras
o	id
o	umidade
o	nivel
o	data_hora
________________________________________
📥 Backup do Banco
No servidor:
mysqldump -u usuario -p irrigacao > banco.sql
________________________________________
📊 Dashboard
Acessar:
http://(http://18.231.109.143/):1880/ui
Exibe:
•	Umidade (%)
•	Nível (%)
•	Status do sistema
________________________________________
📡 MQTT
•	Broker: AWS (Mosquitto)
•	Tópico: irrigacao/dados
•	Payload:
{"umidade":50,"nivel":80}
________________________________________
🚀 Execução
ESP32
•	Configurar WiFi
•	Upload via Arduino IDE
Node-RED
•	Importar flow.json
•	Configurar broker MQTT
Backend
•	Subir dados.php no servidor
•	Criar banco com banco.sql
________________________________________
📷 Demonstração
Ver pasta /imagens
________________________________________
👩‍💻 Autor: Jane de Godoi Lahos
Projeto acadêmico de IoT aplicado a irrigação inteligente.
