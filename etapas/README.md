# Diário de desenvolvimento — Carrinho IoT ESP32

Esta pasta organiza as **etapas do trabalho**. O código atualmente disponível e integrado permanece em [firmware](../firmware/), enquanto o painel web de demonstração está em [web](../web/).

> Importante: estes registros explicam os objetivos e apontam para o código atual. Eles **não são cópias recuperadas dos sketches antigos**. Quando localizarmos os códigos usados em cada teste, eles serão adicionados à respectiva etapa com a configuração de hardware daquele momento.

| Etapa | Tema | Situação da documentação |
| --- | --- | --- |
| [01-testes-iniciais](01-testes-iniciais/) | ESP32 e ligações | Registro inicial; sketch histórico a recuperar |
| [02-motores](02-motores/) | Acionamento e diagnóstico dos motores | Registro inicial; sketch histórico a recuperar |
| [03-controle-wifi](03-controle-wifi/) | Rede criada pelo ESP32 e controle físico | Documentado a partir do firmware atual |
| [04-interface-web](04-interface-web/) | Joystick e painel no navegador | Documentado a partir do firmware atual |
| 05-servo-motor | Mecanismo com servo | Reservada, ainda sem implementação |
| 06-projeto-final | Integração e versão final | Reservada, ainda sem implementação |

## Como adicionar código de uma etapa

1. Salve uma **cópia do sketch que realmente foi utilizado**, preferencialmente em uma subpasta com o mesmo nome do arquivo .ino.
2. Atualize o README da etapa indicando bibliotecas, GPIOs, alimentação e como testar.
3. Anote o que funcionou, o que falhou e o que foi alterado.
4. Não copie o firmware integrado atual chamando-o de teste antigo, a menos que seja de fato a versão daquele teste.
5. Nunca publique senhas, tokens ou outros segredos.

**Segurança:** ao experimentar os motores, mantenha as rodas suspensas, comece em baixa velocidade e tenha um modo de desligar a alimentação.
