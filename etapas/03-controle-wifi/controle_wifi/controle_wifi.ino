// ETAPA 03 - ESP32 do CONTROLE: 4 botoes com envio HTTP ao carrinho.
// Reconstrucao didatica (novembro/2026), NAO sketch historico recuperado.
// Use no segundo ESP32, junto ao carrinho_wifi.ino gravado no primeiro.
// Cada botao liga um GPIO ao GND: INPUT_PULLUP, sem resistor externo.
// FRENTE=21, RE=22, ESQUERDA=18, DIREITA=19.
// Dois botoes juntos: FRENTE+RE acelera; ESQUERDA+DIREITA desacelera.
// A velocidade muda com o carrinho PARADO (mais seguro). Solte os botoes
// apos reconectar para rearmar o controle. Monitor Serial: 9600 baud.
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "secrets.h"

#if !defined(AP_SSID) || !defined(AP_PASSWORD)
#error "Crie secrets.h com AP_SSID e AP_PASSWORD, iguais aos do carrinho."
#endif

const int BOTAO_ESQUERDA = 18, BOTAO_DIREITA = 19;
const int BOTAO_FRENTE = 21, BOTAO_RE = 22;
const char *IP_CARRINHO = "192.168.4.1";
const unsigned long RENOVACAO_MS = 250;
const unsigned long RECONECTAR_MS = 5000;

String ultimoComando = "";
unsigned long ultimoEnvio = 0, ultimaReconexao = 0;
bool conectado = false, armado = false;

bool todosSoltos() {
  return digitalRead(BOTAO_FRENTE) == HIGH &&
         digitalRead(BOTAO_RE) == HIGH &&
         digitalRead(BOTAO_ESQUERDA) == HIGH &&
         digitalRead(BOTAO_DIREITA) == HIGH;
}

String lerBotoes() {
  const bool f = digitalRead(BOTAO_FRENTE) == LOW;
  const bool r = digitalRead(BOTAO_RE) == LOW;
  const bool e = digitalRead(BOTAO_ESQUERDA) == LOW;
  const bool d = digitalRead(BOTAO_DIREITA) == LOW;
  if (f && r && !e && !d) return "acelerar";
  if (e && d && !f && !r) return "desacelerar";
  // Pressionar botoes de direcoes conflitantes nao deve mover o carrinho.
  if ((f && r) || (e && d)) return "parar";
  if (f && !r && !e && !d) return "frente";
  if (r && !f && !e && !d) return "re";
  if (e && !f && !r && !d) return "esquerda";
  if (d && !f && !r && !e) return "direita";
  return "parar";
}

bool enviarComando(const String &comando) {
  if (WiFi.status() != WL_CONNECTED) return false;
  HTTPClient http;
  String url = "http://" + String(IP_CARRINHO) + "/comando?valor=" + comando;
  http.begin(url);
  http.setTimeout(400);
  const int status = http.GET();
  http.end();
  Serial.printf("%s -> HTTP %d\n", comando.c_str(), status);
  return status == 200;
}

void setup() {
  Serial.begin(9600);
  for (int p : {BOTAO_ESQUERDA, BOTAO_DIREITA, BOTAO_FRENTE, BOTAO_RE})
    pinMode(p, INPUT_PULLUP);
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.setAutoReconnect(true);
  WiFi.begin(AP_SSID, AP_PASSWORD);
  ultimaReconexao = millis();
  Serial.println("Controle ESP32: conectando ao AP do carrinho...");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    if (conectado) Serial.println("Conexao perdida. Carrinho para por timeout.");
    conectado = false;
    armado = false;
    ultimoComando = "";
    if (millis() - ultimaReconexao >= RECONECTAR_MS) {
      ultimaReconexao = millis();
      WiFi.reconnect();
    }
    delay(40);
    return;
  }

  if (!conectado) {
    conectado = true;
    armado = false;
    ultimoComando = "";
    Serial.printf("Conectado ao carrinho | IP local %s\n",
                  WiFi.localIP().toString().c_str());
    Serial.println("SOLTE TODOS OS BOTOES para habilitar o controle.");
  }
  if (!armado) {
    if (todosSoltos()) {
      armado = true;
      enviarComando("parar");
      ultimoComando = "parar";
    }
    delay(30);
    return;
  }

  const String comando = lerBotoes();
  const bool movimento = comando == "frente" || comando == "re" ||
                         comando == "esquerda" || comando == "direita";
  const bool mudou = comando != ultimoComando;
  if (mudou || (movimento && millis() - ultimoEnvio >= RENOVACAO_MS)) {
    if (enviarComando(comando)) ultimoComando = comando;
    ultimoEnvio = millis();
  }
  delay(30);
}
