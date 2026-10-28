# Desenvolvimento por etapas

Esta pasta contém **códigos pequenos e independentes para estudar cada entrega**, além do registro do que já foi feito. São **reconstruções didáticas criadas em 28/10/2026**, e não arquivos recuperados de entregas antigas. O código usado como referência integrada está em [`firmware/`](../firmware/). Não declarar os exemplos como versões historicamente apresentadas ao professor.

## Sequência do PDF da disciplina

| Data | Requisito do professor | Código / estado no repositório |
| --- | --- | --- |
| 23/09 | Carrinho percorre as direções sem comandos remotos | [Teste de motores](02-motores/teste_motores/teste_motores.ino); montagem anterior relatada como testada |
| 20/10 | Faróis automáticos e setas | [Exemplo pequeno](02-motores/teste_farois/teste_farois.ino); firmware integrado também implementa |
| 21/10 | Servidor no ESP32; mostrar IP no Serial | [Servidor básico](03-controle-wifi/servidor_basico/servidor_basico.ino); firmware integrado implementa |
| 27/10 | Cliente no segundo ESP32; exibir comandos no Serial | [Cliente didático](03-controle-wifi/cliente_basico/cliente_basico.ino); [controle integrado](../firmware/controle/controle.ino) |
| 04/11 | Movimentar remotamente pelo controle físico | Código integrado já existe; revisão de acelerar/desacelerar e validação ainda necessárias |
| 11/11 | Página web de controle | [Código completo da etapa 04](04-interface-web/carrinho_web/carrinho_web.ino), [interface](04-interface-web/carrinho_web/pagina.html); [painel integrado](../web/index.html) |
| 18/11 | Movimentar exclusivamente pela página web | Firmware e painel existentes; validação da entrega a realizar |
| 25/11 | Içar pequeno objeto com servomotor | Não implementado; **não marcar como concluído** |
| 01/12 | Corrida final | Planejada, sem código adicional agora |

## Organização

- **01-testes-iniciais** — Monitor Serial e leitura simples de GPIO.
- **02-motores** — Movimento local e faróis/setas, em sketches separados.
- **03-controle-wifi** — Servidor AP e cliente por botões, cada qual com código próprio.
- **04-interface-web** — Código ESP32 + HTML/JS independentes para a entrega, com joystick visual de quatro direções e botões de velocidade; o painel integrado com joystick proporcional permanece em `web/index.html`.
- **firmware/** na raiz — Implementação integrada existente, preservada por funcionar no carrinho físico.
- **web/** na raiz — Página de demonstração sincronizada com `firmware/carrinho/pagina.h`.

Não copiar o sketch completo em todas as etapas: isso criaria duplicação e tornaria qualquer correção mais difícil. Cada etapa contém **um programa pequeno para explicar o conceito**, enquanto o firmware integrado contém o conjunto completo.

## Diferenças que precisam ser resolvidas antes da avaliação

1. **Reserva de IP no roteador:** o enunciado exige reserva DHCP no roteador. O projeto integrado atualmente usa o próprio ESP32 como AP, com IP 192.168.4.1. **Essas não são a mesma configuração.** A implementação atual não demonstra o requisito do roteador. Planejar mudança de rede com a dupla/professor **antes de alterar um conjunto físico que funciona**.
2. **Acelerar/desacelerar em ambas as formas de controle:** o servidor recebe esses comandos e o painel web ajusta a velocidade; o controle físico integrado atual usa quatro botões de direção e não tem entradas próprias para acelerar/desacelerar.
3. **Servomotor:** falta a implementação e o teste de içamento, previstos para 25/11.
4. **RGB:** os dois módulos RGB usam os mesmos GPIOs, portanto não conseguem sinalizar lados independentes sem outra ligação/estratégia.
5. **Segurança:** STOP via HTTP e timeout não são desligamento físico. Testar primeiro com rodas suspensas.

## Como executar cada exemplo

Abra o `.ino` em sua pasta, copie `secrets.h.example` para `secrets.h` no exemplo de servidor e configure credenciais localmente. Use Arduino-ESP32 3.x. O HTML da etapa 04 pode ser aberto no navegador em modo de demonstração; com o sketch ESP32 da etapa gravado, ele envia comandos reais apenas no IP local do carrinho.

As atividades com hardware devem ser realizadas em bancada com alimentação desligável. **A presença de código não comprova o resultado de teste físico.**

Referência da disciplina: PDF fornecido pela dupla, “Computação em Nuvem e IoT II”, Tarcísio Barroso Marques, 26/08/2026.
