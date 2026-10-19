# Carrinho IoT — ESP32

Carrinho robótico acadêmico controlado por Wi-Fi com dois ESP32. O firmware preserva a calibração dos motores da montagem testada e acrescenta uma página web local para direção e controle PWM.

## Estado das funcionalidades

| Recurso | Situação |
|---|---|
| Direções frente, ré, esquerda e direita | Testado na montagem anterior |
| Controle de quatro botões via HTTP | Testado na montagem anterior |
| Calibração dos motores (direito 240, esquerdo 243) | Testada |
| Sensor LDR no GPIO 34 | Testado |
| Faróis RGB e sinalização de curvas | Testado na montagem anterior |
| Página web para controle direcional | Implementada; testar no hardware |
| Acelerar e desacelerar em passos de 10% | Implementado; testar no hardware |
| Parada automática se perder comandos | Implementada; testar no hardware |
| Servomotor para içar peso | Ainda não implementado |

## Organização

- `firmware/carrinho/carrinho.ino`: servidor HTTP, motores, LDR, RGB, controle PWM e segurança.
- `firmware/carrinho/pagina.h`: interface embutida no ESP32, que envia `/comando?valor=...`.
- `firmware/carrinho/secrets.h.example`: exemplo de configuração de rede, sem credenciais reais.
- `firmware/controle/controle.ino`: ESP32 do controle com botões e comandos periódicos.
- `web/index.html`: **demonstração** para hospedar no Netlify.
- `netlify.toml`: publicação estática da pasta `web/`.

## Testar com segurança

1. Copie `firmware/carrinho/secrets.h.example` para `firmware/carrinho/secrets.h` e preencha com o nome e senha do Wi-Fi. O `secrets.h` está ignorado pelo Git.
2. Em Arduino IDE, selecione ESP32 Dev Module e instale um núcleo ESP32 recente que suporte `ledcAttach`.
3. Grave `firmware/carrinho/carrinho.ino`. Leia o IP real no Monitor Serial em **9600 baud**.
4. No controle, configure o Wi-Fi e confirme o IP em `firmware/controle/controle.ino`. Atualize o controle se mudar o firmware do carrinho: os comandos são renovados para funcionar com a parada automática.
5. Teste com as rodas **suspensas**. No celular conectado à **mesma rede Wi-Fi**, visite `http://IP-DO-CARRINHO/`.
6. Segure uma seta para andar e solte para parar. Teste `+` e `−` para alterar o PWM de 30% a 100%, preservando a proporção entre motores.

## Sobre o Netlify

O Netlify publica `web/` como **demonstração visual**. O carrinho usa IP privado (`192.168.x.x`), que não fica acessível automaticamente na internet. Além disso, navegadores restringem requisições de sites HTTPS a dispositivos HTTP na rede privada. Para controlar **de verdade**, abra a página servida pelo ESP32 na rede local. Não exponha o ESP32 diretamente na internet.

## Ligações do carrinho

| Função | GPIO |
|---|---|
| Motor direito | 25 e 26 |
| Motor esquerdo | 32 e 33 |
| RGB comum aos dois LEDs: R / G / B | 16 / 17 / 18 |
| Sensor de luz | 34 |
| Botões do controle: esquerda / direita / frente / ré | 18 / 19 / 21 / 22 |

## Segurança

Não versionar credenciais do Wi-Fi; não conectar a rede do carrinho diretamente à internet. O servidor local ainda não tem autenticação. A parada por perda de comunicação complementa, mas não substitui, testes e supervisão física.
