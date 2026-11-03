# Etapa 04 — Controle web completo do carrinho

## Todos os códigos dentro desta etapa

| O que gravar/abrir | Arquivo | Função |
| --- | --- | --- |
| **ESP32 do carrinho** | [carrinho_web/carrinho_web.ino](carrinho_web/carrinho_web.ino) | Motores, LDR, RGB, Wi-Fi, servidor, ajustes e proteção por timeout |
| **Interface servida pelo carrinho** | [carrinho_web/pagina.h](carrinho_web/pagina.h) | HTML/CSS/JavaScript **embarcados** no ESP32 |
| **Página original legível** | [carrinho_web/pagina.html](carrinho_web/pagina.html) | Mesma interface, para consulta e demonstração |
| **Senha de exemplo** | [carrinho_web/secrets.h.example](carrinho_web/secrets.h.example) | Copiar para `carrinho_web/secrets.h` e configurar |
| **ESP32 do controle físico (opcional)** | [controle_fisico/controle_fisico.ino](controle_fisico/controle_fisico.ino) | Quatro botões com HTTP, independente do firmware da raiz |

**A etapa é autossuficiente:** para o modo **web**, só precisa dos arquivos na pasta `carrinho_web`; para demonstrar também o **controle físico**, grave separadamente `controle_fisico.ino` no segundo ESP32. O código do controle é uma cópia identificada do integrado existente, com os mesmos quatro botões; a senha fica como placeholder local no sketch, **não publique senha real**.

## Interface completa

Mantém o layout original mostrado pela dupla: fundo escuro, joystick azul circular, faróis `Auto / Ligar / Desligar`, sensibilidade do LDR, barra de velocidade e botão vermelho **PARAR**. O código do ESP32 implementa `/joystick`, `/config`, `/estado` e `/comando`.

### Como executar o controle web

1. Copie `carrinho_web/secrets.h.example` para `carrinho_web/secrets.h` e configure uma senha Wi-Fi local (8–63 caracteres).
2. Abra `carrinho_web/carrinho_web.ino` no Arduino IDE com **ESP32 Arduino 3.x** e grave no ESP32 do carrinho, com `pagina.h` e `secrets.h` na mesma pasta.
3. Com **rodas suspensas** e meio físico de desligar, abra Monitor Serial a **9600 baud**.
4. No celular, conecte à rede do ESP32, acesse **http://192.168.4.1/** e use o joystick; ao soltá-lo, o comando vai a zero.
5. Teste faróis, leitura LDR, controle de velocidade e botão PARAR.

Para o controle físico opcional, grave `controle_fisico/controle_fisico.ino` **no outro ESP32**, configurando sua senha local para a mesma rede. Botões GPIO **18/19/21/22**, ligados ao GND.

A página `pagina.html` pode ser aberta separadamente para demonstração visual; **fora do IP local ela não movimenta o carrinho real**. Quando a apresentação exigir movimento *exclusivamente* pela web, use apenas o celular.

## Como explicar ao professor

- **HTML/CSS:** estrutura e visual do joystick e painel.
- **JavaScript:** converte toque em `x/y` e envia via HTTP; recebe o estado do carrinho.
- **ESP32 WebServer:** serve a página e processa comandos.
- **PWM e ponte H:** controlam velocidade e sentido dos dois motores.
- **LDR e RGB:** faróis automáticos e indicação de curvas (mesma cor nos dois módulos).
- **Segurança:** ao ficar aproximadamente 1,1 s sem renovar movimento, o firmware tenta parar.

A arquitetura cria uma rede própria (AP) em **192.168.4.1**, não equivale à reserva de IP no roteador do enunciado. STOP/timeout não substituem um desligamento físico.

## Origem e validação

Os códigos desta pasta são **cópias didáticas identificadas do projeto integrado** e não constituem prova de um arquivo histórico realmente entregue. A compilação automática verifica o código; funcionamento físico depende de testes com as placas, alimentação e ligação reais.
