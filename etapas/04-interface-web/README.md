# Etapa 04 — Carrinho controlado pela interface web original

**Entregas do trabalho:** criar a página até **11/11** e movimentar o carrinho exclusivamente pela página até **18/11**.

## Interface preservada

Restauramos **a mesma interface mostrada no vídeo** da dupla: fundo escuro em degradê, joystick circular azul, controles de faróis `Auto / Ligar / Desligar`, sensibilidade do sensor de luz, barra de velocidade e botão vermelho **PARAR**.

A interface da etapa 04 é **idêntica** à [demonstração web principal](../../web/index.html), sem remover funcionalidades visuais. O firmware desta pasta utiliza as rotas correspondentes `/joystick`, `/config`, `/estado` e `/comando`.

## Código completo da etapa

| Arquivo | Finalidade |
| --- | --- |
| [`carrinho_web/carrinho_web.ino`](carrinho_web/carrinho_web.ino) | Programa independente do ESP32, com Wi-Fi, motores, faróis, sensor e rotas HTTP |
| [`carrinho_web/pagina.html`](carrinho_web/pagina.html) | HTML original com CSS e JavaScript |
| [`carrinho_web/pagina.h`](carrinho_web/pagina.h) | Mesmo HTML armazenado na memória do ESP32 |
| [`carrinho_web/secrets.h.example`](carrinho_web/secrets.h.example) | Exemplo de configuração de senha local |

Para garantir o funcionamento da interface **sem reinventar a lógica de direção**, o sketch da etapa 04 usa uma cópia identificada do [firmware integrado](../../firmware/carrinho/carrinho.ino). Essa duplicação é intencional para que a etapa tenha os códigos em sua própria pasta. Não é uma cópia comprovada de um arquivo histórico de entrega; é uma versão reconstruída em 28/10/2026.

## Como usar

1. Copie `carrinho_web/secrets.h.example` para `carrinho_web/secrets.h` e configure sua própria senha do Wi-Fi (8 a 63 caracteres).
2. Abra **`carrinho_web/carrinho_web.ino`** na Arduino IDE, com o pacote ESP32 3.x. Deixe `pagina.h` e `secrets.h` na mesma pasta.
3. Grave o código na placa e abra o Monitor Serial em **9600 baud**.
4. Conecte o telefone à rede criada pelo ESP32 e acesse **http://192.168.4.1/**.
5. Teste o joystick, a parada e os ajustes com as **rodas suspensas** e alimentação fácil de desligar.

A página [`pagina.html`](carrinho_web/pagina.html) também pode ser aberta no computador em modo de **demonstração**. Fora do IP local, não envia comandos para motores.

## Explicação para a apresentação

- **HTML** organiza joystick, faróis e velocidade.
- **CSS** define o visual do vídeo e torna a página adaptável à tela.
- **JavaScript** lê o toque ou mouse, envia coordenadas `x/y` e recebe o estado do ESP32.
- **`WebServer`** serve a página e recebe comandos em `/joystick` e `/config`.
- **PWM** ajusta a potência dos dois motores, usando a ponte H.
- **LDR** aciona automaticamente os LEDs; o firmware indica curvas por alteração das cores.
- **Timeout** tenta interromper o movimento após aproximadamente **1,1 s** sem atualização dos comandos.

O modo atual cria uma rede própria e usa IP local `192.168.4.1`. Ele **não equivale à reserva DHCP no roteador** exigida no PDF, questão ainda a esclarecer com o professor. A parada por software não substitui uma chave física de desligamento.

## Validação

O GitHub Actions está configurado para verificar a sincronização dos arquivos HTML, a sintaxe JavaScript e a compilação dos sketches. **Ainda não é possível afirmar que a cópia da etapa foi aprovada em teste físico ou compilação recente sem consultar os resultados.**
