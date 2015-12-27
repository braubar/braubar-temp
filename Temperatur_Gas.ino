#include <OneWire.h>
#include <TimerOne.h>
#include <SPI.h>
#include <Ethernet.h>

#include "config.h"
#include "ds18b20.h"

// Temp vars
unsigned long now;
double istTemperatur;

double asd = 0.1257765;
unsigned long TIME_SERIAL_LAST_SENT = 0;

// Eth vars
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// IP address arduino
IPAddress ip(192,168,2, 178);

// IP address server
IPAddress server(192, 168, 2, 9);

// Ethernet client
EthernetClient client;
// server port to connect to
int port = 10001;


void connectClient(IPAddress address, int port) {
    if (client.connect(address, port)) {
        Serial.println("connected");
    } else {
        // if you didn't get a connection to the server:
        Serial.println("connection failed");
    }
}

void setup() {
    Serial.begin(9600);
    // start the Ethernet connection:
    Ethernet.begin(mac, ip);
    // Open serial communications and wait for port to open:
    // give the Ethernet shield a second to initialize:
    delay(1000);
    connectClient(server, port);
}

// übersetzt die IP Adresse in einen String, für die debug ausgabe auf dem serial port
String ip2string(IPAddress address) {
    byte a = address[0];
    byte b = address[1];
    byte c = address[2];
    byte d = address[3];
    return String(a)+"."+String(b)+"."+String(c)+"."+String(d);
}

// fuer protokoll auf laptop...
// strings ueber serial
void printSerial() {
    now = millis();
    if ((now - TIME_SERIAL_LAST_SENT) > TIME_SERIAL) {
        TIME_SERIAL_LAST_SENT = now;
        Serial.println(istTemperatur);
        client.print(String(istTemperatur,4));
    }
}

void loop() {
    istTemperatur = getTemperature();
    if (istTemperatur == -1000.0) { // initialisierungswert des temp sensors, soll vermutlich ein break darstellen.
        delay(10);
        return;
    }

    printSerial();
    if (!client.connected()){
        Serial.println("Trying on " + ip2string(server) + " and port "+  String(port));
        Serial.println("client not connected");
        connectClient(server, port);
    }
    now = millis();
}

