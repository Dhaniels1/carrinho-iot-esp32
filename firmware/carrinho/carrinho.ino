#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include "secrets.h"
#include "pagina.h"

#if !defined(AP_SSID) || !defined(AP_PASSWORD)
#error "Defina AP_SSID e AP_PASSWORD no arquivo secrets.h (veja secrets.h.example)"
#endif

// ================= CONEXOES E CALIBRACAO =================

WebServer servidor(80);
Preferences configuracoes;

const int IN1 = 25, IN2 = 26;  // motor direito
const int IN3 = 32, IN4 = 33;  // motor esquerdo
const int VEL_DIREITA_BASE = 240;
const int VEL_ESQUERDA_BASE = 243;

const int LED_R = 16, LED_G = 17, LED_B = 18;
const bool LED_ANODO_COMUM = false; // true para pino comum no 3V3
const int SENSOR_LUZ = 34;

// ================= PARAMETROS E ESTADOS =================

int velocidadePercentual = 100; // 30 a 100
int limiteEscuro = 300;
const int DIFERENCA_CLARO = 150; // histerese
String modoFarol = "auto";       // auto, ligar, desligar
String movimentoAtual = "parar";

int leituraLuz = 0;
bool farolAutomatico = false;
bool farolLigado = false;
bool setaAcesa = true;
int etapaArcoIris = 0;

int comandoEsquerda = 0; // -100 a 100
int comandoDireita = 0;
unsigned long ultimoMovimento = 0;
unsigned long ultimoPisca = 0;
unsigned long ultimoArcoIris = 0;
unsigned long ultimoSerial = 0;

const unsigned long TIMEOUT_MOVIMENTO = 1100;

// ================= CONTROLE DOS MOTORES =================

// Valores negativos invertem o sentido de rotacao.
// Os valores 243 e 240 continuam sendo as potencias maximas calibradas.
void aplicarMotores(int esquerda, int direita) {
  comandoEsquerda = constrain(esquerda, -100, 100);
  comandoDireita = constrain(direita, -100, 100);

  int pwmE = VEL_ESQUERDA_BASE * velocidadePercentual * abs(comandoEsquerda) / 10000;
  int pwmD = VEL_DIREITA_BASE * velocidadePercentual * abs(comandoDireita) / 10000;

  ledcWrite(IN1, comandoDireita < 0 ? pwmD : 0);
  ledcWrite(IN2, comandoDireita > 0 ? pwmD : 0);
  ledcWrite(IN3, comandoEsquerda < 0 ? pwmE : 0);
  ledcWrite(IN4, comandoEsquerda > 0 ? pwmE : 0);
}

void parar() {
  aplicarMotores(0, 0);
  movimentoAtual = "parar";
}

void definirMovimento(int esq, int dir, const String &movimento) {
  if (movimentoAtual != movimento &&
      (movimento == "esquerda" || movimento == "direita")) {
    ultimoPisca = millis();
    setaAcesa = true;
  }
  aplicarMotores(esq, dir);
  movimentoAtual = movimento;
  ultimoMovimento = millis();
}

void movimentoDosBotoes(const String &comando) {
  if (comando == "frente") definirMovimento(100, 100, "frente");
  else if (comando == "re") definirMovimento(-100, -100, "re");
  else if (comando == "esquerda") definirMovimento(0, 100, "esquerda");
  else if (comando == "direita") definirMovimento(100, 0, "direita");
}

// Joystick x/y: -100 ate 100.
// Para cima y negativo; para direita x positivo.
// Mistura diferencial para curvas suaves.
void movimentoJoystick(int x, int y) {
  x = constrain(x, -100, 100);
  y = constrain(y, -100, 100);
  if (abs(x) < 12) x = 0;
  if (abs(y) < 12) y = 0;

  int frente = -y;
  int esq = constrain(frente + x, -100, 100);
  int dir = constrain(frente - x, -100, 100);

  if (esq == 0 && dir == 0) {
    parar();
    return;
  }
  String movimento = frente < 0 ? "re" : "frente";
  if (x < -25) movimento = "esquerda";
  if (x > 25) movimento = "direita";
  definirMovimento(esq, dir, movimento);
}

// ================= RGB, FAROIS E SETAS =================

void escreverRGB(int r, int g, int b) {
  r = constrain(r, 0, 255); g = constrain(g, 0, 255); b = constrain(b, 0, 255);
  if (LED_ANODO_COMUM) {
    r = 255 - r; g = 255 - g; b = 255 - b;
  }
  ledcWrite(LED_R, r);
  ledcWrite(LED_G, g);
  ledcWrite(LED_B, b);
}

void atualizarSensor() {
  leituraLuz = analogRead(SENSOR_LUZ);
  if (leituraLuz < limiteEscuro) farolAutomatico = true;
  else if (leituraLuz > limiteEscuro + DIFERENCA_CLARO) farolAutomatico = false;

  farolLigado = modoFarol == "ligar" || (modoFarol == "auto" && farolAutomatico);

  if (millis() - ultimoSerial >= 3000) {
    ultimoSerial = millis();
    Serial.printf("Luminosidade: %d | Limite: %d | IP: %s | Movimento: %s | Velocidade: %d%%\n",
      leituraLuz, limiteEscuro, WiFi.softAPIP().toString().c_str(),
      movimentoAtual.c_str(), velocidadePercentual);
  }
}

void atualizarLEDs() {
  unsigned long agora = millis();
  // A sinalizacao de curva tem prioridade sobre os modos do farol.
  if (movimentoAtual == "esquerda" || movimentoAtual == "direita") {
    if (agora - ultimoPisca >= 400) {
      ultimoPisca = agora;
      setaAcesa = !setaAcesa;
    }
    if (!setaAcesa) escreverRGB(0, 0, 0);
    else if (movimentoAtual == "esquerda") escreverRGB(255, 120, 0);
    else escreverRGB(0, 0, 255);
    return;
  }

  if (!farolLigado) {
    escreverRGB(0, 0, 0);
    return;
  }

  if (agora - ultimoArcoIris < 20) return;
  ultimoArcoIris = agora;

  int r = 0, g = 0, b = 0;
  if (etapaArcoIris < 256) {
    r = 255 - etapaArcoIris; g = etapaArcoIris;
  } else if (etapaArcoIris < 512) {
    int t = etapaArcoIris - 256; g = 255 - t; b = t;
  } else {
    int t = etapaArcoIris - 512; b = 255 - t; r = t;
  }
  escreverRGB(r, g, b);
  etapaArcoIris = (etapaArcoIris + 1) % 768;
}

// ================= SERVIDOR HTTP =================

// Compatibilidade com os quatro botoes do controle fisico.
void receberComando() {
  if (!servidor.hasArg("valor")) {
    servidor.send(400, "text/plain", "Faltando valor");
    return;
  }
  String comando = servidor.arg("valor");
  if (comando == "parar") parar();
  else if (comando == "frente" || comando == "re" ||
           comando == "esquerda" || comando == "direita") {
    movimentoDosBotoes(comando);
  } else if (comando == "acelerar" || comando == "desacelerar") {
    velocidadePercentual = constrain(velocidadePercentual +
      (comando == "acelerar" ? 10 : -10), 30, 100);
    configuracoes.putInt("vel", velocidadePercentual);
    aplicarMotores(comandoEsquerda, comandoDireita);
  } else {
    servidor.send(400, "text/plain", "Comando desconhecido");
    return;
  }
  servidor.send(200, "text/plain", "OK: " + comando);
}

void receberJoystick() {
  if (!servidor.hasArg("x") || !servidor.hasArg("y")) {
    servidor.send(400, "text/plain", "Faltam x e y");
    return;
  }
  movimentoJoystick(servidor.arg("x").toInt(), servidor.arg("y").toInt());
  servidor.send(200, "text/plain", "OK");
}

void configurarPainel() {
  bool alterou = false;

  if (servidor.hasArg("farol")) {
    String novo = servidor.arg("farol");
    if (novo != "auto" && novo != "ligar" && novo != "desligar") {
      servidor.send(400, "text/plain", "Modo de farol invalido");
      return;
    }
    modoFarol = novo;
    configuracoes.putString("farol", modoFarol);
    alterou = true;
  }
  if (servidor.hasArg("limite")) {
    int novo = servidor.arg("limite").toInt();
    if (novo < 100 || novo > 900) {
      servidor.send(400, "text/plain", "Limite deve estar entre 100 e 900");
      return;
    }
    limiteEscuro = novo;
    configuracoes.putInt("limite", limiteEscuro);
    alterou = true;
  }
  if (servidor.hasArg("velocidade")) {
    int novo = servidor.arg("velocidade").toInt();
    if (novo < 30 || novo > 100) {
      servidor.send(400, "text/plain", "Velocidade deve estar entre 30 e 100");
      return;
    }
    velocidadePercentual = novo;
    configuracoes.putInt("vel", velocidadePercentual);
    aplicarMotores(comandoEsquerda, comandoDireita);
    alterou = true;
  }
  if (!alterou) {
    servidor.send(400, "text/plain", "Nenhuma configuracao recebida");
    return;
  }
  servidor.send(200, "text/plain", "Configuracao atualizada");
}

void mostrarEstado() {
  String json = "{";
  json += "\"luz\":" + String(leituraLuz) + ",";
  json += "\"limite\":" + String(limiteEscuro) + ",";
  json += "\"velocidade\":" + String(velocidadePercentual) + ",";
  json += "\"farolLigado\":" + String(farolLigado ? "true" : "false") + ",";
  json += "\"modoFarol\":\"" + modoFarol + "\",";
  json += "\"movimento\":\"" + movimentoAtual + "\"}";
  servidor.send(200, "application/json", json);
}

// ================= INICIALIZACAO =================

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

  configuracoes.begin("carrinho", false);
  velocidadePercentual = constrain(configuracoes.getInt("vel", 100), 30, 100);
  limiteEscuro = constrain(configuracoes.getInt("limite", 300), 100, 900);
  modoFarol = configuracoes.getString("farol", "auto");
  if (modoFarol != "auto" && modoFarol != "ligar" && modoFarol != "desligar")
    modoFarol = "auto";

  // O carrinho cria sua propria rede Wi-Fi (sem roteador e sem internet).
  // O SSID e a senha ficam definidos APENAS no arquivo local secrets.h.
  WiFi.mode(WIFI_AP);
  IPAddress ipAP(192, 168, 4, 1);
  IPAddress mascara(255, 255, 255, 0);
  if (!WiFi.softAPConfig(ipAP, ipAP, mascara) ||
      !WiFi.softAP(AP_SSID, AP_PASSWORD)) {
    parar();
    Serial.println("ERRO: nao foi possivel criar a rede Carrinho-IoT");
    while (true) delay(1000);
  }
  Serial.printf("\nREDE DO CARRINHO: %s\n", AP_SSID);
  Serial.printf("IP DO CARRINHO: %s\n", WiFi.softAPIP().toString().c_str());

  servidor.on("/", HTTP_GET, []() {
    servidor.send_P(200, "text/html; charset=utf-8", PAGINA_WEB);
  });
  servidor.on("/comando", HTTP_GET, receberComando);
  servidor.on("/joystick", HTTP_GET, receberJoystick);
  servidor.on("/config", HTTP_GET, configurarPainel);
  servidor.on("/estado", HTTP_GET, mostrarEstado);
  servidor.begin();
  Serial.println("Servidor pronto. Pagina no IP acima.");
}

// ================= LOOP =================

void loop() {
  servidor.handleClient();
  atualizarSensor();
  atualizarLEDs();

  // Se o controle deixar de enviar comandos, o carrinho para.
  if (movimentoAtual != "parar" &&
      millis() - ultimoMovimento > TIMEOUT_MOVIMENTO) {
    parar();
    Serial.println("PARADA DE SEGURANCA: sem comandos recentes");
  }

  // Na modalidade ponto de acesso, WiFi.status() nao indica WL_CONNECTED.
  // Sem cliente conectado, o carrinho nao pode permanecer em movimento.
  if (WiFi.softAPgetStationNum() == 0 && movimentoAtual != "parar") {
    parar();
    Serial.println("PARADA DE SEGURANCA: nenhum cliente no Wi-Fi do carrinho");
  }
}
