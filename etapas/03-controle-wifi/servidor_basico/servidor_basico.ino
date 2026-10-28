// Exemplo DIDATICO recriado em 28/10/2026; etapa de servidor (21/10).
// Nao e o firmware integrado: apenas demonstra IP e recebimento de comandos.
// O modo AP reproduz a rede atual do projeto, mas NAO atende, por si so,
// ao requisito de reserva de IP NO ROTEADOR citado pelo professor.
#include <WiFi.h>
#include <WebServer.h>
#include "secrets.h"

WebServer servidor(80);

void receberComando() {
  if (!servidor.hasArg("valor")) {
    servidor.send(400, "text/plain", "Faltou o parametro valor");
    return;
  }
  const String comando = servidor.arg("valor");
  Serial.println("Recebido: " + comando);
  // Este exemplo NAO aciona motores. A integracao fica no firmware/carrinho.
  servidor.send(200, "text/plain", "Recebido: " + comando);
}

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_AP);
  if (!WiFi.softAP(AP_SSID, AP_PASSWORD)) {
    Serial.println("Erro ao iniciar o Wi-Fi");
    while (true) delay(1000);
  }
  Serial.print("IP do servidor: ");
  Serial.println(WiFi.softAPIP());
  servidor.on("/", HTTP_GET, []() {
    servidor.send(200, "text/plain", "Servidor Wi-Fi de exemplo");
  });
  servidor.on("/comando", HTTP_GET, receberComando);
  servidor.begin();
}

void loop() {
  servidor.handleClient();
}
