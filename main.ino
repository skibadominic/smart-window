#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "time.h"
#include "interface.h"

const char* ssid = "GRADUATION-07";
const char* password = "ZZyQvdfQ";

WebServer server(80);
Servo meuServo;

const int pinoSensor = 14;
const int pinoServo = 13;
const int pinoLedWifi = 27;
const int pinoLedVermelho = 25;
const int pinoLedVerde = 32;
int janelaAberta = 1;
int movendo = 0;

#define MAX_HISTORY 10
String historico[MAX_HISTORY];
int indiceHistorico = 0;
int totalEventos = 0;

void registrarEvento(String acao) {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return;
  }

  String dataHora = "";
  if (timeinfo.tm_mday < 10) dataHora += "0";
  dataHora += String(timeinfo.tm_mday) + "/";
  if ((timeinfo.tm_mon + 1) < 10) dataHora += "0";
  dataHora += String(timeinfo.tm_mon + 1) + "/";
  dataHora += String(timeinfo.tm_year + 1900) + " ";
  if (timeinfo.tm_hour < 10) dataHora += "0";
  dataHora += String(timeinfo.tm_hour) + ":";
  if (timeinfo.tm_min < 10) dataHora += "0";
  dataHora += String(timeinfo.tm_min) + ":";
  if (timeinfo.tm_sec < 10) dataHora += "0";
  dataHora += String(timeinfo.tm_sec);

  String registro = "{\"data\":\"" + dataHora + "\", \"acao\":\"" + acao + "\"}";

  historico[indiceHistorico] = registro;
  indiceHistorico = (indiceHistorico + 1) % MAX_HISTORY;
  if (totalEventos < MAX_HISTORY) totalEventos++;
}

void acionarMotor(int fechar) {
  if (movendo) return;

  movendo = 1;
  if (fechar) {
    digitalWrite(pinoLedVerde, LOW);
    meuServo.write(180);
    for (int i = 0; i < 8; i++) {
      digitalWrite(pinoLedVermelho, !digitalRead(pinoLedVermelho));
      delay(500);
    }

    meuServo.write(90);
    digitalWrite(pinoLedVermelho, HIGH);
    janelaAberta = 0;
    registrarEvento("Fechamento Automático (Chuva)");
  } else {
    digitalWrite(pinoLedVermelho, LOW);
    digitalWrite(pinoLedVerde, HIGH);
    meuServo.write(0);
    delay(4000);
    meuServo.write(90);
    janelaAberta = 1;
    registrarEvento("Abertura Automática (Seco)");
  }
  movendo = 0;
}

void setup() {
  Serial.begin(115200);
  pinMode(pinoSensor, INPUT);
  meuServo.setPeriodHertz(50);
  meuServo.attach(pinoServo, 500, 2400);
  meuServo.write(90);
  
  pinMode(pinoLedWifi, OUTPUT);
  pinMode(pinoLedVermelho, OUTPUT);
  pinMode(pinoLedVerde, OUTPUT);

  digitalWrite(pinoLedVerde, HIGH);
  digitalWrite(pinoLedVermelho, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    digitalWrite(pinoLedWifi, !
  digitalRead(pinoLedWifi));
    delay(500);
  }
  digitalWrite(pinoLedWifi, HIGH);

  configTime(-3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  ArduinoOTA.setHostname("SmartWindow");
  ArduinoOTA.begin();

  server.on("/", []() {
    server.send(200, "text/html", INDEX_HTML);
  });

  server.on("/get_status", []() {
    server.send(200, "text/plain", janelaAberta ? "ABERTA" : "FECHADA");
  });

  server.on("/get_history", []() {
    String json = "[";
    for (int i = 0; i < totalEventos; i++) {
      int pos = (indiceHistorico - 1 - i);
      if (pos < 0) pos += MAX_HISTORY;
      json += historico[pos];
      if (i < totalEventos - 1) json += ",";
    }
    json += "]";
    server.send(200, "application/json", json);
  });

  server.begin();
}

void loop() {
  ArduinoOTA.handle();
  server.handleClient();

  if (!movendo) {
    int sensorMolhado = digitalRead(pinoSensor);

    if (sensorMolhado == LOW && janelaAberta == 1) {
      acionarMotor(1);
    }
    else if (sensorMolhado == HIGH && janelaAberta == 0) {
      acionarMotor(0);
    }
  }
  delay(50);
}
