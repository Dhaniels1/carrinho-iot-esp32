# Etapa 03 — Controle por Wi-Fi

## Objetivo
Permitir que o carrinho receba comandos sem depender do roteador da escola ou de internet.

## Como funciona no código atual
O ESP32 do carrinho inicia em modo **ponto de acesso** (`WIFI_AP`) e disponibiliza a rede configurada em `secrets.h`. O endereço local definido é **192.168.4.1**. Um servidor HTTP (`WebServer`) recebe os comandos de movimento.

O firmware de controle físico, quando utilizado, se conecta como cliente (`WIFI_STA`) e envia requisições a `/comando?valor=...`.

## Arquivos de código existentes
- [firmware/carrinho/carrinho.ino](../../firmware/carrinho/carrinho.ino) — ponto de acesso, servidor HTTP e interpretação dos comandos.
- [firmware/controle/controle.ino](../../firmware/controle/controle.ino) — quatro botões e envio via `HTTPClient`.
- [Modelo de credenciais](../../firmware/carrinho/secrets.h.example) — exemplo; não publique a senha real.

## Pinos do controle físico no código atual
Esquerda 18, direita 19, frente 21 e ré 22, com `INPUT_PULLUP`.

## Teste recomendado
Com rodas suspensas, conecte o celular à rede do carrinho e abra `http://192.168.4.1/`. Para o controle físico, grave seu ESP32 separadamente e configure a senha localmente. Verifique a parada quando comandos deixam de chegar.

## Estado
Documentação baseada no **código atual do repositório**; a versão isolada usada no primeiro teste de Wi-Fi ainda não foi identificada.
