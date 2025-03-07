# Sistema-de-Gerenciamento-de-Estacionamento

Sistema de Gerenciamento de Estacionamento
Projeto Final - Marcus Emmanuel Barros e Silva

Introdução
Controlar a ocupação de estacionamentos é fundamental para otimizar espaços e melhorar o gerenciamento. Este projeto utiliza uma Raspberry Pi Pico para monitorar a entrada e saída de veículos, exibindo informações em tempo real em um display LCD 16x2. Embora, neste protótipo, a simulação seja feita por meio de botões, a ideia é que, numa aplicação real, sensores (como o HC-SR04) e um painel de exibição de maior porte sejam empregados para informar o público externo sobre a disponibilidade de vagas.

Objetivos
Objetivo Geral:
Desenvolver um sistema automático que informe em tempo real a quantidade de vagas disponíveis e o status do estacionamento, auxiliando tanto os administradores quanto os usuários na tomada de decisão.

Objetivos Específicos:

Utilizar a Raspberry Pi Pico para processar entradas e controlar periféricos.
Exibir a contagem de veículos e o status do estacionamento (ex.: "Vagas: 3/10", "Vazio!" ou "Cheio!") num painel digital (prototipado com LCD 16x2).
Simular a entrada e saída de veículos através de botões.
Fornecer feedback visual com LEDs:
LED Verde: Pisca ao adicionar um veículo e de forma contínua quando o estacionamento está vazio.
LED Vermelho: Pisca ao remover um veículo e de forma contínua quando o estacionamento está cheio.
Discutir a possibilidade de futura integração com sensores automáticos.
Especificação do Hardware
Raspberry Pi Pico: Processamento central do sistema.
Display LCD 16x2 (via I2C):
SDA: GPIO 26
SCL: GPIO 27
(No protótipo; em uma versão final, será substituído por um painel digital maior)
Botões:
Botão A (GP 18) – Simula a entrada de um veículo.
Botão B (GP 15) – Simula a saída de um veículo.
LEDs:
LED Verde (GP 20) – Indica adição de veículo e pisca quando o estacionamento está vazio.
LED Vermelho (GP 21) – Indica remoção de veículo e pisca quando o estacionamento está cheio.
Capacidade Máxima: 10 veículos.
Especificação do Firmware
O firmware é escrito em C, utilizando o SDK do Raspberry Pi Pico, e possui as seguintes funções:

Inicialização: Configuração de stdio, I2C, LCD, botões e LEDs.
Processamento: No loop principal, o sistema verifica a entrada dos botões para simular a adição ou remoção de veículos, atualizando a contagem e o status exibido.
Feedback Visual:
O LED verde pisca brevemente ao adicionar um veículo e continuamente quando o estacionamento está vazio.
O LED vermelho pisca brevemente ao remover um veículo e continuamente quando o estacionamento está cheio.
Interface de Exibição: Funções para atualizar o display com a contagem de vagas e mensagens de status.
Fluxograma do Software
O fluxo do software embarcado pode ser resumido em:

Inicialização: Configura os periféricos e o display.
Leitura dos Botões: Verifica continuamente se houve a simulação de entrada ou saída de veículos.
Processamento: Atualiza a contagem de veículos e define o status (cheio, vazio ou vagas disponíveis).
Feedback Visual e Atualização do Display: Aciona os LEDs conforme a ação e exibe a nova contagem no LCD.
Loop: Repete o ciclo continuamente.
Execução do Projeto
Desenvolvimento e Metodologia:

Definição do Escopo: Criação de um sistema de gerenciamento de estacionamento para otimizar o uso do espaço.
Seleção dos Componentes: Raspberry Pi Pico, LCD 16x2, botões e LEDs.
Implementação: Desenvolvimento do firmware em C, utilizando o simulador Wokwi para testes e ajustes.
Validação: O protótipo demonstrou que o display exibe corretamente as informações e os botões atualizam a contagem de veículos com feedback visual adequado.
Link do Projeto no Wokwi:
Projeto no Wokwi

Resultados e Conclusão
Resultados:

O painel exibe "Vagas: X/10" e atualiza o status conforme a entrada e saída dos veículos.
Os botões simulam corretamente a adição e remoção de veículos.
O feedback visual por meio dos LEDs indica de forma clara o status do estacionamento (cheio ou vazio).
Conclusão:
Este projeto demonstra a aplicabilidade dos conceitos de sistemas embarcados para o gerenciamento eficiente de estacionamentos. A utilização da Raspberry Pi Pico, juntamente com um LCD, botões e LEDs, resultou em um protótipo funcional que pode ser expandido para uma solução completa com sensores automáticos e um painel de exibição maior, oferecendo uma solução robusta e de baixo custo para o gerenciamento de espaços de estacionamento.

