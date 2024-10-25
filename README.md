Smart Thermostat Control System
Overview
The Smart Thermostat Control System is a WiFi-enabled solution designed to control Ariston boilers efficiently. It leverages the power of the ESP8266 microcontroller combined with a DHT22 temperature and humidity sensor, providing a user-friendly interface for remote heating control. This project aims to enhance energy efficiency and comfort in home heating systems.

Features
WiFi Connectivity: Control your heating system remotely from anywhere.
OLED Display: Real-time temperature and humidity readings for accurate monitoring.
DHT22 Sensor: High-precision environmental data collection.
User-Friendly Web Interface: Easily adjust settings and view data.
Energy Saving: Optimize heating schedules to reduce energy consumption.
Compatibility: Works seamlessly with Ariston gas boilers.
Hardware Requirements
Microcontroller: ESP8266
Temperature & Humidity Sensor: DHT22
Display: OLED display (128x64 recommended)
Power Supply: 5V USB power source
Wiring: Jumper wires, breadboard (optional)
Software Requirements
Arduino IDE: For programming the ESP8266
Libraries:
DHT sensor library for temperature and humidity readings
Adafruit SSD1306 for OLED display
ESP8266WiFi for WiFi connectivity
Installation
Clone the Repository:

bash
Copiază codul
git clone https://github.com/yourusername/smart-thermostat.git
cd smart-thermostat
Install Required Libraries: Open the Arduino IDE, navigate to Sketch > Include Library > Manage Libraries, and install the necessary libraries mentioned above.

Configure WiFi Settings: Open the config.h file and enter your WiFi credentials:

cpp
Copiază codul
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
Upload the Code: Open the thermostat.ino file in Arduino IDE and upload the code to your ESP8266.

Connect Hardware:

Connect the DHT22 sensor to the ESP8266.
Connect the OLED display to the ESP8266 using I2C.
Ensure proper power supply.
Usage
After uploading the code, open the Serial Monitor (set to 115200 baud rate) to get the IP address assigned to your ESP8266.
Enter the IP address in a web browser to access the control interface.
Adjust the temperature settings and view real-time data on the web interface.
Tags
English: DIY Thermostat, Smart Home, ESP8266, IoT Projects, Home Automation, Temperature Control, DHT22 Sensor, WiFi Thermostat, OLED Display, Energy Efficiency, Heating Control, Arduino Projects
Română: Termostat DIY, Casă Inteligentă, ESP8266, Proiecte IoT, Automatizare Acasă, Control Temperatură, Senzor DHT22, Termostat WiFi, Ecran OLED, Eficiență Energetică, Control Încălzire, Proiecte Arduino
Contributing
Contributions are welcome! If you have suggestions for improvements or new features, please open an issue or submit a pull request.
