# Entradas e Saidas Esperadas dos Testes

Este documento descreve as entradas (dados de entrada e acoes realizadas) e as saidas esperadas (resultados e validacoes) para cada caso de teste implementado.

---

## Testes de Quartos (Q-001 a Q-030)

### Q-001: Cadastro de quarto basico

**Entradas:**
- Numero do quarto: 101
- Quantidade de hospedes: 2
- Valor diaria: 150.00
- Status: "desocupado"
- Descricao: "Quarto teste"
- Acao: Salvar quarto no arquivo

**Saidas Esperadas:**
- Quantidade de quartos no arquivo: 1
- Numero do quarto: 101
- Quantidade de hospedes: 2
- Valor diaria: 150.00
- Status: "desocupado"

---

### Q-002: Quarto com numero duplicado

**Entradas:**
- Primeiro quarto: numero 101, 2 hospedes, diaria 150.00
- Segundo quarto: numero 102, 3 hospedes, diaria 200.00
- Acao: Verificar se quarto 101 existe, depois salvar quarto 102

**Saidas Esperadas:**
- Verificacao de existencia do quarto 101: true
- Quantidade total de quartos: 2

---

### Q-003: Capacidade maxima (4 hospedes)

**Entradas:**
- Numero do quarto: 102
- Quantidade de hospedes: 4 (capacidade maxima)
- Valor diaria: 250.00
- Status: "desocupado"

**Saidas Esperadas:**
- Quantidade de quartos: 1
- Quantidade de hospedes do quarto: 4

---

### Q-004: Excedendo capacidade

**Entradas:**
- Numero do quarto: 103
- Quantidade de hospedes: 5 (excede o maximo de 4)
- Valor diaria: 250.00

**Saidas Esperadas:**
- Quantidade de hospedes: 5 (o objeto aceita, validacao na UI)

---

### Q-005: Diaria negativa

**Entradas:**
- Numero do quarto: 104
- Quantidade de hospedes: 2
- Valor diaria: -50.00 (valor negativo)

**Saidas Esperadas:**
- Valor diaria: -50.00 (o objeto aceita, validacao na UI)

---

### Q-007: Listar quartos disponiveis

**Entradas:**
- Quarto 101: 2 hospedes, status "desocupado", diaria 150.00
- Quarto 102: 4 hospedes, status "desocupado", diaria 250.00
- Quarto 103: 1 hospede, status "desocupado", diaria 100.00
- Acao: Buscar quartos disponiveis para 2 hospedes

**Saidas Esperadas:**
- Quantidade de quartos disponiveis: 2
- Quarto 101 presente na lista: true
- Quarto 102 presente na lista: true

---

### Q-008: Listar todos os quartos

**Entradas:**
- Quarto 101: status "desocupado"
- Quarto 102: status "desocupado"
- Quarto 103: status "ocupado"
- Acao: Ler todos os quartos do arquivo

**Saidas Esperadas:**
- Quantidade total de quartos: 3
- Quantidade de quartos ocupados: 1
- Quantidade de quartos desocupados: 2

---

### Q-010: Alterar status de quarto

**Entradas:**
- Quarto 301: status inicial "desocupado"
- Acao: Alterar status para "ocupado"

**Saidas Esperadas:**
- Alteracao bem-sucedida: true
- Status do quarto: "ocupado"

---

### Q-011: Buscar quarto por numero

**Entradas:**
- Quarto 401: descricao "Quarto de luxo"
- Acao: Buscar quarto pelo numero 401

**Saidas Esperadas:**
- Numero do quarto encontrado: 401
- Descricao do quarto: "Quarto de luxo"

---

### Q-012: Quarto com capacidade minima (1 hospede)

**Entradas:**
- Numero do quarto: 501
- Quantidade de hospedes: 1
- Valor diaria: 100.00

**Saidas Esperadas:**
- Quantidade de hospedes: 1

---

### Q-013: Multiplos quartos com diferentes capacidades

**Entradas:**
- 4 quartos: numeros 601-604, capacidades 1-4, diarias 100.00-400.00

**Saidas Esperadas:**
- Quantidade de quartos: 4

---

### Q-014: Quarto com descricao longa

**Entradas:**
- Numero do quarto: 701
- Descricao: "Quarto com vista para o mar, ar condicionado, TV, frigobar e banheiro privativo"

**Saidas Esperadas:**
- Tamanho da descricao > 20 caracteres: true

---

### Q-015: Verificar disponibilidade por periodo - quarto livre

**Entradas:**
- Quarto 801: status "desocupado"
- Periodo: 01/12/2025 a 05/12/2025

**Saidas Esperadas:**
- Disponivel: true

---

### Q-016: Verificar disponibilidade por periodo - conflito de datas

**Entradas:**
- Quarto 901: status "desocupado"
- Estadia existente: 01/12/2025 a 05/12/2025
- Periodo a verificar: 03/12/2025 a 07/12/2025 (conflitante)

**Saidas Esperadas:**
- Disponivel: false

---

### Q-017: Obter quartos disponiveis por periodo

**Entradas:**
- 3 quartos (1001-1003): capacidade 2, status "desocupado"
- Estadia no quarto 1001: 01/12/2025 a 05/12/2025
- Periodo a verificar: 10/12/2025 a 15/12/2025, 2 hospedes

**Saidas Esperadas:**
- Quantidade de quartos disponiveis >= 2: true

---

### Q-018: Quarto com valor zero

**Entradas:**
- Numero do quarto: 1101
- Valor diaria: 0.00

**Saidas Esperadas:**
- Valor diaria: 0.00 (com tolerancia de 0.01)

---

### Q-019: Quarto com valor muito alto

**Entradas:**
- Numero do quarto: 1201
- Valor diaria: 9999.99

**Saidas Esperadas:**
- Valor diaria: 9999.99 (com tolerancia de 0.01)

---

### Q-020: Persistencia de multiplos quartos

**Entradas:**
- 5 quartos: numeros 2001-2005, capacidade 2, diaria 150.00
- Acao: Salvar, limpar memoria, recarregar

**Saidas Esperadas:**
- Quantidade de quartos recarregados: 5

---

### Q-021: Quarto nao existe

**Entradas:**
- Numero do quarto: 9999 (nao cadastrado)
- Acao: Verificar existencia e buscar

**Saidas Esperadas:**
- Quarto existe: false
- Numero do quarto retornado: 0 (quarto padrao)

---

### Q-022: Alterar status de quarto inexistente

**Entradas:**
- Numero do quarto: 8888 (nao existe)
- Acao: Tentar alterar status para "ocupado"

**Saidas Esperadas:**
- Alteracao bem-sucedida: false

---

### Q-023: Quartos com diferentes status

**Entradas:**
- 4 quartos (3000-3003) com status: "desocupado", "ocupado", "manutencao", "reservado"

**Saidas Esperadas:**
- Quantidade de quartos: 4

---

### Q-024: Quarto com numero zero

**Entradas:**
- Numero do quarto: 0
- Quantidade de hospedes: 2

**Saidas Esperadas:**
- Numero do quarto: 0

---

### Q-025: Quarto com numero negativo

**Entradas:**
- Numero do quarto: -1
- Quantidade de hospedes: 2

**Saidas Esperadas:**
- Numero do quarto: -1

---

### Q-026: Quarto com numero muito grande

**Entradas:**
- Numero do quarto: 999999
- Quantidade de hospedes: 2

**Saidas Esperadas:**
- Quarto existe: true

---

### Q-027: Filtrar quartos por capacidade exata

**Entradas:**
- 4 quartos (4001-4004): capacidades 1-4, status "desocupado"
- Acao: Buscar quartos disponiveis para 2 hospedes

**Saidas Esperadas:**
- Existe quarto com capacidade >= 2: true

---

### Q-028: Quarto com descricao vazia

**Entradas:**
- Numero do quarto: 5001
- Descricao: "" (vazia)

**Saidas Esperadas:**
- Descricao: "" (string vazia)

---

### Q-030: Multiplas alteracoes de status

**Entradas:**
- Quarto 7001: status inicial "desocupado"
- Acoes: Alterar para "ocupado", depois "manutencao", depois "desocupado"

**Saidas Esperadas:**
- Status final: "desocupado"

---

## Testes de Clientes (C-001 a C-020)

### C-001: Cadastro de cliente

**Entradas:**
- Codigo: 12345
- Nome: "Joao Silva"
- Endereco: "Rua A (123), Bairro Centro, Cidade Belo Horizonte"
- Telefone: "31999999999"
- Pontos fidelidade: 0
- Acao: Salvar e recarregar

**Saidas Esperadas:**
- Quantidade de clientes: 1
- Codigo do cliente: 12345
- Nome: "Joao Silva"
- Pontos fidelidade: 0

---

### C-002: Busca de cliente

**Entradas:**
- Cliente: codigo 12345, nome "Maria Santos", endereco "Rua B (456)...", telefone "31888888888"
- Acoes: Buscar por codigo 12345, buscar por nome parcial "Maria"

**Saidas Esperadas:**
- Cliente encontrado por codigo: nao nulo
- Nome do cliente encontrado: "Maria Santos"
- Cliente encontrado por nome parcial: true

---

### C-003: Cliente com codigo duplicado

**Entradas:**
- Cliente 1: codigo 12345, nome "Joao"
- Cliente 2: codigo 12345, nome "Maria"
- Acao: Buscar por codigo 12345

**Saidas Esperadas:**
- Cliente encontrado: nao nulo (retorna o primeiro)

---

### C-004: Cliente com nome vazio

**Entradas:**
- Codigo: 11111
- Nome: "" (vazio)

**Saidas Esperadas:**
- Nome: "" (string vazia)

---

### C-005: Cliente com nome muito longo

**Entradas:**
- Codigo: 22222
- Nome: "Joao da Silva Santos Oliveira Pereira de Souza"

**Saidas Esperadas:**
- Tamanho do nome > 20 caracteres: true

---

### C-006: Cliente com telefone em formato diferente

**Entradas:**
- Codigo: 33333
- Telefone: "(31) 99999-9999"

**Saidas Esperadas:**
- Telefone: "(31) 99999-9999"

---

### C-007: Cliente com endereco completo

**Entradas:**
- Codigo: 44444
- Endereco: "Rua das Flores, 123, Apto 101, Bairro Centro, Cidade Belo Horizonte, MG, CEP 30000-000"

**Saidas Esperadas:**
- Tamanho do endereco > 30 caracteres: true

---

### C-008: Multiplos clientes

**Entradas:**
- 10 clientes: codigos 10001-10010, nomes "Cliente 1" a "Cliente 10"

**Saidas Esperadas:**
- Quantidade de clientes: 10

---

### C-009: Buscar cliente inexistente

**Entradas:**
- Codigo: 99999 (nao existe)
- Acoes: Buscar por codigo, validar codigo

**Saidas Esperadas:**
- Cliente encontrado: nulo
- Codigo valido: false

---

### C-010: Pontos de fidelidade - multiplas adicoes

**Entradas:**
- Cliente: codigo 55555, pontos iniciais 0
- Acoes: Adicionar 10 pontos, depois 20, depois 30

**Saidas Esperadas:**
- Pontos finais: 60

---

### C-011: Pontos de fidelidade - valor alto

**Entradas:**
- Cliente: codigo 66666, pontos iniciais 0
- Acao: Adicionar 1000 pontos

**Saidas Esperadas:**
- Pontos: 1000

---

### C-012: Persistencia de cliente

**Entradas:**
- Cliente: codigo 77777, nome "Teste Persistencia", pontos 50
- Acoes: Salvar, limpar memoria, recarregar

**Saidas Esperadas:**
- Quantidade de clientes: 1
- Codigo: 77777
- Pontos: 50

---

### C-013: Cliente com codigo zero

**Entradas:**
- Codigo: 0
- Nome: "Cliente Zero"
- Acao: Validar codigo

**Saidas Esperadas:**
- Codigo valido: true

---

### C-014: Cliente com codigo negativo

**Entradas:**
- Codigo: -1
- Nome: "Cliente Negativo"
- Acao: Buscar por codigo

**Saidas Esperadas:**
- Cliente encontrado: nao nulo

---

### C-015: Cliente com codigo muito grande

**Entradas:**
- Codigo: 999999
- Nome: "Cliente Grande"
- Acao: Validar codigo

**Saidas Esperadas:**
- Codigo valido: true

---

### C-016: Busca parcial de nome - case insensitive

**Entradas:**
- Cliente: codigo 88888, nome "MARIA SILVA"
- Acao: Buscar por "maria" (minusculo)

**Saidas Esperadas:**
- Cliente encontrado: true

---

### C-017: Multiplos clientes com mesmo nome

**Entradas:**
- 3 clientes: codigos 90001-90003, todos com nome "Joao Silva"

**Saidas Esperadas:**
- Quantidade de clientes com nome "Joao Silva": 3

---

### C-018: Cliente sem pontos de fidelidade

**Entradas:**
- Codigo: 11111
- Pontos fidelidade: 0

**Saidas Esperadas:**
- Pontos: 0

---

### C-019: Atualizar pontos de fidelidade diretamente

**Entradas:**
- Cliente: codigo 22222, pontos iniciais 100
- Acao: Definir pontos para 200

**Saidas Esperadas:**
- Pontos: 200

---

### C-020: Persistencia de multiplos clientes

**Entradas:**
- 5 clientes: codigos 50001-50005, nomes "Cliente 1" a "Cliente 5", pontos 10-50
- Acoes: Salvar, limpar memoria, recarregar

**Saidas Esperadas:**
- Quantidade de clientes: 5

---

## Testes de Funcionarios (F-001 a F-015)

### F-001: Cadastro de funcionario

**Entradas:**
- Codigo: 10001
- Nome: "Joao Funcionario"
- Telefone: "31999999999"
- Cargo: "Recepcionista"
- Salario: 2000.00
- Acoes: Salvar e recarregar

**Saidas Esperadas:**
- Quantidade de funcionarios: 1
- Codigo: 10001

---

### F-002: Buscar funcionario por codigo

**Entradas:**
- Funcionario: codigo 10002, nome "Maria Funcionaria"
- Acao: Buscar por codigo 10002

**Saidas Esperadas:**
- Funcionario encontrado: nao nulo
- Nome: "Maria Funcionaria"

---

### F-003: Verificar se funcionario existe

**Entradas:**
- Funcionario: codigo 10003
- Acoes: Verificar existencia de 10003 e 99999

**Saidas Esperadas:**
- Funcionario 10003 existe: true
- Funcionario 99999 existe: false

---

### F-004: Funcionario com salario zero

**Entradas:**
- Codigo: 10004
- Salario: 0.00

**Saidas Esperadas:**
- Salario: 0.00 (com tolerancia de 0.01)

---

### F-005: Funcionario com salario alto

**Entradas:**
- Codigo: 10005
- Salario: 10000.00

**Saidas Esperadas:**
- Salario: 10000.00 (com tolerancia de 0.01)

---

### F-006: Multiplos funcionarios

**Entradas:**
- 5 funcionarios: codigos 20001-20005, nomes "Funcionario 1" a "Funcionario 5"

**Saidas Esperadas:**
- Quantidade de funcionarios: 5

---

### F-007: Funcionario com diferentes cargos

**Entradas:**
- 4 funcionarios: codigos 30000-30003, cargos "Recepcionista", "Gerente", "Camareira", "Seguranca"

**Saidas Esperadas:**
- Quantidade de funcionarios: 4

---

### F-008: Persistencia de funcionario

**Entradas:**
- Funcionario: codigo 40001, nome "Func Persistencia", salario 3000.00
- Acoes: Salvar, limpar memoria, recarregar

**Saidas Esperadas:**
- Quantidade de funcionarios: 1
- Codigo: 40001
- Salario: 3000.00 (com tolerancia de 0.01)

---

### F-009: Funcionario com codigo duplicado

**Entradas:**
- Funcionario 1: codigo 50001
- Funcionario 2: codigo 50001
- Acao: Buscar por codigo 50001

**Saidas Esperadas:**
- Funcionario encontrado: nao nulo (retorna o primeiro)

---

### F-010: Buscar funcionario inexistente

**Entradas:**
- Codigo: 99999 (nao existe)
- Acao: Buscar por codigo

**Saidas Esperadas:**
- Funcionario encontrado: nulo

---

### F-011: Funcionario com nome vazio

**Entradas:**
- Codigo: 60001
- Nome: "" (vazio)

**Saidas Esperadas:**
- Nome: "" (string vazia)

---

### F-012: Funcionario com telefone formatado

**Entradas:**
- Codigo: 70001
- Telefone: "(31) 3333-4444"

**Saidas Esperadas:**
- Telefone: "(31) 3333-4444"

---

### F-013: Persistencia de multiplos funcionarios

**Entradas:**
- 10 funcionarios: codigos 80001-80010, nomes "Func 1" a "Func 10", salarios 2000.00-2900.00
- Acoes: Salvar, limpar memoria, recarregar

**Saidas Esperadas:**
- Quantidade de funcionarios: 10

---

### F-014: Funcionario com cargo longo

**Entradas:**
- Codigo: 90001
- Cargo: "Gerente de Recepcao e Atendimento ao Cliente"

**Saidas Esperadas:**
- Tamanho do cargo > 20 caracteres: true

---

### F-015: Funcionario com salario negativo

**Entradas:**
- Codigo: 100001
- Salario: -1000.00

**Saidas Esperadas:**
- Salario: -1000.00 (com tolerancia de 0.01)

---

## Testes de Estadias (E-001 a E-025)

### E-001: Reserva bem-sucedida

**Entradas:**
- Cliente: codigo 12345, nome "Joao Silva"
- Quarto: numero 101, capacidade 2, diaria 150.00, status "desocupado"
- Estadia: cliente 12345, quarto 101, 2 hospedes, entrada 01/12/2025, saida 03/12/2025, 2 diarias
- Acao: Marcar quarto como ocupado

**Saidas Esperadas:**
- Quantidade de estadias: 1
- Codigo do cliente: 12345
- Numero do quarto: 101
- Quantidade de diarias: 2
- Status do quarto: "ocupado"

---

### E-002: Reserva quarto ocupado

**Entradas:**
- Quarto: numero 101, status "ocupado"
- Estadia existente: quarto 101, entrada 01/12/2025, saida 05/12/2025
- Periodo a verificar: 03/12/2025 a 06/12/2025 (conflitante)

**Saidas Esperadas:**
- Disponivel: false

---

### E-003: Reserva excedendo capacidade

**Entradas:**
- Quarto: numero 101, capacidade 2, status "desocupado"
- Acao: Buscar quartos disponiveis para 3 hospedes

**Saidas Esperadas:**
- Quarto 101 nao esta disponivel para 3 hospedes: true

---

### E-004: Check-out e calculo

**Entradas:**
- Cliente: codigo 12345, nome "Joao Silva", pontos 0
- Quarto: numero 101, diaria 150.00, status "ocupado"
- Estadia: codigo 99999, cliente 12345, quarto 101, 3 diarias
- Acoes: Calcular valor total, adicionar pontos (3 diarias * 10 = 30 pontos), marcar quarto como desocupado

**Saidas Esperadas:**
- Valor total: 450.00 (com tolerancia de 0.01)
- Pontos do cliente: 30
- Alteracao de status: true

---

### E-005: Estadia com multiplas diarias

**Entradas:**
- Estadia: 10 diarias, entrada 01/12/2025, saida 11/12/2025

**Saidas Esperadas:**
- Quantidade de diarias: 10

---

### E-006: Estadia com 1 diaria

**Entradas:**
- Estadia: 1 diaria, entrada 01/12/2025, saida 02/12/2025

**Saidas Esperadas:**
- Quantidade de diarias: 1

---

### E-007: Calculo de diarias - mesmo mes

**Entradas:**
- Data entrada: 01/12/2025
- Data saida: 10/12/2025

**Saidas Esperadas:**
- Quantidade de diarias: 9

---

### E-008: Calculo de diarias - mes diferente

**Entradas:**
- Data entrada: 28/12/2025
- Data saida: 02/01/2026

**Saidas Esperadas:**
- Quantidade de diarias: 5

---

### E-009: Calculo de diarias - ano diferente

**Entradas:**
- Data entrada: 20/12/2025
- Data saida: 05/01/2026

**Saidas Esperadas:**
- Quantidade de diarias: 16

---

### E-010: Estadia com data de entrada igual a saida

**Entradas:**
- Data entrada: 01/12/2025
- Data saida: 01/12/2025

**Saidas Esperadas:**
- Quantidade de diarias: 0

---

### E-011: Multiplas estadias para mesmo cliente

**Entradas:**
- Cliente: codigo 11111
- 3 estadias: cliente 11111, quartos 101-103

**Saidas Esperadas:**
- Quantidade de estadias do cliente: 3

---

### E-012: Estadia com valor total alto

**Entradas:**
- Quarto: diaria 500.00
- Estadia: 30 diarias

**Saidas Esperadas:**
- Valor total: 15000.00 (com tolerancia de 0.01)

---

### E-013: Estadia com quantidade de hospedes variada

**Entradas:**
- 4 estadias: quantidades de hospedes 1-4

**Saidas Esperadas:**
- Quantidade de estadias: 4

---

### E-014: Persistencia de estadia

**Entradas:**
- Estadia: codigo 99999, cliente 12345, quarto 101, 3 diarias
- Acoes: Salvar, limpar memoria, recarregar

**Saidas Esperadas:**
- Quantidade de estadias: 1
- Codigo do cliente: 12345

---

### E-015: Verificar conflito de datas - sobreposicao

**Entradas:**
- Estadia existente: entrada 01/12/2025, saida 05/12/2025
- Nova reserva: entrada 03/12/2025, saida 07/12/2025

**Saidas Esperadas:**
- Ha conflito: true

---

### E-016: Verificar conflito de datas - sem conflito

**Entradas:**
- Estadia existente: entrada 01/12/2025, saida 05/12/2025
- Nova reserva: entrada 06/12/2025, saida 10/12/2025

**Saidas Esperadas:**
- Ha conflito: false

---

### E-017: Estadia com codigo de estadia unico

**Entradas:**
- 5 estadias: codigos 100001-100005

**Saidas Esperadas:**
- Quantidade de estadias: 5

---

### E-018: Calculo de pontos de fidelidade

**Entradas:**
- Cliente: codigo 22222, pontos 0
- Estadia: cliente 22222, 5 diarias
- Acao: Adicionar pontos (5 diarias * 10 = 50 pontos)

**Saidas Esperadas:**
- Pontos do cliente: 50

---

### E-019: Multiplas estadias em quartos diferentes

**Entradas:**
- 5 estadias: quartos 101-105

**Saidas Esperadas:**
- Quantidade de estadias: 5

---

### E-020: Estadia com periodo longo

**Entradas:**
- Data entrada: 01/01/2025
- Data saida: 31/01/2025

**Saidas Esperadas:**
- Quantidade de diarias: 30

---

### E-021: Persistencia de multiplas estadias

**Entradas:**
- 10 estadias: codigos 200001-200010, clientes 30001-30010, quartos 101-110
- Acoes: Salvar, limpar memoria, recarregar

**Saidas Esperadas:**
- Quantidade de estadias: 10

---

### E-022: Verificar disponibilidade - periodo antes

**Entradas:**
- Quarto: numero 8001
- Estadia: quarto 8001, entrada 10/12/2025, saida 15/12/2025
- Periodo a verificar: 01/12/2025 a 05/12/2025

**Saidas Esperadas:**
- Disponivel: true

---

### E-023: Verificar disponibilidade - periodo depois

**Entradas:**
- Quarto: numero 9001
- Estadia: quarto 9001, entrada 01/12/2025, saida 05/12/2025
- Periodo a verificar: 10/12/2025 a 15/12/2025

**Saidas Esperadas:**
- Disponivel: true

---

### E-024: Estadia com zero diarias

**Entradas:**
- Estadia: 0 diarias

**Saidas Esperadas:**
- Quantidade de diarias: 0

---

### E-025: Calculo de valor com desconto de pontos

**Entradas:**
- Quarto: diaria 200.00
- Estadia: 5 diarias
- Desconto: 50.00 (simulado)

**Saidas Esperadas:**
- Valor final: 950.00 (com tolerancia de 0.01)

---

## Testes de Integracao (I-001 a I-014)

### I-001: Fluxo completo

**Entradas:**
- Cliente: codigo 11111, nome "Carlos Oliveira", endereco "Rua C (789)...", telefone "31777777777", pontos 0
- Quarto: numero 201, capacidade 3, diaria 180.00, status "desocupado"
- Estadia: cliente 11111, quarto 201, 2 diarias, 2 hospedes
- Acoes: Marcar quarto como ocupado, calcular valor, adicionar pontos (2 * 10 = 20)

**Saidas Esperadas:**
- Valor final: 360.00 (com tolerancia de 0.01)
- Pontos do cliente: 20

---

### I-002: Multiplas reservas

**Entradas:**
- 3 clientes: codigos 10001-10003, nomes "Cliente 1" a "Cliente 3"
- 3 quartos: numeros 101-103, status "desocupado"
- 3 estadias: clientes 10001-10003, quartos 101-103

**Saidas Esperadas:**
- Quantidade de estadias: 3
- Quantidade de clientes: 3

---

### I-003: Historico do cliente

**Entradas:**
- Cliente: codigo 12345, nome "Joao Silva"
- 2 estadias: cliente 12345, quartos 101 e 102, entradas 01/12/2025 e 10/12/2025, saidas 03/12/2025 e 12/12/2025

**Saidas Esperadas:**
- Quantidade de estadias do cliente: 2

---

### I-004: Fluxo completo com multiplos clientes

**Entradas:**
- 3 clientes: codigos 40001-40003, nomes "Cliente 1" a "Cliente 3"
- 3 quartos: numeros 5001-5003, status "desocupado"
- 3 estadias: clientes 40001-40003, quartos 5001-5003

**Saidas Esperadas:**
- Quantidade de clientes: 3
- Quantidade de estadias: 3

---

### I-005: Cliente com multiplas estadias e pontos

**Entradas:**
- Cliente: codigo 50001, pontos 0
- 3 estadias: cliente 50001, 2 diarias cada
- Acoes: Adicionar 20 pontos apos cada estadia

**Saidas Esperadas:**
- Pontos do cliente: 60

---

### I-006: Reserva, ocupacao e liberacao de quarto

**Entradas:**
- Quarto: numero 6001, status "desocupado"
- Acoes: Criar estadia, marcar como ocupado, depois marcar como desocupado

**Saidas Esperadas:**
- Status apos reserva: "ocupado"
- Status apos liberacao: "desocupado"

---

### I-007: Sistema completo - cadastro ate checkout

**Entradas:**
- Cliente: codigo 60001, nome "Cliente Completo"
- Quarto: numero 7001, diaria 200.00, status "desocupado"
- Estadia: cliente 60001, quarto 7001, 3 diarias
- Acoes: Marcar quarto como ocupado, calcular valor, adicionar 30 pontos, marcar quarto como desocupado

**Saidas Esperadas:**
- Valor: 600.00 (com tolerancia de 0.01)
- Pontos do cliente: 30

---

### I-008: Multiplos quartos, multiplos clientes, multiplas reservas

**Entradas:**
- 5 quartos: numeros 8001-8005, status "desocupado"
- 5 clientes: codigos 70001-70005
- 5 estadias: clientes 70001-70005, quartos 8001-8005

**Saidas Esperadas:**
- Quantidade de estadias: 5

---

### I-009: Historico completo de cliente

**Entradas:**
- Cliente: codigo 80001, nome "Cliente Historico"
- 5 estadias: cliente 80001, quartos 9001-9005, diarias 1-5

**Saidas Esperadas:**
- Quantidade de estadias: 5
- Total de diarias: 15

---

### I-010: Persistencia completa do sistema

**Entradas:**
- Cliente: codigo 90001
- Quarto: numero 10001
- Estadia: cliente 90001, quarto 10001
- Funcionario: codigo 100001
- Acoes: Salvar tudo, limpar memoria, recarregar tudo

**Saidas Esperadas:**
- Quantidade de clientes: 1
- Quantidade de estadias: 1
- Quantidade de funcionarios: 1
- Quantidade de quartos: 1

---

### I-011: Cliente VIP com muitas estadias

**Entradas:**
- Cliente: codigo 100001, pontos 0
- 10 estadias: cliente 100001, 2 diarias cada
- Acoes: Adicionar 20 pontos apos cada estadia

**Saidas Esperadas:**
- Pontos do cliente: 200

---

### I-013: Reserva cancelada - quarto volta a disponivel

**Entradas:**
- Quarto: numero 11001, status "desocupado"
- Acoes: Criar estadia, marcar como ocupado, remover estadia, marcar como desocupado

**Saidas Esperadas:**
- Status final: "desocupado"

---

### I-014: Multiplos checkouts no mesmo dia

**Entradas:**
- 5 clientes: codigos 300001-300005, pontos 0
- 5 estadias: clientes 300001-300005, 2 diarias cada
- Acoes: Adicionar 20 pontos para cada cliente

**Saidas Esperadas:**
- Todos os clientes tem 20 pontos: true

---

## Testes de Sistema (S-001, S-003 a S-015)

### S-001: Persistencia de dados

**Entradas:**
- Quarto: numero 301, capacidade 2, diaria 150.00, status "desocupado"
- Acoes: Salvar, limpar memoria, recarregar

**Saidas Esperadas:**
- Quantidade de quartos recarregados: 1
- Numero do quarto: 301

---

### S-003: Persistencia apos multiplas operacoes

**Entradas:**
- 5 clientes: codigos 400001-400005
- 5 quartos: numeros 40001-40005
- Acoes: Salvar clientes, limpar memoria, recarregar

**Saidas Esperadas:**
- Quantidade de clientes: 5

---

### S-004: Arquivo vazio - carregar sem dados

**Entradas:**
- Nenhum dado salvo
- Acao: Tentar carregar clientes

**Saidas Esperadas:**
- Quantidade de clientes: 0

---

### S-005: Sobrescrever dados existentes

**Entradas:**
- Cliente 1: codigo 500001, nome "Primeiro"
- Acoes: Salvar, limpar, criar cliente 2 (codigo 500002, nome "Segundo"), salvar, recarregar

**Saidas Esperadas:**
- Quantidade de clientes: 1
- Codigo do cliente: 500002

---

### S-009: Integridade de dados apos multiplas operacoes

**Entradas:**
- Cliente: codigo 700001, nome "Teste Integridade", pontos 100
- Quarto: numero 70001, diaria 250.00
- Acoes: Salvar e recarregar 5 vezes

**Saidas Esperadas:**
- Quantidade de clientes: 1
- Codigo: 700001
- Pontos: 100

---

### S-013: Recuperacao apos falha simulada

**Entradas:**
- Cliente: codigo 800001
- Acoes: Salvar, limpar memoria (simular falha), recarregar

**Saidas Esperadas:**
- Quantidade de clientes: 1
- Codigo: 800001

---

### S-014: Consistencia entre memoria e arquivo

**Entradas:**
- Cliente: codigo 900001, nome "Memoria"
- Acoes: Salvar, modificar nome para "Modificado" em memoria, recarregar do arquivo

**Saidas Esperadas:**
- Nome do cliente: "Memoria" (nome original do arquivo)

---

### S-015: Teste completo de sistema - todas as funcionalidades

**Entradas:**
- Funcionario: codigo 1000001
- Cliente: codigo 1000001, pontos 0
- Quarto: numero 100001, diaria 300.00, status "desocupado"
- Estadia: cliente 1000001, quarto 100001, 5 diarias
- Acoes: Marcar quarto como ocupado, calcular valor, adicionar 50 pontos, salvar tudo

**Saidas Esperadas:**
- Quantidade de funcionarios: 1
- Quantidade de clientes: 1
- Quantidade de estadias: 1
- Valor: 1500.00 (com tolerancia de 0.01)
- Pontos do cliente: 50

---

## Testes de Funcoes Auxiliares

### Calcular diarias

**Entradas:**
- Teste 1: entrada 01/12/2025, saida 03/12/2025
- Teste 2: entrada 10/12/2025, saida 12/12/2025
- Teste 3: entrada 01/12/2025, saida 01/12/2025

**Saidas Esperadas:**
- Teste 1: 2 diarias
- Teste 2: 2 diarias
- Teste 3: 0 diarias (mesmo dia)

---

### Validar codigo de cliente

**Entradas:**
- Cliente: codigo 12345
- Acoes: Validar codigo 12345 e codigo 99999

**Saidas Esperadas:**
- Codigo 12345 valido: true
- Codigo 99999 valido: false

---

### Pontos de fidelidade

**Entradas:**
- Cliente: codigo 12345, pontos iniciais 0
- Acoes: Adicionar 30 pontos, depois adicionar 20 pontos

**Saidas Esperadas:**
- Pontos apos primeira adicao: 30
- Pontos apos segunda adicao: 50

---

## Observacoes Gerais

- **Limpeza de arquivos**: Todos os testes limpam os arquivos de teste ao final
- **Inicializacao**: Todos os testes inicializam o ambiente (limpar arquivos e vetores) antes de executar
- **Persistencia**: Testes de persistencia verificam se os dados sao salvos e recarregados corretamente
- **Validacoes**: Alguns testes verificam que o sistema aceita valores invalidos (validacao e feita na UI)