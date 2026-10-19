#include <WiFi.h>
#include <WebServer.h>
#include "secrets.h"
#include "pagina.h"

WebServer servidor(80);

// Pinos e velocidades calibradas na montagem do projeto
const int IN1 = 25, IN2 = 26;    // Motor direito
const int IN3 = 32, IN4 = 33;    // Motor esquerdo
const int VEL_DIR_BASE = 240;
const int VEL_ESQ_BASE = 243;
int velocidadePercentual = 100; // 30% a 100%

// Os dois RGB compartilham os mesmos canais
const int LED_R = 16, LED_G = 17, LED_B = 18;
const bool LED_ANODO_COMUM = false; // true se pino comum vai ao 3V3
const int SENSOR_LUZ = 34;
const int LIMITE_ESCURO = 300, LIMITE_CLARO = 450;

bool farolLigado = false, setaAcesa = true;
String movimentoAtual = "parar";
int etapaArcoIris = 0;
unsigned long ultimoComandoMovimento = 0;
unsigned long ultimoPisca = 0, ultimoArcoIris = 0;
unsigned long ultimoSerial = 0, ultimoWiFi = 0;
const unsigned long TIMEOUT_COMANDO = 1000;

void escreverRGB(int r, int g, int b) {
  r = constrain(r, 0, 255); g = constrain(g, 0, 255); b = constrain(b, 0, 255);
  if (LED_ANODO_COMUM) { r = 255 - r; g = 255 - g; b = 255 - b; }
  ledcWrite(LED_R, r); ledcWrite(LED_G, g); ledcWrite(LED_B, b);
}

void parar() {
  movimentoAtual = "parar";
  ledcWrite(IN1, 0); ledcWrite(IN2, 0);
  ledcWrite(IN3, 0); ledcWrite(IN4, 0);
}

int velD() { return VEL_DIR_BASE * velocidadePercentual / 100; }
int velE() { return VEL_ESQ_BASE * velocidadePercentual / 100; }

void mover(const String &comando) {
  int d = velD(), e = velE();
  if (comando == "frente") {
    ledcWrite(IN1, 0); ledcWrite(IN2, d);
    ledcWrite(IN3, 0); ledcWrite(IN4, e);
  } else if (comando == "re") {
    ledcWrite(IN1, d); ledcWrite(IN2, 0);
    ledcWrite(IN3, e); ledcWrite(IN4, 0);
  } else if (comando == "esquerda") {
    ledcWrite(IN1, 0); ledcWrite(IN2, d);
    ledcWrite(IN3, 0); ledcWrite(IN4, 0);
  } else if (comando == "direita") {
    ledcWrite(IN1, 0); ledcWrite(IN2, 0);
    ledcWrite(IN3, 0); ledcWrite(IN4, e);
  }
  if (movimentoAtual != comando && (comando == "esquerda" || comando == "direita")) {
    ultimoPisca = millis();
    setaAcesa = true;
  }
  movimentoAtual = comando;
  ultimoComandoMovimento = millis();
}

void receberComando() {
  if (!servidor.hasArg("valor")) {
    servidor.send(400, "text/plain", "Parametro valor ausente");
    return;
  }
  String comando = servidor.arg("valor");
  if (comando == "parar") parar();
  else if (comando == "acelerar" || comando == "desacelerar") {
    velocidadePercentual = constrain(velocidadePercentual + (comando == "acelerar" ? 10 : -10), 30, 100);
    if (movimentoAtual != "parar") mover(movimentoAtual);
    Serial.printf("Velocidade: %d%%\n", velocidadePercentual);
  } else if (comando == "frente" || comando == "re" || comando == "esquerda" || comando == "direita") {
    mover(comando);
  } else {
    servidor.send(400, "text/plain", "Comando desconhecido");
    return;
  }
  servidor.send(200, "text/plain", "OK: " + comando);
}

void verificarLuminosidade() {
  int luz = analogRead(SENSOR_LUZ);
  if (luz < LIMITE_ESCURO) farolLigado = true;
  else if (luz > LIMITE_CLARO) farolLigado = false;
  if (millis() - ultimoSerial >= 3000) {
    ultimoSerial = millis();
    Serial.printf("Luz: %d | IP: %s | Movimento: %s | PWM: %d%%\n",
      luz, WiFi.localIP().toString().c_str(), movimentoAtual.c_str(), velocidadePercentual);
  }
}

void atualizarLEDs() {
  unsigned long agora = millis();
  if (movimentoAtual == "esquerda" || movimentoAtual == "direita") {
    if (agora - ultimoPisca >= 400) {
      ultimoPisca = agora;
      setaAcesa = !setaAcesa;
    }
    if (!setaAcesa) escreverRGB(0, 0, 0);
    else if (movimentoAtual == "esquerda") escreverRGB(255, 120, 0);
    else escreverRGB(0, 0, 255);
  } else if (!farolLigado) escreverRGB(0, 0, 0);
  else if (agora - ultimoArcoIris >= 20) {
    ultimoArcoIris = agora;
    int r = 0, g = 0, b = 0;
    if (etapaArcoIris < 256) { r = 255 - etapaArcoIris; g = etapaArcoIris; }
    else if (etapaArcoIris < 512) {
      int x = etapaArcoIris - 256; g = 255 - x; b = x;
    } else {
      int x = etapaArcoIris - 512; b = 255 - x; r = x;
    }
    escreverRGB(r, g, b);
    etapaArcoIris = (etapaArcoIris + 1) % 768;
  }
}

void setup() {
  Serial.begin(9600);
  for (int p : {IN1, IN2, IN3, IN4}) {
    pinMode(p, OUTPUT);
    ledcAttach(p, 5000, 8);
  }
  for (int p : {LED_R, LED_G, LED_B}) ledcAttach(p, 5000, 8);
  pinMode(SENSOR_LUZ, INPUT);
  parar();
  escreverRGB(0, 0, 0);

  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.printf("\nIP DO CARRINHO: %s\n", WiFi.localIP().toString().c_str());

  servidor.on("/", HTTP_GET, []() {
    servidor.send_P(200, "text/html; charset=utf-8", PAGINA_WEB);
  });
  servidor.on("/comando", HTTP_GET, receberComando);
  servidor.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  servidor.handleClient();
  verificarLuminosidade();
  atualizarLEDs();
  if (movimentoAtual != "parar" && millis() - ultimoComandoMovimento > TIMEOUT_COMANDO) {
    parar();
    Serial.println("PARADA: comando nao renovado");
  }
  if (WiFi.status() != WL_CONNECTED) {
    parar();
    if (millis() - ultimoWiFi > 5000) {
      ultimoWiFi = millis();
      WiFi.reconnect();
    }
  }
}
