# Validação do carrinho ESP32 no hardware

Este roteiro é um **plano de teste, não um registro de testes concluídos**. Anote data, placa, driver de motores, fonte/bateria, pinagem e resultado de cada item.

## Antes de ligar

- Deixe **as rodas suspensas** e mantenha um meio de cortar a alimentação imediatamente.
- Confira a polaridade da fonte, a tensão indicada no driver, o GND comum e os fios dos motores. Não conecte motores diretamente ao ESP32.
- Verifique o arquivo local `firmware/carrinho/secrets.h`, sem publicar a senha.
- Grave o sketch do carrinho com o pacote **ESP32 Arduino 3.x**. O controle físico é um **segundo ESP32**, gravado separadamente.
- Anote se havia configurações antigas salvas em Preferences: a velocidade inicial será 30% **somente na ausência de valor salvo**.

## Sequência de teste

| # | Ação | Resultado esperado | Resultado observado |
| --- | --- | --- | --- |
| 1 | Ligar o ESP32 sem tocar no controle | Nenhum motor deve girar ou dar trancos | Pendente |
| 2 | Conectar ao Wi-Fi `Carrinho-IoT` | Rede criada, painel disponível em `http://192.168.4.1/` | Pendente |
| 3 | Com velocidade limitada a 30%, mover o joystick para frente | Os dois motores giram no sentido de avanço | Pendente |
| 4 | Acionar ré e curvas, ainda com rodas suspensas | Movimentos correspondentes sem acionamentos involuntários | Pendente |
| 5 | Soltar o joystick e usar PARAR | Os motores deixam de receber comando de movimento | Pendente |
| 6 | Manter um movimento e interromper o envio de comandos | O firmware deve parar após cerca de 1,1 s sem atualização | Pendente |
| 7 | Testar desligar/religar Wi-Fi do controle físico | O controle deve tentar reconectar sem travar a inicialização | Pendente |
| 8 | Testar farol Auto/Ligar/Desligar e variar luz no LDR | Painel e LEDs respondem conforme as configurações | Pendente |
| 9 | Reiniciar com velocidade previamente configurada | O ESP32 restaura a preferência salva dentro de 30%–100% | Pendente |

**Atenção ao teste 6:** a parada depende do firmware e do driver estarem funcionando; não a trate como parada de emergência física. Não realize testes sem supervisão e acesso à energia.

## Se um motor der trancos ou girar sozinho

1. **Desligue a alimentação imediatamente.** Não continue testes com rodas no chão.
2. Identifique o motor e o momento exato do problema: ao ligar, parado ou depois de comandar.
3. Confira fios soltos, conexões, driver (inclusive entradas de direção) e GND comum; teste alimentação adequada sob carga.
4. Verifique se os pinos do motor são os esperados: direito 25/26, esquerdo 32/33, **no firmware atual**.
5. Compare a resposta do motor com o comando e a saída do Monitor Serial, sem trocar fios energizados.
6. Caso persista, registre fotos da montagem, modelo do driver e comportamento antes de alterar novamente os códigos.

**Limitação importante:** o intervalo de 100 microssegundos na inversão ajuda a evitar sobreposição das entradas, mas não elimina falhas de alimentação, ruído elétrico, conexões ou defeitos no driver.

## Testes automatizados

A verificação do GitHub Actions confere a correspondência entre `web/index.html` e `pagina.h`, a sintaxe JavaScript e a compilação dos dois sketches ESP32. **Uma compilação aprovada não garante funcionamento correto com os componentes reais.**
