/*
 * Copyright (c) 2015, Majenko Technologies
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#undef __STRICT_ANSI__
#include "Arduino.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#define ARDUINOJSON_USE_LONG_LONG 1 //our http posts contain long long; default is 0; define before include
//https://arduinojson.org/v6/api/config/use_long_long/
#include <ArduinoJson.hpp>

void handleRoot();
void handleNotFound();
int mode_to_int(String mode);
String string_to_hex(String col);

const char *ssid = "t0";
const char *password = "vmcg5983";

ESP8266WebServer server(80);

const int led = 2;
String incoming, msg;

void setup(void)
{
    pinMode(led, OUTPUT);
    digitalWrite(led, 1);
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if (MDNS.begin("esp8266")) {
        Serial.println("MDNS responder started");
    }

    server.on("/", handleRoot);
    server.on("/post", []() { //Handler for the body
        if (server.hasArg("plain") == true) //Check if body received
        {
            //send HTTP response
            incoming += "`Body received:\n`";

            ArduinoJson::StaticJsonDocument<1024> post;// Calculated needed size here: https://arduinojson.org/v6/assistant/
            ArduinoJson::deserializeJson(post, server.arg("plain"));

            // From here the json response is parsed in post
            // Access members like this:

            String mode = post["MODE"];// mode is implicitly casted to String because of right side static binding
            // Explicit: String mode = dataGroup["MODE"].as<String>();
            String color = post["COLOR"];

            server.send(200, "text/plain", incoming);

            incoming.clear();

            //send body to msp432
            incoming += mode_to_int(mode);
            incoming += "|";
            incoming += string_to_hex(color);

            Serial.println(incoming);
            // Reset message buffer
            incoming.clear();
        } else {
            server.send(200, "text/plain", "Body not received.");
            return;
        }
    });

    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("HTTP server started");
}

int mode_to_int(String mode)
{
    if (mode == "STATIC") {
        return 1;
    } else if (mode == "PULSE") {
        return 2;
    } else if (mode == "DISCO") {
        return 3;
    } else if (mode == "GRADIENT") {
        return 4;
    } else if (mode == "WAVE") {
        return 5;
    } else if (mode == "RAINBOW" || mode == "CYCLE") {
        return 6;
    } else {
        return 0;
    } //handling typos
}

String string_to_hex(String col)
{
    if (col == "WHITE") {
        return "#FFFFFF";
    } else if (col == "BLACK") {
        return "#000000";
    } else if (col == "RED") {
        return "#FF0000";
    } else if (col == "GREEN") {
        return "#00FF00";
    } else if (col == "BLUE") {
        return "#0000FF";
    } else if (col == "YELLOW") {
        return "#FFFF00";
    } else if (col == "FUCHSIA" || col == "MAGENTA") {
        return "#FF00FF";
    } else if (col == "AQUA") {
        return "#00FFFF";
    } else if (col == "ORANGE") {
        return "#FF8000";
    } else if (col == "PINK") {
        return "#FF0080";
    } else if (col == "LIME") {
        return "#80FF00";
    } else if (col == "CYAN") {
        return "#00FF80";
    } else if (col == "VIOLET") {
        return "#8000FF";
    } else if (col == "LBLUE") {
        return "#0080FF";
    } else if (col == "") {
        return "#FFFFFF";
    } else if(col[0] == '#' && col.length() == 8) //1 "#" + 6 "hex" + 1 "\0"
    {
     return col;
    }
    else
    {
        return "#000000";
    } //handling typos
}

void loop(void)
{
    server.handleClient();

    if (Serial.available()) {
        // Get the char
        char c = Serial.read();
        // Did we reach EOL?
        if (c == '\n')
        {
            msg.clear();
        } else {
            msg += c;
        }
    }
}

void handleRoot()
{
    digitalWrite(led, 0);
    char temp[400];

    snprintf(
            temp,
            400,
            "<html>"
            "<head>"
            "<meta http-equiv='refresh' content='5'/>"
            "<title>ESP8266</title>"
            "<style>"
            "body { background-color: #000000; font-family: Arial, Helvetica, Sans-Serif; Color: #FFFFFF; }"
            "</style>"
            "</head>"
            "<body>"
            "<h1><center>WebServer running!</center></h1>"
            "</body>"
            "</html>");
    server.send(200, "text/html", temp);
    digitalWrite(led, 1);
}

void handleNotFound()
{
    digitalWrite(led, 0);
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";

    for (uint8_t i = 0; i < server.args(); i++) {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }

    server.send(404, "text/plain", message);
    digitalWrite(led, 1);
}
