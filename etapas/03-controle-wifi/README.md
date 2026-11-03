# Etapa 03 — Carrinho e controle físico via Wi-Fi

## Versão completa: dois ESP32, dois programas independentes

| ESP32 | Programa completo | Função |
| --- | --- | --- |
| **Carrinho** | [carrinho_wifi/carrinho_wifi.ino](carrinho_wifi/carrinho_wifi.ino) | Comanda os **dois motores**, LDR, RGB, Wi-Fi AP, servidor HTTP, velocidade e parada por timeout |
| **Controle físico** | [controle_wifi/controle_wifi.ino](controle_wifi/controle_wifi.ino) | Lê os **quatro botões**, conecta ao Wi-Fi criado pelo carrinho e **envia comandos HTTP reais** |

Cada sketch compila separadamente e tem seu próprio `secrets.h.example`. Não depende do `firmware/` da raiz.

### Montagem e gravação

1. Com alimentação desligada, confira o **driver de motor**, os dois motores e o GND comum. No ESP32 do carrinho: direito GPIO **25/26**, esquerdo **32/33**, RGB **16/17/18**, LDR **34**.
2. No **outro** ESP32, ligue botões entre GPIO e GND: **esquerda 18**, **direita 19**, **frente 21**, **ré 22**. O código usa `INPUT_PULLUP`.
3. Copie **cada** `secrets.h.example` para `secrets.h` em sua respectiva pasta. Coloque **o mesmo SSID e a mesma senha** nos dois, com senha de 8 a 63 caracteres. Não publique `secrets.h`.
4. Na Arduino IDE com **ESP32 Arduino 3.x**, grave `carrinho_wifi/carrinho_wifi.ino` no **ESP32 do carrinho** e `controle_wifi/controle_wifi.ino` no **ESP32 do controle**, **um de cada vez**.
5. Ligue primeiro o carrinho. Ele cria a rede **Carrinho-IoT** (salvo alteração no `secrets.h`) e mostra **192.168.4.1** no Monitor Serial (9600 baud).
6. Ligue o controle. Após ele conectar, **solte todos os botões** para habilitar o movimento. Teste com **rodas suspensas**.

### Operação do controle

- Um botão: frente, ré, esquerda ou direita.
- Soltar: parar.
- **Frente + ré ao mesmo tempo**: aumentar velocidade em 10%.
- **Esquerda + direita ao mesmo tempo**: diminuir velocidade em 10%.

O ajuste de velocidade sempre **para o carrinho primeiro**, por segurança. A velocidade parte de 30%. O controle renova o comando de movimento a cada **250 ms** e o servidor o interrompe depois de cerca de **1 segundo** sem atualização. É uma proteção por software, **não um desligamento físico de emergência**.

Você também pode conferir o servidor pelo navegador, ligado à rede do carrinho: `http://192.168.4.1/estado`. A rota `/comando?valor=frente` aciona movimento temporário: use com extremo cuidado e rodas suspensas.

## Testes pequenos anteriores, preservados

- [servidor_basico/servidor_basico.ino](servidor_basico/servidor_basico.ino) — demonstra criar Wi-Fi, IP e receber comando, sem mover motores.
- [cliente_basico/cliente_basico.ino](cliente_basico/cliente_basico.ino) — demonstra ler os botões e imprimir no Serial, sem enviar HTTP.

Esses são exemplos isolados para explicar os requisitos iniciais; **o par `carrinho_wifi` + `controle_wifi` demonstra o controle remoto funcionando em código**.

## Limitações

O modo AP (IP **192.168.4.1**) **não é uma reserva DHCP no roteador**. A diferença em relação ao enunciado da disciplina deve ser esclarecida. Os dois LEDs RGB compartilham pinos e não acendem setas de lados independentes.

**Origem:** a dupla de códigos completos foi reconstruída didaticamente em novembro/2026 a partir da pinagem e das funções do projeto; não é prova de uma versão histórica nem de testes físicos bem-sucedidos.
