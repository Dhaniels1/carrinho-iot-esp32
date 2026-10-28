// Exemplo DIDATICO de 28/10/2026 para farol automatico e seta (20/10).
// Nao e copia do sketch historico. Os dois RGB compartilham os mesmos pinos.
const int LDR = 34, R = 16, G = 17, B = 18;
const int LIMIAR = 300;
unsigned long ultimoPisca = 0;
bool pisca = false;
char direcao = 'p';

void cor(int r, int g, int b) {
  ledcWrite(R, r); ledcWrite(G, g); ledcWrite(B, b);
}

void setup() {
  Serial.begin(9600);
  for (int pin : {R, G, B}) {
    if (!ledcAttach(pin, 5000, 8)) {
      Serial.println("Falha no LED PWM.");
      while (true) delay(1000);
    }
  }
  pinMode(LDR, INPUT);
  cor(0, 0, 0);
  Serial.println("Digite e, d ou p (esquerda, direita ou parar).");
}

void loop() {
  if (Serial.available()) {
    char recebido = Serial.read();
    if (recebido == 'e' || recebido == 'd' || recebido == 'p') direcao = recebido;
  }
  if (millis() - ultimoPisca >= 400) {
    ultimoPisca = millis();
    pisca = !pisca;
  }
  if (direcao == 'e') cor(pisca ? 255 : 0, pisca ? 100 : 0, 0);
  else if (direcao == 'd') cor(0, 0, pisca ? 255 : 0);
  else if (analogRead(LDR) < LIMIAR) cor(255, 255, 255);
  else cor(0, 0, 0);
}
