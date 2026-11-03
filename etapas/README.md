# Desenvolvimento do carrinho por etapas — códigos completos

O objetivo desta pasta é mostrar **a evolução do trabalho com programas independentes e executáveis**. Cada etapa tem seu próprio código de demonstração, sem exigir que o professor procure funções espalhadas no firmware principal.

> **Origem:** os exemplos dos primeiros testes e os novos códigos integrados de 02 e 03 são **reconstruções didáticas feitas em 2026**, não arquivos históricos recuperados das entregas. O firmware da etapa 04 é uma cópia identificada do código integrado atual. A presença no GitHub não comprova validação física.

## Comece pela versão completa de cada etapa

| Etapa | O que demonstra | Código completo |
| --- | --- | --- |
| [01 — Testes iniciais](01-testes-iniciais/) | Comunicação Serial e leitura do LDR | [serial_basico.ino](01-testes-iniciais/serial_basico/serial_basico.ino) |
| [02 — Movimento local e faróis](02-motores/) | Dois motores, setas, LDR e RGB com controle pelo Serial | [carrinho_local.ino](02-motores/carrinho_local/carrinho_local.ino) |
| [03 — Controle remoto por Wi-Fi](03-controle-wifi/) | **Dois ESP32 reais**: carrinho com motores/faróis/servidor e controle com botões/HTTP | [Carrinho Wi-Fi](03-controle-wifi/carrinho_wifi/carrinho_wifi.ino) + [Controle Wi-Fi](03-controle-wifi/controle_wifi/controle_wifi.ino) |
| [04 — Interface web](04-interface-web/) | Painel com joystick, motores, LEDs e sensor; controle físico opcional | [Carrinho web](04-interface-web/carrinho_web/carrinho_web.ino) + [Página embarcada](04-interface-web/carrinho_web/pagina.h) + [Controle físico](04-interface-web/controle_fisico/controle_fisico.ino) |
| 05 — Servo-motor | Mecanismo ainda não implementado | Pasta reservada, **sem código** |
| 06 — Projeto final | Integração e apresentação final | Pasta reservada, **sem código** |

Os sketches menores que já existiam (`teste_motores`, `teste_farois`, `servidor_basico`, `cliente_basico`) foram **preservados como testes unitários de bancada**. Eles não substituem os programas integrados que reproduzem o funcionamento de cada etapa.

## Como usar

1. Escolha a etapa e consulte o **README dentro dela** (pinos, materiais, comandos, credenciais e instruções).
2. Grave um **.ino por placa ESP32**, mantendo arquivos `.h` que vierem na mesma pasta.
3. Na etapa 03, são **dois programas simultâneos em dois ESP32 diferentes**; configure o mesmo SSID/senha de `secrets.h` localmente.
4. Na etapa 04, o ESP32 do carrinho já serve todo o HTML. A pasta `controle_fisico` é uma opção para mostrar os quatro botões em outro ESP32; para a entrega exclusiva da página, ele pode ficar desligado.
5. Primeiro teste **com as rodas suspensas**, alimentação com GND comum e botão/chave para cortar energia.

### Código central (em uso como referência)

- [firmware/carrinho/](../firmware/carrinho/) — firmware integrado atual, preservado.
- [firmware/controle/](../firmware/controle/) — firmware do controle físico atual.
- [web/index.html](../web/index.html) — demonstração do painel, sem enviar comando ao carrinho fora da rede do ESP32.

**Não edite o `firmware/` da raiz só para modificar uma etapa passada:** as versões por etapa são cópias didáticas independentes.

## Sequência do enunciado e limitações

| Data do roteiro | Meta | Como o repositório aborda |
| --- | --- | --- |
| 23/09 | Movimento sem controle remoto | Etapa 02 completa + teste isolado de motores |
| 20/10 | Faróis automáticos e setas | Etapa 02 completa + teste isolado de LEDs |
| 21/10 | ESP32 servidor mostrando IP | Etapa 03 (servidor básico e carrinho Wi-Fi completo) |
| 27/10 | ESP32 cliente lendo botões | Etapa 03 (cliente básico e controle Wi-Fi completo) |
| 04/11 | Movimentação remota pelo controle | Par completo da etapa 03 |
| 11/11 | Interface web de controle | Etapa 04 com HTML e ESP32 completo |
| 18/11 | Movimentar apenas pela interface web | Etapa 04 usando o celular e o carrinho |
| 25/11 | Içar objeto com servomotor | Pendente, não implementado |
| 01/12 | Corrida final | Pendente |

**Diferenças importantes:** o modo ponto de acesso **não equivale à reserva de IP no roteador** pedida no enunciado. Os dois RGB compartilham pinos e não podem mostrar setas de lados independentes. STOP via HTTP/timeout não substitui desligamento físico.

A compilação automatizada é uma verificação de código, **não é prova de que o hardware foi montado e testado**. Consultar também o [roteiro de validação](../docs/validacao-hardware.md).

Referência do roteiro: documento de disciplina já descrito no repositório, “Computação em Nuvem e IoT II”, Tarcísio Barroso Marques, 26/08/2026.
