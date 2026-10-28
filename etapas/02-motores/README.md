# Etapa 02 — Motores, faróis e setas

## Código separado por assunto

- [`teste_motores/teste_motores.ino`](teste_motores/teste_motores.ino): controla dois motores com quatro pinos PWM. Pelo Monitor Serial, `f` avança, `r` dá ré, `e` vira à esquerda, `d` vira à direita e `p` para. A velocidade está limitada a 30% e um comando expira em 800 ms.
- [`teste_farois/teste_farois.ino`](teste_farois/teste_farois.ino): mostra LDR, farol automático e sinalização de curvas separadamente. Com `e` ou `d`, os RGB piscam; com `p`, o sensor de luz controla a iluminação.

**Hardware do projeto:** direito nos GPIOs 25/26, esquerdo nos 32/33; sensor de luz no 34; RGB nos 16/17/18. Os dois módulos RGB compartilham os mesmos sinais, por isso **não há seta física independente de cada lado**.

Os exemplos foram **reconstruídos em 28/10/2026** para fins didáticos. Não substituem os códigos realmente utilizados nas entregas anteriores, ainda não recuperados. O firmware completo atual fica em [`firmware/carrinho/carrinho.ino`](../../firmware/carrinho/carrinho.ino).

**Segurança:** não conectar motores diretamente ao ESP32; use o driver instalado, alimentação apropriada, GND comum e rodas suspensas.
