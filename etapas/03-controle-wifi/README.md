# Etapa 03 — Servidor e cliente Wi-Fi

Esta etapa foi separada em **dois códigos pequenos**, pois o professor pede demonstrar o servidor em 21/10 e o cliente em 27/10.

## 21/10 — Servidor no ESP32 do carrinho

[`servidor_basico/servidor_basico.ino`](servidor_basico/servidor_basico.ino)

- Inicia uma rede Wi-Fi local de exemplo (`WiFi.softAP`).
- Mostra seu IP no Monitor Serial.
- Recebe `GET /comando?valor=frente` e imprime o texto recebido.
- **Não aciona motores**, para manter o exemplo focado na rede.

Copie `servidor_basico/secrets.h.example` para `secrets.h` e use sua própria senha.

## 27/10 — Cliente no segundo ESP32

[`cliente_basico/cliente_basico.ino`](cliente_basico/cliente_basico.ino)

- Lê os quatro botões em `INPUT_PULLUP` (esquerda 18, direita 19, frente 21 e ré 22).
- Mostra no Monitor Serial o comando que seria enviado.
- O exemplo deliberadamente **não transmite movimento**, porque o enunciado de 27/10 pede demonstrar os comandos; o deslocamento remoto está previsto para 04/11.

O código integrado que **já envia HTTP pelo Wi-Fi** está em [`firmware/controle/controle.ino`](../../firmware/controle/controle.ino), com o servidor correspondente em [`firmware/carrinho/carrinho.ino`](../../firmware/carrinho/carrinho.ino). Não substituir o programa físico por estes exemplos sem verificar o objetivo do teste.

## Requisito pendente: IP reservado no roteador

O PDF exige reserva de IP no roteador. O modo AP do projeto atual oferece IP próprio `192.168.4.1`, **mas isso não equivale a uma reserva de IP no roteador**. Antes da entrega final, combinar com o professor se o modo AP é permitido ou preparar uma versão usando Wi-Fi do roteador com reserva DHCP.

**Origem:** todos os exemplos desta pasta foram reconstruídos didaticamente em 28/10/2026; não são cópias dos sketches históricos.
