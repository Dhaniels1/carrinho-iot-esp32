// Exemplo DIDATICO recriado em 28/10/2026; etapa do cliente (27/10).
// Mostra os comandos dos quatro botoes no Serial. A transmissao real
// de movimentos sera demonstrada na entrega de 04/11.
// ESP32 do controle: botoes ligados ao GND (INPUT_PULLUP).
const int ESQUERDA = 18, DIREITA = 19, FRENTE = 21, RE = 22;
String anterior = "";

void setup() {
  Serial.begin(9600);
  for (int pin : {ESQUERDA, DIREITA, FRENTE, RE}) pinMode(pin, INPUT_PULLUP);
  Serial.println("Controle pronto. Pressione um botao.");
}

void loop() {
  String comando = "parar";
  if (digitalRead(FRENTE) == LOW) comando = "frente";
  else if (digitalRead(RE) == LOW) comando = "re";
  else if (digitalRead(ESQUERDA) == LOW) comando = "esquerda";
  else if (digitalRead(DIREITA) == LOW) comando = "direita";

  if (comando != anterior) {
    Serial.println("Comando a enviar: " + comando);
    anterior = comando;
  }
  delay(50);
}
