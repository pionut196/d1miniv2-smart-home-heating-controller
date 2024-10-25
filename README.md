<h1>Smart Thermostat Control System</h1>

<h2>Overview</h2>
<p>The Smart Thermostat Control System is a WiFi-enabled solution designed to control home heating. Currently, I'm using this thermostat efficiently with my Ariston Clas Evo. This script closes the circuit from the wires coming out of your central unit, allowing the heating to start when the circuit is closed. It leverages the power of the ESP8266 microcontroller combined with a DHT22 temperature and humidity sensor, providing a user-friendly interface for remote heating control. This project aims to enhance energy efficiency and comfort in home heating systems.</p>


<h2>Features</h2>
<ul>
    <li><strong>WiFi Connectivity</strong>: Control your heating system remotely from anywhere.</li>
    <li><strong>OLED Display</strong>: Real-time temperature and humidity readings for accurate monitoring.</li>
    <li><strong>DHT22 Sensor</strong>: High-precision environmental data collection.</li>
    <li><strong>User-Friendly Web Interface</strong>: Easily adjust settings and view data.</li>
    <li><strong>Energy Saving</strong>: Optimize heating schedules to reduce energy consumption.</li>
    <li><strong>Compatibility</strong>: Works seamlessly with Ariston gas boilers.</li>
</ul>

<h2>Hardware Requirements</h2>
<ul>
    <li><strong>Microcontroller</strong>: ESP8266</li>
    <li><strong>Temperature & Humidity Sensor</strong>: DHT22</li>
    <li><strong>Display</strong>: OLED display (128x64 recommended)</li>
    <li><strong>Power Supply</strong>: 5V USB power source</li>
    <li><strong>Wiring</strong>: Jumper wires, breadboard (optional)</li>
</ul>

<h2>Software Requirements</h2>
<ul>
    <li><strong>Arduino IDE</strong>: For programming the ESP8266</li>
    <li><strong>Libraries</strong>:
        <ul>
            <li><code>DHT sensor library</code> for temperature and humidity readings</li>
            <li><code>Adafruit SSD1306</code> for OLED display</li>
            <li><code>ESP8266WiFi</code> for WiFi connectivity</li>
        </ul>
    </li>
</ul>

<h2>Installation</h2>
<ol>
    <li><strong>Clone the Repository</strong>:
        <pre><code>git clone https://github.com/yourusername/smart-thermostat.git
cd smart-thermostat</code></pre>
    </li>
    <li><strong>Install Required Libraries</strong>: Open the Arduino IDE, navigate to <strong>Sketch</strong> > <strong>Include Library</strong> > <strong>Manage Libraries</strong>, and install the necessary libraries mentioned above.</li>
    <li><strong>Configure WiFi Settings</strong>: Open the <code>config.h</code> file and enter your WiFi credentials:
        <pre><code>const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";</code></pre>
    </li>
    <li><strong>Upload the Code</strong>: Open the <code>thermostat.ino</code> file in Arduino IDE and upload the code to your ESP8266.</li>
    <li><strong>Connect Hardware</strong>: Connect the DHT22 sensor to the ESP8266. Connect the OLED display to the ESP8266 using I2C. Ensure proper power supply.</li>
</ol>

<h2>Usage</h2>
<ol>
    <li>After uploading the code, open the Serial Monitor (set to 115200 baud rate) to get the IP address assigned to your ESP8266.</li>
    <li>Enter the IP address in a web browser to access the control interface.</li>
    <li>Adjust the temperature settings and view real-time data on the web interface.</li>
</ol>

<h2>Tags</h2>
<ul>
    <li><strong>English</strong>: DIY Thermostat, Smart Home, ESP8266, IoT Projects, Home Automation, Temperature Control, DHT22 Sensor, WiFi Thermostat, OLED Display, Energy Efficiency, Heating Control, Arduino Projects</li>
    <li><strong>Română</strong>: Termostat DIY, Casă Inteligentă, ESP8266, Proiecte IoT, Automatizare Acasă, Control Temperatură, Senzor DHT22, Termostat WiFi, Ecran OLED, Eficiență Energetică, Control Încălzire, Proiecte Arduino</li>
</ul>

<h2>Contributing</h2>
<p>Contributions are welcome! If you have suggestions for improvements or new features, please open an issue or submit a pull request.</p>

<h2>License</h2>
<p>This project is licensed under the MIT License - see the <a href="LICENSE">LICENSE</a> file for details.</p>
