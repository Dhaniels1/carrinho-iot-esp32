# Etapa 02 — Carrinho local: motores, faróis e setas

## Programa completo para demonstrar a etapa

**[carrinho_local/carrinho_local.ino](carrinho_local/carrinho_local.ino)** — um único programa para o ESP32 do carrinho, com os **dois motores**, sinalização de curvas por RGB, farol automático por LDR, controle de velocidade e parada de segurança. **Não é necessário copiar trechos de outros sketches.**

### Ligações no código completo

| Componente | GPIOs |
| --- | --- |
| Motor direito (entradas do driver) | 25 e 26 |
| Motor esquerdo (entradas do driver) | 32 e 33 |
| LED RGB (R, G, B) | 16, 17 e 18 |
| LDR (saída analógica) | 34 |

Os dois módulos RGB compartilham as três linhas: **mudam de cor simultaneamente**, portanto a seta não é individual por lado. Os motores **não** devem ser ligados diretamente aos GPIOs: use o driver e alimentação adequada com GND comum.

### Como executar

1. Abra `carrinho_local/carrinho_local.ino` na Arduino IDE com **ESP32 Arduino 3.x**; grave no **ESP32 do carrinho**.
2. Deixe **rodas suspensas** e tenha como desligar a alimentação.
3. Abra o Monitor Serial em **9600 baud**. Envie **um caractere**:
   - `f`: frente; `r`: ré; `e`: esquerda; `d`: direita; `p`: parar.
   - `+` / `-`: aumentar/diminuir o limite de velocidade entre 30% e 100%.
   - `a`: farol automático por LDR; `l`: forçar ligado; `o`: desligado.
4. Cada comando de movimento expira após **900 ms**: para continuar andando, é preciso reenviá-lo. Isso é intencional neste teste local.

## Testes individuais mantidos

- [teste_motores/teste_motores.ino](teste_motores/teste_motores.ino): apenas motores.
- [teste_farois/teste_farois.ino](teste_farois/teste_farois.ino): apenas sensor e LEDs.

O **carrinho_local.ino** é o exemplo integrado da entrega; os dois códigos pequenos são úteis para diagnosticar cada componente isoladamente.

**Origem:** reconstrução didática em novembro/2026, não cópia comprovada de código de uma entrega antiga. Compilar e testar em bancada antes de apresentar como funcionamento validado.
