#ifndef UNIT_TEST

#include <Arduino.h>

#endif

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

/*********************************************************************/

const char *ssid = "YOUR_WIFI_SSID";
const char *password = "YOUR_WIFI_PASSWORD";
ESP8266WebServer server(80);

/*********************************************************************/

// RELAY PINS
int relay[] = {0, 4, 5, 16};
bool status_relay[] = {false, false, false, false};

/*********************************************************************/

/**
 * Size of relay array
 * @return number of element in array
 */
int get_relay_size() {
    return sizeof(relay) / sizeof(relay[0]);
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
 * Check if the relay pin received is correct
 * @param num relay pin
 * @return "true" if is correct | "false" if is incorrect
 */
bool is_valid_relay_pin(int num) {
    if (num >= 0 && num <= get_relay_size() - 1) {
        return true;
    }
    return false;
}

/*********************************************************************/

/**
 * Turn on a specific relay pin
 * @param pin Pin of relay pin
 */
void turnOn(int pin) {
    digitalWrite(pin, LOW);
}

/**
 * Turn off a specific relay pin
 * @param pin Pin of relay pin
 */
void turnOff(int pin) {
    digitalWrite(pin, HIGH);
}

/*********************************************************************/

/**
 * Turn on/off a specific relay by parameters
 */
void smartArgs() {
    String message = "";
    int num;

    if (server.arg(0) == "on") {
        num = server.arg(1).toInt() - 1;

        if (is_valid_relay_pin(num)) {
            turnOn(relay[num]);
            status_relay[num] = true;

            message = "On = " + server.arg(1);

            server.send(200, "text/plain", message);
        } else {
            message = "Error: num of relay";

            server.send(400, "text/plain", message);
        }
    } else if (server.arg(0) == "off") {
        num = server.arg(1).toInt() - 1;

        if (is_valid_relay_pin(num)) {
            turnOff(relay[num]);
            status_relay[num] = false;

            message = "Off = " + server.arg(1);

            server.send(200, "text/plain", message);
        } else {
            message = "Error: num of relay";

            server.send(400, "text/plain", message);
        }
    } else {
        message = "Error: incorrect action";

        server.send(400, "text/plain", message);
    }
}

/*********************************************************************/

/**
 * Send the status of relays
 */
void relayStatus() {
    String status;
    DynamicJsonDocument doc(1024);

    for (int i = 0; i < get_relay_size(); i++) {
        int num = i + 1;
        String name = "in" + String(num);

        doc[name] = status_relay[i];
    }

    serializeJson(doc, status);

    server.send(200, "text/json", status);
}

/*********************************************************************/

void setup() {
    initRelay(get_relay_size());

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

    WiFi.hostname("NodeMCU-PowerStripTwo");

    server.on("/smart", smartArgs);
    server.on("/smart/status", relayStatus);

    server.begin();
}

/*********************************************************************/

void loop() {
    server.handleClient();
}
