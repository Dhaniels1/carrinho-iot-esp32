# Etapa 04 — Página web

**Entregas no PDF:** desenvolver página web até 11/11 e controlar o carrinho exclusivamente pelo navegador até 18/11.

## Código da etapa

- [`mini_painel.html`](mini_painel.html) — exemplo mínimo de HTML, CSS e JavaScript com botões e eventos. **Simula** comandos sem acessar o ESP32; serve para que a dupla compreenda o funcionamento.
- [`../../web/index.html`](../../web/index.html) — interface integrada, com joystick, controles de velocidade e faróis. A mesma página fica embarcada no [`pagina.h`](../../firmware/carrinho/pagina.h).

O painel usa a rede local do ESP32; o GitHub Pages é somente uma demonstração pública. No carrinho físico, abrir `http://192.168.4.1/` conectado à rede dele. O botão PARAR de software não substitui um corte físico de energia.

**Status:** o HTML integrado existe e foi simplificado em 28/10. Testes de apresentação e controle remoto da versão atual ainda devem ser registrados antes das datas de 11/11 e 18/11. O mecanismo de içamento com servo, previsto para 25/11, não foi implementado.
