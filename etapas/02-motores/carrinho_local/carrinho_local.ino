// ETAPA 02 - Carrinho local completo: motores + LDR + farois RGB + setas.
// Reconstrucao didatica (novembro/2026), NAO sketch historico recuperado.
// Abra somente este .ino na Arduino IDE (ESP32 Arduino core 3.x).
// COM SEGURANCA: driver de motores, GND comum, rodas suspensas e corte de energia.
// Monitor Serial em 9600 baud: f=frente, r=re, e=esquerda, d=direita,
// p=parar, +=acelerar, -=desacelerar, a=farol automatico, l=ligar, o=desligar.
#include <Arduino.h>

const int MOTOR_D1 = 25, MOTOR_D2 = 26;
const int MOTOR_E1 = 32, MOTOR_E2 = 33;
const int LED_R = 16, LED_G = 17, LED_B = 18;
const int SENSOR_LUZ = 34;
const bool LED_ANODO_COMUM = false;

const int PWM_DIREITO = 240;
const int PWM_ESQUERDO = 243;
const unsigned long TIMEOUT_MS = 900;
const int LIMITE_ESCURO = 300;
const int HISTERESE = 150;

int velocidade = 30;  // 30 a 100% do PWM calibrado
int motorE = 0, motorD = 0; // -1, 0 ou 1
char movimento = 'p';
char farolModo = 'a'; // a=auto, l=ligado, o=desligado
bool escuro = false, pisca = true;
unsigned long ultimoMovimento = 0, ultimoPisca = 0, ultimoLog = 0;

void desligarMotores() {
  for (int p : {MOTOR_D1, MOTOR_D2, MOTOR_E1, MOTOR_E2}) ledcWrite(p, 0);
}

void aplicarMotores(int esquerda, int direita) {
  // Primeiro todos os canais em zero, depois define o novo sentido.
  desligarMotores();
  delayMicroseconds(120);
  const int e = PWM_ESQUERDO * velocidade / 100;
  const int d = PWM_DIREITO * velocidade / 100;
  // Mesma polaridade usada no firmware integrado: GPIO 32 = re, 33 = frente.
  ledcWrite(MOTOR_E1, esquerda < 0 ? e : 0);
  ledcWrite(MOTOR_E2, esquerda > 0 ? e : 0);
  ledcWrite(MOTOR_D1, direita < 0 ? d : 0);
  ledcWrite(MOTOR_D2, direita > 0 ? d : 0);
  motorE = esquerda;
  motorD = direita;
}

void parar() {
  desligarMotores();
  motorE = motorD = 0;
  movimento = 'p';
}

void andar(char comando) {
  if (comando == 'f') aplicarMotores(1, 1);
  else if (comando == 'r') aplicarMotores(-1, -1);
  else if (comando == 'e') aplicarMotores(0, 1);
  else if (comando == 'd') aplicarMotores(1, 0);
  movimento = comando;
  ultimoMovimento = millis();
}

void cor(int vermelho, int verde, int azul) {
  if (LED_ANODO_COMUM) {
    vermelho = 255 - vermelho;
    verde = 255 - verde;
    azul = 255 - azul;
  }
  ledcWrite(LED_R, vermelho);
  ledcWrite(LED_G, verde);
  ledcWrite(LED_B, azul);
}

void atualizarLuzes() {
  const int leitura = analogRead(SENSOR_LUZ);
  if (leitura < LIMITE_ESCURO) escuro = true;
  else if (leitura > LIMITE_ESCURO + HISTERESE) escuro = false;

  if (millis() - ultimoPisca >= 400) {
    ultimoPisca = millis();
    pisca = !pisca;
  }

  // Os dois LEDs RGB compartilham os pinos: nao ha seta lateral independente.
  if (movimento == 'e') cor(pisca ? 255 : 0, pisca ? 110 : 0, 0);
  else if (movimento == 'd') cor(0, 0, pisca ? 255 : 0);
  else if (farolModo == 'l' || (farolModo == 'a' && escuro)) cor(255, 255, 255);
  else cor(0, 0, 0);

  if (millis() - ultimoLog >= 1500) {
    ultimoLog = millis();
    Serial.printf("LDR=%d | luz=%s | movimento=%c | velocidade=%d%%\n",
                  leitura, escuro ? "escuro" : "claro", movimento, velocidade);
  }
}

void lerSerial() {
  while (Serial.available()) {
    const char c = Serial.read();
    if (c == '\n' || c == '\r' || c == ' ') continue;
    if (c == 'f' || c == 'r' || c == 'e' || c == 'd') andar(c);
    else if (c == 'p') parar();
    else if (c == '+' || c == '-') {
      velocidade = constrain(velocidade + (c == '+' ? 10 : -10), 30, 100);
      // Uma regulagem NAO renova o timeout de movimento.
      aplicarMotores(motorE, motorD);
      Serial.printf("Velocidade: %d%%\n", velocidade);
    }
    else if (c == 'a' || c == 'l' || c == 'o') {
      farolModo = c;
      Serial.printf("Modo do farol: %c\n", farolModo);
    }
    else Serial.printf("Comando desconhecido: %c\n", c);
  }
}

void setup() {
  Serial.begin(9600);
  for (int p : {MOTOR_D1, MOTOR_D2, MOTOR_E1, MOTOR_E2}) {
    pinMode(p, OUTPUT);
    digitalWrite(p, LOW);
    if (!ledcAttach(p, 5000, 8)) {
      Serial.printf("ERRO: canal PWM motor GPIO %d\n", p);
      while (true) delay(1000);
    }
    ledcWrite(p, 0);
  }
  for (int p : {LED_R, LED_G, LED_B}) {
    if (!ledcAttach(p, 5000, 8)) {
      parar();
      Serial.printf("ERRO: canal LED GPIO %d\n", p);
      while (true) delay(1000);
    }
  }
  pinMode(SENSOR_LUZ, INPUT);
  parar();
  cor(0, 0, 0);
  Serial.println("ETAPA 02: f/r/e/d/p | +/- velocidade | a/l/o farol");
}

void loop() {
  lerSerial();
  if (movimento != 'p' && millis() - ultimoMovimento > TIMEOUT_MS) {
    parar();
    Serial.println("PARADA: comando local expirou.");
  }
  atualizarLuzes();
}
