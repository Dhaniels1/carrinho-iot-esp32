// ETAPA 03 - ESP32 do CARRINHO: motor, farol, LDR e servidor Wi-Fi completo.
// Reconstrucao didatica de novembro/2026, nao sketch historico recuperado.
// Use este sketch JUNTO com ../controle_wifi/controle_wifi.ino em outro ESP32.
// Exige secrets.h nesta pasta (copie secrets.h.example e configure senha).
// Arduino-ESP32 3.x. Teste com rodas suspensas e corte fisico de alimentacao.
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "secrets.h"

#if !defined(AP_SSID) || !defined(AP_PASSWORD)
#error "Crie secrets.h com AP_SSID e AP_PASSWORD (veja secrets.h.example)"
#endif

WebServer servidor(80);
const int MOTOR_D1 = 25, MOTOR_D2 = 26, MOTOR_E1 = 32, MOTOR_E2 = 33;
const int LED_R = 16, LED_G = 17, LED_B = 18, LDR = 34;
const int PWM_D = 240, PWM_E = 243;
const unsigned long TIMEOUT_MS = 1000;
const int LIMITE_ESCURO = 300, HISTERESE = 150;
const bool LED_ANODO_COMUM = false;

int velocidade = 30;
int leituraLuz = 0;
char direcao = 'p'; // f=frente, r=re, e=esquerda, d=direita, p=parar
bool escuro = false, pisca = false;
unsigned long ultimoComando = 0, ultimoPisca = 0, ultimoLog = 0;

void parar() {
  for (int p : {MOTOR_D1, MOTOR_D2, MOTOR_E1, MOTOR_E2}) ledcWrite(p, 0);
  direcao = 'p';
}

void andar(char modo) {
  // Zera entradas de sentido antes de aplicar PWM.
  parar();
  delayMicroseconds(120);
  const int e = PWM_E * velocidade / 100;
  const int d = PWM_D * velocidade / 100;

  if (modo == 'f') {
    ledcWrite(MOTOR_E2, e);
    ledcWrite(MOTOR_D2, d);
  } else if (modo == 'r') {
    ledcWrite(MOTOR_E1, e);
    ledcWrite(MOTOR_D1, d);
  } else if (modo == 'e') {
    ledcWrite(MOTOR_D2, d);
  } else if (modo == 'd') {
    ledcWrite(MOTOR_E2, e);
  } else return;
  direcao = modo;
  ultimoComando = millis();
}

void escreverCor(int r, int g, int b) {
  if (LED_ANODO_COMUM) { r = 255-r; g = 255-g; b = 255-b; }
  ledcWrite(LED_R, r);
  ledcWrite(LED_G, g);
  ledcWrite(LED_B, b);
}

void atualizarLuzes() {
  leituraLuz = analogRead(LDR);
  if (leituraLuz < LIMITE_ESCURO) escuro = true;
  else if (leituraLuz > LIMITE_ESCURO + HISTERESE) escuro = false;
  if (millis() - ultimoPisca >= 400) {
    ultimoPisca = millis();
    pisca = !pisca;
  }

  // Ambos os RGB usam os mesmos 3 GPIOs e mudam de cor juntos.
  if (direcao == 'e') escreverCor(pisca ? 255 : 0, pisca ? 110 : 0, 0);
  else if (direcao == 'd') escreverCor(0, 0, pisca ? 255 : 0);
  else if (escuro) escreverCor(255, 255, 255);
  else escreverCor(0, 0, 0);

  if (millis() - ultimoLog >= 3000) {
    ultimoLog = millis();
    Serial.printf("LDR=%d | movimento=%c | velocidade=%d%% | clientes=%d\n",
      leituraLuz, direcao, velocidade, WiFi.softAPgetStationNum());
  }
}

void receberComando() {
  if (!servidor.hasArg("valor")) {
    parar();
    servidor.send(400, "text/plain", "Faltou o parametro valor");
    return;
  }
  const String comando = servidor.arg("valor");
  if (comando == "frente") andar('f');
  else if (comando == "re") andar('r');
  else if (comando == "esquerda") andar('e');
  else if (comando == "direita") andar('d');
  else if (comando == "parar") parar();
  else if (comando == "acelerar" || comando == "desacelerar") {
    parar(); // ajustar a velocidade com o carrinho parado e mais seguro
    velocidade = constrain(velocidade + (comando == "acelerar" ? 10 : -10), 30, 100);
  } else {
    parar();
    servidor.send(400, "text/plain", "Comando desconhecido");
    return;
  }
  Serial.println("HTTP: " + comando);
  servidor.send(200, "text/plain", "OK: " + comando);
}

void mostrarEstado() {
  const String json = "{\"movimento\":\"" + String(direcao) +
    "\",\"velocidade\":" + String(velocidade) +
    ",\"ldr\":" + String(leituraLuz) + "}";
  servidor.send(200, "application/json", json);
}

void setup() {
  Serial.begin(9600);
  for (int p : {MOTOR_D1, MOTOR_D2, MOTOR_E1, MOTOR_E2}) {
    pinMode(p, OUTPUT);
    digitalWrite(p, LOW);
    if (!ledcAttach(p, 5000, 8)) {
      Serial.printf("ERRO PWM MOTOR GPIO %d\n", p);
      while (true) delay(1000);
    }
    ledcWrite(p, 0);
  }
  for (int p : {LED_R, LED_G, LED_B}) {
    if (!ledcAttach(p, 5000, 8)) {
      parar();
      Serial.printf("ERRO PWM RGB GPIO %d\n", p);
      while (true) delay(1000);
    }
  }
  pinMode(LDR, INPUT);
  parar();
  escreverCor(0, 0, 0);

  WiFi.mode(WIFI_AP);
  const IPAddress ip(192, 168, 4, 1);
  if (!WiFi.softAPConfig(ip, ip, IPAddress(255,255,255,0)) ||
      !WiFi.softAP(AP_SSID, AP_PASSWORD)) {
    Serial.println("ERRO: rede Wi-Fi nao iniciou");
    parar();
    while (true) delay(1000);
  }
  Serial.print("Wi-Fi do carrinho | IP: ");
  Serial.println(WiFi.softAPIP());

  servidor.on("/", HTTP_GET, []() {
    servidor.send(200, "text/plain",
      "Etapa 03: ESP32 do carrinho online. Use /comando?valor=frente ou /estado");
  });
  servidor.on("/comando", HTTP_GET, receberComando);
  servidor.on("/estado", HTTP_GET, mostrarEstado);
  servidor.begin();
  Serial.println("Servidor pronto; ligue o segundo ESP32 (controle).");
}

void loop() {
  servidor.handleClient();
  if (direcao != 'p' && millis() - ultimoComando >= TIMEOUT_MS) {
    parar();
    Serial.println("PARADA DE SEGURANCA: timeout de 1s.");
  }
  if (direcao != 'p' && WiFi.softAPgetStationNum() == 0) {
    parar();
    Serial.println("PARADA: nenhum cliente conectado.");
  }
  atualizarLuzes();
}
