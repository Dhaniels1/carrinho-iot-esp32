// Etapa 04 — exemplo INDEPENDENTE, criado a partir dos requisitos do trabalho.
// Nao substitui o firmware integrado ja testado em firmware/carrinho/.
// Dois motores + MX1508 presumido, Wi-Fi proprio, controle web e LDR/RGB.
#include <WiFi.h>
#include <WebServer.h>
#include "secrets.h"
#include "pagina.h"

#if !defined(AP_SSID) || !defined(AP_PASSWORD)
#error "Crie secrets.h usando secrets.h.example"
#endif

WebServer servidor(80);

// Entradas da ponte H: dois motores, conforme firmware atual.
const int DIR_A = 25, DIR_B = 26;       // motor direito
const int ESQ_A = 32, ESQ_B = 33;       // motor esquerdo
const int POT_DIR = 240, POT_ESQ = 243; // calibracoes existentes

const int R = 16, G = 17, B = 18;       // LEDs RGB compartilhados
const int LDR = 34;                     // sensor de luminosidade
const int LIMITE_ESCURO = 300;

int velocidade = 30;                   // percentual inicial
String movimento = "parar";
unsigned long ultimoComando = 0;
unsigned long ultimoPisca = 0;
bool aceso = false;

void leds(int vermelho, int verde, int azul) {
  ledcWrite(R, vermelho);
  ledcWrite(G, verde);
  ledcWrite(B, azul);
}

void pararMotores() {
  ledcWrite(DIR_A, 0); ledcWrite(DIR_B, 0);
  ledcWrite(ESQ_A, 0); ledcWrite(ESQ_B, 0);
  movimento = "parar";
}

void mover(const String &direcao) {
  // Sempre desligar as entradas antes de aplicar o novo sentido.
  pararMotores();
  delayMicroseconds(100);

  const int pwmD = POT_DIR * velocidade / 100;
  const int pwmE = POT_ESQ * velocidade / 100;

  if (direcao == "frente") {
    ledcWrite(DIR_B, pwmD);
    ledcWrite(ESQ_B, pwmE);
  } else if (direcao == "re") {
    ledcWrite(DIR_A, pwmD);
    ledcWrite(ESQ_A, pwmE);
  } else if (direcao == "esquerda") {
    ledcWrite(DIR_B, pwmD);
  } else if (direcao == "direita") {
    ledcWrite(ESQ_B, pwmE);
  }
  movimento = direcao;
  ultimoComando = millis();
}

void receberComando() {
  const String valor = servidor.arg("valor");
  if (valor == "parar") {
    pararMotores();
  } else if (valor == "acelerar" || valor == "desacelerar") {
    velocidade = constrain(velocidade + (valor == "acelerar" ? 10 : -10), 30, 100);
    // Copia a direcao ANTES de mover(): pararMotores() altera o estado global.
    // Passar a String global por referencia faria o movimento virar "parar".
    if (movimento != "parar") {
      const String direcaoAnterior = movimento;
      mover(direcaoAnterior);
    }
  } else if (valor == "frente" || valor == "re" ||
             valor == "esquerda" || valor == "direita") {
    mover(valor);
  } else {
    pararMotores();
    servidor.send(400, "text/plain", "Comando invalido");
    return;
  }
  servidor.send(200, "text/plain", "OK");
}

void atualizarIluminacao() {
  // Os dois LEDs RGB compartilhados nao conseguem sinalizar lados isolados.
  if (movimento == "esquerda" || movimento == "direita") {
    if (millis() - ultimoPisca >= 400) {
      ultimoPisca = millis();
      aceso = !aceso;
    }
    leds(aceso ? 255 : 0, aceso ? 110 : 0, 0); // pisca em laranja
  } else if (analogRead(LDR) < LIMITE_ESCURO) {
    leds(255, 255, 255); // farol automatico
  } else {
    leds(0, 0, 0);
  }
}

void setup() {
  Serial.begin(9600);
  for (int p : {DIR_A, DIR_B, ESQ_A, ESQ_B}) {
    pinMode(p, OUTPUT);
    digitalWrite(p, LOW);
    if (!ledcAttach(p, 5000, 8)) {
      Serial.println("Falha ao inicializar PWM dos motores");
      while (true) delay(1000);
    }
  }
  pararMotores();
  for (int p : {R, G, B}) {
    if (!ledcAttach(p, 5000, 8)) {
      Serial.println("Falha ao inicializar PWM dos LEDs");
      while (true) delay(1000);
    }
  }
  leds(0, 0, 0);
  pinMode(LDR, INPUT);

  WiFi.mode(WIFI_AP);
  if (!WiFi.softAP(AP_SSID, AP_PASSWORD)) {
    Serial.println("Falha no Wi-Fi");
    while (true) delay(1000);
  }
  Serial.print("IP do carrinho: ");
  Serial.println(WiFi.softAPIP());

  servidor.on("/", HTTP_GET, []() {
    servidor.send_P(200, "text/html; charset=utf-8", PAGINA_WEB);
  });
  servidor.on("/comando", HTTP_GET, receberComando);
  servidor.begin();
}

void loop() {
  servidor.handleClient();
  atualizarIluminacao();

  // Falha de comunicacao: interromper movimento, mesmo sem comando PARAR.
  if (movimento != "parar" && millis() - ultimoComando > 800)
    pararMotores();
  if (movimento != "parar" && WiFi.softAPgetStationNum() == 0)
    pararMotores();
}
