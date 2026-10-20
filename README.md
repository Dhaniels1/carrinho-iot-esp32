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
- web/index.html: demonstração estática publicada no GitHub Pages (e, se ativo, no Netlify).
- netlify.toml: publica a pasta web.

## Conectar diretamente ao carrinho — sem roteador, sem internet

O **ESP32 do carrinho é um ponto de acesso Wi-Fi**. Ele cria a rede **Carrinho-IoT** e hospeda a interface do joystick em **http://192.168.4.1/**. Toda a interface (HTML/CSS/JS) continua gravada na memória do ESP32, dentro de `firmware/carrinho/pagina.h`.

1. Copie `firmware/carrinho/secrets.h.example` para `firmware/carrinho/secrets.h` (o arquivo `secrets.h` que você criou na versão anterior precisa ser atualizado). Defina `AP_SSID` e uma senha `AP_PASSWORD` **de 8 a 63 caracteres**, de preferência exclusiva para o carrinho. Não publique senhas no GitHub.
2. Mantenha `carrinho.ino`, `pagina.h` e `secrets.h` na **mesma pasta**. Grave `carrinho.ino` no Arduino IDE com núcleo ESP32 recente (3.x) e abra o Monitor Serial a **9600 baud**.
3. Confirme as mensagens **REDE DO CARRINHO: Carrinho-IoT** e **IP DO CARRINHO: 192.168.4.1**.
4. No celular, desligue temporariamente os dados móveis se ele insistir em trocar de rede. Conecte-se ao Wi-Fi **Carrinho-IoT** e entre em **http://192.168.4.1/** (HTTP, não HTTPS). É normal o celular avisar que a rede está *sem internet*.
5. Teste o joystick e o botão PARAR **com as rodas suspensas**. Comece com velocidade máxima a 30%. Confira depois LEDs, controle manual de farol, leitura de luminosidade e sensibilidade do LDR.
6. O **ESP32 do controle físico**, se usado, precisa ser atualizado separadamente com a mesma senha do AP. O código `firmware/controle/controle.ino` já aponta para SSID `Carrinho-IoT` e IP `192.168.4.1`, mas ainda exige inserir a senha localmente.

A página no **GitHub Pages** permanece uma demonstração pública; para controlar o carrinho real, use a página no IP do ESP32. O modo AP não depende de sinal da faculdade ou do Wi-Fi `iot2`.

### Limites e segurança

- O ESP32 cria Wi-Fi de 2,4 GHz, com alcance limitado. A senha da rede protege o acesso básico ao controle. Não use uma senha publicada no repositório.
- O servidor HTTP é local e **não tem autenticação adicional**; quem entrar na rede do carrinho poderá enviar comandos.
- Se a comunicação deixar de renovar comandos por ~1,1 s, o firmware tenta parar automaticamente. Teste essa proteção antes de colocar o carrinho no chão.
- Ao ativar o AP, o ESP32 do carrinho **não se conecta à internet**; esse é o comportamento intencional.
- O firmware atualizado **ainda precisa ser compilado, gravado e validado no hardware**.

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

## GitHub Pages e Netlify

O projeto público pode ser acessado em https://dhaniels1.github.io/carrinho-iot-esp32/ e a versão Netlify anterior em https://carrinho-iot-esp32.netlify.app/ (quando disponíveis). Estes endereços são para **demonstração**. O controle real acontece em **http://192.168.4.1/** depois de conectar ao Wi-Fi **Carrinho-IoT**. Não exponha o servidor HTTP diretamente à internet.

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
