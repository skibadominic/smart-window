#include <WiFi.h> //biblioteca para a conexão com wifi
#include <WebServer.h> //biblioteca para o servidor web
#include <ESP32Servo.h> //dependencias do esp
#include "time.h" //biblioteca para funções de tempo para a log
#include "interface.h" //arquivo de interface do site

//rede e senha para a conexão wifi
const char* ssid = "iPhone de Dominic";
const char* password = "dom230307";

WebServer server(80);
Servo meuServo;

//definição de pinos e estados
const int pinoSensor = 14;
const int pinoServo = 13;
int janelaAberta = 1;
int movendo = 0;

#define MAX_HISTORY 10
String historico[MAX_HISTORY];
int indiceHistorico = 0;
int totalEventos = 0;

void registrarEvento(String acao) {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        return;
    }

    String dataHora = "";
    if(timeinfo.tm_mday < 10) dataHora += "0";
    dataHora += String(timeinfo.tm_mday) + "/";
    if((timeinfo.tm_mon + 1) < 10) dataHora += "0";
    dataHora += String(timeinfo.tm_mon + 1) + "/";
    dataHora += String(timeinfo.tm_year + 1900) + " ";
    if(timeinfo.tm_hour < 10) dataHora += "0";
    dataHora += String(timeinfo.tm_hour) + ":";
    if(timeinfo.tm_min < 10) dataHora += "0";
    dataHora += String(timeinfo.tm_min) + ":";
    if(timeinfo.tm_sec < 10) dataHora += "0";
    dataHora += String(timeinfo.tm_sec);
    
    String registro = "{\"data\":\"" + dataHora + "\", \"acao\":\"" + acao + "\"}";

    historico[indiceHistorico] = registro;
    indiceHistorico = (indiceHistorico + 1) % MAX_HISTORY;
    if(totalEventos < MAX_HISTORY) totalEventos++;
}

void acionarMotor(int fechar) {
    if (movendo) return;
    
    movendo = 1;
    if (fechar) {
        meuServo.write(180);
        delay(4000);
        meuServo.write(90);
        janelaAberta = 0;
        registrarEvento("Fechamento Automático (Chuva)");
    } else {
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

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) { 
        delay(500); 
    }

    configTime(-3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

    server.on("/", []() { 
        server.send(200, "text/html", INDEX_HTML); 
    });
    
    server.on("/get_status", []() {
        server.send(200, "text/plain", janelaAberta ? "ABERTA" : "FECHADA");
    });

    server.on("/get_history", []() {
        String json = "[";
        for(int i = 0; i < totalEventos; i++) {
            int pos = (indiceHistorico - 1 - i);
            if (pos < 0) pos += MAX_HISTORY;
            json += historico[pos];
            if(i < totalEventos - 1) json += ",";
        }
        json += "]";
        server.send(200, "application/json", json);
    });

    server.begin();
}

void loop() {
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
