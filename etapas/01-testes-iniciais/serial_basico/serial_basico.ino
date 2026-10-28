// Exemplo DIDATICO recriado em 28/10/2026. Nao e o sketch historico da entrega.
// Etapa inicial: verificar o Monitor Serial e a leitura do sensor no GPIO 34.
const int SENSOR_LUZ = 34;

void setup() {
  Serial.begin(9600);
  pinMode(SENSOR_LUZ, INPUT);
  Serial.println("ESP32 pronto para testes.");
}

void loop() {
  Serial.print("LDR: ");
  Serial.println(analogRead(SENSOR_LUZ));
  delay(1000);
}
