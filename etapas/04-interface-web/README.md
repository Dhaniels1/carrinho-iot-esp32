# Etapa 04 — Interface web e joystick

## Objetivo
Controlar o carrinho por uma página aberta no navegador, usando joystick virtual e configurações de velocidade e faróis.

## Como funciona no código atual
O arquivo `pagina.h` guarda HTML, CSS e JavaScript na memória do ESP32. A rota `/` serve esse painel ao navegador conectado à rede do carrinho.

O JavaScript converte o movimento do joystick em coordenadas **x/y de -100 a 100** e chama a rota `/joystick`. O ESP32 combina os valores para definir a potência de cada motor. Os controles de velocidade, farol e limite do sensor enviam dados para `/config`; o painel lê `/estado` para atualizar as informações.

## Arquivos de código existentes
- [firmware/carrinho/pagina.h](../../firmware/carrinho/pagina.h) — página web embarcada usada no carrinho.
- [firmware/carrinho/carrinho.ino](../../firmware/carrinho/carrinho.ino) — rotas, motores, LEDs e sensor LDR.
- [web/index.html](../../web/index.html) — demonstração web; não substitui a página local para controlar o carrinho.

## Funcionalidades presentes no código
Joystick, botão PARAR, ajuste de velocidade de 30% a 100%, modos de farol, limiar de luminosidade de 100 a 900 e consulta de estado. O firmware define parada de segurança se ficar mais de **1,1 segundo** sem renovar comandos de movimento.

## Validação
O README principal registra que algumas funcionalidades foram implementadas, mas ainda precisam de testes no hardware. Portanto, **presença no código não significa validação física**. Teste inicialmente com rodas suspensas.

## Arquivo histórico
Uma versão independente e comprovadamente usada nesta etapa ainda pode ser adicionada depois. O que está linkado acima é a versão integrada atual.
