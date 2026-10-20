# Carrinho IoT — ESP32

Carrinho acadêmico com controle físico por Wi-Fi e painel web com joystick virtual, ajustes de velocidade, faróis e sensor de luminosidade.

## Estado do projeto

| Recurso | Status |
|---|---|
| Motores e controle de quatro botões | Testados na montagem anterior |
| Motores calibrados: esquerdo 243 e direito 240 | Testados anteriormente |
| Sensor de luz e LEDs RGB com curvas sinalizadas | Testados anteriormente |
| Joystick proporcional e curvas suaves | Implementado em código; testar no hardware |
| PARAR e parada por ausência de comunicação (1,1 segundo) | Implementado; testar |
| Faróis Auto / Ligar / Desligar | Implementado; testar |
| Limiar de luminosidade ajustável entre 100 e 900 | Implementado; testar |
| Velocidade máxima entre 30 e 100% | Implementado; testar |
| Preferências persistidas no ESP32 | Implementado; testar |
| Servomotor para levantar peso | Pendente |

## Arquivos

- firmware/carrinho/carrinho.ino: firmware do servidor HTTP, motores, RGB e LDR.
- firmware/carrinho/pagina.h: painel web embarcado no ESP32.
- firmware/carrinho/secrets.h.example: modelo de credenciais (sem senha real).
- firmware/controle/controle.ino: firmware de quatro botões, compatível com o novo carrinho.
- web/index.html: demonstração estática hospedada no Netlify.
- netlify.toml: publica a pasta web.

## Instruções

1. Copie firmware/carrinho/secrets.h.example para um arquivo chamado secrets.h na mesma pasta e informe a rede Wi-Fi. NÃO envie este arquivo ao GitHub.
2. Grave carrinho.ino com Arduino IDE e um núcleo ESP32 recente que suporte ledcAttach.
3. Abra Monitor Serial a 9600 baud e identifique o IP atualizado do carrinho.
4. Coloque as rodas SUSPENSAS antes dos testes. No celular ou computador ligado ao mesmo Wi-Fi, abra http://IP-DO-CARRINHO/ .
5. Arraste o joystick para frente, ré ou diagonais. Solte para parar e verifique o botão PARAR. Confira LEDs, LDR e calibração física antes de colocar o carrinho no chão.
6. O controle físico mantém a rota /comando?valor=..., mas precisa de comandos periódicos para não disparar a parada de segurança. Confira seu IP no controle físico.

## Joystick

Posição x/y vai de -100 a 100. Eixo y negativo é para a frente, eixo x positivo é para a direita. A mistura de potência nos dois motores permite curvas suaves e giros sobre o eixo. Os valores máximos continuam calibrados em 243 (esquerdo) e 240 (direito). O slider de velocidade define o percentual máximo. Ao soltar o joystick ou perder comunicação por mais de 1,1 segundo, o carrinho para.

## Painel de faróis e sensor

- Auto: LDR decide quando ligar o arco-íris.
- Ligar: LEDs acesos independentemente da leitura de luz.
- Desligar: LEDs apagados; durante as curvas, a sinalização continua tendo prioridade.
- Sensibilidade: valor entre 100 e 900; no circuito atual, um valor de leitura menor indica mais escuro. Ligar abaixo do limite, desligar acima de limite + 150 (histerese).
- As configurações de farol, limite e velocidade são salvas no ESP32 usando Preferences.

## Rotas HTTP

| Rota | Parâmetro | Uso |
|---|---|---|
| / | nenhum | Página de controle local |
| /comando | valor=frente, re, esquerda, direita, parar | Botões físicos |
| /joystick | x=30&y=-60 | Joystick proporcional |
| /config | farol=auto / ligar / desligar | Modo do farol |
| /config | limite=300 | Sensibilidade |
| /config | velocidade=80 | Velocidade máxima |
| /estado | nenhum | JSON de luminosidade, configurações e movimento |

## GitHub e Netlify

O projeto público mostra a interface de demonstração em https://carrinho-iot-esp32.netlify.app/ .
A versão publicada em HTTPS não consegue comandar diretamente um IP privado HTTP do carrinho pela internet. Para controle real, use a página servida pelo ESP32 dentro da mesma rede Wi-Fi. Não exponha o servidor do carrinho à internet: atualmente não há autenticação.

## GPIOs

| Função | GPIO |
|---|---|
| Motor direito | 25 e 26 |
| Motor esquerdo | 32 e 33 |
| Dois RGB compartilhados R/G/B | 16, 17 e 18 |
| Sensor de luz | 34 |
| Controle físico (esquerda, direita, frente, ré) | 18, 19, 21 e 22 |

## Próximas etapas

Testar no hardware o novo joystick e os controles web, validar o mecanismo de parada e depois implementar o servomotor para içar peso.
