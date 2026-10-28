#include <WiFi.h>
#include <HTTPClient.h>

// O carrinho cria uma rede propria. Use a senha AP_PASSWORD de secrets.h
// do ESP32 do carrinho (copie apenas LOCALMENTE, nao publique credenciais).
const char* ssid = "Carrinho-IoT";
const char* senha = "SENHA_DO_AP_DO_CARRINHO"; // altere antes de carregar
const char* ipCarrinho = "192.168.4.1"; // IP fixo do carrinho no modo AP

const int BOTAO_ESQUERDA = 18, BOTAO_DIREITA = 19;
const int BOTAO_FRENTE = 21, BOTAO_RE = 22;
String ultimoComando = "";
unsigned long ultimoEnvio = 0;
unsigned long ultimaTentativaWiFi = 0;
bool conectadoAntes = false;

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
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, senha);
  ultimaTentativaWiFi = millis();
  Serial.println("Conectando ao Wi-Fi do carrinho...");
  // Nao bloquear o setup caso o carrinho esteja desligado.
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    if (conectadoAntes) {
      Serial.println("Wi-Fi perdido. Aguardando reconexao...");
      conectadoAntes = false;
    }
    ultimoComando = "";
    if (millis() - ultimaTentativaWiFi >= 5000) {
      ultimaTentativaWiFi = millis();
      WiFi.reconnect();
    }
    delay(50);
    return;
  }

  if (!conectadoAntes) {
    Serial.printf("Controle conectado | IP: %s\n", WiFi.localIP().toString().c_str());
    conectadoAntes = true;
  }

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
