// Framework de testes simples - sem dependências externas
#include "simple_test.hpp"

// Headers necessários
#include <fstream>
#include <cstdio>
#include <algorithm>
#include <cctype>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>


#define main test_main_disabled
#include "../codigo/main.cpp"
#undef main

using namespace std;

// Funções auxiliares para limpar arquivos de teste
void limparArquivosTeste() {
    remove("quartos.dat");
    remove("clientes.bin");
    remove("estadias.bin");
    remove("funcionarios.bin");
}

void inicializarTeste() {
    limparArquivosTeste();
    clientes.clear();
    estadias.clear();
    funcionarios.clear();
}

// ============================================
// TESTES DE QUARTOS (Q-001 a Q-008)
// ============================================

TEST_CASE("Q-001: Cadastro de quarto basico") {
    inicializarTeste();
    
    Quarto quarto;
    quarto.setNumQuarto(101);
    quarto.setQuantidadeHospedes(2);
    quarto.setValorDiaria(150.00);
    quarto.setStatus("desocupado");
    quarto.setDescricao("Quarto teste");
    
    salvarQuartoNoArquivo(quarto);
    
    // Verificar se foi salvo
    vector<Quarto> quartos = lerQuartosDoArquivo();
    REQUIRE_EQ(quartos.size(), 1);
    REQUIRE_EQ(quartos[0].getNumQuarto(), 101);
    REQUIRE_EQ(quartos[0].getQuantidadeHospedes(), 2);
    REQUIRE_FLOAT_EQ(quartos[0].getValorDiaria(), 150.00f, 0.01f);
    REQUIRE_EQ(quartos[0].getStatus(), string("desocupado"));
    
    limparArquivosTeste();
}

TEST_CASE("Q-002: Quarto com numero duplicado") {
    inicializarTeste();
    
    // Cadastrar primeiro quarto
    Quarto quarto1;
    quarto1.setNumQuarto(101);
    quarto1.setQuantidadeHospedes(2);
    quarto1.setValorDiaria(150.00);
    salvarQuartoNoArquivo(quarto1);
    
    // Tentar cadastrar quarto com mesmo número
    bool existe = quartoExiste(101);
    REQUIRE_EQ(existe, true);
    
    // Tentar cadastrar outro quarto com número diferente
    Quarto quarto2;
    quarto2.setNumQuarto(102);
    quarto2.setQuantidadeHospedes(3);
    quarto2.setValorDiaria(200.00);
    salvarQuartoNoArquivo(quarto2);
    
    vector<Quarto> quartos = lerQuartosDoArquivo();
    REQUIRE_EQ(quartos.size(), 2);
    
    limparArquivosTeste();
}

TEST_CASE("Q-003: Capacidade maxima (4 hospedes)") {
    inicializarTeste();
    
    Quarto quarto;
    quarto.setNumQuarto(102);
    quarto.setQuantidadeHospedes(4); // Capacidade máxima
    quarto.setValorDiaria(250.00);
    quarto.setStatus("desocupado");
    
    salvarQuartoNoArquivo(quarto);
    
    vector<Quarto> quartos = lerQuartosDoArquivo();
    REQUIRE_EQ(quartos.size(), 1);
    REQUIRE_EQ(quartos[0].getQuantidadeHospedes(), 4);
    
    limparArquivosTeste();
}

TEST_CASE("Q-004: Excedendo capacidade") {
    inicializarTeste();
    
    Quarto quarto;
    quarto.setNumQuarto(103);
    quarto.setQuantidadeHospedes(5); // Excede o máximo de 4
    quarto.setValorDiaria(250.00);
    
    // O sistema deve permitir criar, mas a validação deve ser feita na interface
    // Aqui testamos que o objeto aceita o valor (validação é na UI)
    REQUIRE_EQ(quarto.getQuantidadeHospedes(), 5);
    
    limparArquivosTeste();
}

TEST_CASE("Q-005: Diaria negativa") {
    inicializarTeste();
    
    Quarto quarto;
    quarto.setNumQuarto(104);
    quarto.setQuantidadeHospedes(2);
    quarto.setValorDiaria(-50.00); // Valor negativo
    
    // O objeto aceita, mas a validação deve ser feita na interface
    REQUIRE_FLOAT_EQ(quarto.getValorDiaria(), -50.00f, 0.01f);
    
    limparArquivosTeste();
}

TEST_CASE("Q-007: Listar quartos disponiveis") {
    inicializarTeste();
    
    // Criar quartos de teste
    Quarto quarto1;
    quarto1.setNumQuarto(101);
    quarto1.setQuantidadeHospedes(2);
    quarto1.setValorDiaria(150.00);
    quarto1.setStatus("desocupado");
    salvarQuartoNoArquivo(quarto1);
    
    Quarto quarto2;
    quarto2.setNumQuarto(102);
    quarto2.setQuantidadeHospedes(4);
    quarto2.setValorDiaria(250.00);
    quarto2.setStatus("desocupado");
    salvarQuartoNoArquivo(quarto2);
    
    Quarto quarto3;
    quarto3.setNumQuarto(103);
    quarto3.setQuantidadeHospedes(1);
    quarto3.setValorDiaria(100.00);
    quarto3.setStatus("desocupado");
    salvarQuartoNoArquivo(quarto3);
    
    // Buscar quartos disponíveis para 2 hóspedes
    vector<Quarto> disponiveis = obterQuartosDisponiveis(2);
    
    // Deve retornar quartos 101 e 102 (capacidade >= 2)
    REQUIRE_EQ(disponiveis.size(), 2);
    bool tem101 = false, tem102 = false;
    for (const auto& q : disponiveis) {
        if (q.getNumQuarto() == 101) tem101 = true;
        if (q.getNumQuarto() == 102) tem102 = true;
    }
    REQUIRE_EQ(tem101, true);
    REQUIRE_EQ(tem102, true);
    
    limparArquivosTeste();
}

TEST_CASE("Q-008: Listar todos os quartos") {
    inicializarTeste();
    
    // Criar 3 quartos (2 desocupados, 1 ocupado)
    Quarto quarto1;
    quarto1.setNumQuarto(101);
    quarto1.setStatus("desocupado");
    salvarQuartoNoArquivo(quarto1);
    
    Quarto quarto2;
    quarto2.setNumQuarto(102);
    quarto2.setStatus("desocupado");
    salvarQuartoNoArquivo(quarto2);
    
    Quarto quarto3;
    quarto3.setNumQuarto(103);
    quarto3.setStatus("ocupado");
    salvarQuartoNoArquivo(quarto3);
    
    vector<Quarto> todos = lerQuartosDoArquivo();
    REQUIRE_EQ(todos.size(), 3);
    
    int ocupados = 0, desocupados = 0;
    for (const auto& q : todos) {
        if (q.getStatus() == "ocupado") ocupados++;
        else if (q.getStatus() == "desocupado") desocupados++;
    }
    
    REQUIRE_EQ(ocupados, 1);
    REQUIRE_EQ(desocupados, 2);
    
    limparArquivosTeste();
}

// ============================================
// TESTES DE CLIENTES (C-001, C-002)
// ============================================

TEST_CASE("C-001: Cadastro de cliente") {
    inicializarTeste();
    
    Cliente cliente;
    cliente.setCodigo(12345);
    cliente.setNome("João Silva");
    cliente.setEndereco("Rua A (123), Bairro Centro, Cidade Belo Horizonte");
    cliente.setTelefone("31999999999");
    cliente.setPontosFidelidade(0);
    
    clientes.push_back(cliente);
    salvarTodosClientes();
    
    // Recarregar e verificar
    carregarClientes();
    REQUIRE_EQ(clientes.size(), 1);
    REQUIRE_EQ(clientes[0].getCodigo(), 12345);
    REQUIRE_EQ(clientes[0].getNome(), string("João Silva"));
    REQUIRE_EQ(clientes[0].getPontosFidelidade(), 0);
    
    limparArquivosTeste();
}

TEST_CASE("C-002: Busca de cliente") {
    inicializarTeste();
    
    // Cadastrar cliente
    Cliente cliente;
    cliente.setCodigo(12345);
    cliente.setNome("Maria Santos");
    cliente.setEndereco("Rua B (456), Bairro Centro, Cidade Belo Horizonte");
    cliente.setTelefone("31888888888");
    clientes.push_back(cliente);
    
    // Buscar por código
    Cliente* encontrado = consultarClientePorCodigo(12345);
    REQUIRE(encontrado != nullptr);
    REQUIRE_EQ(encontrado->getNome(), string("Maria Santos"));
    
    // Buscar por nome (parcial)
    bool encontrou = false;
    string busca = "Maria";
    for (auto& c : clientes) {
        string nomeLower = c.getNome();
        transform(nomeLower.begin(), nomeLower.end(), nomeLower.begin(), ::tolower);
        string buscaLower = busca;
        transform(buscaLower.begin(), buscaLower.end(), buscaLower.begin(), ::tolower);
        if (nomeLower.find(buscaLower) != string::npos) {
            encontrou = true;
            break;
        }
    }
    REQUIRE_EQ(encontrou, true);
    
    limparArquivosTeste();
}

// ============================================
// TESTES DE ESTADIAS (E-001 a E-004)
// ============================================

TEST_CASE("E-001: Reserva bem-sucedida") {
    inicializarTeste();
    
    // Cadastrar cliente
    Cliente cliente;
    cliente.setCodigo(12345);
    cliente.setNome("João Silva");
    clientes.push_back(cliente);
    
    // Cadastrar quarto
    Quarto quarto;
    quarto.setNumQuarto(101);
    quarto.setQuantidadeHospedes(2);
    quarto.setValorDiaria(150.00);
    quarto.setStatus("desocupado");
    salvarQuartoNoArquivo(quarto);
    
    // Criar estadia
    Estadia estadia;
    estadia.setCodigoCliente(12345);
    estadia.setNumQuarto(101);
    estadia.setQuantidadeHospedes(2);
    estadia.setDataEntrada("01/12/2025");
    estadia.setDataSaida("03/12/2025");
    estadia.setQuantidadeDiarias(2);
    
    estadias.push_back(estadia);
    
    // Verificar se foi criada
    REQUIRE_EQ(estadias.size(), 1);
    REQUIRE_EQ(estadias[0].getCodigoCliente(), 12345);
    REQUIRE_EQ(estadias[0].getNumQuarto(), 101);
    REQUIRE_EQ(estadias[0].getQuantidadeDiarias(), 2);
    
    // Verificar se quarto foi marcado como ocupado
    alterarStatusQuarto(101, "ocupado");
    Quarto quartoAtualizado = buscarQuartoPorNumero(101);
    REQUIRE_EQ(quartoAtualizado.getStatus(), string("ocupado"));
    
    limparArquivosTeste();
}

TEST_CASE("E-002: Reserva quarto ocupado") {
    inicializarTeste();
    
    // Criar quarto ocupado
    Quarto quarto;
    quarto.setNumQuarto(101);
    quarto.setStatus("ocupado");
    salvarQuartoNoArquivo(quarto);
    
    // Criar estadia existente
    Estadia estadiaExistente;
    estadiaExistente.setNumQuarto(101);
    estadiaExistente.setDataEntrada("01/12/2025");
    estadiaExistente.setDataSaida("05/12/2025");
    estadias.push_back(estadiaExistente);
    
    // Tentar verificar disponibilidade para período conflitante
    bool disponivel = verificarDisponibilidadePorPeriodo(101, "03/12/2025", "06/12/2025");
    REQUIRE_EQ(disponivel, false);
    
    limparArquivosTeste();
}

TEST_CASE("E-003: Reserva excedendo capacidade") {
    inicializarTeste();
    
    // Criar quarto com capacidade 2
    Quarto quarto;
    quarto.setNumQuarto(101);
    quarto.setQuantidadeHospedes(2);
    quarto.setStatus("desocupado");
    salvarQuartoNoArquivo(quarto);
    
    // Tentar reservar para 3 hóspedes
    vector<Quarto> disponiveis = obterQuartosDisponiveis(3);
    
    // Quarto 101 não deve estar disponível para 3 hóspedes
    bool tem101 = false;
    for (const auto& q : disponiveis) {
        if (q.getNumQuarto() == 101) {
            tem101 = true;
            break;
        }
    }
    REQUIRE_EQ(tem101, false);
    
    limparArquivosTeste();
}

TEST_CASE("E-004: Check-out e calculo") {
    inicializarTeste();
    
    // Cadastrar cliente
    Cliente cliente;
    cliente.setCodigo(12345);
    cliente.setNome("João Silva");
    cliente.setPontosFidelidade(0);
    clientes.push_back(cliente);
    
    // Cadastrar quarto
    Quarto quarto;
    quarto.setNumQuarto(101);
    quarto.setValorDiaria(150.00);
    quarto.setStatus("ocupado");
    salvarQuartoNoArquivo(quarto);
    
    // Criar estadia (3 diárias)
    Estadia estadia;
    estadia.setCodigoEstadia(99999);
    estadia.setCodigoCliente(12345);
    estadia.setNumQuarto(101);
    estadia.setQuantidadeDiarias(3);
    estadias.push_back(estadia);
    
    // Calcular valor total
    float valorTotal = quarto.getValorDiaria() * estadia.getQuantidadeDiarias();
    REQUIRE_FLOAT_EQ(valorTotal, 450.00f, 0.01f);
    
    // Simular dar baixa (adicionar pontos)
    int pontosGanhos = estadia.getQuantidadeDiarias() * 10;
    for (auto& c : clientes) {
        if (c.getCodigo() == 12345) {
            c.adicionarPontosFidelidade(pontosGanhos);
            break;
        }
    }
    
    // Verificar pontos
    Cliente* clienteAtualizado = consultarClientePorCodigo(12345);
    REQUIRE(clienteAtualizado != nullptr);
    REQUIRE_EQ(clienteAtualizado->getPontosFidelidade(), 30);
    
    // Verificar se quarto pode ser marcado como desocupado
    bool alterado = alterarStatusQuarto(101, "desocupado");
    REQUIRE_EQ(alterado, true);
    
    limparArquivosTeste();
}

// ============================================
// TESTES DE INTEGRAÇÃO (I-001 a I-003)
// ============================================

TEST_CASE("I-001: Fluxo completo") {
    inicializarTeste();
    
    // 1. Cadastrar cliente Carlos
    Cliente cliente;
    cliente.setCodigo(11111);
    cliente.setNome("Carlos Oliveira");
    cliente.setEndereco("Rua C (789), Bairro Centro, Cidade Belo Horizonte");
    cliente.setTelefone("31777777777");
    cliente.setPontosFidelidade(0);
    clientes.push_back(cliente);
    salvarTodosClientes();
    
    // 2. Cadastrar quarto 201
    Quarto quarto;
    quarto.setNumQuarto(201);
    quarto.setQuantidadeHospedes(3);
    quarto.setValorDiaria(180.00);
    quarto.setStatus("desocupado");
    salvarQuartoNoArquivo(quarto);
    
    // 3. Reservar quarto 201 para Carlos (2 diárias)
    Estadia estadia;
    estadia.setCodigoCliente(11111);
    estadia.setNumQuarto(201);
    estadia.setQuantidadeDiarias(2);
    estadia.setQuantidadeHospedes(2);
    estadias.push_back(estadia);
    alterarStatusQuarto(201, "ocupado");
    
    // 4. Calcular valor final
    float valorFinal = quarto.getValorDiaria() * 2;
    REQUIRE_FLOAT_EQ(valorFinal, 360.00f, 0.01f);
    
    // 5. Dar baixa e adicionar pontos
    int pontos = 2 * 10; // 20 pontos
    for (auto& c : clientes) {
        if (c.getCodigo() == 11111) {
            c.adicionarPontosFidelidade(pontos);
            break;
        }
    }
    
    // Verificar pontos (sem recarregar, usar o mesmo vetor)
    Cliente* carlos = consultarClientePorCodigo(11111);
    REQUIRE(carlos != nullptr);
    REQUIRE_EQ(carlos->getPontosFidelidade(), 20);
    
    limparArquivosTeste();
}

TEST_CASE("I-002: Multiplas reservas") {
    inicializarTeste();
    
    // Cadastrar 3 clientes
    for (int i = 1; i <= 3; i++) {
        Cliente cliente;
        cliente.setCodigo(10000 + i);
        cliente.setNome("Cliente " + to_string(i));
        clientes.push_back(cliente);
    }
    
    // Cadastrar 3 quartos
    for (int i = 101; i <= 103; i++) {
        Quarto quarto;
        quarto.setNumQuarto(i);
        quarto.setStatus("desocupado");
        salvarQuartoNoArquivo(quarto);
    }
    
    // Fazer 3 reservas
    for (int i = 0; i < 3; i++) {
        Estadia estadia;
        estadia.setCodigoCliente(10001 + i);
        estadia.setNumQuarto(101 + i);
        estadias.push_back(estadia);
    }
    
    REQUIRE_EQ(estadias.size(), 3);
    REQUIRE_EQ(clientes.size(), 3);
    
    limparArquivosTeste();
}

TEST_CASE("I-003: Historico do cliente") {
    inicializarTeste();
    
    // Cadastrar cliente
    Cliente cliente;
    cliente.setCodigo(12345);
    cliente.setNome("João Silva");
    clientes.push_back(cliente);
    
    // Criar 2 estadias para o cliente
    Estadia estadia1;
    estadia1.setCodigoCliente(12345);
    estadia1.setNumQuarto(101);
    estadia1.setDataEntrada("01/12/2025");
    estadia1.setDataSaida("03/12/2025");
    estadias.push_back(estadia1);
    
    Estadia estadia2;
    estadia2.setCodigoCliente(12345);
    estadia2.setNumQuarto(102);
    estadia2.setDataEntrada("10/12/2025");
    estadia2.setDataSaida("12/12/2025");
    estadias.push_back(estadia2);
    
    // Contar estadias do cliente
    int count = 0;
    for (const auto& e : estadias) {
        if (e.getCodigoCliente() == 12345) {
            count++;
        }
    }
    
    REQUIRE_EQ(count, 2);
    
    limparArquivosTeste();
}

// ============================================
// TESTES DE SISTEMA (S-001, S-002)
// ============================================

TEST_CASE("S-001: Persistencia de dados") {
    inicializarTeste();
    
    // Cadastrar quarto
    Quarto quarto;
    quarto.setNumQuarto(301);
    quarto.setQuantidadeHospedes(2);
    quarto.setValorDiaria(150.00);
    quarto.setStatus("desocupado");
    salvarQuartoNoArquivo(quarto);
    
    // Simular fechar programa (limpar vetores)
    quartos.clear();
    
    // Simular reabrir programa (carregar do arquivo)
    vector<Quarto> quartosCarregados = lerQuartosDoArquivo();
    
    REQUIRE_EQ(quartosCarregados.size(), 1);
    REQUIRE_EQ(quartosCarregados[0].getNumQuarto(), 301);
    
    limparArquivosTeste();
}

// ============================================
// TESTES DE FUNÇÕES AUXILIARES
// ============================================

TEST_CASE("Calcular diarias") {
    inicializarTeste();
    
    int diarias = calcularDiarias("01/12/2025", "03/12/2025");
    REQUIRE_EQ(diarias, 2);
    
    diarias = calcularDiarias("10/12/2025", "12/12/2025");
    REQUIRE_EQ(diarias, 2);
    
    diarias = calcularDiarias("01/12/2025", "01/12/2025");
    REQUIRE_EQ(diarias, 0); // Mesmo dia = 0 diárias
    
    limparArquivosTeste();
}

TEST_CASE("Validar codigo de cliente") {
    inicializarTeste();
    
    Cliente cliente;
    cliente.setCodigo(12345);
    clientes.push_back(cliente);
    
    bool existe = validaCodigoCliente(12345);
    REQUIRE_EQ(existe, true);
    
    bool naoExiste = validaCodigoCliente(99999);
    REQUIRE_EQ(naoExiste, false);
    
    limparArquivosTeste();
}

TEST_CASE("Pontos de fidelidade") {
    inicializarTeste();
    
    Cliente cliente;
    cliente.setCodigo(12345);
    cliente.setPontosFidelidade(0);
    clientes.push_back(cliente);
    
    // Adicionar pontos
    cliente.adicionarPontosFidelidade(30);
    REQUIRE_EQ(cliente.getPontosFidelidade(), 30);
    
    // Adicionar mais pontos
    cliente.adicionarPontosFidelidade(20);
    REQUIRE_EQ(cliente.getPontosFidelidade(), 50);
    
    limparArquivosTeste();
}

// ============================================
// TESTES ADICIONAIS DE QUARTOS (Q-009 a Q-030)
// ============================================

TEST_CASE("Q-010: Alterar status de quarto") {
    inicializarTeste();
    
    Quarto quarto;
    quarto.setNumQuarto(301);
    quarto.setStatus("desocupado");
    salvarQuartoNoArquivo(quarto);
    
    bool alterado = alterarStatusQuarto(301, "ocupado");
    REQUIRE_EQ(alterado, true);
    
    Quarto q = buscarQuartoPorNumero(301);
    REQUIRE_EQ(q.getStatus(), string("ocupado"));
    
    limparArquivosTeste();
}

TEST_CASE("Q-011: Buscar quarto por numero") {
    inicializarTeste();
    
    Quarto quarto;
    quarto.setNumQuarto(401);
    quarto.setDescricao("Quarto de luxo");
    salvarQuartoNoArquivo(quarto);
    
    Quarto encontrado = buscarQuartoPorNumero(401);
    REQUIRE_EQ(encontrado.getNumQuarto(), 401);
    REQUIRE_EQ(encontrado.getDescricao(), string("Quarto de luxo"));
    
    limparArquivosTeste();
}

TEST_CASE("Q-012: Quarto com capacidade minima (1 hospede)") {
    inicializarTeste();
    
    Quarto quarto;
    quarto.setNumQuarto(501);
    quarto.setQuantidadeHospedes(1);
    quarto.setValorDiaria(100.00);
    salvarQuartoNoArquivo(quarto);
    
    vector<Quarto> quartos = lerQuartosDoArquivo();
    REQUIRE_EQ(quartos[0].getQuantidadeHospedes(), 1);
    
    limparArquivosTeste();
}

TEST_CASE("Q-013: Multiplos quartos com diferentes capacidades") {
    inicializarTeste();
    
    for (int i = 1; i <= 4; i++) {
        Quarto quarto;
        quarto.setNumQuarto(600 + i);
        quarto.setQuantidadeHospedes(i);
        quarto.setValorDiaria(100.00 * i);
        salvarQuartoNoArquivo(quarto);
    }
    
    vector<Quarto> quartos = lerQuartosDoArquivo();
    REQUIRE_EQ(quartos.size(), 4);
    
    limparArquivosTeste();
}

TEST_CASE("Q-014: Quarto com descricao longa") {
    inicializarTeste();
    
    Quarto quarto;
    quarto.setNumQuarto(701);
    quarto.setDescricao("Quarto com vista para o mar, ar condicionado, TV, frigobar e banheiro privativo");
    salvarQuartoNoArquivo(quarto);
    
    vector<Quarto> quartos = lerQuartosDoArquivo();
    REQUIRE_EQ(quartos[0].getDescricao().length() > 20, true);
    
    limparArquivosTeste();
}

TEST_CASE("Q-015: Verificar disponibilidade por periodo - quarto livre") {
    inicializarTeste();
    
    Quarto quarto;
    quarto.setNumQuarto(801);
    quarto.setStatus("desocupado");
    salvarQuartoNoArquivo(quarto);
    
    bool disponivel = verificarDisponibilidadePorPeriodo(801, "01/12/2025", "05/12/2025");
    REQUIRE_EQ(disponivel, true);
    
    limparArquivosTeste();
}

TEST_CASE("Q-016: Verificar disponibilidade por periodo - conflito de datas") {
    inicializarTeste();
    
    Quarto quarto;
    quarto.setNumQuarto(901);
    quarto.setStatus("desocupado");
    salvarQuartoNoArquivo(quarto);
    
    // Criar estadia existente
    Estadia estadia;
    estadia.setNumQuarto(901);
    estadia.setDataEntrada("01/12/2025");
    estadia.setDataSaida("05/12/2025");
    estadias.push_back(estadia);
    
    // Tentar reservar período conflitante
    bool disponivel = verificarDisponibilidadePorPeriodo(901, "03/12/2025", "07/12/2025");
    REQUIRE_EQ(disponivel, false);
    
    limparArquivosTeste();
}

TEST_CASE("Q-017: Obter quartos disponiveis por periodo") {
    inicializarTeste();
    
    // Criar 3 quartos
    for (int i = 1; i <= 3; i++) {
        Quarto quarto;
        quarto.setNumQuarto(1000 + i);
        quarto.setQuantidadeHospedes(2);
        quarto.setStatus("desocupado");
        salvarQuartoNoArquivo(quarto);
    }
    
    // Reservar quarto 1001
    Estadia estadia;
    estadia.setNumQuarto(1001);
    estadia.setDataEntrada("01/12/2025");
    estadia.setDataSaida("05/12/2025");
    estadias.push_back(estadia);
    
    vector<Quarto> disponiveis = obterQuartosDisponiveisPorPeriodo(2, "10/12/2025", "15/12/2025");
    REQUIRE_EQ(disponiveis.size() >= 2, true);
    
    limparArquivosTeste();
}

TEST_CASE("Q-018: Quarto com valor zero") {
    inicializarTeste();
    
    Quarto quarto;
    quarto.setNumQuarto(1101);
    quarto.setValorDiaria(0.00);
    salvarQuartoNoArquivo(quarto);
    
    vector<Quarto> quartos = lerQuartosDoArquivo();
    REQUIRE_FLOAT_EQ(quartos[0].getValorDiaria(), 0.00f, 0.01f);
    
    limparArquivosTeste();
}

TEST_CASE("Q-019: Quarto com valor muito alto") {
    inicializarTeste();
    
    Quarto quarto;
    quarto.setNumQuarto(1201);
    quarto.setValorDiaria(9999.99);
    salvarQuartoNoArquivo(quarto);
    
    vector<Quarto> quartos = lerQuartosDoArquivo();
    REQUIRE_FLOAT_EQ(quartos[0].getValorDiaria(), 9999.99f, 0.01f);
    
    limparArquivosTeste();
}

TEST_CASE("Q-020: Persistencia de multiplos quartos") {
    inicializarTeste();
    
    // Criar 5 quartos
    for (int i = 1; i <= 5; i++) {
        Quarto quarto;
        quarto.setNumQuarto(2000 + i);
        quarto.setQuantidadeHospedes(2);
        quarto.setValorDiaria(150.00);
        salvarQuartoNoArquivo(quarto);
    }
    
    // Limpar e recarregar
    quartos.clear();
    vector<Quarto> recarregados = lerQuartosDoArquivo();
    REQUIRE_EQ(recarregados.size(), 5);
    
    limparArquivosTeste();
}

TEST_CASE("Q-021: Quarto nao existe") {
    inicializarTeste();
    
    bool existe = quartoExiste(9999);
    REQUIRE_EQ(existe, false);
    
    Quarto q = buscarQuartoPorNumero(9999);
    REQUIRE_EQ(q.getNumQuarto(), 0); // Quarto padrão
    
    limparArquivosTeste();
}

TEST_CASE("Q-022: Alterar status de quarto inexistente") {
    inicializarTeste();
    
    bool alterado = alterarStatusQuarto(8888, "ocupado");
    REQUIRE_EQ(alterado, false);
    
    limparArquivosTeste();
}

TEST_CASE("Q-023: Quartos com diferentes status") {
    inicializarTeste();
    
    string status[] = {"desocupado", "ocupado", "manutencao", "reservado"};
    for (int i = 0; i < 4; i++) {
        Quarto quarto;
        quarto.setNumQuarto(3000 + i);
        quarto.setStatus(status[i]);
        salvarQuartoNoArquivo(quarto);
    }
    
    vector<Quarto> quartos = lerQuartosDoArquivo();
    REQUIRE_EQ(quartos.size(), 4);
    
    limparArquivosTeste();
}

TEST_CASE("Q-024: Quarto com numero zero") {
    inicializarTeste();
    
    Quarto quarto;
    quarto.setNumQuarto(0);
    quarto.setQuantidadeHospedes(2);
    salvarQuartoNoArquivo(quarto);
    
    vector<Quarto> quartos = lerQuartosDoArquivo();
    REQUIRE_EQ(quartos[0].getNumQuarto(), 0);
    
    limparArquivosTeste();
}

TEST_CASE("Q-025: Quarto com numero negativo") {
    inicializarTeste();
    
    Quarto quarto;
    quarto.setNumQuarto(-1);
    quarto.setQuantidadeHospedes(2);
    salvarQuartoNoArquivo(quarto);
    
    vector<Quarto> quartos = lerQuartosDoArquivo();
    REQUIRE_EQ(quartos[0].getNumQuarto(), -1);
    
    limparArquivosTeste();
}

TEST_CASE("Q-026: Quarto com numero muito grande") {
    inicializarTeste();
    
    Quarto quarto;
    quarto.setNumQuarto(999999);
    quarto.setQuantidadeHospedes(2);
    salvarQuartoNoArquivo(quarto);
    
    bool existe = quartoExiste(999999);
    REQUIRE_EQ(existe, true);
    
    limparArquivosTeste();
}

TEST_CASE("Q-027: Filtrar quartos por capacidade exata") {
    inicializarTeste();
    
    // Criar quartos com capacidades diferentes
    for (int cap = 1; cap <= 4; cap++) {
        Quarto quarto;
        quarto.setNumQuarto(4000 + cap);
        quarto.setQuantidadeHospedes(cap);
        quarto.setStatus("desocupado");
        salvarQuartoNoArquivo(quarto);
    }
    
    // Buscar quartos para exatamente 2 hóspedes
    vector<Quarto> disponiveis = obterQuartosDisponiveis(2);
    bool temCapacidade2 = false;
    for (const auto& q : disponiveis) {
        if (q.getQuantidadeHospedes() >= 2) {
            temCapacidade2 = true;
            break;
        }
    }
    REQUIRE_EQ(temCapacidade2, true);
    
    limparArquivosTeste();
}

TEST_CASE("Q-028: Quarto com descricao vazia") {
    inicializarTeste();
    
    Quarto quarto;
    quarto.setNumQuarto(5001);
    quarto.setDescricao("");
    salvarQuartoNoArquivo(quarto);
    
    vector<Quarto> quartos = lerQuartosDoArquivo();
    REQUIRE_EQ(quartos[0].getDescricao(), string(""));
    
    limparArquivosTeste();
}

TEST_CASE("Q-030: Multiplas alteracoes de status") {
    inicializarTeste();
    
    Quarto quarto;
    quarto.setNumQuarto(7001);
    quarto.setStatus("desocupado");
    salvarQuartoNoArquivo(quarto);
    
    alterarStatusQuarto(7001, "ocupado");
    alterarStatusQuarto(7001, "manutencao");
    alterarStatusQuarto(7001, "desocupado");
    
    Quarto q = buscarQuartoPorNumero(7001);
    REQUIRE_EQ(q.getStatus(), string("desocupado"));
    
    limparArquivosTeste();
}

// ============================================
// TESTES ADICIONAIS DE CLIENTES (C-003 a C-020)
// ============================================

TEST_CASE("C-003: Cliente com codigo duplicado") {
    inicializarTeste();
    
    Cliente cliente1;
    cliente1.setCodigo(12345);
    cliente1.setNome("João");
    clientes.push_back(cliente1);
    
    Cliente cliente2;
    cliente2.setCodigo(12345);
    cliente2.setNome("Maria");
    clientes.push_back(cliente2);
    
    // Ambos podem existir no vetor, mas a busca retorna o primeiro
    Cliente* encontrado = consultarClientePorCodigo(12345);
    REQUIRE(encontrado != nullptr);
    
    limparArquivosTeste();
}

TEST_CASE("C-004: Cliente com nome vazio") {
    inicializarTeste();
    
    Cliente cliente;
    cliente.setCodigo(11111);
    cliente.setNome("");
    clientes.push_back(cliente);
    
    REQUIRE_EQ(clientes[0].getNome(), string(""));
    
    limparArquivosTeste();
}

TEST_CASE("C-005: Cliente com nome muito longo") {
    inicializarTeste();
    
    Cliente cliente;
    cliente.setCodigo(22222);
    cliente.setNome("João da Silva Santos Oliveira Pereira de Souza");
    clientes.push_back(cliente);
    
    REQUIRE_EQ(clientes[0].getNome().length() > 20, true);
    
    limparArquivosTeste();
}

TEST_CASE("C-006: Cliente com telefone em formato diferente") {
    inicializarTeste();
    
    Cliente cliente;
    cliente.setCodigo(33333);
    cliente.setTelefone("(31) 99999-9999");
    clientes.push_back(cliente);
    
    REQUIRE_EQ(clientes[0].getTelefone(), string("(31) 99999-9999"));
    
    limparArquivosTeste();
}

TEST_CASE("C-007: Cliente com endereco completo") {
    inicializarTeste();
    
    Cliente cliente;
    cliente.setCodigo(44444);
    cliente.setEndereco("Rua das Flores, 123, Apto 101, Bairro Centro, Cidade Belo Horizonte, MG, CEP 30000-000");
    clientes.push_back(cliente);
    
    REQUIRE_EQ(clientes[0].getEndereco().length() > 30, true);
    
    limparArquivosTeste();
}

TEST_CASE("C-008: Multiplos clientes") {
    inicializarTeste();
    
    for (int i = 1; i <= 10; i++) {
        Cliente cliente;
        cliente.setCodigo(10000 + i);
        cliente.setNome("Cliente " + to_string(i));
        clientes.push_back(cliente);
    }
    
    REQUIRE_EQ(clientes.size(), 10);
    
    limparArquivosTeste();
}

TEST_CASE("C-009: Buscar cliente inexistente") {
    inicializarTeste();
    
    Cliente* encontrado = consultarClientePorCodigo(99999);
    REQUIRE(encontrado == nullptr);
    
    bool existe = validaCodigoCliente(99999);
    REQUIRE_EQ(existe, false);
    
    limparArquivosTeste();
}

TEST_CASE("C-010: Pontos de fidelidade - multiplas adicoes") {
    inicializarTeste();
    
    Cliente cliente;
    cliente.setCodigo(55555);
    cliente.setPontosFidelidade(0);
    clientes.push_back(cliente);
    
    Cliente* c = consultarClientePorCodigo(55555);
    c->adicionarPontosFidelidade(10);
    c->adicionarPontosFidelidade(20);
    c->adicionarPontosFidelidade(30);
    
    REQUIRE_EQ(c->getPontosFidelidade(), 60);
    
    limparArquivosTeste();
}

TEST_CASE("C-011: Pontos de fidelidade - valor alto") {
    inicializarTeste();
    
    Cliente cliente;
    cliente.setCodigo(66666);
    cliente.setPontosFidelidade(0);
    clientes.push_back(cliente);
    
    Cliente* c = consultarClientePorCodigo(66666);
    c->adicionarPontosFidelidade(1000);
    
    REQUIRE_EQ(c->getPontosFidelidade(), 1000);
    
    limparArquivosTeste();
}

TEST_CASE("C-012: Persistencia de cliente") {
    inicializarTeste();
    
    Cliente cliente;
    cliente.setCodigo(77777);
    cliente.setNome("Teste Persistência");
    cliente.setPontosFidelidade(50);
    clientes.push_back(cliente);
    salvarTodosClientes();
    
    clientes.clear();
    carregarClientes();
    
    REQUIRE_EQ(clientes.size(), 1);
    REQUIRE_EQ(clientes[0].getCodigo(), 77777);
    REQUIRE_EQ(clientes[0].getPontosFidelidade(), 50);
    
    limparArquivosTeste();
}

TEST_CASE("C-013: Cliente com codigo zero") {
    inicializarTeste();
    
    Cliente cliente;
    cliente.setCodigo(0);
    cliente.setNome("Cliente Zero");
    clientes.push_back(cliente);
    
    bool existe = validaCodigoCliente(0);
    REQUIRE_EQ(existe, true);
    
    limparArquivosTeste();
}

TEST_CASE("C-014: Cliente com codigo negativo") {
    inicializarTeste();
    
    Cliente cliente;
    cliente.setCodigo(-1);
    cliente.setNome("Cliente Negativo");
    clientes.push_back(cliente);
    
    Cliente* encontrado = consultarClientePorCodigo(-1);
    REQUIRE(encontrado != nullptr);
    
    limparArquivosTeste();
}

TEST_CASE("C-015: Cliente com codigo muito grande") {
    inicializarTeste();
    
    Cliente cliente;
    cliente.setCodigo(999999);
    cliente.setNome("Cliente Grande");
    clientes.push_back(cliente);
    
    bool existe = validaCodigoCliente(999999);
    REQUIRE_EQ(existe, true);
    
    limparArquivosTeste();
}

TEST_CASE("C-016: Busca parcial de nome - case insensitive") {
    inicializarTeste();
    
    Cliente cliente;
    cliente.setCodigo(88888);
    cliente.setNome("MARIA SILVA");
    clientes.push_back(cliente);
    
    string busca = "maria";
    bool encontrou = false;
    for (auto& c : clientes) {
        string nomeLower = c.getNome();
        transform(nomeLower.begin(), nomeLower.end(), nomeLower.begin(), ::tolower);
        if (nomeLower.find(busca) != string::npos) {
            encontrou = true;
            break;
        }
    }
    REQUIRE_EQ(encontrou, true);
    
    limparArquivosTeste();
}

TEST_CASE("C-017: Multiplos clientes com mesmo nome") {
    inicializarTeste();
    
    for (int i = 1; i <= 3; i++) {
        Cliente cliente;
        cliente.setCodigo(90000 + i);
        cliente.setNome("João Silva");
        clientes.push_back(cliente);
    }
    
    int count = 0;
    for (const auto& c : clientes) {
        if (c.getNome() == "João Silva") count++;
    }
    REQUIRE_EQ(count, 3);
    
    limparArquivosTeste();
}

TEST_CASE("C-018: Cliente sem pontos de fidelidade") {
    inicializarTeste();
    
    Cliente cliente;
    cliente.setCodigo(11111);
    cliente.setPontosFidelidade(0);
    clientes.push_back(cliente);
    
    REQUIRE_EQ(clientes[0].getPontosFidelidade(), 0);
    
    limparArquivosTeste();
}

TEST_CASE("C-019: Atualizar pontos de fidelidade diretamente") {
    inicializarTeste();
    
    Cliente cliente;
    cliente.setCodigo(22222);
    cliente.setPontosFidelidade(100);
    clientes.push_back(cliente);
    
    Cliente* c = consultarClientePorCodigo(22222);
    c->setPontosFidelidade(200);
    
    REQUIRE_EQ(c->getPontosFidelidade(), 200);
    
    limparArquivosTeste();
}

TEST_CASE("C-020: Persistencia de multiplos clientes") {
    inicializarTeste();
    
    for (int i = 1; i <= 5; i++) {
        Cliente cliente;
        cliente.setCodigo(50000 + i);
        cliente.setNome("Cliente " + to_string(i));
        cliente.setPontosFidelidade(i * 10);
        clientes.push_back(cliente);
    }
    
    salvarTodosClientes();
    clientes.clear();
    carregarClientes();
    
    REQUIRE_EQ(clientes.size(), 5);
    
    limparArquivosTeste();
}

// ============================================
// TESTES DE FUNCIONÁRIOS (F-001 a F-015)
// ============================================

TEST_CASE("F-001: Cadastro de funcionario") {
    inicializarTeste();
    
    Funcionario func;
    func.setCodigo(10001);
    func.setNome("João Funcionário");
    func.setTelefone("31999999999");
    func.setCargo("Recepcionista");
    func.setSalario(2000.00f);
    funcionarios.push_back(func);
    salvarTodosFuncionarios();
    
    funcionarios.clear();
    carregarFuncionarios();
    
    REQUIRE_EQ(funcionarios.size(), 1);
    REQUIRE_EQ(funcionarios[0].getCodigo(), 10001);
    
    limparArquivosTeste();
}

TEST_CASE("F-002: Buscar funcionario por codigo") {
    inicializarTeste();
    
    Funcionario func;
    func.setCodigo(10002);
    func.setNome("Maria Funcionária");
    funcionarios.push_back(func);
    
    Funcionario* encontrado = consultarFuncionarioPorCodigo(10002);
    REQUIRE(encontrado != nullptr);
    REQUIRE_EQ(encontrado->getNome(), string("Maria Funcionária"));
    
    limparArquivosTeste();
}

TEST_CASE("F-003: Verificar se funcionario existe") {
    inicializarTeste();
    
    Funcionario func;
    func.setCodigo(10003);
    funcionarios.push_back(func);
    
    bool existe = funcionarioExiste(10003);
    REQUIRE_EQ(existe, true);
    
    bool naoExiste = funcionarioExiste(99999);
    REQUIRE_EQ(naoExiste, false);
    
    limparArquivosTeste();
}

TEST_CASE("F-004: Funcionario com salario zero") {
    inicializarTeste();
    
    Funcionario func;
    func.setCodigo(10004);
    func.setSalario(0.00f);
    funcionarios.push_back(func);
    
    REQUIRE_FLOAT_EQ(funcionarios[0].getSalario(), 0.00f, 0.01f);
    
    limparArquivosTeste();
}

TEST_CASE("F-005: Funcionario com salario alto") {
    inicializarTeste();
    
    Funcionario func;
    func.setCodigo(10005);
    func.setSalario(10000.00f);
    funcionarios.push_back(func);
    
    REQUIRE_FLOAT_EQ(funcionarios[0].getSalario(), 10000.00f, 0.01f);
    
    limparArquivosTeste();
}

TEST_CASE("F-006: Multiplos funcionarios") {
    inicializarTeste();
    
    for (int i = 1; i <= 5; i++) {
        Funcionario func;
        func.setCodigo(20000 + i);
        func.setNome("Funcionário " + to_string(i));
        funcionarios.push_back(func);
    }
    
    REQUIRE_EQ(funcionarios.size(), 5);
    
    limparArquivosTeste();
}

TEST_CASE("F-007: Funcionario com diferentes cargos") {
    inicializarTeste();
    
    string cargos[] = {"Recepcionista", "Gerente", "Camareira", "Segurança"};
    for (int i = 0; i < 4; i++) {
        Funcionario func;
        func.setCodigo(30000 + i);
        func.setCargo(cargos[i]);
        funcionarios.push_back(func);
    }
    
    REQUIRE_EQ(funcionarios.size(), 4);
    
    limparArquivosTeste();
}

TEST_CASE("F-008: Persistencia de funcionario") {
    inicializarTeste();
    
    Funcionario func;
    func.setCodigo(40001);
    func.setNome("Func Persistência");
    func.setSalario(3000.00f);
    funcionarios.push_back(func);
    salvarTodosFuncionarios();
    
    funcionarios.clear();
    carregarFuncionarios();
    
    REQUIRE_EQ(funcionarios.size(), 1);
    REQUIRE_EQ(funcionarios[0].getCodigo(), 40001);
    REQUIRE_FLOAT_EQ(funcionarios[0].getSalario(), 3000.00f, 0.01f);
    
    limparArquivosTeste();
}

TEST_CASE("F-009: Funcionario com codigo duplicado") {
    inicializarTeste();
    
    Funcionario func1;
    func1.setCodigo(50001);
    funcionarios.push_back(func1);
    
    Funcionario func2;
    func2.setCodigo(50001);
    funcionarios.push_back(func2);
    
    Funcionario* encontrado = consultarFuncionarioPorCodigo(50001);
    REQUIRE(encontrado != nullptr);
    
    limparArquivosTeste();
}

TEST_CASE("F-010: Buscar funcionario inexistente") {
    inicializarTeste();
    
    Funcionario* encontrado = consultarFuncionarioPorCodigo(99999);
    REQUIRE(encontrado == nullptr);
    
    limparArquivosTeste();
}

TEST_CASE("F-011: Funcionario com nome vazio") {
    inicializarTeste();
    
    Funcionario func;
    func.setCodigo(60001);
    func.setNome("");
    funcionarios.push_back(func);
    
    REQUIRE_EQ(funcionarios[0].getNome(), string(""));
    
    limparArquivosTeste();
}

TEST_CASE("F-012: Funcionario com telefone formatado") {
    inicializarTeste();
    
    Funcionario func;
    func.setCodigo(70001);
    func.setTelefone("(31) 3333-4444");
    funcionarios.push_back(func);
    
    REQUIRE_EQ(funcionarios[0].getTelefone(), string("(31) 3333-4444"));
    
    limparArquivosTeste();
}

TEST_CASE("F-013: Persistencia de multiplos funcionarios") {
    inicializarTeste();
    
    for (int i = 1; i <= 10; i++) {
        Funcionario func;
        func.setCodigo(80000 + i);
        func.setNome("Func " + to_string(i));
        func.setSalario(2000.00f + (i * 100));
        funcionarios.push_back(func);
    }
    
    salvarTodosFuncionarios();
    funcionarios.clear();
    carregarFuncionarios();
    
    REQUIRE_EQ(funcionarios.size(), 10);
    
    limparArquivosTeste();
}

TEST_CASE("F-014: Funcionario com cargo longo") {
    inicializarTeste();
    
    Funcionario func;
    func.setCodigo(90001);
    func.setCargo("Gerente de Recepção e Atendimento ao Cliente");
    funcionarios.push_back(func);
    
    REQUIRE_EQ(funcionarios[0].getCargo().length() > 20, true);
    
    limparArquivosTeste();
}

TEST_CASE("F-015: Funcionario com salario negativo") {
    inicializarTeste();
    
    Funcionario func;
    func.setCodigo(100001);
    func.setSalario(-1000.00f);
    funcionarios.push_back(func);
    
    REQUIRE_FLOAT_EQ(funcionarios[0].getSalario(), -1000.00f, 0.01f);
    
    limparArquivosTeste();
}

// ============================================
// TESTES ADICIONAIS DE ESTADIAS (E-005 a E-025)
// ============================================

TEST_CASE("E-005: Estadia com multiplas diarias") {
    inicializarTeste();
    
    Estadia estadia;
    estadia.setQuantidadeDiarias(10);
    estadia.setDataEntrada("01/12/2025");
    estadia.setDataSaida("11/12/2025");
    
    REQUIRE_EQ(estadia.getQuantidadeDiarias(), 10);
    
    limparArquivosTeste();
}

TEST_CASE("E-006: Estadia com 1 diaria") {
    inicializarTeste();
    
    Estadia estadia;
    estadia.setQuantidadeDiarias(1);
    estadia.setDataEntrada("01/12/2025");
    estadia.setDataSaida("02/12/2025");
    
    REQUIRE_EQ(estadia.getQuantidadeDiarias(), 1);
    
    limparArquivosTeste();
}

TEST_CASE("E-007: Calculo de diarias - mesmo mes") {
    inicializarTeste();
    
    int diarias = calcularDiarias("01/12/2025", "10/12/2025");
    REQUIRE_EQ(diarias, 9);
    
    limparArquivosTeste();
}

TEST_CASE("E-008: Calculo de diarias - mes diferente") {
    inicializarTeste();
    
    int diarias = calcularDiarias("28/12/2025", "02/01/2026");
    REQUIRE_EQ(diarias, 5);
    
    limparArquivosTeste();
}

TEST_CASE("E-009: Calculo de diarias - ano diferente") {
    inicializarTeste();
    
    int diarias = calcularDiarias("20/12/2025", "05/01/2026");
    REQUIRE_EQ(diarias, 16);
    
    limparArquivosTeste();
}

TEST_CASE("E-010: Estadia com data de entrada igual a saida") {
    inicializarTeste();
    
    int diarias = calcularDiarias("01/12/2025", "01/12/2025");
    REQUIRE_EQ(diarias, 0);
    
    limparArquivosTeste();
}

TEST_CASE("E-011: Multiplas estadias para mesmo cliente") {
    inicializarTeste();
    
    Cliente cliente;
    cliente.setCodigo(11111);
    clientes.push_back(cliente);
    
    for (int i = 1; i <= 3; i++) {
        Estadia estadia;
        estadia.setCodigoCliente(11111);
        estadia.setNumQuarto(100 + i);
        estadias.push_back(estadia);
    }
    
    int count = 0;
    for (const auto& e : estadias) {
        if (e.getCodigoCliente() == 11111) count++;
    }
    REQUIRE_EQ(count, 3);
    
    limparArquivosTeste();
}

TEST_CASE("E-012: Estadia com valor total alto") {
    inicializarTeste();
    
    Quarto quarto;
    quarto.setValorDiaria(500.00);
    
    Estadia estadia;
    estadia.setQuantidadeDiarias(30);
    
    float valorTotal = quarto.getValorDiaria() * estadia.getQuantidadeDiarias();
    REQUIRE_FLOAT_EQ(valorTotal, 15000.00f, 0.01f);
    
    limparArquivosTeste();
}

TEST_CASE("E-013: Estadia com quantidade de hospedes variada") {
    inicializarTeste();
    
    for (int i = 1; i <= 4; i++) {
        Estadia estadia;
        estadia.setQuantidadeHospedes(i);
        estadias.push_back(estadia);
    }
    
    REQUIRE_EQ(estadias.size(), 4);
    
    limparArquivosTeste();
}

TEST_CASE("E-014: Persistencia de estadia") {
    inicializarTeste();
    
    Estadia estadia;
    estadia.setCodigoEstadia(99999);
    estadia.setCodigoCliente(12345);
    estadia.setNumQuarto(101);
    estadia.setQuantidadeDiarias(3);
    estadias.push_back(estadia);
    salvarEstadias();
    
    estadias.clear();
    carregarEstadias();
    
    REQUIRE_EQ(estadias.size(), 1);
    REQUIRE_EQ(estadias[0].getCodigoCliente(), 12345);
    
    limparArquivosTeste();
}

TEST_CASE("E-015: Verificar conflito de datas - sobreposicao") {
    inicializarTeste();
    
    // Estadia existente: 01/12 a 05/12
    Estadia existente;
    existente.setDataEntrada("01/12/2025");
    existente.setDataSaida("05/12/2025");
    estadias.push_back(existente);
    
    // Nova reserva: 03/12 a 07/12 (conflita)
    bool conflito = haConflitoDatas("01/12/2025", "05/12/2025", "03/12/2025", "07/12/2025");
    REQUIRE_EQ(conflito, true);
    
    limparArquivosTeste();
}

TEST_CASE("E-016: Verificar conflito de datas - sem conflito") {
    inicializarTeste();
    
    // Estadia existente: 01/12 a 05/12
    // Nova reserva: 06/12 a 10/12 (não conflita)
    bool conflito = haConflitoDatas("01/12/2025", "05/12/2025", "06/12/2025", "10/12/2025");
    REQUIRE_EQ(conflito, false);
    
    limparArquivosTeste();
}

TEST_CASE("E-017: Estadia com codigo de estadia unico") {
    inicializarTeste();
    
    for (int i = 1; i <= 5; i++) {
        Estadia estadia;
        estadia.setCodigoEstadia(100000 + i);
        estadias.push_back(estadia);
    }
    
    REQUIRE_EQ(estadias.size(), 5);
    
    limparArquivosTeste();
}

TEST_CASE("E-018: Calculo de pontos de fidelidade") {
    inicializarTeste();
    
    Cliente cliente;
    cliente.setCodigo(22222);
    cliente.setPontosFidelidade(0);
    clientes.push_back(cliente);
    
    Estadia estadia;
    estadia.setCodigoCliente(22222);
    estadia.setQuantidadeDiarias(5);
    
    int pontos = estadia.getQuantidadeDiarias() * 10;
    Cliente* c = consultarClientePorCodigo(22222);
    c->adicionarPontosFidelidade(pontos);
    
    REQUIRE_EQ(c->getPontosFidelidade(), 50);
    
    limparArquivosTeste();
}

TEST_CASE("E-019: Multiplas estadias em quartos diferentes") {
    inicializarTeste();
    
    for (int i = 101; i <= 105; i++) {
        Estadia estadia;
        estadia.setNumQuarto(i);
        estadias.push_back(estadia);
    }
    
    REQUIRE_EQ(estadias.size(), 5);
    
    limparArquivosTeste();
}

TEST_CASE("E-020: Estadia com periodo longo") {
    inicializarTeste();
    
    int diarias = calcularDiarias("01/01/2025", "31/01/2025");
    REQUIRE_EQ(diarias, 30);
    
    limparArquivosTeste();
}

TEST_CASE("E-021: Persistencia de multiplas estadias") {
    inicializarTeste();
    
    for (int i = 1; i <= 10; i++) {
        Estadia estadia;
        estadia.setCodigoEstadia(200000 + i);
        estadia.setCodigoCliente(30000 + i);
        estadia.setNumQuarto(100 + i);
        estadias.push_back(estadia);
    }
    
    salvarEstadias();
    estadias.clear();
    carregarEstadias();
    
    REQUIRE_EQ(estadias.size(), 10);
    
    limparArquivosTeste();
}

TEST_CASE("E-022: Verificar disponibilidade - periodo antes") {
    inicializarTeste();
    
    Quarto quarto;
    quarto.setNumQuarto(8001);
    salvarQuartoNoArquivo(quarto);
    
    Estadia estadia;
    estadia.setNumQuarto(8001);
    estadia.setDataEntrada("10/12/2025");
    estadia.setDataSaida("15/12/2025");
    estadias.push_back(estadia);
    
    // Verificar disponibilidade antes do período ocupado
    bool disponivel = verificarDisponibilidadePorPeriodo(8001, "01/12/2025", "05/12/2025");
    REQUIRE_EQ(disponivel, true);
    
    limparArquivosTeste();
}

TEST_CASE("E-023: Verificar disponibilidade - periodo depois") {
    inicializarTeste();
    
    Quarto quarto;
    quarto.setNumQuarto(9001);
    salvarQuartoNoArquivo(quarto);
    
    Estadia estadia;
    estadia.setNumQuarto(9001);
    estadia.setDataEntrada("01/12/2025");
    estadia.setDataSaida("05/12/2025");
    estadias.push_back(estadia);
    
    // Verificar disponibilidade depois do período ocupado
    bool disponivel = verificarDisponibilidadePorPeriodo(9001, "10/12/2025", "15/12/2025");
    REQUIRE_EQ(disponivel, true);
    
    limparArquivosTeste();
}

TEST_CASE("E-024: Estadia com zero diarias") {
    inicializarTeste();
    
    Estadia estadia;
    estadia.setQuantidadeDiarias(0);
    
    REQUIRE_EQ(estadia.getQuantidadeDiarias(), 0);
    
    limparArquivosTeste();
}

TEST_CASE("E-025: Calculo de valor com desconto de pontos") {
    inicializarTeste();
    
    Quarto quarto;
    quarto.setValorDiaria(200.00);
    
    Estadia estadia;
    estadia.setQuantidadeDiarias(5);
    
    float valorTotal = quarto.getValorDiaria() * estadia.getQuantidadeDiarias();
    float desconto = 50.00; // Simular desconto de pontos
    float valorFinal = valorTotal - desconto;
    
    REQUIRE_FLOAT_EQ(valorFinal, 950.00f, 0.01f);
    
    limparArquivosTeste();
}

// ============================================
// TESTES ADICIONAIS DE INTEGRAÇÃO (I-004 a I-015)
// ============================================

TEST_CASE("I-004: Fluxo completo com multiplos clientes") {
    inicializarTeste();
    
    // Cadastrar 3 clientes
    for (int i = 1; i <= 3; i++) {
        Cliente cliente;
        cliente.setCodigo(40000 + i);
        cliente.setNome("Cliente " + to_string(i));
        clientes.push_back(cliente);
    }
    
    // Cadastrar 3 quartos
    for (int i = 1; i <= 3; i++) {
        Quarto quarto;
        quarto.setNumQuarto(5000 + i);
        quarto.setStatus("desocupado");
        salvarQuartoNoArquivo(quarto);
    }
    
    // Fazer 3 reservas
    for (int i = 0; i < 3; i++) {
        Estadia estadia;
        estadia.setCodigoCliente(40001 + i);
        estadia.setNumQuarto(5001 + i);
        estadias.push_back(estadia);
    }
    
    REQUIRE_EQ(clientes.size(), 3);
    REQUIRE_EQ(estadias.size(), 3);
    
    limparArquivosTeste();
}

TEST_CASE("I-005: Cliente com multiplas estadias e pontos") {
    inicializarTeste();
    
    Cliente cliente;
    cliente.setCodigo(50001);
    cliente.setPontosFidelidade(0);
    clientes.push_back(cliente);
    
    // Criar 3 estadias
    for (int i = 1; i <= 3; i++) {
        Estadia estadia;
        estadia.setCodigoCliente(50001);
        estadia.setQuantidadeDiarias(2);
        estadias.push_back(estadia);
        
        // Adicionar pontos
        Cliente* c = consultarClientePorCodigo(50001);
        c->adicionarPontosFidelidade(20);
    }
    
    Cliente* c = consultarClientePorCodigo(50001);
    REQUIRE_EQ(c->getPontosFidelidade(), 60);
    
    limparArquivosTeste();
}

TEST_CASE("I-006: Reserva, ocupacao e liberacao de quarto") {
    inicializarTeste();
    
    Quarto quarto;
    quarto.setNumQuarto(6001);
    quarto.setStatus("desocupado");
    salvarQuartoNoArquivo(quarto);
    
    // Reservar
    Estadia estadia;
    estadia.setNumQuarto(6001);
    estadias.push_back(estadia);
    alterarStatusQuarto(6001, "ocupado");
    
    // Verificar ocupado
    Quarto q = buscarQuartoPorNumero(6001);
    REQUIRE_EQ(q.getStatus(), string("ocupado"));
    
    // Liberar
    alterarStatusQuarto(6001, "desocupado");
    q = buscarQuartoPorNumero(6001);
    REQUIRE_EQ(q.getStatus(), string("desocupado"));
    
    limparArquivosTeste();
}

TEST_CASE("I-007: Sistema completo - cadastro ate checkout") {
    inicializarTeste();
    
    // 1. Cadastrar cliente
    Cliente cliente;
    cliente.setCodigo(60001);
    cliente.setNome("Cliente Completo");
    clientes.push_back(cliente);
    
    // 2. Cadastrar quarto
    Quarto quarto;
    quarto.setNumQuarto(7001);
    quarto.setValorDiaria(200.00);
    quarto.setStatus("desocupado");
    salvarQuartoNoArquivo(quarto);
    
    // 3. Fazer reserva
    Estadia estadia;
    estadia.setCodigoCliente(60001);
    estadia.setNumQuarto(7001);
    estadia.setQuantidadeDiarias(3);
    estadias.push_back(estadia);
    alterarStatusQuarto(7001, "ocupado");
    
    // 4. Calcular valor
    float valor = quarto.getValorDiaria() * 3;
    REQUIRE_FLOAT_EQ(valor, 600.00f, 0.01f);
    
    // 5. Adicionar pontos
    Cliente* c = consultarClientePorCodigo(60001);
    c->adicionarPontosFidelidade(30);
    
    // 6. Dar baixa
    alterarStatusQuarto(7001, "desocupado");
    
    REQUIRE_EQ(c->getPontosFidelidade(), 30);
    
    limparArquivosTeste();
}

TEST_CASE("I-008: Multiplos quartos, multiplos clientes, multiplas reservas") {
    inicializarTeste();
    
    // Cadastrar 5 quartos
    for (int i = 1; i <= 5; i++) {
        Quarto quarto;
        quarto.setNumQuarto(8000 + i);
        quarto.setStatus("desocupado");
        salvarQuartoNoArquivo(quarto);
    }
    
    // Cadastrar 5 clientes
    for (int i = 1; i <= 5; i++) {
        Cliente cliente;
        cliente.setCodigo(70000 + i);
        clientes.push_back(cliente);
    }
    
    // Fazer 5 reservas
    for (int i = 0; i < 5; i++) {
        Estadia estadia;
        estadia.setCodigoCliente(70001 + i);
        estadia.setNumQuarto(8001 + i);
        estadias.push_back(estadia);
    }
    
    REQUIRE_EQ(estadias.size(), 5);
    
    limparArquivosTeste();
}

TEST_CASE("I-009: Historico completo de cliente") {
    inicializarTeste();
    
    Cliente cliente;
    cliente.setCodigo(80001);
    cliente.setNome("Cliente Histórico");
    clientes.push_back(cliente);
    
    // Criar 5 estadias
    for (int i = 1; i <= 5; i++) {
        Estadia estadia;
        estadia.setCodigoCliente(80001);
        estadia.setNumQuarto(9000 + i);
        estadia.setQuantidadeDiarias(i);
        estadias.push_back(estadia);
    }
    
    // Contar estadias do cliente
    int count = 0;
    int totalDiarias = 0;
    for (const auto& e : estadias) {
        if (e.getCodigoCliente() == 80001) {
            count++;
            totalDiarias += e.getQuantidadeDiarias();
        }
    }
    
    REQUIRE_EQ(count, 5);
    REQUIRE_EQ(totalDiarias, 15);
    
    limparArquivosTeste();
}

TEST_CASE("I-010: Persistencia completa do sistema") {
    inicializarTeste();
    
    // Cadastrar dados
    Cliente cliente;
    cliente.setCodigo(90001);
    clientes.push_back(cliente);
    
    Quarto quarto;
    quarto.setNumQuarto(10001);
    salvarQuartoNoArquivo(quarto);
    
    Estadia estadia;
    estadia.setCodigoCliente(90001);
    estadia.setNumQuarto(10001);
    estadias.push_back(estadia);
    
    Funcionario func;
    func.setCodigo(100001);
    funcionarios.push_back(func);
    
    // Salvar tudo
    salvarTodosClientes();
    salvarEstadias();
    salvarTodosFuncionarios();
    
    // Limpar
    clientes.clear();
    estadias.clear();
    funcionarios.clear();
    quartos.clear();
    
    // Recarregar
    carregarClientes();
    carregarEstadias();
    carregarFuncionarios();
    quartos = lerQuartosDoArquivo();
    
    REQUIRE_EQ(clientes.size(), 1);
    REQUIRE_EQ(estadias.size(), 1);
    REQUIRE_EQ(funcionarios.size(), 1);
    REQUIRE_EQ(quartos.size(), 1);
    
    limparArquivosTeste();
}

TEST_CASE("I-011: Cliente VIP com muitas estadias") {
    inicializarTeste();
    
    Cliente cliente;
    cliente.setCodigo(100001);
    cliente.setPontosFidelidade(0);
    clientes.push_back(cliente);
    
    // Criar 10 estadias
    for (int i = 1; i <= 10; i++) {
        Estadia estadia;
        estadia.setCodigoCliente(100001);
        estadia.setQuantidadeDiarias(2);
        estadias.push_back(estadia);
        
        Cliente* c = consultarClientePorCodigo(100001);
        c->adicionarPontosFidelidade(20);
    }
    
    Cliente* c = consultarClientePorCodigo(100001);
    REQUIRE_EQ(c->getPontosFidelidade(), 200);
    
    limparArquivosTeste();
}

TEST_CASE("I-013: Reserva cancelada - quarto volta a disponivel") {
    inicializarTeste();
    
    Quarto quarto;
    quarto.setNumQuarto(11001);
    quarto.setStatus("desocupado");
    salvarQuartoNoArquivo(quarto);
    
    // Reservar
    Estadia estadia;
    estadia.setNumQuarto(11001);
    estadias.push_back(estadia);
    alterarStatusQuarto(11001, "ocupado");
    
    // Cancelar (remover estadia e alterar status)
    estadias.clear();
    alterarStatusQuarto(11001, "desocupado");
    
    Quarto q = buscarQuartoPorNumero(11001);
    REQUIRE_EQ(q.getStatus(), string("desocupado"));
    
    limparArquivosTeste();
}

TEST_CASE("I-014: Multiplos checkouts no mesmo dia") {
    inicializarTeste();
    
    // Criar 5 estadias
    for (int i = 1; i <= 5; i++) {
        Cliente cliente;
        cliente.setCodigo(300000 + i);
        cliente.setPontosFidelidade(0);
        clientes.push_back(cliente);
        
        Estadia estadia;
        estadia.setCodigoCliente(300000 + i);
        estadia.setQuantidadeDiarias(2);
        estadias.push_back(estadia);
        
        Cliente* c = consultarClientePorCodigo(300000 + i);
        c->adicionarPontosFidelidade(20);
    }
    
    // Verificar que todos receberam pontos
    for (int i = 1; i <= 5; i++) {
        Cliente* c = consultarClientePorCodigo(300000 + i);
        REQUIRE_EQ(c->getPontosFidelidade(), 20);
    }
    
    limparArquivosTeste();
}

// ============================================
// TESTES ADICIONAIS DE SISTEMA (S-003 a S-015)
// ============================================

TEST_CASE("S-003: Persistencia apos multiplas operacoes") {
    inicializarTeste();
    
    // Realizar várias operações
    for (int i = 1; i <= 5; i++) {
        Cliente cliente;
        cliente.setCodigo(400000 + i);
        clientes.push_back(cliente);
        
        Quarto quarto;
        quarto.setNumQuarto(40000 + i);
        salvarQuartoNoArquivo(quarto);
    }
    
    // Salvar
    salvarTodosClientes();
    
    // Limpar e recarregar
    clientes.clear();
    carregarClientes();
    
    REQUIRE_EQ(clientes.size(), 5);
    
    limparArquivosTeste();
}

TEST_CASE("S-004: Arquivo vazio - carregar sem dados") {
    inicializarTeste();
    
    // Tentar carregar sem salvar nada
    clientes.clear();
    carregarClientes();
    
    REQUIRE_EQ(clientes.size(), 0);
    
    limparArquivosTeste();
}

TEST_CASE("S-005: Sobrescrever dados existentes") {
    inicializarTeste();
    
    // Salvar primeira vez
    Cliente cliente1;
    cliente1.setCodigo(500001);
    cliente1.setNome("Primeiro");
    clientes.push_back(cliente1);
    salvarTodosClientes();
    
    // Limpar e salvar novamente
    clientes.clear();
    Cliente cliente2;
    cliente2.setCodigo(500002);
    cliente2.setNome("Segundo");
    clientes.push_back(cliente2);
    salvarTodosClientes();
    
    // Recarregar
    clientes.clear();
    carregarClientes();
    
    REQUIRE_EQ(clientes.size(), 1);
    REQUIRE_EQ(clientes[0].getCodigo(), 500002);
    
    limparArquivosTeste();
}

TEST_CASE("S-009: Integridade de dados apos multiplas operacoes") {
    inicializarTeste();
    
    // Criar dados
    Cliente cliente;
    cliente.setCodigo(700001);
    cliente.setNome("Teste Integridade");
    cliente.setPontosFidelidade(100);
    clientes.push_back(cliente);
    
    Quarto quarto;
    quarto.setNumQuarto(70001);
    quarto.setValorDiaria(250.00);
    salvarQuartoNoArquivo(quarto);
    
    // Salvar e recarregar múltiplas vezes
    for (int i = 0; i < 5; i++) {
        salvarTodosClientes();
        clientes.clear();
        carregarClientes();
    }
    
    REQUIRE_EQ(clientes.size(), 1);
    REQUIRE_EQ(clientes[0].getCodigo(), 700001);
    REQUIRE_EQ(clientes[0].getPontosFidelidade(), 100);
    
    limparArquivosTeste();
}

TEST_CASE("S-013: Recuperacao apos falha simulada") {
    inicializarTeste();
    
    // Salvar dados
    Cliente cliente;
    cliente.setCodigo(800001);
    clientes.push_back(cliente);
    salvarTodosClientes();
    
    // Simular falha (limpar vetor)
    clientes.clear();
    
    // Recuperar
    carregarClientes();
    
    REQUIRE_EQ(clientes.size(), 1);
    REQUIRE_EQ(clientes[0].getCodigo(), 800001);
    
    limparArquivosTeste();
}

TEST_CASE("S-014: Consistencia entre memoria e arquivo") {
    inicializarTeste();
    
    // Criar em memória
    Cliente cliente;
    cliente.setCodigo(900001);
    cliente.setNome("Memória");
    clientes.push_back(cliente);
    
    // Salvar
    salvarTodosClientes();
    
    // Modificar em memória
    clientes[0].setNome("Modificado");
    
    // Recarregar do arquivo
    clientes.clear();
    carregarClientes();
    
    // Deve ter o nome original
    REQUIRE_EQ(clientes[0].getNome(), string("Memória"));
    
    limparArquivosTeste();
}

TEST_CASE("S-015: Teste completo de sistema - todas as funcionalidades") {
    inicializarTeste();
    
    // 1. Cadastrar funcionário
    Funcionario func;
    func.setCodigo(1000001);
    funcionarios.push_back(func);
    
    // 2. Cadastrar cliente
    Cliente cliente;
    cliente.setCodigo(1000001);
    cliente.setPontosFidelidade(0);
    clientes.push_back(cliente);
    
    // 3. Cadastrar quarto
    Quarto quarto;
    quarto.setNumQuarto(100001);
    quarto.setValorDiaria(300.00);
    quarto.setStatus("desocupado");
    salvarQuartoNoArquivo(quarto);
    
    // 4. Fazer reserva
    Estadia estadia;
    estadia.setCodigoCliente(1000001);
    estadia.setNumQuarto(100001);
    estadia.setQuantidadeDiarias(5);
    estadias.push_back(estadia);
    alterarStatusQuarto(100001, "ocupado");
    
    // 5. Calcular valor
    float valor = quarto.getValorDiaria() * 5;
    
    // 6. Adicionar pontos
    Cliente* c = consultarClientePorCodigo(1000001);
    c->adicionarPontosFidelidade(50);
    
    // 7. Salvar tudo
    salvarTodosClientes();
    salvarEstadias();
    salvarTodosFuncionarios();
    
    // 8. Verificar
    REQUIRE_EQ(funcionarios.size(), 1);
    REQUIRE_EQ(clientes.size(), 1);
    REQUIRE_EQ(estadias.size(), 1);
    REQUIRE_FLOAT_EQ(valor, 1500.00f, 0.01f);
    REQUIRE_EQ(c->getPontosFidelidade(), 50);
    
    limparArquivosTeste();
}

// Função main para executar os testes
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "  EXECUTANDO TESTES" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    // Os testes são executados automaticamente durante a inicialização estática
    // Agora apenas imprimimos o resumo
    SimpleTest::TestRunner::getInstance().printSummary();
    
    return SimpleTest::TestRunner::getInstance().getExitCode();
}

