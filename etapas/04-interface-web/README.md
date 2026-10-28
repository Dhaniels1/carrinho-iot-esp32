# Etapa 04 — Carrinho controlado por página web

**Entregas do PDF:** página web até **11/11** e deslocamento exclusivamente pelo navegador até **18/11**.

## Código completo desta etapa

Abra a pasta **[`carrinho_web/`](carrinho_web/)**:

| Arquivo | Função |
| --- | --- |
| [`carrinho_web.ino`](carrinho_web/carrinho_web.ino) | ESP32: Wi-Fi, servidor HTTP, ponte H dos dois motores, faróis LDR e sinalização de curvas |
| [`pagina.html`](carrinho_web/pagina.html) | HTML + CSS + JavaScript da interface, legível e testável no navegador |
| [`pagina.h`](carrinho_web/pagina.h) | A **mesma página** gravada na memória do ESP32 |
| [`secrets.h.example`](carrinho_web/secrets.h.example) | Modelo local de SSID e senha Wi-Fi; não publicar a senha |

É uma versão **didática e independente**, criada para estudar a etapa. **Não é cópia comprovada de uma entrega antiga nem substituição automática do firmware que vocês já testaram.** O projeto principal continua em [`firmware/carrinho/`](../../firmware/carrinho/).

## Como gravar no ESP32

1. Na pasta `carrinho_web/`, copie `secrets.h.example` para `secrets.h`.
2. Configure a senha da rede Wi-Fi do carrinho (mínimo de 8 caracteres).
3. Abra **`carrinho_web.ino`** na Arduino IDE (núcleo ESP32 3.x). Mantenha `pagina.h` e `secrets.h` na **mesma pasta**.
4. Grave o programa no ESP32 do carrinho e abra o Monitor Serial a **9600 baud**.
5. Conecte o celular à rede `Carrinho-IoT` e acesse `http://192.168.4.1/`.
6. Com **rodas suspensas** e acesso ao corte de alimentação, teste direção e PARAR, primeiro com velocidade de 30%.
7. Confira farol automático e seta piscante durante curvas; os LEDs RGB compartilham os mesmos pinos.

O HTML usa botões que precisam ficar pressionados para manter a direção, em vez de um joystick proporcional. O ESP32 interrompe o movimento se não receber renovação do comando por **800 ms**; o servidor fornece `GET /comando?valor=frente/re/esquerda/direita/parar/acelerar/desacelerar`.

**Atenção:** o exemplo usa a rede Wi-Fi criada pelo ESP32 e não configura reserva DHCP no roteador — exigência específica do enunciado que precisa ser esclarecida/adequada. PARAR e timeout são funções de software, não desligamento físico.

## Por que há outro HTML na raiz?

A versão mais completa, que vocês já utilizaram, tem joystick proporcional e ajustes de faróis: [`web/index.html`](../../web/index.html), embarcada em [`firmware/carrinho/pagina.h`](../../firmware/carrinho/pagina.h). Ela foi preservada, evitando alterar o sistema físico sem testes.

**Status:** exemplo didático implementado no repositório. Compilação no CI e testes na placa ainda precisam ser confirmados.
