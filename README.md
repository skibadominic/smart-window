# Smart Window IoT (Janela Automática Ciberfísica)

Um sistema ciberfísico autônomo baseado no **DOIT ESP32 DEVKIT V1** para automação residencial. O projeto monitora as condições climáticas locais e atua fisicamente sobre uma janela, fechando-a automaticamente em caso de chuva e notificando o usuário em tempo real.

Projeto desenvolvido para a disciplina de Conectividade em Sistemas Ciberfísicos do curso de Engenharia de Software da Pontifícia Universidade Católica do Paraná (PUCPR).

## 🚀 Funcionalidades

* **Atuação Autônoma:** Fechamento e abertura automática baseada na leitura instantânea do sensor de chuva e travamento por limites físicos (Fim de Curso).
* **Dashboard Web Local:** Interface de controle servida diretamente pelo ESP32 via rede local, acessível por meio do protocolo mDNS (`http://janela.local`).
* **Previsão do Tempo Integrada:** Consumo da API do OpenWeatherMap realizado via JavaScript diretamente pelo lado do cliente (Client-Side), economizando largura de banda e memória do microcontrolador.
* **Notificações Push:** Alertas instantâneos enviados para o celular via Telegram sempre que o motor atua por motivos de segurança climática.
* **Histórico de Eventos:** Registro cronológico de aberturas e fechamentos sincronizado com o fuso horário global via protocolo NTP (UDP).

## 🛠️ Hardware Utilizado

* Placa **DOIT ESP32 DEVKIT V1**
* Servomotor (Atuador da Janela)
* Sensor de Chuva (Placa Condutiva)
* 2x Chaves Fim de Curso (Microswitches com PULLUP interno)
* 3x LEDs Indicadores (Verde, Vermelho e Status Wi-Fi)

## 🔌 Esquema de Pinos (Pinout)

| Componente | Pino ESP32 | Função |
| :--- | :--- | :--- |
| Sensor de Chuva | `GPIO 34` | Leitura digital da presença de água |
| Servomotor | `GPIO 13` | Sinal PWM para controle de ângulo |
| Fim de Curso Fechada | `GPIO 25` | Limite de fechamento (`INPUT_PULLUP`) |
| Fim de Curso Aberta | `GPIO 26` | Limite de abertura (`INPUT_PULLUP`) |
| LED Wi-Fi | `GPIO 14` | Status da conexão de rede local |
| LED Vermelho | `GPIO 27` | Indicador visual de janela fechada |
| LED Verde | `GPIO 32` | Indicador visual de janela aberta |

## ⚙️ Configuração e Instalação

### 1. Dependências (Arduino IDE)
Certifique-se de ter as seguintes bibliotecas instaladas no seu ambiente de desenvolvimento:
* `WiFi` e `WiFiClientSecure` (Nativas do core ESP32)
* `WebServer` e `ESPmDNS` (Nativas)
* `HTTPClient` e `WiFiUdp` (Nativas)
* `ESP32Servo` (Instalar via Gerenciador de Bibliotecas)

### 2. Configuração de Credenciais
Antes de compilar, abra o arquivo principal `.ino` e preencha suas credenciais de rede e tokens de API:

```cpp
const char* ssid = "NOME_DO_SEU_WIFI_OU_HOTSPOT";
const char* password = "SENHA_DO_WIFI";

const char* tokenTelegram = "SEU_TOKEN_DO_BOT";
const char* chatIdTelegram = "SEU_CHAT_ID";
No arquivo interface.h, insira sua chave da API do clima:

JavaScript
const apiKey = "SUA_API_KEY_OPENWEATHERMAP";
3. Acesso à Interface Web
Após realizar o upload do código, não é necessário monitorar a porta serial para descobrir o IP da placa. Certifique-se de que o seu dispositivo (celular ou notebook) está na mesma rede Wi-Fi que o ESP32 e acesse no navegador:

http://janela.local

Engenharia de Software - PUCPR