# Carrinho IoT — ESP32

Projeto acadêmico de um carrinho robô com **dois motores DC**, controle por **outro ESP32** e por **página web**. O repositório está organizado por entregas para que os dois integrantes consigam explicar cada parte.

**Disciplina:** Computação em Nuvem e IoT II · Prof. Tarcísio Barroso Marques · enunciado de 26/08/2026.

## Comece por aqui

- **[Código de cada etapa e datas do trabalho](etapas/README.md)** — exemplos pequenos, separados e comentados.
- **[Firmware integrado do carrinho](firmware/carrinho/carrinho.ino)** — motores, Wi-Fi, servidor HTTP, faróis, LDR e página.
- **[Código completo da etapa 04](etapas/04-interface-web/carrinho_web/)** — programa do ESP32 e HTML independentes para dirigir pelo navegador.
- **[Firmware do controle físico](firmware/controle/controle.ino)** — quatro botões e envio via Wi-Fi.
- **[Página de demonstração](web/index.html)** — HTML/CSS/JS; conteúdo sincronizado com [`pagina.h`](firmware/carrinho/pagina.h).
- **[Checklist para testes físicos](docs/validacao-hardware.md)** — teste de motores, Wi-Fi e parada, com resultados ainda pendentes na versão atual.

> Os códigos curtos nas pastas `etapas/` são **reconstruções didáticas de 28/10/2026**, não cópias comprovadas de sketches das entregas antigas. Não alteramos o registro histórico para fingir testes já realizados.

## Estado do trabalho em 28/10/2026

| Funcionalidade obrigatória | Situação |
| --- | --- |
| Motores nas direções básicas | Testados na montagem anterior; firmware atual presente |
| Faróis automáticos quando escurece | Implementados; validar novamente na versão atual |
| Setas ao virar | Implementadas; dois RGB compartilham os mesmos GPIOs |
| Servidor Wi-Fi com IP no Monitor Serial | Implementado como AP local |
| Cliente no segundo ESP32 com comandos no Serial | Implementado; exemplo simples da etapa 27/10 disponível |
| Controle remoto pelo segundo ESP32 | Código integrado implementado; entrega em 04/11 |
| Controle por página web | Exemplo funcional independente com botões e página; versão integrada com joystick preservada; entregas em 11/11 e 18/11 |
| Acelerar e desacelerar nos **dois** tipos de controle | Web e servidor permitem ajuste; faltam comandos dedicados no controle físico |
| Içar peso com servomotor | **Pendente** (25/11) |
| Reserva de IP no roteador | **Pendente ou a esclarecer**: o projeto usa IP próprio de AP |

### O que foi simplificado

- Interface web integrada reorganizada: **direção, velocidade e PARAR** em destaque; faróis e sensibilidade em seção opcional.
- Criado **um exemplo completo e separado na etapa 04**, com sketch ESP32, HTML, cabeçalho da página e arquivo de configuração Wi-Fi.
- Botões de **acelerar/desacelerar** na página, além do controle de velocidade por slider.
- Pequenos códigos independentes por etapa, sem copiar o firmware grande em todas as pastas.
- Documentação separa o **que já funciona fisicamente** do **que existe somente no código**.

Mantivemos o firmware integrado e seus recursos já funcionais. A cópia anterior a esta reorganização permanece na branch **[`backup/antes-da-simplificacao-2026-10-28`](https://github.com/Dhaniels1/carrinho-iot-esp32/tree/backup/antes-da-simplificacao-2026-10-28)**.

## Uso do carrinho físico

1. Copie `firmware/carrinho/secrets.h.example` para `firmware/carrinho/secrets.h` e configure **senha Wi-Fi exclusiva de 8 a 63 caracteres**. `secrets.h` não deve ir para o GitHub.
2. Carregue `firmware/carrinho/carrinho.ino` no ESP32 do carrinho usando Arduino-ESP32 **3.x**.
3. Abra o Monitor Serial a **9600 baud**. O ESP32 cria a rede `Carrinho-IoT` e usa o IP `192.168.4.1`.
4. Conecte o celular à rede do carrinho e abra **http://192.168.4.1/**. É normal a rede não ter internet.
5. Para usar o segundo ESP32, configure localmente a senha no `firmware/controle/controle.ino` e grave-o separadamente.
6. **Teste com rodas suspensas, velocidade baixa e acesso fácil ao corte de energia.**

A página pública em **https://dhaniels1.github.io/carrinho-iot-esp32/** apenas simula comandos. Não controla o carrinho físico através da internet.

## Mapa rápido de conexões do firmware atual

| Item | GPIO |
| --- | --- |
| Motor direito, entradas do driver | 25 e 26 |
| Motor esquerdo, entradas do driver | 32 e 33 |
| Dois RGB compartilhados, R/G/B | 16, 17, 18 |
| LDR | 34 |
| Botões do segundo ESP32: esquerda, direita, frente, ré | 18, 19, 21, 22 |

Driver presumido: **MX1508**, ainda sem identificação visual confirmada. Os motores são controlados pelo driver, **não diretamente pelos GPIOs**. Verifique a tensão, a corrente e o GND comum.

## Comandos do firmware integrado

| Rota local | Parâmetros principais |
| --- | --- |
| `/` | Interface no ESP32 |
| `/comando` | `valor=frente/re/esquerda/direita/parar/acelerar/desacelerar` |
| `/joystick` | `x` e `y` entre -100 e 100 |
| `/config` | `farol`, `limite`, `velocidade` |
| `/estado` | Estado informativo em JSON |

A rede Wi-Fi tem senha, mas **não existe autenticação HTTP adicional**. Quem entrar nessa rede pode enviar comandos. O firmware tenta interromper movimento se não receber atualização por aproximadamente 1,1 segundo, mas o botão PARAR e esse timeout **não substituem uma chave física de desligamento**.

## Testes e próximos passos

O workflow [`verificar.yml`](.github/workflows/verificar.yml) verifica a sincronização HTML/página embarcada, a sintaxe JavaScript e a compilação dos firmwares. O resultado de testes físicos da versão atual deve ser anotado em [`docs/validacao-hardware.md`](docs/validacao-hardware.md).

**Prioridades da próxima entrega:** demonstrar movimento remoto com o segundo ESP32 (04/11); completar acelerar/desacelerar no controle; discutir com o professor a exigência de IP reservado no roteador. A implementação do servomotor vem depois, para 25/11.
