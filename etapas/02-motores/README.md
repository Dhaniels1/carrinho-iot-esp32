# Etapa 02 — Testes dos motores

## Objetivo
Testar individualmente os motores esquerdo e direito, conferir sentido de rotação e corrigir problemas de ligação ou acionamento.

## Registro de desenvolvimento
Durante a montagem, houve investigação de um motor que não girava e de acionamentos inesperados em outro motor. As causas e correções devem ser registradas apenas quando confirmadas.

## Código desta etapa
**Sketch de teste original ainda não arquivado.** Não substituímos o teste por um código inventado.

## Referência no firmware atual
- [carrinho.ino](../../firmware/carrinho/carrinho.ino): função `aplicarMotores`, controle PWM com `ledcWrite`, funções `parar`, `definirMovimento` e `movimentoDosBotoes`.
- GPIOs no código atual: direito **25 e 26**; esquerdo **32 e 33**.
- Limites PWM definidos: direito **240**; esquerdo **243**; isso não comprova calibração do teste antigo.

## Antes de testar
Mantenha as rodas suspensas e alimentação acessível para desligamento. Verifique driver, GND comum e ligações; nunca alimente motor diretamente de um GPIO.

## Próximos registros
Adicionar sketch usado nos primeiros testes, esquema real das conexões e resultados de cada motor.
