// Exemplo DIDATICO recriado em 28/10/2026, nao recuperado de entregas anteriores.
// Teste local: f = frente, r = re, e = esquerda, d = direita, p = parar.
// Teste com as rodas suspensas e meio fisico de cortar a energia.
const int DIR_A = 25, DIR_B = 26;  // motor direito
const int ESQ_A = 32, ESQ_B = 33;  // motor esquerdo
const int POT_DIREITO = 240 * 30 / 100; // limite inicial a 30%
const int POT_ESQUERDO = 243 * 30 / 100;
unsigned long ultimoComando = 0;

void pararMotores() {
  ledcWrite(DIR_A, 0);
  ledcWrite(DIR_B, 0);
  ledcWrite(ESQ_A, 0);
  ledcWrite(ESQ_B, 0);
}

void mover(int esq, int dir) {
  pararMotores();
  delayMicroseconds(100);
  ledcWrite(ESQ_A, esq > 0 ? POT_ESQUERDO : 0);
  ledcWrite(ESQ_B, esq < 0 ? POT_ESQUERDO : 0);
  ledcWrite(DIR_A, dir > 0 ? POT_DIREITO : 0);
  ledcWrite(DIR_B, dir < 0 ? POT_DIREITO : 0);
  ultimoComando = millis();
}

void setup() {
  Serial.begin(9600);
  for (int pin : {DIR_A, DIR_B, ESQ_A, ESQ_B}) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    if (!ledcAttach(pin, 5000, 8)) {
      Serial.println("ERRO AO CONFIGURAR PWM. Desligue a energia.");
      while (true) delay(1000);
    }
  }
  pararMotores();
  Serial.println("Envie f/r/e/d/p pelo Monitor Serial.");
}

void loop() {
  if (Serial.available()) {
    const char comando = Serial.read();
    if (comando == 'f') mover(1, 1);
    else if (comando == 'r') mover(-1, -1);
    else if (comando == 'e') mover(0, 1);
    else if (comando == 'd') mover(1, 0);
    else if (comando == 'p') pararMotores();
  }
  // Cada comando dura no maximo 800 ms: teste local, nao controle continuo.
  if (millis() - ultimoComando >= 800) pararMotores();
}
