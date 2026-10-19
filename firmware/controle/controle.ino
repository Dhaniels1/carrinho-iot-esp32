#include <WiFi.h>
#include <HTTPClient.h>

// Coloque aqui a MESMA rede do carrinho. Nao publique credenciais reais.
const char* ssid = "NOME_DA_REDE";
const char* senha = "SENHA_DA_REDE";
const char* ipCarrinho = "192.168.10.100"; // confira IP atual no Monitor Serial

const int BOTAO_ESQUERDA = 18, BOTAO_DIREITA = 19;
const int BOTAO_FRENTE = 21, BOTAO_RE = 22;
String ultimoComando = "";
unsigned long ultimoEnvio = 0;

bool enviarComando(const String &comando) {
  if (WiFi.status() != WL_CONNECTED) return false;
  HTTPClient http;
  String url = "http://" + String(ipCarrinho) + "/comando?valor=" + comando;
  http.begin(url);
  http.setTimeout(700);
  int codigo = http.GET();
  http.end();
  Serial.printf("%s -> HTTP %d\n", comando.c_str(), codigo);
  return codigo == 200;
}

void setup() {
  Serial.begin(9600);
  for (int p : {BOTAO_ESQUERDA, BOTAO_DIREITA, BOTAO_FRENTE, BOTAO_RE})
    pinMode(p, INPUT_PULLUP);
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.begin(ssid, senha);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\nControle pronto | IP: %s\n", WiFi.localIP().toString().c_str());
}

void loop() {
  String comando = "parar";
  if (!digitalRead(BOTAO_ESQUERDA)) comando = "esquerda";
  else if (!digitalRead(BOTAO_DIREITA)) comando = "direita";
  else if (!digitalRead(BOTAO_FRENTE)) comando = "frente";
  else if (!digitalRead(BOTAO_RE)) comando = "re";

  // Renovacao a cada 300 ms enquanto um botao esta pressionado:
  // o carrinho para se passar mais de 1 s sem comando.
  if (comando != ultimoComando ||
      (comando != "parar" && millis() - ultimoEnvio >= 300)) {
    if (enviarComando(comando)) ultimoComando = comando;
    ultimoEnvio = millis();
  }
  delay(30);
}
