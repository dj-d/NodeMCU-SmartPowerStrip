#ifndef UNIT_TEST

#include <Arduino.h>

#endif

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/*********************************************************************/

const char *ssid = "ALBANESE";
const char *password = "0101010101";
ESP8266WebServer server(80);

/*********************************************************************/

// RELAY PINS
int relay[] = {0, 4, 5, 16};

/*********************************************************************/

/**
 * Size of relay array
 * @return number of element in array
 */
int get_relay_size() {
    return sizeof(relay);
}

/**
 * Initialization of relay pins
 */
void initRelay(int relay_size) {
    for (int i = 0; i < relay_size; i++) {
        pinMode(relay[i], OUTPUT);

        digitalWrite(relay[i], HIGH);
    }
}

/*********************************************************************/

/**
 * Turn on a specific relay
 * @param pin Pin of relay pin
 */
void turnOn(int pin) {
    digitalWrite(pin, LOW);
}

/**
 * Turn off a specific relay
 * @param pin Pin of relay pin
 */
void turnOff(int pin) {
    digitalWrite(pin, HIGH);
}

/*********************************************************************/

/**
 * Turn on or turn off a specific relay by parameters
 */
void smartArg() {
    String message = "";
    int num;

    if (server.arg(0) == "on") {
        num = server.arg(1).toInt() - 1;

        if (num >= 0 && num <= get_relay_size() - 1) {
            turnOn(relay[num]);
            message = "On = " + server.arg(1);

        } else {
            message = "Erro: num of relay";
        }


    } else if (server.arg(0) == "off") {
        num = server.arg(1).toInt() - 1;

        if (num >= 0 && num <= get_relay_size() - 1) {
            turnOff(relay[num]);
            message = "Off = " + server.arg(1);

        } else {
            message = "Erro: num of relay";
        }
    }

    server.send(200, "text/plain", message);
}

/*********************************************************************/

void setup() {
    initRelay(get_relay_size());

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

    server.on("/smart", smartArg);

    server.begin();
}

/*********************************************************************/

void loop() {
    server.handleClient();
}