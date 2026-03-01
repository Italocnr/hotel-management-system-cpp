#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <cstddef>
#include <iomanip>
#include <regex> // para obrigar o usuario a usar o modelo de escrita de telefone correto.

// Definição de cores para o terminal
#define preto "\033[0;30m"
#define vermelho "\033[0;31m"
#define verde "\033[0;32m"
#define amarelo "\033[0;33m"
#define azul "\033[0;34m"
#define roxo "\033[0;35m"
#define ciano "\033[0;36m"
#define branco "\033[0;37m"
#define reset "\033[0m"

// Definição de cores BOLD
#define pretoBold "\033[1;30m"
#define vermelhoBold "\033[1;31m"
#define verdeBold "\033[1;32m"
#define amareloBold "\033[1;33m"
#define azulBold "\033[1;34m"
#define roxoBold "\033[1;35m"
#define cianoBold "\033[1;36m"
#define brancoBold "\033[1;37m"

using namespace std;

// Forward declaration da classe Quarto
class Quarto;

// Assinaturas das funções EXISTENTES (do grupo)
int gerarCodigos();
void menu();
void cadastrarClientes();
void cadastrarFuncionarios();
void cadastrarEstadia();
void cadastrarQuarto();
void filtrarFuncionarios();
void filtrarClientes();
int quartosDisponiveis(int quantHospedes);
bool validaCodigoCliente(int clienteCode);
void salvarEstadias();
void carregarEstadias();
int calcularDiarias(string dataEntrada, string dataSaida);
bool quartoExiste(int numeroQuarto);
void salvarQuartoNoArquivo(Quarto quarto);
vector<Quarto> lerQuartosDoArquivo();
void limparBuffer();
void listarQuartosDisponiveis();
void listarTodosQuartos();
void editarQuarto();
void excluirQuarto();
vector<Quarto> obterQuartosDisponiveis(int quantidadeHospedes);
bool alterarStatusQuarto(int numeroQuarto, string novoStatus);
void inicializarQuartosFixos();
bool verificarDisponibilidadePorPeriodo(int numQuarto, string dataCheckin, string dataCheckout);
vector<Quarto> obterQuartosDisponiveisPorPeriodo(int quantidadeHospedes, string dataCheckin, string dataCheckout);
void darBaixaEstadia();
Quarto buscarQuartoPorNumero(int numeroQuarto);
void listarTodasEstadias();
int calcularPontosFidelidade(int codigoCliente);

// Definição das classes EXISTENTES (do grupo)
class Pessoa{
private:
    int codigo;
    string nome;
    string telefone;

public:
   int getCodigo() const { return codigo; }
    void setCodigo(int c) { codigo = c; }

    string getNome() const { return nome; }
    void setNome(const std::string& n) { nome = n; }

    string getTelefone() const { return telefone; }
    void setTelefone(const std::string& t) { telefone = t; }


};
class Cliente: public Pessoa{
private:
    string endereco;
    int pontosFidelidade;

public:
    Cliente() : pontosFidelidade(0) {}

    string getEndereco() const { return endereco; }
    void setEndereco(const string& e) { endereco = e; }

    int getPontosFidelidade() const { return pontosFidelidade; }
    void setPontosFidelidade(int pontos) { pontosFidelidade = pontos; }
    void adicionarPontosFidelidade(int pontos) { pontosFidelidade += pontos; }

    // Salvar em arquivo binário
    void salvarBinario(FILE* fp) const {
        // Salva código
        int codigo = getCodigo();
        fwrite(&codigo, sizeof(int), 1, fp);

        // Salva nome
        string nome = getNome();
        int tamNome = nome.size();
        fwrite(&tamNome, sizeof(int), 1, fp);
        fwrite(nome.c_str(), sizeof(char), tamNome, fp);

        // Salva telefone
        string telefone = getTelefone();
        int tamTel = telefone.size();
        fwrite(&tamTel, sizeof(int), 1, fp);
        fwrite(telefone.c_str(), sizeof(char), tamTel, fp);

        // Salva endereço
        int tamEndereco = endereco.size();
        fwrite(&tamEndereco, sizeof(int), 1, fp);
        fwrite(endereco.c_str(), sizeof(char), tamEndereco, fp);

        // Salva pontos de fidelidade
        fwrite(&pontosFidelidade, sizeof(int), 1, fp);
    }

    void lerBinario(FILE* fp) {
        // Lê código
        int codigo;
        fread(&codigo, sizeof(int), 1, fp);
        setCodigo(codigo);

        // Lê nome
        int tamNome;
        fread(&tamNome, sizeof(int), 1, fp);
        string nome(tamNome, '\0');
        fread(&nome[0], sizeof(char), tamNome, fp);
        setNome(nome);

        // Lê telefone
        int tamTel;
        fread(&tamTel, sizeof(int), 1, fp);
        string telefone(tamTel, '\0');
        fread(&telefone[0], sizeof(char), tamTel, fp);
        setTelefone(telefone);

        // Lê endereço
        int tamEndereco;
        fread(&tamEndereco, sizeof(int), 1, fp);
        endereco.resize(tamEndereco);
        fread(&endereco[0], sizeof(char), tamEndereco, fp);

        // Lê pontos de fidelidade (se existir no arquivo, senão mantém 0)
        // Verifica se ainda há dados suficientes no arquivo antes de ler
        long posicaoAtual = ftell(fp);
        fseek(fp, 0, SEEK_END);
        long tamanhoArquivo = ftell(fp);
        fseek(fp, posicaoAtual, SEEK_SET);

        // Verifica se há pelo menos sizeof(int) bytes restantes
        // Isso garante compatibilidade com arquivos antigos que não têm pontos de fidelidade
        if (posicaoAtual >= 0 && tamanhoArquivo > posicaoAtual && (tamanhoArquivo - posicaoAtual) >= (long)sizeof(int)) {
            if (fread(&pontosFidelidade, sizeof(int), 1, fp) != 1) {
                pontosFidelidade = 0;
            }
        } else {
            pontosFidelidade = 0;
        }
    }
};

// Declaração do vetor de clientes (antes das funções que o usam)
vector<Cliente> clientes;

class Funcionario: public Pessoa{
private:
    float salario;
    string cargo;
public:

    void setSalario(float s){ salario = s; }
    float getSalario() const { return salario; }

    void setCargo(const string& c) { cargo = c; }
    string getCargo() const { return cargo; }

    void exibirDados() const{
        cout << "\n   ┌─────────────────────────────────────────────────────────────┐\n";
        cout << "   │" << cianoBold << "                📋 DADOS DO FUNCIONÁRIO            "<< reset <<"          │\n";
        cout << "   ├─────────────────────────────────────────────────────────────┤\n";
        cout << "   │" << azul << "   Código:           " << reset << verdeBold << getCodigo() << reset;
        int espacos = 40 - (int)to_string(getCodigo()).length();
        for (int i = 0; i < espacos; i++) cout << " ";
        cout << "│\n";
        cout << "   │" << azul << "   Nome:             " << reset << verde << getNome() << reset;
        espacos = 40 - (int)getNome().length();
        if (espacos < 0) espacos = 0;
        for (int i = 0; i < espacos; i++) cout << " ";
        cout << "│\n";
        cout << "   │" << azul << "   Telefone:         " << reset << verde << getTelefone() << reset;
        espacos = 40 - (int)getTelefone().length();
        if (espacos < 0) espacos = 0;
        for (int i = 0; i < espacos; i++) cout << " ";
        cout << "│\n";
        cout << "   │" << azul << "   Cargo:            " << reset << verde << cargo << reset;
        espacos = 40 - (int)cargo.length();
        if (espacos < 0) espacos = 0;
        for (int i = 0; i < espacos; i++) cout << " ";
        cout << "│\n";
        cout << "   │" << azul << "   Salário:          " << reset << verdeBold << "R$ " << fixed << setprecision(2) << salario << reset;
        int tamanhoSalario = (salario >= 10000) ? 9 : ((salario >= 1000) ? 8 : 7);
        espacos = 38 - tamanhoSalario;
        if (espacos < 0) espacos = 0;
        for (int i = 0; i < espacos; i++) cout << " ";
        cout << "│\n";
        cout << "   └─────────────────────────────────────────────────────────────┘\n";
    }

    //Salvar em arquivo binário
    void salvarBinario(FILE* fp) const {
        // Salva código e salário
        int codigo = getCodigo();
        fwrite(&codigo, sizeof(int), 1, fp);
        fwrite(&salario, sizeof(float), 1, fp);

        // Salva nome
        string nome = getNome();
        int tamNome = nome.size();
        fwrite(&tamNome, sizeof(int), 1, fp);
        fwrite(nome.c_str(), sizeof(char), tamNome, fp);

        // Salva telefone
        string telefone = getTelefone();
        int tamTel = telefone.size();
        fwrite(&tamTel, sizeof(int), 1, fp);
        fwrite(telefone.c_str(), sizeof(char), tamTel, fp);

        // Salva cargo
        int tamCargo = cargo.size();
        fwrite(&tamCargo, sizeof(int), 1, fp);
        fwrite(cargo.c_str(), sizeof(char), tamCargo, fp);
    }

    void lerBinario(FILE* fp) {
        // Lê código e salário
        int codigo;
        fread(&codigo, sizeof(int), 1, fp);
        setCodigo(codigo);
        fread(&salario, sizeof(float), 1, fp);

        // Lê nome
        int tamNome;
        fread(&tamNome, sizeof(int), 1, fp);
        string nome(tamNome, '\0');
        fread(&nome[0], sizeof(char), tamNome, fp);
        setNome(nome);

        // Lê telefone
        int tamTel;
        fread(&tamTel, sizeof(int), 1, fp);
        string telefone(tamTel, '\0');
        fread(&telefone[0], sizeof(char), tamTel, fp);
        setTelefone(telefone);

        // Lê cargo
        int tamCargo;
        fread(&tamCargo, sizeof(int), 1, fp);
        cargo.resize(tamCargo);
        fread(&cargo[0], sizeof(char), tamCargo, fp);
    }
};

// Vetor para manter os dados em memória enquanto o programa roda
vector<Funcionario> funcionarios;

void salvarTodosFuncionarios() {
    FILE* fp = fopen("funcionarios.bin", "wb");
    if (!fp) return;

    int total = funcionarios.size();
    fwrite(&total, sizeof(int), 1, fp);

    for (auto& f : funcionarios)
        f.salvarBinario(fp);

    fclose(fp);
}

void carregarFuncionarios() {
    FILE* fp = fopen("funcionarios.bin", "rb");
    if (!fp) return;

    int total = 0;
    fread(&total, sizeof(int), 1, fp);

    funcionarios.resize(total);
    for (int i = 0; i < total; i++)
        funcionarios[i].lerBinario(fp);

    fclose(fp);
}

// Verifica se o funcionário já existe por meio do código
Funcionario* consultarFuncionarioPorCodigo(int codigoBusca) {
    for (auto& f : funcionarios) {
        if (f.getCodigo() == codigoBusca)
            return &f; // Retorna ponteiro para o funcionário encontrado
    }
    return nullptr;
}

//  Checar existência do codigo
bool funcionarioExiste(int codigoBusca) {
    return consultarFuncionarioPorCodigo(codigoBusca) != nullptr;
}

// Função para validar código de cliente
bool validaCodigoCliente(int clienteCode) {
    for (const auto& cliente : clientes) {
        if (cliente.getCodigo() == clienteCode) {
            return true;
        }
    }
    return false;
}

// Verifica se o cliente já existe por meio do código e retorna ponteiro
Cliente* consultarClientePorCodigo(int codigoBusca) {
    for (auto& c : clientes) {
        if (c.getCodigo() == codigoBusca)
            return &c; // Retorna ponteiro para o cliente encontrado
    }
    return nullptr;
}

// Função para exibir dados do cliente de forma formatada
void exibirDadosCliente(const Cliente& cliente) {
    // Usar pontos de fidelidade diretamente do objeto cliente
    int pontosFidelidade = cliente.getPontosFidelidade();

    cout << "\n   ┌─────────────────────────────────────────────────────────────┐\n";
    cout << "   │" << cianoBold << "                📋 DADOS DO CLIENTE             "<< reset <<"          │\n";
    cout << "   ├─────────────────────────────────────────────────────────────┤\n";
    cout << "   │" << azul << "   Código:           " << reset << verdeBold << cliente.getCodigo() << reset;
    int espacos = 40 - (int)to_string(cliente.getCodigo()).length();
    for (int i = 0; i < espacos; i++) cout << " ";
    cout << "│\n";
    cout << "   │" << azul << "   Nome:             " << reset << verde << cliente.getNome() << reset;
    espacos = 40 - (int)cliente.getNome().length();
    if (espacos < 0) espacos = 0;
    for (int i = 0; i < espacos; i++) cout << " ";
    cout << "│\n";
    cout << "   │" << azul << "   Telefone:         " << reset << verde << cliente.getTelefone() << reset;
    espacos = 40 - (int)cliente.getTelefone().length();
    if (espacos < 0) espacos = 0;
    for (int i = 0; i < espacos; i++) cout << " ";
    cout << "│\n";
    cout << "   │" << azul << "   Endereço:         " << reset << verde << cliente.getEndereco() << reset;
    espacos = 40 - (int)cliente.getEndereco().length();
    if (espacos < 0) espacos = 0;
    for (int i = 0; i < espacos; i++) cout << " ";
    cout << "│\n";
    cout << "   │" << azul << "   Pontos Fidelidade: " << reset << roxoBold << pontosFidelidade << " pontos" << reset;
    string pontosStr = to_string(pontosFidelidade) + " pontos";
    espacos = 40 - (int)pontosStr.length();
    if (espacos < 0) espacos = 0;
    for (int i = 0; i < espacos; i++) cout << " ";
    cout << "│\n";
    cout << "   └─────────────────────────────────────────────────────────────┘\n";
}

// Funções para salvar e carregar clientes
void salvarTodosClientes() {
    FILE* fp = fopen("clientes.bin", "wb");
    if (!fp) return;

    int total = clientes.size();
    fwrite(&total, sizeof(int), 1, fp);

    for (auto& c : clientes)
        c.salvarBinario(fp);

    fclose(fp);
}

void carregarClientes() {
    FILE* fp = fopen("clientes.bin", "rb");
    if (!fp) return;

    int total = 0;
    fread(&total, sizeof(int), 1, fp);

    clientes.resize(total);
    for (int i = 0; i < total; i++)
        clientes[i].lerBinario(fp);

    fclose(fp);
}

// Função para verificar se cliente já existe por código
bool clienteExiste(int codigoBusca) {
    for (const auto& c : clientes) {
        if (c.getCodigo() == codigoBusca)
            return true;
    }
    return false;
}

// Funções auxiliares para validação de dados
string lerEndereco() {
    // Regex flexível:
    // ^Rua <nomeRua> (opcional espaço) (numero com ou sem parênteses) , (opcional "Bairro ") <nomeBairro> , (opcional "Cidade ") <nomeCidade>$
    // Permite letras acentuadas (intervalo À-ÿ), números e espaços.
    regex padrao(R"(^Rua\s+[A-Za-zÀ-ÿ0-9 ]+\s*\(?[0-9]+\)?\s*,\s*(?:Bairro\s*)?[A-Za-zÀ-ÿ0-9 ]+\s*,\s*(?:Cidade\s*)?[A-Za-zÀ-ÿ0-9 ]+$)");

    string endereco;
    while (true) {
        cout << amareloBold << "   ➤ Endereço" << reset << " " << azul << "(ex.: Rua Martinica (432), Santa Branca, Belo Horizonte): " << reset;
        getline(cin, endereco);

        if (regex_match(endereco, padrao)) {
            return endereco; // válido
        } else {
            cout << vermelhoBold << "   ✖ Endereço inválido!" << reset << "\n";
            cout << amarelo << "   Use o formato: Rua Nome (numero), Bairro Nome, Cidade Nome" << reset << "\n";
            cout << amarelo << "   Ou: Rua Nome (numero), NomeDoBairro, NomeDaCidade" << reset << "\n\n";
        }
    }
}

string lerTelefone() {
    regex padrao("^31 9[0-9]{4}-[0-9]{4}$"); // formato exato
    string telefone;

    while (true) {
        cout << amareloBold << "   ➤ Telefone" << reset << " " << azul << "(formato 31 9xxxx-xxxx): " << reset;
        getline(cin, telefone);

        if (regex_match(telefone, padrao)) {
            return telefone; // válido
        } else {
            cout << vermelhoBold << "   ✖ Telefone inválido!" << reset << "\n";
            cout << amarelo << "   Siga o formato: 31 9xxxx-xxxx" << reset << "\n\n";
        }
    }
}

class Estadia{
    int codigoEstadia;
    int codigoCliente;
    int quantidadeDiarias;
    int quantidadeHospedes;
    int numQuarto;
    string dataEntrada;
    string dataSaida;

    public:
    Estadia(){
        codigoEstadia = gerarCodigos();
        codigoCliente = 0;
        quantidadeDiarias = 0;
        quantidadeHospedes = 0;
        numQuarto = 0;
        dataEntrada = "";
        dataSaida = "";
    }

    // Definição dos metodos Setters
    void setCodigoEstadia(int code){codigoEstadia = code;}
    void setCodigoCliente(int code){codigoCliente = code;}
    void setQuantidadeDiarias(int quant) {quantidadeDiarias = quant;}
    void setNumQuarto(int num) {numQuarto = num;}
    void setDataEntrada(string checkin) {dataEntrada = checkin;}
    void setDataSaida(string checkout) {dataSaida = checkout;}
    void setQuantidadeHospedes(int quant) {quantidadeHospedes = quant;}

    //Definição dos metodos Getters
    int getCodigoCliente() const {return codigoCliente;}
    int getCodigoEstadia() const {return codigoEstadia;}
    int getQuantidadeDiarias() const {return quantidadeDiarias;}
    int getQuantidadeHospedes() const {return quantidadeHospedes;}
    int getNumQuarto() const {return numQuarto;}
    string getDataEntrada() const {return dataEntrada;}
    string getDataSaida() const {return dataSaida;}

};

// CLASSE QUARTO ATUALIZADA (Eduardo)
class Quarto{
private:
    int numQuarto;
    int quantidadeHospedes;
    float valorDiaria;
    string status;
    string descricao;

public:
    // Construtor padrão
    Quarto() : numQuarto(0), quantidadeHospedes(0), valorDiaria(0.0), status("desocupado"), descricao("") {}

    // Getters
    int getNumQuarto() const { return numQuarto; }
    int getQuantidadeHospedes() const { return quantidadeHospedes; }
    float getValorDiaria() const { return valorDiaria; }
    string getStatus() const { return status; }
    string getDescricao() const { return descricao; }

    // Setters
    void setNumQuarto(int num) { numQuarto = num; }
    void setQuantidadeHospedes(int hospedes) { quantidadeHospedes = hospedes; }
    void setValorDiaria(float diaria) { valorDiaria = diaria; }
    void setStatus(string stat) { status = stat; }
    void setDescricao(string desc) { descricao = desc; }

    // Método para exibir informações do quarto
    void exibirInfo() const {
        cout << azulBold << "   ┌─ Quarto " << numQuarto << " ───────────────────────────────────────────────────────────────┐" << reset << "\n";
        cout << "   │ " << ciano << "Capacidade: " << reset << verdeBold << quantidadeHospedes << " hóspedes" << reset;
        // Ajustar espaçamento
        int espacos = 68 - ((int)to_string(quantidadeHospedes).length() + 10);
        if (espacos < 0) espacos = 0;
        for (int i = 0; i < espacos; i++) cout << " ";
        cout << "│\n";
        cout << "   │ " << ciano << "Diária: " << reset << verdeBold << "R$ " << valorDiaria << reset;
        // Calcular espaços considerando o valor formatado
        string valorStr = to_string((int)valorDiaria);
        espacos = 68 - (valorStr.length() + 7);
        if (espacos < 0) espacos = 0;
        for (int i = 0; i < espacos; i++) cout << " ";
        cout << "│\n";
        cout << "   │ " << ciano << "Status: " << reset << (status == "desocupado" ? verdeBold : vermelhoBold) << status << reset;
        espacos = 68 - (int)status.length() - 8;
        for (int i = 0; i < espacos; i++) cout << " ";
        cout << "│\n";
        if (!descricao.empty()) {
            cout << "   │ " << ciano << "Descrição: " << reset << amarelo << descricao << reset;
            espacos = 68 - (int)descricao.length() - 11;
            if (espacos < 0) espacos = 0;
            for (int i = 0; i < espacos; i++) cout << " ";
            cout << "│\n";
        }
        cout << azulBold << "   └────────────────────────────────────────────────────────────────────────────┘" << reset << "\n";
    }

    // Método para verificar se está disponível para X hóspedes
    bool disponivelPara(int numHospedes) const {
        return (status == "desocupado" && quantidadeHospedes >= numHospedes);
    }
};

// Vetores para as classes correspondentes

vector<Estadia> estadias;
vector <Quarto> quartos;

// Função main para controle de fluxo
int main()
{

    SetConsoleOutputCP(CP_UTF8); // Permite a utilização de simbolos especiais nas entradas
    SetConsoleCP(CP_UTF8);       // Permite a utilização de simbolos especiais nas saidas

    // 1. Inicializar o gerador (Seed)
    srand(time(0));
    // Inicializar quartos fixos (se não existirem)
    inicializarQuartosFixos();
    // Carregar dados do arquivo binário ao iniciar
    carregarEstadias();
    carregarFuncionarios();
    carregarClientes();
    menu();
    // Salvar dados no arquivo binário ao encerrar
    salvarEstadias();
    salvarTodosFuncionarios();
    salvarTodosClientes();

    return 0;
}

// FUNÇÕES EXISTENTES (do grupo)

// Função para limpar a tela
void limparTela()
{
    system("cls");
}

int gerarCodigos()
{
    // Definindo o intervalo [MIN, MAX]
    const int MIN = 10000;
    const int MAX = 99999;

    // Formula: (rand() % (MAX - MIN + 1)) + MIN
    // Isso garante que o numero gerado esteja sempre entre 10000 e 99999.
    int numeroAleatorio = (rand() % (MAX - MIN + 1)) + MIN;

    return numeroAleatorio;
}

// Função para exibição do menu e controle principal de ações
void menu()
{
    int option = 0;
    while (option != 9)
    {
        limparTela();
        cout << "╔══════════════════════════════════════════════════════╗\n";
        cout << "║" << cianoBold << "         🏨 HOTEL DESCANSO GARANTIDO 🏨               " << reset << "║\n";
        cout << "╚══════════════════════════════════════════════════════╝\n\n";
        cout << verdeBold << "              ════════════════════════\n";
        cout << "              ⬇︎    MENU PRINCIPAL    ⬇︎\n";
        cout << "              ════════════════════════\n\n" << reset;
        cout << amareloBold << "   SELECIONE UMA OPÇÃO:\n\n" << reset;
        cout << "   ┌─────────────────────────────────────────────────┐\n";
        cout << "   │ " << azulBold << "1" << reset << "  " << azul << "👤  Cadastrar Cliente" << reset << "                        │\n";
        cout << "   ├─────────────────────────────────────────────────┤\n";
        cout << "   │ " << azulBold << "2" << reset << "  " << ciano << "👔  Cadastrar Funcionário" << reset << "                    │\n";
        cout << "   ├─────────────────────────────────────────────────┤\n";
        cout << "   │ " << azulBold << "3" << reset << "  " << verde << "🏨  Cadastrar Estadia" << reset << "                        │\n";
        cout << "   ├─────────────────────────────────────────────────┤\n";
        cout << "   │ " << azulBold << "4" << reset << "  " << roxo << "🛏️  Cadastrar Quarto" << reset << "                         │\n";
        cout << "   ├─────────────────────────────────────────────────┤\n";
        cout << "   │ " << azulBold << "5" << reset << "  " << amarelo << "🔍  Filtrar Clientes" << reset << "                         │\n";
        cout << "   ├─────────────────────────────────────────────────┤\n";
        cout << "   │ " << azulBold << "6" << reset << "  " << amarelo << "🔍  Filtrar Funcionários" << reset << "                     │\n";
        cout << "   ├─────────────────────────────────────────────────┤\n";
        cout << "   │ " << azulBold << "7" << reset << "  " << cianoBold << "📋  Listar Todas as Estadias" << reset << "                 │\n";
        cout << "   ├─────────────────────────────────────────────────┤\n";
        cout << "   │ " << azulBold << "8" << reset << "  " << roxoBold << "💰  Dar Baixa em Estadia" << reset << "                     │\n";
        cout << "   ├─────────────────────────────────────────────────┤\n";
        cout << "   │ " << azulBold << "9" << reset << "  " << vermelho << "🚪  Fechar Programa" << reset << "                          │\n";
        cout << "   └─────────────────────────────────────────────────┘\n\n";
        cout << amareloBold << "   Digite sua opção" << reset << amarelo << " e pressione ENTER: " << reset;
        cin >> option;

        switch (option)
        {
        case 1:
            cadastrarClientes();
            break;
        case 2:
            cadastrarFuncionarios();
            break;
        case 3:
            cadastrarEstadia();
            break;
        case 4:
            cadastrarQuarto();
            break;
        case 5:
            filtrarClientes();
            break;
        case 6:
            filtrarFuncionarios();
            break;
        case 7:
            listarTodasEstadias();
            break;
        case 8:
            darBaixaEstadia();
            break;
        case 9:
            limparTela();
            cout << "╔══════════════════════════════════════════════════════╗\n";
            cout << "║" << cianoBold << "         🏨 HOTEL DESCANSO GARANTIDO 🏨               " << reset << "║\n";
            cout << "╚══════════════════════════════════════════════════════╝\n\n";
            cout << verdeBold << "   ✨ Obrigado por utilizar o sistema! ✨\n\n" << reset;
            cout << amarelo << "   Encerrando programa";
            for (int i = 0; i < 3; i++)
            {
                cout << ".";
                Sleep(1000);
            }
            cout << reset << "\n";
            break;
        default:
            limparTela();
            cout << "╔══════════════════════════════════════════════════════╗\n";
            cout << "║" << vermelhoBold << "              ⚠ OPÇÃO INVÁLIDA!⚠                     " << reset << "║\n";
            cout << "╚══════════════════════════════════════════════════════╝\n\n";
            cout << amarelo << "   ⏳ Retornando ao menu principal";
            for (int i = 0; i < 3; i++)
            {
                cout << ".";
                Sleep(1000);
            }
            cout << reset << "\n";
            limparTela();
            break;
        }
    }
}

// Função para cadastro de novos clientes
void cadastrarClientes(){
    limparTela();
    int option = 0;

    while (true)
    {
        cout << "╔══════════════════════════════════════════════════════╗\n";
        cout << "║" << cianoBold << "        👤 CADASTRO DE CLIENTES 👤                    " << reset << "║\n";
        cout << "╚══════════════════════════════════════════════════════╝\n\n";
        cout << verdeBold << "   SELECIONE UMA OPÇÃO:\n\n" << reset;
        cout << verde << "   ➤  1" << reset << " - " << ciano << "Cadastrar novo cliente" << reset << endl;
        cout << vermelho << "   ➤  2" << reset << " - " << ciano << "Voltar ao menu principal" << reset << endl;
        cout << "\n" << amareloBold << "   Digite sua opção" << reset << amarelo << " e pressione ENTER: " << reset;
        cin >> option;
        cout << "\n";

        switch (option)
        {
        case 1:
        {
            limparTela();
            cin.ignore();
            cout << "╔══════════════════════════════════════════════════════╗\n";
            cout << "║" << cianoBold << "        👤 CADASTRO DE CLIENTE 👤                     " << reset << "║\n";
            cout << "╚══════════════════════════════════════════════════════╝\n\n";

            // Criar novo cliente (garantir que pontos de fidelidade sejam 0)
            Cliente novoCliente;

            // Geração do código
            cout << amareloBold << "   🔄 Gerando código único do cliente" << reset;
            int codigoGerado = gerarCodigos();

            // Verificar se o código já existe
            while (clienteExiste(codigoGerado)) {
                codigoGerado = gerarCodigos();
            }

            for (int i = 0; i < 3; i++) {
                cout << amarelo << ".";
                Sleep(1000);
            }
            cout << reset << "\n";
            novoCliente.setCodigo(codigoGerado);
            novoCliente.setPontosFidelidade(0); // Garantir que começa com 0 pontos
            cout << verdeBold << "   ✔ Código gerado com sucesso!" << reset << "\n";
            cout << azulBold << "   📋 Código do Cliente: " << reset << verde << codigoGerado << reset << "\n\n";

            // Nome
            cout << "   ──────────────────────────────────────────────\n";
            cout << cianoBold << "   👤 INFORMAÇÕES PESSOAIS\n" << reset;
            cout << amarelo << "   ➤ Nome completo: " << reset;
            string nome;
            getline(cin, nome);
            novoCliente.setNome(nome);

            // Endereço com validação
            novoCliente.setEndereco(lerEndereco());

            // Telefone com validação
            novoCliente.setTelefone(lerTelefone());

            // Adicionar cliente ao vetor
            clientes.push_back(novoCliente);

            // Salvar no arquivo
            salvarTodosClientes();

            // Mensagem de sucesso
            limparTela();
            cout << "╔══════════════════════════════════════════════════════╗\n";
            cout << "║" << verdeBold << "     ✅ CLIENTE CADASTRADO COM SUCESSO! ✅            " << reset << "║\n";
            cout << "╚══════════════════════════════════════════════════════╝\n\n";
            cout << "   ┌─────────────────────────────────────────────┐\n";
            cout << "   │" << cianoBold << "              📋 RESUMO DO CADASTRO            "<< reset <<"│\n";
            cout << "   ├─────────────────────────────────────────────┤\n";
            cout << "   │" << azul << "   Código:           " << reset << verdeBold << novoCliente.getCodigo() << reset;
            int espacos = 25 - (int)to_string(novoCliente.getCodigo()).length();
            for (int i = 0; i < espacos; i++) cout << " ";
            cout << "│\n";
            cout << "   │" << azul << "   Nome:             " << reset << verde << novoCliente.getNome() << reset;
            espacos = 25 - (int)novoCliente.getNome().length();
            if (espacos < 0) espacos = 0;
            for (int i = 0; i < espacos; i++) cout << " ";
            cout << "│\n";
            cout << "   │" << azul << "   Telefone:         " << reset << verde << novoCliente.getTelefone() << reset;
            espacos = 25 - (int)novoCliente.getTelefone().length();
            if (espacos < 0) espacos = 0;
            for (int i = 0; i < espacos; i++) cout << " ";
            cout << "│\n";
            cout << "   │" << azul << "   Endereço:         " << reset << verde << novoCliente.getEndereco() << reset;
            espacos = 25 - (int)novoCliente.getEndereco().length();
            if (espacos < 0) espacos = 0;
            for (int i = 0; i < espacos; i++) cout << " ";
            cout << "│\n";
            cout << "   └─────────────────────────────────────────────┘\n\n";
            cout << amareloBold << "   Deseja cadastrar outro cliente? (s/n): " << reset;
            char continuar;
            cin >> continuar;
            limparBuffer();
            if (tolower(continuar) != 's') {
                limparTela();
                break;
            }
            limparTela();
            continue;
        }

        case 2:
            limparTela();
            return;

        default:
            limparTela();
            cout << "╔══════════════════════════════════════════════════════╗\n";
            cout << "║" << vermelhoBold << "          ⚠ OPÇÃO INVÁLIDA! ⚠                     " << reset << "║\n";
            cout << "╚══════════════════════════════════════════════════════╝\n\n";
            cout << amarelo << "   ⏳ Retornando ao menu de cadastro";
            for (int i = 0; i < 3; i++) {
                cout << ".";
                Sleep(1000);
            }
            cout << reset << "\n";
            limparTela();
            break;
        }
    }
}

// Função para cadastro de novos funcionários (ATUALIZADA)
void cadastrarFuncionarios()
{
    limparTela();
    int option = 0;
    Funcionario novoFuncionario;

    while (true)
    {
        cout << "╔══════════════════════════════════════════════════════╗\n";
        cout << "║" << cianoBold << "        👔 CADASTRO DE FUNCIONÁRIOS 👔                  " << reset << "║\n";
        cout << "╚══════════════════════════════════════════════════════╝\n\n";
        cout << verdeBold << "   SELECIONE UMA OPÇÃO:\n\n" << reset;
        cout << verde << "   ➤  1" << reset << " - " << ciano << "Cadastrar novo funcionário" << reset << endl;
        cout << vermelho << "   ➤  2" << reset << " - " << ciano << "Voltar ao menu principal" << reset << endl;
        cout << "\n" << amareloBold << "   Digite sua opção" << reset << amarelo << " e pressione ENTER: " << reset;
        cin >> option;
        cout << "\n";

        switch (option)
        {
        case 1:
        {
            limparTela();
            cin.ignore();
            cout << "╔══════════════════════════════════════════════════════╗\n";
            cout << "║" << cianoBold << "        👔 CADASTRO DE FUNCIONÁRIO 👔               " << reset << "║\n";
            cout << "╚══════════════════════════════════════════════════════╝\n\n";

            // Geração do código
            cout << amareloBold << "   🔄 Gerando código único do funcionário" << reset;
            int codigoGerado = gerarCodigos();

            // Verificar se o código já existe
            while (funcionarioExiste(codigoGerado)) {
                codigoGerado = gerarCodigos();
            }

            for (int i = 0; i < 3; i++) {
                cout << amarelo << ".";
                Sleep(1000);
            }
            cout << reset << "\n";
            novoFuncionario.setCodigo(codigoGerado);
            cout << verdeBold << "   ✔ Código gerado com sucesso!" << reset << "\n";
            cout << azulBold << "   📋 Código do Funcionário: " << reset << verde << codigoGerado << reset << "\n\n";

            // Nome
            cout << "   ──────────────────────────────────────────────\n";
            cout << cianoBold << "   👤 INFORMAÇÕES PESSOAIS\n" << reset;
            cout << amarelo << "   ➤ Nome completo: " << reset;
            string nome;
            getline(cin, nome);
            novoFuncionario.setNome(nome);

            // Telefone
            cout << amarelo << "   ➤ Telefone: " << reset;
            string telefone;
            getline(cin, telefone);
            novoFuncionario.setTelefone(telefone);

            // Cargo
            cout << "\n   ──────────────────────────────────────────────\n";
            cout << cianoBold << "   💼 INFORMAÇÕES PROFISSIONAIS\n" << reset;
            cout << amarelo << "   ➤ Cargo: " << reset;
            string cargo;
            getline(cin, cargo);
            novoFuncionario.setCargo(cargo);

            // Salário
            bool salarioValido = false;
            float salario;
            while (!salarioValido) {
                cout << amarelo << "   ➤ Salário (R$): " << reset;
                if (!(cin >> salario)) {
                    cout << vermelhoBold << "   ✖ Erro: Digite um valor válido!\n" << reset;
                    limparBuffer();
                    continue;
                }
                if (salario <= 0) {
                    cout << vermelhoBold << "   ✖ Erro: O salário deve ser positivo!\n" << reset;
                } else if (salario > 100000) {
                    cout << vermelhoBold << "   ✖ Erro: Valor muito alto (máximo: R$ 100.000)!\n" << reset;
                } else {
                    salarioValido = true;
                    novoFuncionario.setSalario(salario);
                }
            }
            limparBuffer();

            // Adicionar funcionário ao vetor
            funcionarios.push_back(novoFuncionario);

            // Salvar no arquivo
            salvarTodosFuncionarios();

            // Mensagem de sucesso
            limparTela();
            cout << "╔══════════════════════════════════════════════════════╗\n";
            cout << "║" << verdeBold << "     ✅ FUNCIONÁRIO CADASTRADO COM SUCESSO! ✅         " << reset << "║\n";
            cout << "╚══════════════════════════════════════════════════════╝\n\n";
            cout << "   ┌─────────────────────────────────────────────┐\n";
            cout << "   │" << cianoBold << "              📋 RESUMO DO CADASTRO            "<< reset <<"│\n";
            cout << "   ├─────────────────────────────────────────────┤\n";
            cout << "   │" << azul << "   Código:           " << reset << verdeBold << novoFuncionario.getCodigo() << reset;
            int espacos = 25 - (int)to_string(novoFuncionario.getCodigo()).length();
            for (int i = 0; i < espacos; i++) cout << " ";
            cout << "│\n";
            cout << "   │" << azul << "   Nome:             " << reset << verde << novoFuncionario.getNome() << reset;
            espacos = 25 - (int)novoFuncionario.getNome().length();
            if (espacos < 0) espacos = 0;
            for (int i = 0; i < espacos; i++) cout << " ";
            cout << "│\n";
            cout << "   │" << azul << "   Telefone:         " << reset << verde << novoFuncionario.getTelefone() << reset;
            espacos = 25 - (int)novoFuncionario.getTelefone().length();
            if (espacos < 0) espacos = 0;
            for (int i = 0; i < espacos; i++) cout << " ";
            cout << "│\n";
            cout << "   │" << azul << "   Cargo:            " << reset << verde << novoFuncionario.getCargo() << reset;
            espacos = 25 - (int)novoFuncionario.getCargo().length();
            if (espacos < 0) espacos = 0;
            for (int i = 0; i < espacos; i++) cout << " ";
            cout << "│\n";
            cout << "   │" << azul << "   Salário:          " << reset << verdeBold << "R$ " << fixed << setprecision(2) << novoFuncionario.getSalario() << reset;
            int tamanhoSalario = (novoFuncionario.getSalario() >= 10000) ? 9 : ((novoFuncionario.getSalario() >= 1000) ? 8 : 7);
            espacos = 25 - tamanhoSalario;
            if (espacos < 0) espacos = 0;
            for (int i = 0; i < espacos; i++) cout << " ";
            cout << "│\n";
            cout << "   └─────────────────────────────────────────────┘\n\n";
            cout << amareloBold << "   Deseja cadastrar outro funcionário? (s/n): " << reset;
            char continuar;
            cin >> continuar;
            limparBuffer();
            if (tolower(continuar) != 's') {
                limparTela();
                break;
            }
            limparTela();
            continue;
        }

        case 2:
            limparTela();
            return;

        default:
            limparTela();
            cout << "╔══════════════════════════════════════════════════════╗\n";
            cout << "║" << vermelhoBold << "          ⚠ OPÇÃO INVÁLIDA! ⚠                     " << reset << "║\n";
            cout << "╚══════════════════════════════════════════════════════╝\n\n";
            cout << amarelo << "   ⏳ Retornando ao menu de cadastro";
            for (int i = 0; i < 3; i++) {
                cout << ".";
                Sleep(1000);
            }
            cout << reset << "\n";
            limparTela();
            break;
        }
    }
}

// Função para cadastro de nova estadia (EXISTENTE - Italo)
void cadastrarEstadia()
{
    limparTela();
    int option = 0;
    Estadia novaEstadia;

    while (true)
    {
        cout << "╔══════════════════════════════════════════════════════╗\n";
        cout << "║" << cianoBold << "          🏨 CADASTRO DE ESTÁDIA 🏨               " << reset << "║\n";
        cout << "╚══════════════════════════════════════════════════════╝\n\n";
        cout << azulBold << "   SELECIONE UMA OPÇÃO:\n\n" << reset;
        cout << verde << "   ➤  1" << reset << " - " << ciano << "Cadastrar nova estadia" << reset << endl;
        cout << vermelho << "   ➤  2" << reset << " - " << ciano << "Voltar ao menu principal" << reset << endl;
        cout << "\n" << amareloBold << "   Digite sua opção" << reset << amarelo << " e pressione ENTER: " << reset;
        cin >> option;

        switch(option){
            case 1: {
                int estadiaCode;
                int clienteCode;
                bool valido;
                int quantHospedes;
                int  quartoID;
                int diarias;
                string checkin;
                string checkout;
                limparTela();
                cout << "╔══════════════════════════════════════════════════════╗\n";
                cout << "║" << cianoBold << "        📝 NOVA ESTADIA - PREENCHIMENTO           " << reset << "║\n";
                cout << "╚══════════════════════════════════════════════════════╝\n\n";

                // Geração do código
                cout << amareloBold << "   🔄 Gerando código único da estadia" << reset;
                estadiaCode = novaEstadia.getCodigoEstadia();
                for (int i = 0; i < 3; i++){
                    cout << amarelo << ".";
                    Sleep(1000);
                }
                cout << reset << "\n";
                // Incrementar uma função que verifica se o código que foi gerado existe no arquivo binario com a tag (códigoEstadia)
                // valido = validaCodigoEstadia(estadiaCode);
                cout << verdeBold << "   ✔ Código gerado com sucesso!" << reset << "\n";
                cout << azulBold << "   📋 Código da Estadia: " << reset << verde << estadiaCode << reset << "\n\n";

                // Código do cliente
                cout << "   ──────────────────────────────────────────────\n";
                cout << cianoBold << "   👤 INFORMAÇÕES DO CLIENTE\n" << reset;
                cout << amarelo << "   Informe o código do cliente: " << reset;
                cin >> clienteCode;
                cin.ignore(10000, '\n');
                cout << amareloBold << "   🔍 Verificando código do cliente" << reset;
                for (int i = 0; i < 3; i++){
                    cout << amarelo << ".";
                    Sleep(1000);
                }
                cout << reset << "\n";
                // Verificar se o código do cliente existe
                valido = validaCodigoCliente(clienteCode);
                if (valido == true){
                    cout << verdeBold << "   ✔ Código do cliente validado com sucesso!" << reset << "\n\n";

                    // Buscar e exibir dados completos do cliente
                    Cliente* clienteEncontrado = consultarClientePorCodigo(clienteCode);
                    if (clienteEncontrado != nullptr) {
                        cout << amareloBold << "   📋 Dados do cliente encontrado:" << reset << "\n";
                        exibirDadosCliente(*clienteEncontrado);
                        cout << "\n";
                    }

                    novaEstadia.setCodigoCliente(clienteCode);
                    clienteCode = novaEstadia.getCodigoCliente();
                }else{
                    cout << vermelhoBold << "   ✖ Código do cliente não encontrado!" << reset << "\n";
                    cout << amarelo << "   Deseja tentar novamente? (s/n): " << reset;
                    char tentarNovamente;
                    cin >> tentarNovamente;
                    limparBuffer();
                    if (tolower(tentarNovamente) == 's') {
                        continue; // Volta para o início do loop
                    } else {
                        break; // Volta ao menu
                    }
                }

                // Quantidade de hóspedes
                cout << "   ──────────────────────────────────────────────\n";
                cout << cianoBold << "   👥 QUANTIDADE DE HÓSPEDES\n" << reset;
                cout << amarelo << "   Defina a quantidade de hóspedes" << reset << " " << azul << "(máximo 4): " << reset;
                cin >> quantHospedes;
                cin.ignore(10000, '\n');
                if (quantHospedes <= 4 && quantHospedes > 0){
                    novaEstadia.setQuantidadeHospedes(quantHospedes);
                    cout << verdeBold << "   ✔ Quantidade de hóspedes registrada: " << reset << verde << quantHospedes << reset << "\n\n";
                }else{
                    cout << vermelhoBold << "   ✖ Quantidade inválida!" << reset << "\n";
                    cout << vermelho << "   ⚠ O valor deve ser entre 1 e 4 hóspedes por quarto." << reset << "\n";
                    cout << amarelo << "   Deseja tentar novamente? (s/n): " << reset;
                    char tentarNovamente;
                    cin >> tentarNovamente;
                    limparBuffer();
                    if (tolower(tentarNovamente) == 's') {
                        continue; // Volta para o início do loop
                    } else {
                        limparTela();
                        return; // Volta ao menu
                    }
                }

                // Datas
                cout << "   ──────────────────────────────────────────────\n";
                cout << cianoBold << "   📅 DATAS DA ESTADIA\n" << reset;
                cout << amarelo << "   Data de check-in" << reset << " " << azul << "(DD/MM/AAAA): " << reset;
                getline(cin, checkin);
                novaEstadia.setDataEntrada(checkin);
                cout << amarelo << "   Data de checkout" << reset << " " << azul << "(DD/MM/AAAA): " << reset;
                getline(cin, checkout);
                novaEstadia.setDataSaida(checkout);

                // Calcular quantidade de diárias
                cout << amareloBold << "   🧮 Calculando quantidade de diárias" << reset;
                for (int i = 0; i < 3; i++){
                    cout << amarelo << ".";
                    Sleep(1000);
                }
                cout << reset << "\n";
                diarias = calcularDiarias(checkin, checkout);
                if (diarias > 0) {
                    novaEstadia.setQuantidadeDiarias(diarias);
                    cout << verdeBold << "   ✔ Quantidade de diárias calculada: " << reset << verde << diarias << " diária(s)" << reset << "\n\n";
                } else {
                    cout << vermelhoBold << "   ✖ Erro ao calcular diárias!" << reset << "\n";
                    cout << vermelho << "   ⚠ Verifique se as datas estão corretas (formato DD/MM/AAAA)." << reset << "\n";
                    cout << amarelo << "   Deseja tentar novamente? (s/n): " << reset;
                    char tentarNovamente;
                    cin >> tentarNovamente;
                    limparBuffer();
                    if (tolower(tentarNovamente) == 's') {
                        continue; // Volta para o início do loop
                    } else {
                        break; // Volta ao menu
                    }
                }

                // Quartos
                cout << "   ──────────────────────────────────────────────\n";
                cout << cianoBold << "   🛏️  SELEÇÃO DE QUARTO\n" << reset;
                cout << amareloBold << "   🔍 Verificando quartos disponíveis" << reset;
                for (int i = 0; i < 3; i++){
                    cout << amarelo << ".";
                    Sleep(1000);
                }
                cout << reset << "\n";

                // Buscar quartos disponíveis para o período e capacidade
                vector<Quarto> quartosDisponiveis = obterQuartosDisponiveisPorPeriodo(quantHospedes, checkin, checkout);

                if (quartosDisponiveis.empty()) {
                    cout << vermelhoBold << "   ✖ Nenhum quarto disponível!" << reset << "\n";
                    cout << vermelho << "   ⚠ Não há quartos disponíveis para " << quantHospedes << " hóspede(s) no período de " << checkin << " a " << checkout << reset << "\n";
                    cout << amarelo << "   💡 Tente alterar as datas ou a quantidade de hóspedes." << reset << "\n";
                    cout << amarelo << "   Deseja tentar novamente? (s/n): " << reset;
                    char tentarNovamente;
                    cin >> tentarNovamente;
                    limparBuffer();
                    if (tolower(tentarNovamente) == 's') {
                        continue; // Volta para o início do loop
                    } else {
                        limparTela();
                        return; // Volta ao menu
                    }
                }

                // Exibir quartos disponíveis
                limparTela();
                cout << "╔══════════════════════════════════════════════════════╗\n";
                cout << "║" << cianoBold << "        🛏️  QUARTOS DISPONÍVEIS 🛏️                " << reset << "║\n";
                cout << "╚══════════════════════════════════════════════════════╝\n\n";
                cout << verdeBold << "   Período: " << reset << checkin << " até " << checkout << "\n";
                cout << verdeBold << "   Hóspedes: " << reset << quantHospedes << "\n";
                cout << verdeBold << "   Diárias: " << reset << diarias << "\n\n";
                cout << amareloBold << "   Quartos disponíveis para sua estadia:\n" << reset;
                cout << "   ════════════════════════════════════════════════════\n\n";

                for (size_t i = 0; i < quartosDisponiveis.size(); i++) {
                    cout << azulBold << "   [" << (i + 1) << "] " << reset;
                    quartosDisponiveis[i].exibirInfo();
                    cout << "\n";
                }

                // Solicitar seleção do quarto
                int opcaoQuarto = 0;
                cout << amareloBold << "   ➤ Selecione o número do quarto desejado (1-" << quartosDisponiveis.size() << "): " << reset;
                cin >> opcaoQuarto;
                cin.ignore(10000, '\n');

                if (opcaoQuarto >= 1 && opcaoQuarto <= (int)quartosDisponiveis.size()) {
                    quartoID = quartosDisponiveis[opcaoQuarto - 1].getNumQuarto();
                    novaEstadia.setNumQuarto(quartoID);
                    cout << verdeBold << "\n   ✔ Quarto selecionado: " << reset << verde << quartoID << reset << "\n";
                    cout << verde << "   Descrição: " << quartosDisponiveis[opcaoQuarto - 1].getDescricao() << reset << "\n";
                    cout << verde << "   Valor total da estadia: R$ " << (quartosDisponiveis[opcaoQuarto - 1].getValorDiaria() * diarias) << reset << "\n\n";
                } else {
                    cout << vermelhoBold << "\n   ✖ Opção inválida!" << reset << "\n";
                    cout << amarelo << "   Deseja tentar novamente? (s/n): " << reset;
                    char tentarNovamente;
                    cin >> tentarNovamente;
                    limparBuffer();
                    if (tolower(tentarNovamente) == 's') {
                        continue; // Volta para o início do loop
                    } else {
                        limparTela();
                        return; // Volta ao menu
                    }
                }

                // Adicionar estadia ao vetor
                estadias.push_back(novaEstadia);

                // Salvar estadias no arquivo binário
                salvarEstadias();

                // Resumo final
                limparTela();
                cout << "╔══════════════════════════════════════════════════════╗\n";
                cout << "║" << verdeBold << "     ✅ ESTADIA CADASTRADA COM SUCESSO! ✅            " << reset << "║\n";
                cout << "╚══════════════════════════════════════════════════════╝\n\n";
                cout << "   ┌─────────────────────────────────────────────┐\n";
                cout << "   │" << cianoBold << "              📋 RESUMO DA ESTADIA           "<< reset <<"│\n";
                cout << "   ├─────────────────────────────────────────────┤\n";
                cout << "   │" << azul << "   Código da Estadia: " << reset << verdeBold << novaEstadia.getCodigoEstadia() << reset << "                  │\n";
                cout << "   │" << azul << "   Código do Cliente: " << reset << verdeBold << novaEstadia.getCodigoCliente() << reset << "                  │\n";
                cout << "   │" << azul << "   Check-in:          " << reset << verde << novaEstadia.getDataEntrada() << " às 14:00H" << reset << "   │\n";
                cout << "   │" << azul << "   Checkout:          " << reset << verde << novaEstadia.getDataSaida() << " às 12:00H"<< reset << "  │\n";
                cout << "   │" << azul << "   Diárias:           " << reset << verdeBold << novaEstadia.getQuantidadeDiarias() << " diária(s)" << reset;
                if (novaEstadia.getQuantidadeDiarias() < 10) cout << "            │\n";
                else if (novaEstadia.getQuantidadeDiarias() < 100) cout << "   │\n";
                else cout << "         │\n";
                cout << "   │" << azul << "   Hóspedes:          " << reset << verdeBold << novaEstadia.getQuantidadeHospedes() << reset;
                if (novaEstadia.getNumQuarto() > 0) {
                    cout << "         │\n";
                    cout << "   │" << azul << "   Quarto:            " << reset << verdeBold << novaEstadia.getNumQuarto() << reset << "                  │\n";
                } else {
                    cout << "                          │\n";
                }
                cout << "   └─────────────────────────────────────────────┘\n\n";
                cout << amareloBold << "   Deseja cadastrar outra estadia? (s/n): " << reset;
                char continuar;
                cin >> continuar;
                limparBuffer();
                if (tolower(continuar) != 's') {
                    limparTela();
                    break;
                }
                limparTela();
                continue;
            }

            case 2:
                limparTela();
                return;

            default:
                limparTela();
                cout << "╔══════════════════════════════════════════════════════╗\n";
                cout << "║" << vermelhoBold << "          ⚠ OPÇÃO INVÁLIDA! ⚠                     " << reset << "║\n";
                cout << "╚══════════════════════════════════════════════════════╝\n\n";
                cout << amarelo << "   ⏳ Retornando ao menu de cadastro";
                for (int i = 0; i < 3; i++){
                    cout << ".";
                    Sleep(1000);
                }
                cout << reset << "\n";
                limparTela();
                break;
        }
        limparTela();
    }
}

// Função para cadastro de novo quarto (ATUALIZADA - Eduardo)
void cadastrarQuarto() {
    limparTela();

    int opcaoMenu = 0;
    while (opcaoMenu != 6) {
        cout << "╔══════════════════════════════════════════════════════╗\n";
        cout << "║" << cianoBold << "           🛏️  GESTÃO DE QUARTOS 🛏️                  " << reset << "║\n";
        cout << "╚══════════════════════════════════════════════════════╝\n\n";
        cout << verdeBold << "   SELECIONE UMA OPÇÃO:\n\n" << reset;
        cout << verde << "   ➤  1" << reset << " - " << ciano << "Cadastrar novo quarto" << reset << endl;
        cout << verde << "   ➤  2" << reset << " - " << ciano << "Listar quartos disponíveis" << reset << endl;
        cout << verde << "   ➤  3" << reset << " - " << ciano << "Listar todos os quartos" << reset << endl;
        cout << verde << "   ➤  4" << reset << " - " << ciano << "Editar quarto" << reset << endl;
        cout << verde << "   ➤  5" << reset << " - " << ciano << "Excluir quarto" << reset << endl;
        cout << vermelho << "   ➤  6" << reset << " - " << ciano << "Voltar ao menu principal" << reset << endl;
        cout << "\n" << amareloBold << "   Digite sua opção: " << reset;

        if (!(cin >> opcaoMenu)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n" << vermelhoBold << "   ✖ Opção inválida. Digite um número." << reset << "\n";
            Sleep(2000);
            limparTela();
            continue;
        }
        limparBuffer();

        switch (opcaoMenu) {
            case 1: {
                // Cadastrar novo quarto
                int option = 0;
                while (option != 4) {
                    limparTela();
                    cout << cianoBold << "╔══════════════════════════════════════════════════════╗\n";
                    cout << "║              CADASTRO DE QUARTO           ║\n";
                    cout << "╚══════════════════════════════════════════════════════╝\n\n" << reset;

        Quarto novoQuarto;

        // Solicitar número do quarto com validação
        bool numeroValido = false;
        while (!numeroValido) {
            int numQuarto;
            cout << amareloBold << "➤ NUMERO DO QUARTO: " << reset;

            if (!(cin >> numQuarto)) {
                cout << vermelhoBold << "ERRO: Digite apenas numeros!\n" << reset;
                limparBuffer();
                continue;
            }

            if (numQuarto <= 0) {
                cout << vermelhoBold << "ERRO: O numero deve ser positivo!\n" << reset;
                continue;
            }

            if (numQuarto > 999) {
                cout << vermelhoBold << "ERRO: Numero muito alto (max: 999)!\n" << reset;
                continue;
            }

            if (quartoExiste(numQuarto)) {
                cout << vermelhoBold << "ERRO: Quarto " << numQuarto << " ja existe!\n" << reset;
                cout << amarelo << "   Deseja tentar outro numero? (s/n): " << reset;
                char opcao;
                cin >> opcao;
                limparBuffer();

                if (tolower(opcao) == 'n') {
                    cout << "\n" << amareloBold << "<- Retornando ao menu...\n" << reset;
                    Sleep(1500);
                    limparTela();
                    return;
                }
                cout << "\n";
            } else {
                numeroValido = true;
                novoQuarto.setNumQuarto(numQuarto);
            }
        }

        // Solicitar quantidade de hóspedes (MÁXIMO 4)
        bool quantidadeValida = false;
        int quantidadeHospedes;
        while (!quantidadeValida) {
            cout << amareloBold << "➤ CAPACIDADE MAXIMA DE HOSPEDES (1-4): " << reset;

            if (!(cin >> quantidadeHospedes)) {
                cout << vermelhoBold << "ERRO: Digite apenas numeros!\n" << reset;
                limparBuffer();
                continue;
            }

            if (quantidadeHospedes <= 0) {
                cout << vermelhoBold << "ERRO: Minimo 1 hospede!\n" << reset;
            } else if (quantidadeHospedes > 4) {
                cout << vermelhoBold << "ERRO: Capacidade maxima e 4 hospedes!\n" << reset;
            } else {
                quantidadeValida = true;
                novoQuarto.setQuantidadeHospedes(quantidadeHospedes);
            }
        }

        // Solicitar valor da diária
        bool valorValido = false;
        float valorDiaria;
        while (!valorValido) {
            cout << amareloBold << "➤ VALOR DA DIARIA R$: " << reset;

            if (!(cin >> valorDiaria)) {
                cout << vermelhoBold << "ERRO: Digite um valor valido!\n" << reset;
                limparBuffer();
                continue;
            }

            if (valorDiaria <= 0) {
                cout << vermelhoBold << "ERRO: Valor deve ser positivo!\n" << reset;
            } else if (valorDiaria > 10000) {
                cout << vermelhoBold << "ERRO: Valor muito alto (max: R$ 10.000)!\n" << reset;
            } else {
                valorValido = true;
                novoQuarto.setValorDiaria(valorDiaria);
            }
        }

        // Solicitar descrição
        limparBuffer();
        string descricao;
        cout << amareloBold << "➤ DESCRICAO DO QUARTO: " << reset;
        getline(cin, descricao);
        novoQuarto.setDescricao(descricao);

        // Status inicial
        novoQuarto.setStatus("desocupado");

        // Confirmação antes de salvar
        limparTela();
        cout << verdeBold << "╔══════════════════════════════════════════════════════╗\n";
        cout << "║               CONFIRMACAO DE DADOS        ║\n";
        cout << "╚══════════════════════════════════════════════════════╝\n\n" << reset;

        cout << azulBold << "   NUMERO DO QUARTO: " << brancoBold << novoQuarto.getNumQuarto() << "\n";
        cout << azulBold << "   CAPACIDADE MAXIMA: " << brancoBold << novoQuarto.getQuantidadeHospedes() << " hospedes\n";
        cout << azulBold << "   VALOR DA DIARIA: " << brancoBold << "R$ " << novoQuarto.getValorDiaria() << "\n";
        cout << azulBold << "   DESCRICAO: " << brancoBold << novoQuarto.getDescricao() << "\n";
        cout << azulBold << "   STATUS INICIAL: " << brancoBold << novoQuarto.getStatus() << "\n\n";

        cout << amareloBold << "➤ Confirmar cadastro? (s/n): " << reset;
        char confirmar;
        cin >> confirmar;
        limparBuffer();

        if (tolower(confirmar) == 's') {
            // Salvar no arquivo
            salvarQuartoNoArquivo(novoQuarto);

            cout << "\n" << verdeBold << "QUARTO CADASTRADO COM SUCESSO!\n\n" << reset;
            cout << verde << "   Numero: " << novoQuarto.getNumQuarto() << "\n";
            cout << "   Capacidade maxima: " << novoQuarto.getQuantidadeHospedes() << " hospedes\n";
            cout << "   Diaria: R$ " << novoQuarto.getValorDiaria() << "\n";
            cout << "   Descricao: " << novoQuarto.getDescricao() << "\n";
            cout << "   Status: " << novoQuarto.getStatus() << "\n\n";
        } else {
            cout << "\n" << amareloBold << "Cadastro cancelado pelo usuario.\n" << reset;
        }

                    limparTela();
                    // Menu de opções pós-cadastro
                    cout << "\n" << cianoBold << "╔══════════════════════════════════════════════════════╗\n";
                    cout << "║               O QUE DESEJA FAZER?         ║\n";
                    cout << "╚══════════════════════════════════════════════════════╝\n" << reset;
                    cout << verdeBold << "[1] " << reset << "Cadastrar outro quarto\n";
                    cout << amareloBold << "[2] " << reset << "Ver quartos disponiveis\n";
                    cout << roxoBold << "[3] " << reset << "Ver todos os quartos\n";
                    cout << vermelhoBold << "[4] " << reset << "Voltar ao menu de gestão\n";
                    cout << amareloBold << "➤ Opcao: " << reset;

                    cin >> option;
                    limparBuffer();

                    switch (option) {
                        case 1:
                            limparTela();
                            cout << verdeBold << "INICIANDO NOVO CADASTRO...\n\n" << reset;
                            break;
                        case 2:
                            listarQuartosDisponiveis();
                            break;
                        case 3:
                            listarTodosQuartos();
                            break;
                        case 4:
                            option = 4; // Sair do loop interno
                            break;
                        default:
                            cout << vermelhoBold << "Opcao invalida!\n" << reset;
                            Sleep(1000);
                            option = 1; // Para continuar no loop
                            break;
                    }
                }
                limparTela();
                break;
            }
            case 2:
                listarQuartosDisponiveis();
                limparTela();
                break;
            case 3:
                listarTodosQuartos();
                limparTela();
                break;
            case 4:
                editarQuarto();
                limparTela();
                break;
            case 5:
                excluirQuarto();
                limparTela();
                break;
            case 6:
                limparTela();
                break;
            default:
                limparTela();
                cout << "╔══════════════════════════════════════════════════════╗\n";
                cout << "║" << vermelhoBold << "          ⚠ OPÇÃO INVÁLIDA! ⚠                     " << reset << "║\n";
                cout << "╚══════════════════════════════════════════════════════╝\n\n";
                cout << amarelo << "   ⏳ Retornando ao menu";
                for (int i = 0; i < 3; i++){
                    cout << ".";
                    Sleep(1000);
                }
                cout << reset << "\n";
                limparTela();
                break;
        }
    }

    limparTela();
}

// Função para filtrar funcionários (ATUALIZADA)
void filtrarFuncionarios()
{
    limparTela();
    int option = 0;

    while (true)
    {
        cout << "╔══════════════════════════════════════════════════════╗\n";
        cout << "║" << cianoBold << "        🔍 PESQUISAR FUNCIONÁRIOS 🔍                  " << reset << "║\n";
        cout << "╚══════════════════════════════════════════════════════╝\n\n";
        cout << verdeBold << "   SELECIONE UMA OPÇÃO DE PESQUISA:\n\n" << reset;
        cout << verde << "   ➤  1" << reset << " - " << ciano << "Pesquisar por código" << reset << endl;
        cout << verde << "   ➤  2" << reset << " - " << ciano << "Pesquisar por nome" << reset << endl;
        cout << verde << "   ➤  3" << reset << " - " << ciano << "Pesquisar por cargo" << reset << endl;
        cout << verde << "   ➤  4" << reset << " - " << ciano << "Listar todos os funcionários" << reset << endl;
        cout << vermelho << "   ➤  5" << reset << " - " << ciano << "Voltar ao menu principal" << reset << endl;
        cout << "\n" << amareloBold << "   Digite sua opção" << reset << amarelo << " e pressione ENTER: " << reset;
        cin >> option;
        limparBuffer();

        switch (option)
        {
        case 1: // Pesquisar por código
        {
            limparTela();
            cout << "╔══════════════════════════════════════════════════════╗\n";
            cout << "║" << cianoBold << "        🔍 PESQUISA POR CÓDIGO 🔍                     " << reset << "║\n";
            cout << "╚══════════════════════════════════════════════════════╝\n\n";
            cout << amareloBold << "   ➤ Informe o código do funcionário: " << reset;
            int codigoBusca;
            cin >> codigoBusca;
            limparBuffer();

            cout << amareloBold << "   🔍 Buscando funcionário" << reset;
            for (int i = 0; i < 3; i++) {
                cout << amarelo << ".";
                Sleep(1000);
            }
            cout << reset << "\n\n";

            Funcionario* funcionario = consultarFuncionarioPorCodigo(codigoBusca);
            if (funcionario != nullptr) {
                limparTela();
                cout << "╔══════════════════════════════════════════════════════╗\n";
                cout << "║" << verdeBold << "     ✅ FUNCIONÁRIO ENCONTRADO! ✅            " << reset << "║\n";
                cout << "╚══════════════════════════════════════════════════════╝\n\n";
                funcionario->exibirDados();
                cout << "\n" << amareloBold << "   Deseja fazer outra pesquisa? (s/n): " << reset;
                char continuar;
                cin >> continuar;
                limparBuffer();
                if (tolower(continuar) != 's') {
                    limparTela();
                    break;
                }
                limparTela();
                continue;
            } else {
                cout << vermelhoBold << "   ✖ Funcionário não encontrado!" << reset << "\n";
                cout << vermelho << "   ⚠ Não existe nenhum funcionário com o código " << codigoBusca << reset << "\n";
                cout << amarelo << "   Deseja tentar novamente? (s/n): " << reset;
                char tentarNovamente;
                cin >> tentarNovamente;
                limparBuffer();
                if (tolower(tentarNovamente) == 's') {
                    limparTela();
                    continue;
                } else {
                    limparTela();
                    break;
                }
            }
        }

        case 2: // Pesquisar por nome
        {
            limparTela();
            cout << "╔══════════════════════════════════════════════════════╗\n";
            cout << "║" << cianoBold << "        🔍 PESQUISA POR NOME 🔍                       " << reset << "║\n";
            cout << "╚══════════════════════════════════════════════════════╝\n\n";
            cout << amareloBold << "   ➤ Informe o nome (ou parte do nome): " << reset;
            string nomeBusca;
            getline(cin, nomeBusca);

            // Converter para minúsculas para busca case-insensitive
            string nomeBuscaLower = nomeBusca;
            transform(nomeBuscaLower.begin(), nomeBuscaLower.end(), nomeBuscaLower.begin(), ::tolower);

            cout << amareloBold << "   🔍 Buscando funcionários" << reset;
            for (int i = 0; i < 3; i++) {
                cout << amarelo << ".";
                Sleep(1000);
            }
            cout << reset << "\n\n";

            vector<Funcionario> resultados;
            for (auto& f : funcionarios) {
                string nomeLower = f.getNome();
                transform(nomeLower.begin(), nomeLower.end(), nomeLower.begin(), ::tolower);
                if (nomeLower.find(nomeBuscaLower) != string::npos) {
                    resultados.push_back(f);
                }
            }

            limparTela();
            if (!resultados.empty()) {
                cout << "╔══════════════════════════════════════════════════════╗\n";
                cout << "║" << verdeBold << "        ✅ FUNCIONÁRIOS ENCONTRADOS! ✅            " << reset << "║\n";
                cout << "╚══════════════════════════════════════════════════════╝\n\n";
                cout << verdeBold << "   Total encontrado: " << reset << verde << resultados.size() << " funcionário(s)\n\n" << reset;
                cout << "   ═══════════════════════════════════════════════════════════════\n\n";
                for (size_t i = 0; i < resultados.size(); i++) {
                    cout << azulBold << "   [" << (i + 1) << "] " << reset;
                    resultados[i].exibirDados();
                    if (i < resultados.size() - 1) cout << "\n";
                }
            } else {
                cout << "╔══════════════════════════════════════════════════════╗\n";
                cout << "║" << vermelhoBold << "     ✖ NENHUM FUNCIONÁRIO ENCONTRADO! ✖            " << reset << "║\n";
                cout << "╚══════════════════════════════════════════════════════╝\n\n";
                cout << vermelho << "   ⚠ Não foi encontrado nenhum funcionário com o nome \"" << nomeBusca << "\"" << reset << "\n";
            }
            cout << "\n" << amareloBold << "   Deseja fazer outra pesquisa? (s/n): " << reset;
            char continuar;
            cin >> continuar;
            limparBuffer();
            if (tolower(continuar) != 's') {
                limparTela();
                break;
            }
            limparTela();
            continue;
        }

        case 3: // Pesquisar por cargo
        {
            limparTela();
            cout << "╔══════════════════════════════════════════════════════╗\n";
            cout << "║" << cianoBold << "                🔍 PESQUISA POR CARGO 🔍              " << reset << "║\n";
            cout << "╚══════════════════════════════════════════════════════╝\n\n";
            cout << amareloBold << "   ➤ Informe o cargo (ou parte do cargo): " << reset;
            string cargoBusca;
            getline(cin, cargoBusca);

            // Converter para minúsculas para busca case-insensitive
            string cargoBuscaLower = cargoBusca;
            transform(cargoBuscaLower.begin(), cargoBuscaLower.end(), cargoBuscaLower.begin(), ::tolower);

            cout << amareloBold << "   🔍 Buscando funcionários" << reset;
            for (int i = 0; i < 3; i++) {
                cout << amarelo << ".";
                Sleep(1000);
            }
            cout << reset << "\n\n";

            vector<Funcionario> resultados;
            for (auto& f : funcionarios) {
                string cargoLower = f.getCargo();
                transform(cargoLower.begin(), cargoLower.end(), cargoLower.begin(), ::tolower);
                if (cargoLower.find(cargoBuscaLower) != string::npos) {
                    resultados.push_back(f);
                }
            }

            limparTela();
            if (!resultados.empty()) {
                cout << "╔══════════════════════════════════════════════════════╗\n";
                cout << "║" << verdeBold << "        ✅ FUNCIONÁRIOS ENCONTRADOS! ✅            " << reset << "║\n";
                cout << "╚══════════════════════════════════════════════════════╝\n\n";
                cout << verdeBold << "   Total encontrado: " << reset << verde << resultados.size() << " funcionário(s)\n\n" << reset;
                cout << "   ═══════════════════════════════════════════════════════════════\n\n";
                for (size_t i = 0; i < resultados.size(); i++) {
                    cout << azulBold << "   [" << (i + 1) << "] " << reset;
                    resultados[i].exibirDados();
                    if (i < resultados.size() - 1) cout << "\n";
                }
            } else {
                cout << "╔══════════════════════════════════════════════════════╗\n";
                cout << "║" << vermelhoBold << "     ✖ NENHUM FUNCIONÁRIO ENCONTRADO! ✖            " << reset << "║\n";
                cout << "╚══════════════════════════════════════════════════════╝\n\n";
                cout << vermelho << "   ⚠ Não foi encontrado nenhum funcionário com o cargo \"" << cargoBusca << "\"" << reset << "\n";
            }
            cout << "\n" << amareloBold << "   Deseja fazer outra pesquisa? (s/n): " << reset;
            char continuar;
            cin >> continuar;
            limparBuffer();
            if (tolower(continuar) != 's') {
                limparTela();
                break;
            }
            limparTela();
            continue;
        }

        case 4: // Listar todos
        {
            limparTela();
            cout << "╔══════════════════════════════════════════════════════╗\n";
            cout << "║" << cianoBold << "        📋 TODOS OS FUNCIONÁRIOS 📋                  " << reset << "║\n";
            cout << "╚══════════════════════════════════════════════════════╝\n\n";

            if (funcionarios.empty()) {
                cout << amareloBold << "   ⚠ Nenhum funcionário cadastrado no sistema!" << reset << "\n\n";
                cout << amarelo << "   Não há funcionários para exibir no momento." << reset << "\n";
            } else {
                cout << verdeBold << "   Total de funcionários cadastrados: " << reset << verde << funcionarios.size() << reset << "\n\n";
                cout << "   ═══════════════════════════════════════════════════════════════\n\n";
                for (size_t i = 0; i < funcionarios.size(); i++) {
                    cout << azulBold << "   [" << (i + 1) << "] " << reset;
                    funcionarios[i].exibirDados();
                    if (i < funcionarios.size() - 1) cout << "\n";
                }
            }
            cout << "\n" << amareloBold << "   Pressione ENTER para continuar..." << reset;
            cin.get();
            limparTela();
            break;
        }

        case 5:
            limparTela();
            return;

        default:
            limparTela();
            cout << "╔══════════════════════════════════════════════════════╗\n";
            cout << "║" << vermelhoBold << "          ⚠ OPÇÃO INVÁLIDA! ⚠                     " << reset << "║\n";
            cout << "╚══════════════════════════════════════════════════════╝\n\n";
            cout << amarelo << "   ⏳ Retornando ao menu de pesquisa";
            for (int i = 0; i < 3; i++) {
                cout << ".";
                Sleep(1000);
            }
            cout << reset << "\n";
            limparTela();
            break;
        }
    }
}
// Função para filtrar clientes (EXISTENTE - Vinicius)

void filtrarClientes()
{
    limparTela();
    int option = 0;

    // Recarregar clientes do arquivo para garantir dados atualizados
    carregarClientes();

    // Limpa o buffer de entrada antes de iniciar o loop principal, se necessário.
    limparBuffer();

    while (true)
    {
        limparTela();
        cout << "╔══════════════════════════════════════════════════════╗\n";
        cout << "║" << cianoBold << "              🔍 PESQUISAR CLIENTES 🔍                  " << reset << "║\n";
        cout << "╚══════════════════════════════════════════════════════╝\n\n";

        if (clientes.empty()) {
            cout << vermelhoBold << "   ⚠ Não há clientes cadastrados para filtrar! ⚠\n\n" << reset;
            cout << amareloBold << "   Pressione ENTER para voltar ao menu principal..." << reset;
            cin.get();
            return;
        }

        cout << verdeBold << "   SELECIONE UMA OPÇÃO DE PESQUISA:\n\n" << reset;
        cout << verde << "   ➤  1" << reset << " - " << ciano << "Pesquisar por código" << reset << endl;
        cout << verde << "   ➤  2" << reset << " - " << ciano << "Pesquisar por nome" << reset << endl;
        cout << verde << "   ➤  3" << reset << " - " << ciano << "Pesquisar por endereço" << reset << endl;
        cout << verde << "   ➤  4" << reset << " - " << ciano << "Listar todos os clientes" << reset << endl;
        cout << vermelho << "   ➤  5" << reset << " - " << ciano << "Voltar ao menu principal" << reset << endl;
        cout << "\n" << amareloBold << "   Digite sua opção" << reset << amarelo << " e pressione ENTER: " << reset;

        // Tenta ler a opção, tratando erro de entrada
        if (!(cin >> option)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n" << vermelhoBold << "   ✖ Opção inválida. Digite um número." << reset;
            Sleep(2000);
            continue;
        }
        limparBuffer(); // Garante a limpeza do buffer após a leitura da opção

        // Vetor para armazenar resultados da busca por nome ou endereço
        vector<Cliente*> resultados;

        switch (option)
        {
        case 1: // Pesquisar por código (Consulta exata)
        {
            limparTela();
            cout << "╔══════════════════════════════════════════════════════╗\n";
            cout << "║" << cianoBold << "        🔍 PESQUISA POR CÓDIGO 🔍                     " << reset << "║\n";
            cout << "╚══════════════════════════════════════════════════════╝\n\n";
            cout << amareloBold << "   ➤ Informe o código do cliente: " << reset;
            int codigoBusca;

            if (!(cin >> codigoBusca)) {
                cout << vermelhoBold << "\n   ✖ Código inválido. Digite apenas números." << reset << "\n";
                limparBuffer();
                Sleep(2000);
                limparTela();
                continue;
            }
            limparBuffer();

            cout << amareloBold << "   🔍 Buscando cliente" << reset;
            for (int i = 0; i < 3; i++) {
                cout << amarelo << ".";
                Sleep(1000);
            }
            cout << reset << "\n\n";

            Cliente* cliente = consultarClientePorCodigo(codigoBusca);

            limparTela();
            if (cliente != nullptr) {
                cout << "╔══════════════════════════════════════════════════════╗\n";
                cout << "║" << verdeBold << "     ✅ CLIENTE ENCONTRADO! ✅           " << reset << "║\n";
                cout << "╚══════════════════════════════════════════════════════╝\n\n";
                exibirDadosCliente(*cliente);
                cout << "\n" << amareloBold << "   Deseja fazer outra pesquisa? (s/n): " << reset;
                char continuar;
                cin >> continuar;
                limparBuffer();
                if (tolower(continuar) != 's') {
                    limparTela();
                    break;
                }
                limparTela();
                continue;
            } else {
                cout << vermelhoBold << "   ✖ Cliente não encontrado!" << reset << "\n";
                cout << vermelho << "   ⚠ Não existe nenhum cliente com o código " << codigoBusca << reset << "\n";
                cout << amarelo << "   Deseja tentar novamente? (s/n): " << reset;
                char tentarNovamente;
                cin >> tentarNovamente;
                limparBuffer();
                if (tolower(tentarNovamente) == 's') {
                    limparTela();
                    continue;
                } else {
                    limparTela();
                    break;
                }
            }
        }

        case 2: // Pesquisar por nome (Consulta parcial e Case-Insensitive)
        {
            limparTela();
            cout << "╔══════════════════════════════════════════════════════╗\n";
            cout << "║" << cianoBold << "        🔍 PESQUISA POR NOME 🔍                       " << reset << "║\n";
            cout << "╚══════════════════════════════════════════════════════╝\n\n";
            cout << amareloBold << "   ➤ Informe o nome (ou parte do nome): " << reset;
            string nomeBusca;
            getline(cin, nomeBusca);

            string nomeBuscaLower = nomeBusca;
            transform(nomeBuscaLower.begin(), nomeBuscaLower.end(), nomeBuscaLower.begin(), ::tolower);

            cout << amareloBold << "   🔍 Buscando clientes" << reset;
            for (int i = 0; i < 3; i++) {
                cout << amarelo << ".";
                Sleep(1000);
            }
            cout << reset << "\n\n";

            for (auto& c : clientes) {
                string nomeLower = c.getNome();
                transform(nomeLower.begin(), nomeLower.end(), nomeLower.begin(), ::tolower);
                if (nomeLower.find(nomeBuscaLower) != string::npos) {
                    resultados.push_back(&c);
                }
            }

            goto exibirResultados;
        }

        case 3: // Pesquisar por endereço (Consulta parcial e Case-Insensitive)
        {
            limparTela();
            cout << "╔══════════════════════════════════════════════════════╗\n";
            cout << "║" << cianoBold <<          "🔍 PESQUISA POR ENDEREÇO 🔍" << reset << "║\n";
            cout << "╚══════════════════════════════════════════════════════╝\n\n";
            cout << amareloBold << "  ➤ Informe o endereço (ou parte do endereço): " << reset;
            string enderecoBusca;
            getline(cin, enderecoBusca);

            string enderecoBuscaLower = enderecoBusca;
            transform(enderecoBuscaLower.begin(), enderecoBuscaLower.end(), enderecoBuscaLower.begin(), ::tolower);

            cout << amareloBold << "  🔍 Buscando clientes" << reset;
            for (int i = 0; i < 3; i++) {
                cout << amarelo << ".";
                Sleep(1000);
            }
            cout << reset << "\n\n";

            for (auto& c : clientes) {
                string enderecoLower = c.getEndereco();
                transform(enderecoLower.begin(), enderecoLower.end(), enderecoLower.begin(), ::tolower);
                if (enderecoLower.find(enderecoBuscaLower) != string::npos) {
                    resultados.push_back(&c);
                }
            }

            goto exibirResultados;
        }

        case 4: // Listar todos
        {
            limparTela();
            cout << "╔══════════════════════════════════════════════════════╗\n";
            cout << "║" << cianoBold << "                 📋 TODOS OS CLIENTES 📋                 " << reset << "║\n";
            cout << "╚══════════════════════════════════════════════════════╝\n\n";

            if (clientes.empty()) {
                // Já tratado no início do loop, mas é bom ter aqui como fallback
                cout << amareloBold << "   ⚠ Nenhum cliente cadastrado no sistema!" << reset << "\n\n";
                cout << amarelo << "   Não há clientes para exibir no momento." << reset << "\n";
            } else {
                cout << verdeBold << "  Total de clientes cadastrados: " << reset << verde << clientes.size() << reset << "\n\n";
                cout << "═══════════════════════════════════════════════════════════════\n\n";
                for (size_t i = 0; i < clientes.size(); i++) {
                    cout << azulBold << "  [" << (i + 1) << "] " << reset;
                    exibirDadosCliente(clientes[i]);
                    if (i < clientes.size() - 1) cout << "\n";
                }
            }
            cout << "\n" << amareloBold << "   Deseja fazer outra pesquisa? (s/n): " << reset;
            char continuar;
            cin >> continuar;
            limparBuffer();
            if (tolower(continuar) != 's') {
                limparTela();
                break;
            }
            limparTela();
            continue;
        }

        case 5:
            limparTela();
            return; // Voltar ao Menu Principal

        default:
            limparTela();
            cout << "╔══════════════════════════════════════════════════════╗\n";
            cout << "║" << vermelhoBold << "          ⚠ OPÇÃO INVÁLIDA! ⚠          " << reset << "║\n";
            cout << "╚══════════════════════════════════════════════════════╝\n\n";
            cout << amarelo << "    ⏳ Retornando ao menu de pesquisa";
            for (int i = 0; i < 3; i++) {
                cout << ".";
                Sleep(1000);
            }
            cout << reset << "\n";
            limparTela();
            break;
        }

        // Seção de exibição de resultados para Case 2 e 3 (Nome e Endereço)
        if (option == 2 || option == 3) {
            exibirResultados:
            limparTela();
            if (!resultados.empty()) {
                cout << "╔══════════════════════════════════════════════════════╗\n";
                cout << "║" << verdeBold << "        ✅ CLIENTES ENCONTRADOS! ✅          " << reset << "║\n";
                cout << "╚══════════════════════════════════════════════════════╝\n\n";
                cout << verdeBold << "    Total encontrado: " << reset << verde << resultados.size() << " cliente(s)\n\n" << reset;
                cout << " ═══════════════════════════════════════════════════════════════\n\n";
                for (size_t i = 0; i < resultados.size(); i++) {
                    cout << azulBold << "[" << (i + 1) << "] " << reset;
                    exibirDadosCliente(*resultados[i]);
                    if (i < resultados.size() - 1) cout << "\n";
                }
            } else {
                cout << "╔══════════════════════════════════════════════════════╗\n";
                cout << "║" << vermelhoBold << "    ✖ NENHUM CLIENTE ENCONTRADO! ✖          " << reset << "║\n";
                cout << "╚══════════════════════════════════════════════════════╝\n\n";
                cout << vermelho << "⚠ Não foi encontrado nenhum cliente com o critério de busca." << reset << "\n";
            }
            cout << "\n" << amareloBold << "   Deseja fazer outra pesquisa? (s/n): " << reset;
            char continuar;
            cin >> continuar;
            limparBuffer();
            if (tolower(continuar) != 's') {
                limparTela();
                break;
            }
            limparTela();
            continue;
        }
    }
}

// ====================================================
// FUNÇÕES AUXILIARES PARA O CADASTRO DE QUARTOS (Eduardo)
// ====================================================

// Função auxiliar para limpar buffer do teclado
void limparBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Função para listar quartos disponíveis
void listarQuartosDisponiveis() {
    limparTela();

    // Carregar estadias para verificar disponibilidade por período
    carregarEstadias();

    vector<Quarto> quartos = lerQuartosDoArquivo();

    if (quartos.empty()) {
        cout << vermelhoBold << "Nenhum quarto cadastrado no sistema!\n" << reset;
        system("pause");
        return;
    }

    cout << cianoBold << "╔══════════════════════════════════════════════════════╗\n";
    cout << "║           QUARTOS DISPONIVEIS                  ║\n";
    cout << "╚══════════════════════════════════════════════════════╝\n\n" << reset;

    // Perguntar quantidade de hóspedes para filtrar
    int qtdHospedes;
    cout << amareloBold << "➤ Quantidade de hospedes para a estadia: " << reset;
    cin >> qtdHospedes;
    limparBuffer();

    if (qtdHospedes <= 0 || qtdHospedes > 4) {
        cout << vermelhoBold << "Quantidade invalida! Use entre 1-4 hospedes.\n" << reset;
        system("pause");
        return;
    }

    // Perguntar datas de check-in e check-out
    string dataCheckin, dataCheckout;
    cout << amareloBold << "➤ Data de check-in (DD/MM/AAAA): " << reset;
    getline(cin, dataCheckin);
    
    cout << amareloBold << "➤ Data de check-out (DD/MM/AAAA): " << reset;
    getline(cin, dataCheckout);

    // Validar formato das datas
    if (dataCheckin.length() != 10 || dataCheckout.length() != 10) {
        cout << vermelhoBold << "Formato de data invalido! Use o formato DD/MM/AAAA.\n" << reset;
        system("pause");
        return;
    }

    // Calcular diárias para validação
    int diarias = calcularDiarias(dataCheckin, dataCheckout);
    if (diarias <= 0) {
        cout << vermelhoBold << "Erro ao validar datas! Verifique se as datas estao corretas.\n" << reset;
        system("pause");
        return;
    }

    // Buscar quartos disponíveis para o período e capacidade
    vector<Quarto> quartosDisponiveis = obterQuartosDisponiveisPorPeriodo(qtdHospedes, dataCheckin, dataCheckout);

    cout << verdeBold << "\nQUARTOS DISPONIVEIS PARA " << qtdHospedes << " HOSPEDES:\n" << reset;
    cout << verdeBold << "Periodo: " << reset << dataCheckin << " ate " << dataCheckout << "\n";
    cout << verdeBold << "Diarias: " << reset << diarias << "\n";
    cout << "========================================\n\n";

    if (quartosDisponiveis.empty()) {
        cout << vermelhoBold << "Nenhum quarto disponivel para " << qtdHospedes << " hospedes no periodo informado!\n" << reset;
        cout << "Recomendacoes:\n";
        cout << amarelo << "1. Tente alterar as datas ou a quantidade de hospedes\n";
        cout << "2. Cadastre um novo quarto com capacidade suficiente\n" << reset;
    } else {
        for (const auto& quarto : quartosDisponiveis) {
            quarto.exibirInfo();
            cout << "\n";
        }
        cout << verdeBold << "Total encontrado: " << quartosDisponiveis.size() << " quarto(s) disponivel(is)\n" << reset;
    }

    cout << "\n";
    system("pause");
    limparTela();
}

// Função para listar todos os quartos
void listarTodosQuartos() {
    limparTela();

    vector<Quarto> quartos = lerQuartosDoArquivo();

    if (quartos.empty()) {
        cout << vermelhoBold << "Nenhum quarto cadastrado no sistema!\n" << reset;
        system("pause");
        return;
    }

    cout << cianoBold << "╔══════════════════════════════════════════════════════╗\n";
    cout << "║           TODOS OS QUARTOS CADASTRADOS               ║\n";
    cout << "╚══════════════════════════════════════════════════════╝\n\n" << reset;

    for (const auto& quarto : quartos) {
        quarto.exibirInfo();
    }

    cout << "\n";
    system("pause");
    limparTela();
}

// Função para editar um quarto
void editarQuarto() {
    limparTela();
    cout << "╔══════════════════════════════════════════════════════╗\n";
    cout << "║" << cianoBold << "           ✏️  EDITAR QUARTO ✏️                  " << reset << "║\n";
    cout << "╚══════════════════════════════════════════════════════╝\n\n";

    vector<Quarto> quartos = lerQuartosDoArquivo();

    if (quartos.empty()) {
        cout << vermelhoBold << "   ⚠ Nenhum quarto cadastrado no sistema!\n\n" << reset;
        cout << amareloBold << "   Pressione ENTER para voltar..." << reset;
        cin.get();
        return;
    }

    int numQuarto;
    bool encontrado = false;
    int indice = -1;

    while (!encontrado) {
        cout << amareloBold << "   ➤ Informe o número do quarto a editar: " << reset;
        if (!(cin >> numQuarto)) {
            cout << vermelhoBold << "   ✖ Digite apenas números!\n" << reset;
            limparBuffer();
            continue;
        }
        limparBuffer();

        for (size_t i = 0; i < quartos.size(); i++) {
            if (quartos[i].getNumQuarto() == numQuarto) {
                encontrado = true;
                indice = i;
                break;
            }
        }

        if (!encontrado) {
            cout << vermelhoBold << "   ✖ Quarto não encontrado!\n" << reset;
            cout << amarelo << "   Deseja tentar novamente? (s/n): " << reset;
            char opcao;
            cin >> opcao;
            limparBuffer();
            if (tolower(opcao) != 's') {
                return;
            }
        }
    }

    Quarto& quarto = quartos[indice];

    limparTela();
    cout << "╔══════════════════════════════════════════════════════╗\n";
    cout << "║" << cianoBold << "        📋 DADOS ATUAIS DO QUARTO 📋              " << reset << "║\n";
    cout << "╚══════════════════════════════════════════════════════╝\n\n";
    quarto.exibirInfo();
    cout << "\n";

    cout << amareloBold << "   ➤ Deseja editar a capacidade de hóspedes? (s/n): " << reset;
    char editarCapacidade;
    cin >> editarCapacidade;
    limparBuffer();

    if (tolower(editarCapacidade) == 's') {
        bool quantidadeValida = false;
        int quantidadeHospedes;
        while (!quantidadeValida) {
            cout << amareloBold << "   ➤ Nova capacidade máxima (1-4): " << reset;
            if (!(cin >> quantidadeHospedes)) {
                cout << vermelhoBold << "   ✖ Digite apenas números!\n" << reset;
                limparBuffer();
                continue;
            }
            limparBuffer();

            if (quantidadeHospedes <= 0 || quantidadeHospedes > 4) {
                cout << vermelhoBold << "   ✖ Capacidade deve ser entre 1 e 4!\n" << reset;
            } else {
                quantidadeValida = true;
                quarto.setQuantidadeHospedes(quantidadeHospedes);
            }
        }
    }

    cout << amareloBold << "   ➤ Deseja editar o valor da diária? (s/n): " << reset;
    char editarValor;
    cin >> editarValor;
    limparBuffer();

    if (tolower(editarValor) == 's') {
        bool valorValido = false;
        float valorDiaria;
        while (!valorValido) {
            cout << amareloBold << "   ➤ Novo valor da diária R$: " << reset;
            if (!(cin >> valorDiaria)) {
                cout << vermelhoBold << "   ✖ Digite um valor válido!\n" << reset;
                limparBuffer();
                continue;
            }
            limparBuffer();

            if (valorDiaria <= 0 || valorDiaria > 10000) {
                cout << vermelhoBold << "   ✖ Valor deve ser entre R$ 0,01 e R$ 10.000!\n" << reset;
            } else {
                valorValido = true;
                quarto.setValorDiaria(valorDiaria);
            }
        }
    }

    cout << amareloBold << "   ➤ Deseja editar a descrição? (s/n): " << reset;
    char editarDescricao;
    cin >> editarDescricao;
    limparBuffer();

    if (tolower(editarDescricao) == 's') {
        string descricao;
        cout << amareloBold << "   ➤ Nova descrição: " << reset;
        getline(cin, descricao);
        quarto.setDescricao(descricao);
    }

    // Reescrever todos os quartos no arquivo
    ofstream arquivo("quartos.dat", ios::binary | ios::trunc);
    if (!arquivo) {
        cout << vermelhoBold << "   ✖ Erro ao salvar alterações!\n" << reset;
        cin.get();
        return;
    }

    for (const auto& q : quartos) {
        int numQuarto = q.getNumQuarto();
        int quantidadeHospedes = q.getQuantidadeHospedes();
        float valorDiaria = q.getValorDiaria();

        arquivo.write(reinterpret_cast<const char*>(&numQuarto), sizeof(int));
        arquivo.write(reinterpret_cast<const char*>(&quantidadeHospedes), sizeof(int));
        arquivo.write(reinterpret_cast<const char*>(&valorDiaria), sizeof(float));

        string status = q.getStatus();
        string descricao = q.getDescricao();

        size_t tamStatus = status.size();
        size_t tamDescricao = descricao.size();

        arquivo.write(reinterpret_cast<const char*>(&tamStatus), sizeof(size_t));
        arquivo.write(status.c_str(), tamStatus);
        arquivo.write(reinterpret_cast<const char*>(&tamDescricao), sizeof(size_t));
        arquivo.write(descricao.c_str(), tamDescricao);
    }

    arquivo.close();

    cout << "\n" << verdeBold << "   ✅ Quarto editado com sucesso!\n\n" << reset;
    cout << amareloBold << "   Deseja editar outro quarto? (s/n): " << reset;
    char continuar;
    cin >> continuar;
    limparBuffer();
    if (tolower(continuar) == 's') {
        editarQuarto();
    }
}

// Função para excluir um quarto
void excluirQuarto() {
    limparTela();
    cout << "╔══════════════════════════════════════════════════════╗\n";
    cout << "║" << cianoBold << "          🗑️  EXCLUIR QUARTO 🗑️                  " << reset << "║\n";
    cout << "╚══════════════════════════════════════════════════════╝\n\n";

    vector<Quarto> quartos = lerQuartosDoArquivo();

    if (quartos.empty()) {
        cout << vermelhoBold << "   ⚠ Nenhum quarto cadastrado no sistema!\n\n" << reset;
        cout << amareloBold << "   Pressione ENTER para voltar..." << reset;
        cin.get();
        return;
    }

    int numQuarto;
    bool encontrado = false;
    int indice = -1;

    while (!encontrado) {
        cout << amareloBold << "   ➤ Informe o número do quarto a excluir: " << reset;
        if (!(cin >> numQuarto)) {
            cout << vermelhoBold << "   ✖ Digite apenas números!\n" << reset;
            limparBuffer();
            continue;
        }
        limparBuffer();

        for (size_t i = 0; i < quartos.size(); i++) {
            if (quartos[i].getNumQuarto() == numQuarto) {
                encontrado = true;
                indice = i;
                break;
            }
        }

        if (!encontrado) {
            cout << vermelhoBold << "   ✖ Quarto não encontrado!\n" << reset;
            cout << amarelo << "   Deseja tentar novamente? (s/n): " << reset;
            char opcao;
            cin >> opcao;
            limparBuffer();
            if (tolower(opcao) != 's') {
                return;
            }
        }
    }

    Quarto quarto = quartos[indice];

    // Verificar se o quarto está ocupado
    if (quarto.getStatus() == "ocupado") {
        cout << vermelhoBold << "   ⚠ Este quarto está ocupado e não pode ser excluído!\n" << reset;
        cout << amareloBold << "   Pressione ENTER para voltar..." << reset;
        cin.get();
        return;
    }

    // Verificar se há estadias futuras para este quarto
    bool temEstadias = false;
    for (const auto& estadia : estadias) {
        if (estadia.getNumQuarto() == numQuarto) {
            temEstadias = true;
            break;
        }
    }

    if (temEstadias) {
        cout << vermelhoBold << "   ⚠ Este quarto possui estadias cadastradas e não pode ser excluído!\n" << reset;
        cout << amareloBold << "   Pressione ENTER para voltar..." << reset;
        cin.get();
        return;
    }

    limparTela();
    cout << "╔══════════════════════════════════════════════════════╗\n";
    cout << "║" << vermelhoBold << "        ⚠ CONFIRMAÇÃO DE EXCLUSÃO ⚠              " << reset << "║\n";
    cout << "╚══════════════════════════════════════════════════════╝\n\n";

    quarto.exibirInfo();
    cout << "\n";
    cout << vermelhoBold << "   ⚠ ATENÇÃO: Esta ação não pode ser desfeita!\n\n" << reset;
    cout << amareloBold << "   ➤ Deseja realmente excluir este quarto? (s/n): " << reset;
    char confirmar;
    cin >> confirmar;
    limparBuffer();

    if (tolower(confirmar) != 's') {
        cout << amareloBold << "   Operação cancelada.\n" << reset;
        cout << amareloBold << "   Pressione ENTER para voltar..." << reset;
        cin.get();
        return;
    }

    // Remover quarto do vetor
    quartos.erase(quartos.begin() + indice);

    // Reescrever todos os quartos no arquivo
    ofstream arquivo("quartos.dat", ios::binary | ios::trunc);
    if (!arquivo) {
        cout << vermelhoBold << "   ✖ Erro ao salvar alterações!\n" << reset;
        cin.get();
        return;
    }

    for (const auto& q : quartos) {
        int numQuarto = q.getNumQuarto();
        int quantidadeHospedes = q.getQuantidadeHospedes();
        float valorDiaria = q.getValorDiaria();

        arquivo.write(reinterpret_cast<const char*>(&numQuarto), sizeof(int));
        arquivo.write(reinterpret_cast<const char*>(&quantidadeHospedes), sizeof(int));
        arquivo.write(reinterpret_cast<const char*>(&valorDiaria), sizeof(float));

        string status = q.getStatus();
        string descricao = q.getDescricao();

        size_t tamStatus = status.size();
        size_t tamDescricao = descricao.size();

        arquivo.write(reinterpret_cast<const char*>(&tamStatus), sizeof(size_t));
        arquivo.write(status.c_str(), tamStatus);
        arquivo.write(reinterpret_cast<const char*>(&tamDescricao), sizeof(size_t));
        arquivo.write(descricao.c_str(), tamDescricao);
    }

    arquivo.close();

    cout << "\n" << verdeBold << "   ✅ Quarto excluído com sucesso!\n\n" << reset;
    cout << amareloBold << "   Deseja excluir outro quarto? (s/n): " << reset;
    char continuar;
    cin >> continuar;
    limparBuffer();
    if (tolower(continuar) == 's') {
        excluirQuarto();
    }
}

// Função para verificar se quarto já existe
bool quartoExiste(int numeroQuarto) {
    vector<Quarto> quartos = lerQuartosDoArquivo();

    for (const auto& quarto : quartos) {
        if (quarto.getNumQuarto() == numeroQuarto) {
            return true;
        }
    }

    return false;
}

// Função para obter quartos disponíveis com capacidade
vector<Quarto> obterQuartosDisponiveis(int quantidadeHospedes) {
    vector<Quarto> quartos = lerQuartosDoArquivo();
    vector<Quarto> disponiveis;

    for (const auto& quarto : quartos) {
        if (quarto.getStatus() == "desocupado" && quarto.getQuantidadeHospedes() >= quantidadeHospedes) {
            disponiveis.push_back(quarto);
        }
    }

    return disponiveis;
}

// Função para alterar status do quarto
bool alterarStatusQuarto(int numeroQuarto, string novoStatus) {
    vector<Quarto> quartos = lerQuartosDoArquivo();
    bool encontrado = false;

    // Atualizar status do quarto específico
    for (auto& quarto : quartos) {
        if (quarto.getNumQuarto() == numeroQuarto) {
            quarto.setStatus(novoStatus);
            encontrado = true;
            break;
        }
    }

    if (!encontrado) {
        return false;
    }

    // Reescrever todos os quartos no arquivo
    ofstream arquivo("quartos.dat", ios::binary | ios::trunc);
    if (!arquivo) {
        return false;
    }

    for (const auto& quarto : quartos) {
        // Salvar campos inteiros e float
        int numQuarto = quarto.getNumQuarto();
        int quantidadeHospedes = quarto.getQuantidadeHospedes();
        float valorDiaria = quarto.getValorDiaria();

        arquivo.write(reinterpret_cast<const char*>(&numQuarto), sizeof(int));
        arquivo.write(reinterpret_cast<const char*>(&quantidadeHospedes), sizeof(int));
        arquivo.write(reinterpret_cast<const char*>(&valorDiaria), sizeof(float));

        // Salvar strings
        string status = quarto.getStatus();
        string descricao = quarto.getDescricao();

        size_t tamanhoStatus = status.size();
        arquivo.write(reinterpret_cast<const char*>(&tamanhoStatus), sizeof(size_t));
        arquivo.write(status.c_str(), tamanhoStatus);

        size_t tamanhoDescricao = descricao.size();
        arquivo.write(reinterpret_cast<const char*>(&tamanhoDescricao), sizeof(size_t));
        arquivo.write(descricao.c_str(), tamanhoDescricao);
    }

    arquivo.close();
    return true;
}

// Função para salvar quarto no arquivo
void salvarQuartoNoArquivo(Quarto quarto) {
    ofstream arquivo("quartos.dat", ios::binary | ios::app);

    if (!arquivo) {
        arquivo.open("quartos.dat", ios::binary | ios::out);
        if (!arquivo) {
            cout << vermelhoBold << "ERRO CRITICO: Nao foi possivel criar o arquivo!\n" << reset;
            return;
        }
    }

    // Salvar campos inteiros e float
    int numQuarto = quarto.getNumQuarto();
    int quantidadeHospedes = quarto.getQuantidadeHospedes();
    float valorDiaria = quarto.getValorDiaria();

    arquivo.write(reinterpret_cast<const char*>(&numQuarto), sizeof(int));
    arquivo.write(reinterpret_cast<const char*>(&quantidadeHospedes), sizeof(int));
    arquivo.write(reinterpret_cast<const char*>(&valorDiaria), sizeof(float));

    // Salvar strings (tamanho + conteúdo)
    string status = quarto.getStatus();
    string descricao = quarto.getDescricao();

    size_t tamanhoStatus = status.size();
    arquivo.write(reinterpret_cast<const char*>(&tamanhoStatus), sizeof(size_t));
    arquivo.write(status.c_str(), tamanhoStatus);

    size_t tamanhoDescricao = descricao.size();
    arquivo.write(reinterpret_cast<const char*>(&tamanhoDescricao), sizeof(size_t));
    arquivo.write(descricao.c_str(), tamanhoDescricao);

    arquivo.close();
}

// Função para ler todos os quartos do arquivo
vector<Quarto> lerQuartosDoArquivo() {
    vector<Quarto> quartos;
    ifstream arquivo("quartos.dat", ios::binary);

    if (!arquivo) {
        return quartos;
    }

    // Ler até o final do arquivo
    while (arquivo.peek() != EOF) {
        Quarto quarto;

        // Ler campos inteiros e float
        int numQuarto, quantidadeHospedes;
        float valorDiaria;

        arquivo.read(reinterpret_cast<char*>(&numQuarto), sizeof(int));
        if (arquivo.eof()) break;

        arquivo.read(reinterpret_cast<char*>(&quantidadeHospedes), sizeof(int));
        arquivo.read(reinterpret_cast<char*>(&valorDiaria), sizeof(float));

        // Ler strings
        size_t tamanhoStatus, tamanhoDescricao;
        arquivo.read(reinterpret_cast<char*>(&tamanhoStatus), sizeof(size_t));
        string status(tamanhoStatus, '\0');
        arquivo.read(&status[0], tamanhoStatus);

        arquivo.read(reinterpret_cast<char*>(&tamanhoDescricao), sizeof(size_t));
        string descricao(tamanhoDescricao, '\0');
        arquivo.read(&descricao[0], tamanhoDescricao);

        // Configurar quarto
        quarto.setNumQuarto(numQuarto);
        quarto.setQuantidadeHospedes(quantidadeHospedes);
        quarto.setValorDiaria(valorDiaria);
        quarto.setStatus(status);
        quarto.setDescricao(descricao);

        quartos.push_back(quarto);
    }

    arquivo.close();
    return quartos;
}


int quartosDisponiveis(int quantHospedes){
    bool valido = false;

    ifstream arquivo;
    arquivo.open("quartos.bin", ios::binary);

    if (!arquivo.is_open()){
        cout << "Erro ao abrir o arquivo"; // Fazer tratamento de exceções
        return 0;
    }

    arquivo.close();
    return 0;
}

// Função para salvar estadias em arquivo binário
void salvarEstadias() {
    ofstream arquivo;
    arquivo.open("estadias.bin", ios::binary | ios::out | ios::trunc);

    if (!arquivo.is_open()) {
        cout << vermelho << "Erro ao abrir arquivo para salvar estadias!" << reset << endl;
        return;
    }

    size_t tamanho = estadias.size();
    arquivo.write(reinterpret_cast<const char*>(&tamanho), sizeof(size_t));

    // Escrever cada estadia
    for (size_t i = 0; i < estadias.size(); i++) {
        int codigoEstadia = estadias[i].getCodigoEstadia();
        int codigoCliente = estadias[i].getCodigoCliente();
        int quantidadeDiarias = estadias[i].getQuantidadeDiarias();
        int quantidadeHospedes = estadias[i].getQuantidadeHospedes();
        int numQuarto = estadias[i].getNumQuarto();

        arquivo.write(reinterpret_cast<const char*>(&codigoEstadia), sizeof(int));
        arquivo.write(reinterpret_cast<const char*>(&codigoCliente), sizeof(int));
        arquivo.write(reinterpret_cast<const char*>(&quantidadeDiarias), sizeof(int));
        arquivo.write(reinterpret_cast<const char*>(&quantidadeHospedes), sizeof(int));
        arquivo.write(reinterpret_cast<const char*>(&numQuarto), sizeof(int));

        // Escrever strings (tamanho + conteúdo)
        string dataEntrada = estadias[i].getDataEntrada();
        string dataSaida = estadias[i].getDataSaida();

        size_t tamanhoEntrada = dataEntrada.size();
        arquivo.write(reinterpret_cast<const char*>(&tamanhoEntrada), sizeof(size_t));
        arquivo.write(dataEntrada.c_str(), tamanhoEntrada);

        size_t tamanhoSaida = dataSaida.size();
        arquivo.write(reinterpret_cast<const char*>(&tamanhoSaida), sizeof(size_t));
        arquivo.write(dataSaida.c_str(), tamanhoSaida);
    }

    arquivo.close();
}

// Função para carregar estadias do arquivo binário
void carregarEstadias() {
    ifstream arquivo;
    arquivo.open("estadias.bin", ios::binary | ios::in);

    if (!arquivo.is_open()) {
        // Arquivo não existe ainda, inicializar vetor vazio
        estadias.clear();
        return;
    }

    // Verificar se o arquivo está vazio
    arquivo.seekg(0, ios::end);
    if (arquivo.tellg() == 0) {
        estadias.clear();
        arquivo.close();
        return;
    }
    arquivo.seekg(0, ios::beg);

    // Ler quantidade de estadias
    size_t tamanho;
    arquivo.read(reinterpret_cast<char*>(&tamanho), sizeof(size_t));

    // Verificar se conseguiu ler o tamanho
    if (arquivo.fail()) {
        estadias.clear();
        arquivo.close();
        return;
    }

    estadias.clear();

    // Ler cada estadia
    for (size_t i = 0; i < tamanho; i++) {
        Estadia estadia;

        int codigoEstadia, codigoCliente, quantidadeDiarias, quantidadeHospedes, numQuarto;

        arquivo.read(reinterpret_cast<char*>(&codigoEstadia), sizeof(int));
        arquivo.read(reinterpret_cast<char*>(&codigoCliente), sizeof(int));
        arquivo.read(reinterpret_cast<char*>(&quantidadeDiarias), sizeof(int));
        arquivo.read(reinterpret_cast<char*>(&quantidadeHospedes), sizeof(int));
        arquivo.read(reinterpret_cast<char*>(&numQuarto), sizeof(int));

        // Ler strings
        size_t tamanhoEntrada, tamanhoSaida;
        arquivo.read(reinterpret_cast<char*>(&tamanhoEntrada), sizeof(size_t));
        string dataEntrada(tamanhoEntrada, '\0');
        arquivo.read(&dataEntrada[0], tamanhoEntrada);

        arquivo.read(reinterpret_cast<char*>(&tamanhoSaida), sizeof(size_t));
        string dataSaida(tamanhoSaida, '\0');
        arquivo.read(&dataSaida[0], tamanhoSaida);

        // Configurar estadia
        estadia.setCodigoEstadia(codigoEstadia);
        estadia.setCodigoCliente(codigoCliente);
        estadia.setQuantidadeDiarias(quantidadeDiarias);
        estadia.setQuantidadeHospedes(quantidadeHospedes);
        estadia.setNumQuarto(numQuarto);
        estadia.setDataEntrada(dataEntrada);
        estadia.setDataSaida(dataSaida);

        estadias.push_back(estadia);
    }

    arquivo.close();
}

// Função para calcular quantidade de diárias entre duas datas
int calcularDiarias(string dataEntrada, string dataSaida) {
    // Formato esperado: DD/MM/AAAA
    if (dataEntrada.length() != 10 || dataSaida.length() != 10) {
        return 0;
    }

    // Extrair dia, mês e ano
    int diaEntrada, mesEntrada, anoEntrada;
    int diaSaida, mesSaida, anoSaida;

    try {
        diaEntrada = stoi(dataEntrada.substr(0, 2));
        mesEntrada = stoi(dataEntrada.substr(3, 2));
        anoEntrada = stoi(dataEntrada.substr(6, 4));

        diaSaida = stoi(dataSaida.substr(0, 2));
        mesSaida = stoi(dataSaida.substr(3, 2));
        anoSaida = stoi(dataSaida.substr(6, 4));
    } catch (...) {
        return 0;
    }

    // Converter para dias desde uma data base (1/1/2000)
    auto converterParaDias = [](int dia, int mes, int ano) -> int {
        int dias = (ano - 2000) * 365;
        dias += (ano - 2000) / 4; // Aproximação para anos bissextos

        int diasPorMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        for (int i = 0; i < mes - 1; i++) {
            dias += diasPorMes[i];
        }
        dias += dia;
        return dias;
    };

    int diasEntrada = converterParaDias(diaEntrada, mesEntrada, anoEntrada);
    int diasSaida = converterParaDias(diaSaida, mesSaida, anoSaida);

    int diarias = diasSaida - diasEntrada;

    return (diarias > 0) ? diarias : 0;
}

// Função para converter data DD/MM/AAAA para dias desde 01/01/2000
int converterDataParaDias(string data) {
    if (data.length() != 10) return 0;

    try {
        int dia = stoi(data.substr(0, 2));
        int mes = stoi(data.substr(3, 2));
        int ano = stoi(data.substr(6, 4));

        // Validação básica
        if (dia < 1 || dia > 31 || mes < 1 || mes > 12 || ano < 2000) {
            return 0;
        }

        int dias = (ano - 2000) * 365;
        dias += (ano - 2000) / 4; // Aproximação para anos bissextos

        int diasPorMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        for (int i = 0; i < mes - 1; i++) {
            dias += diasPorMes[i];
        }
        dias += dia;

        return dias;
    } catch (...) {
        return 0;
    }
}

// Função para verificar se há conflito de datas entre dois períodos
bool haConflitoDatas(string checkin1, string checkout1, string checkin2, string checkout2) {
    int diasCheckin1 = converterDataParaDias(checkin1);
    int diasCheckout1 = converterDataParaDias(checkout1);
    int diasCheckin2 = converterDataParaDias(checkin2);
    int diasCheckout2 = converterDataParaDias(checkout2);

    // Verifica se há sobreposição de períodos
    return !(diasCheckout1 < diasCheckin2 || diasCheckout2 < diasCheckin1);
}

// Função para verificar disponibilidade de quarto por período
bool verificarDisponibilidadePorPeriodo(int numQuarto, string dataCheckin, string dataCheckout) {
    // Verificar se o quarto existe e está desocupado
    vector<Quarto> quartos = lerQuartosDoArquivo();
    bool quartoExiste = false;
    bool quartoDesocupado = false;

    for (const auto& quarto : quartos) {
        if (quarto.getNumQuarto() == numQuarto) {
            quartoExiste = true;
            quartoDesocupado = (quarto.getStatus() == "desocupado");
            break;
        }
    }

    if (!quartoExiste || !quartoDesocupado) {
        return false;
    }

    // Verificar conflitos com estadias existentes
    for (const auto& estadia : estadias) {
        if (estadia.getNumQuarto() == numQuarto) {
            if (haConflitoDatas(dataCheckin, dataCheckout,
                               estadia.getDataEntrada(), estadia.getDataSaida())) {
                return false;
            }
        }
    }

    return true;
}

// Função para obter quartos disponíveis por período e capacidade
vector<Quarto> obterQuartosDisponiveisPorPeriodo(int quantidadeHospedes, string dataCheckin, string dataCheckout) {
    vector<Quarto> todosQuartos = lerQuartosDoArquivo();
    vector<Quarto> disponiveis;

    for (const auto& quarto : todosQuartos) {
        // Se for 1 hóspede, mostrar quartos que suportam 1 ou 2 hóspedes
        // Caso contrário, mostrar apenas quartos com capacidade exata
        bool capacidadeValida = false;
        if (quantidadeHospedes == 1) {
            // Para 1 hóspede, aceitar quartos que suportam 1 ou 2 hóspedes
            capacidadeValida = (quarto.getQuantidadeHospedes() == 1 || quarto.getQuantidadeHospedes() == 2);
        } else {
            // Para outros casos, apenas capacidade exata
            capacidadeValida = (quarto.getQuantidadeHospedes() == quantidadeHospedes);
        }

        if (capacidadeValida) {
            // Verificar disponibilidade no período
            if (verificarDisponibilidadePorPeriodo(quarto.getNumQuarto(), dataCheckin, dataCheckout)) {
                disponiveis.push_back(quarto);
            }
        }
    }

    return disponiveis;
}

// Função auxiliar para exibir uma estadia formatada
void exibirEstadiaFormatada(const Estadia& estadia, size_t indice, const Cliente* cliente = nullptr) {
    // Buscar informações do quarto para exibir valor da diária
    Quarto quarto = buscarQuartoPorNumero(estadia.getNumQuarto());
    float valorTotal = 0.0;
    if (quarto.getNumQuarto() != 0) {
        valorTotal = quarto.getValorDiaria() * estadia.getQuantidadeDiarias();
    }

    cout << "   ┌─────────────────────────────────────────────┐\n";
    cout << "   │" << cianoBold << "              ESTADIA #" << (indice + 1) << "                        "<< reset <<"│\n";
    cout << "   ├─────────────────────────────────────────────┤\n";
    cout << "   │" << azul << "   Código da Estadia: " << reset << verdeBold << estadia.getCodigoEstadia() << reset;
    int espacos = 25 - (int)to_string(estadia.getCodigoEstadia()).length();
    if (espacos < 0) espacos = 0;
    for (int j = 0; j < espacos; j++) cout << " ";
    cout << "│\n";

    // Exibir informações do cliente se disponível
    if (cliente != nullptr) {
        cout << "   │" << azul << "   Cliente:           " << reset << verde << cliente->getNome() << reset;
        espacos = 25 - (int)cliente->getNome().length();
        if (espacos < 0) espacos = 0;
        for (int j = 0; j < espacos; j++) cout << " ";
        cout << "│\n";
    }

    cout << "   │" << azul << "   Código do Cliente: " << reset << verdeBold << estadia.getCodigoCliente() << reset;
    espacos = 25 - (int)to_string(estadia.getCodigoCliente()).length();
    if (espacos < 0) espacos = 0;
    for (int j = 0; j < espacos; j++) cout << " ";
    cout << "│\n";
    cout << "   │" << azul << "   Quarto:            " << reset << verdeBold << estadia.getNumQuarto() << reset;
    espacos = 25 - (int)to_string(estadia.getNumQuarto()).length();
    if (espacos < 0) espacos = 0;
    for (int j = 0; j < espacos; j++) cout << " ";
    cout << "│\n";
    cout << "   │" << azul << "   Check-in:          " << reset << verde << estadia.getDataEntrada() << reset;
    espacos = 25 - (int)estadia.getDataEntrada().length();
    if (espacos < 0) espacos = 0;
    for (int j = 0; j < espacos; j++) cout << " ";
    cout << "│\n";
    cout << "   │" << azul << "   Checkout:          " << reset << verde << estadia.getDataSaida() << reset;
    espacos = 25 - (int)estadia.getDataSaida().length();
    if (espacos < 0) espacos = 0;
    for (int j = 0; j < espacos; j++) cout << " ";
    cout << "│\n";
    cout << "   │" << azul << "   Diárias:           " << reset << verdeBold << estadia.getQuantidadeDiarias() << " diária(s)" << reset;
    espacos = 25 - (int)to_string(estadia.getQuantidadeDiarias()).length() - 9;
    if (espacos < 0) espacos = 0;
    for (int j = 0; j < espacos; j++) cout << " ";
    cout << "│\n";
    cout << "   │" << azul << "   Hóspedes:          " << reset << verdeBold << estadia.getQuantidadeHospedes() << reset;
    espacos = 25 - (int)to_string(estadia.getQuantidadeHospedes()).length();
    if (espacos < 0) espacos = 0;
    for (int j = 0; j < espacos; j++) cout << " ";
    cout << "│\n";

    // Exibir valor total se o quarto foi encontrado
    if (quarto.getNumQuarto() != 0) {
        cout << "   │" << azul << "   Valor da Diária:   " << reset << verdeBold << "R$ " << fixed << setprecision(2) << quarto.getValorDiaria() << reset;
        int tamanhoValorDiaria = (quarto.getValorDiaria() >= 1000) ? 8 : ((quarto.getValorDiaria() >= 100) ? 7 : 6);
        espacos = 25 - tamanhoValorDiaria;
        if (espacos < 0) espacos = 0;
        for (int j = 0; j < espacos; j++) cout << " ";
        cout << "│\n";
        cout << "   │" << roxoBold << "   Valor Total:        " << reset << verdeBold << "R$ " << fixed << setprecision(2) << valorTotal << reset;
        int tamanhoValorTotal = (valorTotal >= 10000) ? 9 : ((valorTotal >= 1000) ? 8 : ((valorTotal >= 100) ? 7 : 6));
        espacos = 25 - tamanhoValorTotal;
        if (espacos < 0) espacos = 0;
        for (int j = 0; j < espacos; j++) cout << " ";
        cout << "│\n";
    }

    cout << "   └─────────────────────────────────────────────┘\n\n";
}

// Função para filtrar e exibir estadias por cliente
void filtrarEstadiasPorCliente() {
    limparTela();

    // Recarregar estadias do arquivo para garantir dados atualizados
    carregarEstadias();

    cout << "╔══════════════════════════════════════════════════════╗\n";
    cout << "║" << cianoBold << "     🔍 FILTRAR ESTADIAS POR CLIENTE 🔍            " << reset << "║\n";
    cout << "╚══════════════════════════════════════════════════════╝\n\n";

    cout << verdeBold << "   SELECIONE O TIPO DE BUSCA:\n\n" << reset;
    cout << verde << "   ➤  1" << reset << " - " << ciano << "Buscar por código do cliente" << reset << endl;
    cout << verde << "   ➤  2" << reset << " - " << ciano << "Buscar por nome do cliente" << reset << endl;
    cout << vermelho << "   ➤  3" << reset << " - " << ciano << "Voltar" << reset << endl;
    cout << "\n" << amareloBold << "   Digite sua opção" << reset << amarelo << " e pressione ENTER: " << reset;

    int opcao;
    cin >> opcao;
    limparBuffer();

    vector<Estadia> estadiasFiltradas;
    Cliente* clienteEncontrado = nullptr;

    switch (opcao) {
        case 1: { // Buscar por código
            limparTela();
            cout << "╔══════════════════════════════════════════════════════╗\n";
            cout << "║" << cianoBold << "     🔍 BUSCAR POR CÓDIGO DO CLIENTE 🔍            " << reset << "║\n";
            cout << "╚══════════════════════════════════════════════════════╝\n\n";
            cout << amareloBold << "   ➤ Informe o código do cliente: " << reset;
            int codigoCliente;
            cin >> codigoCliente;
            limparBuffer();

            clienteEncontrado = consultarClientePorCodigo(codigoCliente);
            if (clienteEncontrado == nullptr) {
                cout << "\n" << vermelhoBold << "   ✖ Cliente não encontrado!" << reset << "\n";
                cout << vermelho << "   ⚠ Não existe nenhum cliente com o código " << codigoCliente << reset << "\n";
                cout << amareloBold << "\n   Pressione ENTER para voltar..." << reset;
                cin.get();
                return;
            }

            // Filtrar estadias deste cliente
            for (const auto& estadia : estadias) {
                if (estadia.getCodigoCliente() == codigoCliente) {
                    estadiasFiltradas.push_back(estadia);
                }
            }
            break;
        }

        case 2: { // Buscar por nome
            limparTela();
            cout << "╔══════════════════════════════════════════════════════╗\n";
            cout << "║" << cianoBold << "     🔍 BUSCAR POR NOME DO CLIENTE 🔍              " << reset << "║\n";
            cout << "╚══════════════════════════════════════════════════════╝\n\n";
            cout << amareloBold << "   ➤ Informe o nome (ou parte do nome): " << reset;
            string nomeBusca;
            getline(cin, nomeBusca);

            // Converter para minúsculas para busca case-insensitive
            string nomeBuscaLower = nomeBusca;
            transform(nomeBuscaLower.begin(), nomeBuscaLower.end(), nomeBuscaLower.begin(), ::tolower);

            // Buscar clientes que correspondem ao nome
            vector<Cliente*> clientesEncontrados;
            for (auto& c : clientes) {
                string nomeLower = c.getNome();
                transform(nomeLower.begin(), nomeLower.end(), nomeLower.begin(), ::tolower);
                if (nomeLower.find(nomeBuscaLower) != string::npos) {
                    clientesEncontrados.push_back(&c);
                }
            }

            if (clientesEncontrados.empty()) {
                cout << "\n" << vermelhoBold << "   ✖ Nenhum cliente encontrado!" << reset << "\n";
                cout << vermelho << "   ⚠ Não foi encontrado nenhum cliente com o nome \"" << nomeBusca << "\"" << reset << "\n";
                cout << amareloBold << "\n   Pressione ENTER para voltar..." << reset;
                cin.get();
                return;
            }

            // Se houver múltiplos clientes, mostrar e pedir para escolher
            if (clientesEncontrados.size() > 1) {
                cout << "\n" << verdeBold << "   ✅ Foram encontrados " << clientesEncontrados.size() << " cliente(s):" << reset << "\n\n";
                for (size_t i = 0; i < clientesEncontrados.size(); i++) {
                    cout << "   [" << (i + 1) << "] ";
                    exibirDadosCliente(*clientesEncontrados[i]);
                    cout << "\n";
                }
                cout << amareloBold << "   ➤ Selecione o cliente (1-" << clientesEncontrados.size() << "): " << reset;
                int escolha;
                cin >> escolha;
                limparBuffer();

                if (escolha >= 1 && escolha <= (int)clientesEncontrados.size()) {
                    clienteEncontrado = clientesEncontrados[escolha - 1];
                } else {
                    cout << vermelhoBold << "\n   ✖ Opção inválida!" << reset << "\n";
                    cout << amareloBold << "\n   Pressione ENTER para voltar..." << reset;
                    cin.get();
                    return;
                }
            } else {
                clienteEncontrado = clientesEncontrados[0];
            }

            // Filtrar estadias deste cliente
            for (const auto& estadia : estadias) {
                if (estadia.getCodigoCliente() == clienteEncontrado->getCodigo()) {
                    estadiasFiltradas.push_back(estadia);
                }
            }
            break;
        }

        case 3:
            return;

        default:
            limparTela();
            cout << "╔══════════════════════════════════════════════════════╗\n";
            cout << "║" << vermelhoBold << "          ⚠ OPÇÃO INVÁLIDA! ⚠                     " << reset << "║\n";
            cout << "╚══════════════════════════════════════════════════════╝\n\n";
            cout << amarelo << "   ⏳ Retornando";
            for (int i = 0; i < 3; i++) {
                cout << ".";
                Sleep(1000);
            }
            cout << reset << "\n";
            return;
    }

    // Exibir resultados
    limparTela();
    cout << "╔══════════════════════════════════════════════════════╗\n";
    cout << "║" << cianoBold << "        📋 ESTADIAS DO CLIENTE 📋                 " << reset << "║\n";
    cout << "╚══════════════════════════════════════════════════════╝\n\n";

    if (clienteEncontrado != nullptr) {
        cout << verdeBold << "   Dados do Cliente:" << reset << "\n";
        exibirDadosCliente(*clienteEncontrado);
        cout << "\n";
    }

    if (estadiasFiltradas.empty()) {
        cout << amareloBold << "   ⚠ Nenhuma estadia encontrada para este cliente!" << reset << "\n\n";
        cout << amarelo << "   Este cliente não possui estadias cadastradas no sistema." << reset << "\n";
    } else {
        cout << verdeBold << "   Total de estadias encontradas: " << reset << verde << estadiasFiltradas.size() << reset << "\n\n";
        cout << "   ════════════════════════════════════════════════════\n\n";

        for (size_t i = 0; i < estadiasFiltradas.size(); i++) {
            exibirEstadiaFormatada(estadiasFiltradas[i], i, clienteEncontrado);
        }
    }

    cout << amareloBold << "   Deseja filtrar estadias de outro cliente? (s/n): " << reset;
    char continuar;
    cin >> continuar;
    limparBuffer();
    if (tolower(continuar) == 's') {
        filtrarEstadiasPorCliente();
    }
}

// Função para listar todas as estadias atuais
void listarTodasEstadias() {
    limparTela();
    int option = 0;

    while (true) {
        cout << "╔══════════════════════════════════════════════════════╗\n";
        cout << "║" << cianoBold << "        📋 LISTAGEM DE ESTADIAS 📋               " << reset << "║\n";
        cout << "╚══════════════════════════════════════════════════════╝\n\n";
        cout << verdeBold << "   SELECIONE UMA OPÇÃO:\n\n" << reset;
        cout << verde << "   ➤  1" << reset << " - " << ciano << "Filtrar estadias por cliente" << reset << endl;
        cout << verde << "   ➤  2" << reset << " - " << ciano << "Ver todas as estadias" << reset << endl;
        cout << vermelho << "   ➤  3" << reset << " - " << ciano << "Voltar ao menu principal" << reset << endl;
        cout << "\n" << amareloBold << "   Digite sua opção" << reset << amarelo << " e pressione ENTER: " << reset;
        cin >> option;
        limparBuffer();

        switch (option) {
            case 1:
                filtrarEstadiasPorCliente();
                limparTela();
                break;

            case 2: {
                limparTela();

                // Recarregar estadias do arquivo para garantir dados atualizados
                carregarEstadias();

                cout << "╔══════════════════════════════════════════════════════╗\n";
                cout << "║" << cianoBold << "        📋 TODAS AS ESTADIAS 📋                  " << reset << "║\n";
                cout << "╚══════════════════════════════════════════════════════╝\n\n";

                // Verificar se há estadias cadastradas
                if (estadias.empty()) {
                    cout << amareloBold << "   ⚠ Nenhuma estadia cadastrada no sistema!" << reset << "\n\n";
                    cout << amarelo << "   Não há estadias para exibir no momento." << reset << "\n";
                    cout << amareloBold << "\n   Pressione ENTER para voltar..." << reset;
                    cin.get();
                    break;
                }

                // Exibir quantidade total de estadias
                cout << verdeBold << "   Total de estadias cadastradas: " << reset << verde << estadias.size() << reset << "\n\n";
                cout << "   ════════════════════════════════════════════════════\n\n";

                // Listar cada estadia
                for (size_t i = 0; i < estadias.size(); i++) {
                    // Buscar cliente para exibir nome
                    Cliente* cliente = consultarClientePorCodigo(estadias[i].getCodigoCliente());
                    exibirEstadiaFormatada(estadias[i], i, cliente);
                }

                cout << amareloBold << "   Pressione ENTER para voltar..." << reset;
                cin.get();
                break;
            }

            case 3:
                return;

            default:
                limparTela();
                cout << "╔══════════════════════════════════════════════════════╗\n";
                cout << "║" << vermelhoBold << "          ⚠ OPÇÃO INVÁLIDA! ⚠                     " << reset << "║\n";
                cout << "╚══════════════════════════════════════════════════════╝\n\n";
                cout << amarelo << "   ⏳ Retornando ao menu";
                for (int i = 0; i < 3; i++) {
                    cout << ".";
                    Sleep(1000);
                }
                cout << reset << "\n";
                limparTela();
                break;
        }
    }
}

// Função auxiliar para buscar um quarto pelo número
Quarto buscarQuartoPorNumero(int numeroQuarto) {
    vector<Quarto> quartos = lerQuartosDoArquivo();
    Quarto quartoVazio;

    for (const auto& quarto : quartos) {
        if (quarto.getNumQuarto() == numeroQuarto) {
            return quarto;
        }
    }

    return quartoVazio; // Retorna quarto vazio se não encontrar
}

// Função para dar baixa em uma estadia
void darBaixaEstadia() {
    limparTela();
    int codigoEstadia;
    bool encontrada = false;
    int indiceEstadia = -1;

    cout << "╔══════════════════════════════════════════════════════╗\n";
    cout << "║" << cianoBold << "        💰 DAR BAIXA EM ESTADIA 💰               " << reset << "║\n";
    cout << "╚══════════════════════════════════════════════════════╝\n\n";

    // Solicitar código da estadia
    cout << amareloBold << "   ➤ Informe o código da estadia: " << reset;
    cin >> codigoEstadia;
    cin.ignore(10000, '\n');

    // Verificar se a estadia existe
    cout << amareloBold << "   🔍 Verificando estadia no sistema" << reset;
    for (int i = 0; i < 3; i++) {
        cout << amarelo << ".";
        Sleep(1000);
    }
    cout << reset << "\n";

    // Buscar estadia no vetor
    for (size_t i = 0; i < estadias.size(); i++) {
        if (estadias[i].getCodigoEstadia() == codigoEstadia) {
            encontrada = true;
            indiceEstadia = i;
            break;
        }
    }

    if (!encontrada) {
        cout << vermelhoBold << "   ✖ Estadia não encontrada!" << reset << "\n";
        cout << vermelho << "   ⚠ Não existe nenhuma estadia com o código " << codigoEstadia << reset << "\n";
        cout << amarelo << "   Deseja tentar novamente? (s/n): " << reset;
        char tentarNovamente;
        cin >> tentarNovamente;
        limparBuffer();
        if (tolower(tentarNovamente) == 's') {
            return darBaixaEstadia(); // Chama a função novamente
        } else {
            return; // Volta ao menu
        }
    }

    // Estadia encontrada - obter dados
    Estadia estadia = estadias[indiceEstadia];
    int numQuarto = estadia.getNumQuarto();

    // Buscar informações do quarto
    Quarto quarto = buscarQuartoPorNumero(numQuarto);

    if (quarto.getNumQuarto() == 0) {
        cout << vermelhoBold << "   ✖ Quarto não encontrado!" << reset << "\n";
        cout << vermelho << "   ⚠ Não foi possível encontrar o quarto " << numQuarto << " no sistema." << reset << "\n";
        Sleep(3000);
        return;
    }

    // Calcular valor total
    float valorTotal = quarto.getValorDiaria() * estadia.getQuantidadeDiarias();

    // Exibir informações
    limparTela();
    cout << "╔══════════════════════════════════════════════════════╗\n";
    cout << "║" << cianoBold << "        📋 DADOS DA ESTADIA PARA BAIXA 📋         " << reset << "║\n";
    cout << "╚══════════════════════════════════════════════════════╝\n\n";

    cout << "   ┌─────────────────────────────────────────────┐\n";
    cout << "   │" << cianoBold << "              📋 DADOS DA ESTADIA            "<< reset <<"│\n";
    cout << "   ├─────────────────────────────────────────────┤\n";
    cout << "   │" << azul << "   Código da Estadia: " << reset << verdeBold << estadia.getCodigoEstadia() << reset;
    int espacos = 25 - (int)to_string(estadia.getCodigoEstadia()).length();
    for (int i = 0; i < espacos; i++) cout << " ";
    cout << "│\n";
    cout << "   │" << azul << "   Código do Cliente: " << reset << verdeBold << estadia.getCodigoCliente() << reset;
    espacos = 25 - (int)to_string(estadia.getCodigoCliente()).length();
    for (int i = 0; i < espacos; i++) cout << " ";
    cout << "│\n";
    cout << "   │" << azul << "   Check-in:          " << reset << verde << estadia.getDataEntrada() << reset;
    espacos = 25 - (int)estadia.getDataEntrada().length();
    for (int i = 0; i < espacos; i++) cout << " ";
    cout << "│\n";
    cout << "   │" << azul << "   Checkout:          " << reset << verde << estadia.getDataSaida() << reset;
    espacos = 25 - (int)estadia.getDataSaida().length();
    for (int i = 0; i < espacos; i++) cout << " ";
    cout << "│\n";
    cout << "   │" << azul << "   Diárias:           " << reset << verdeBold << estadia.getQuantidadeDiarias() << " diária(s)" << reset;
    espacos = 25 - (int)to_string(estadia.getQuantidadeDiarias()).length() - 9;
    for (int i = 0; i < espacos; i++) cout << " ";
    cout << "│\n";
    cout << "   │" << azul << "   Hóspedes:          " << reset << verdeBold << estadia.getQuantidadeHospedes() << reset;
    espacos = 25 - (int)to_string(estadia.getQuantidadeHospedes()).length();
    for (int i = 0; i < espacos; i++) cout << " ";
    cout << "│\n";
    cout << "   │" << azul << "   Quarto:            " << reset << verdeBold << estadia.getNumQuarto() << reset;
    espacos = 25 - (int)to_string(estadia.getNumQuarto()).length();
    for (int i = 0; i < espacos; i++) cout << " ";
    cout << "│\n";
    cout << "   ├─────────────────────────────────────────────┤\n";
    cout << "   │" << cianoBold << "              💰 VALOR A PAGAR                "<< reset <<"│\n";
    cout << "   ├─────────────────────────────────────────────┤\n";
    cout << "   │" << azul << "   Valor da Diária:   " << reset << verdeBold << "R$ " << fixed << setprecision(2) << quarto.getValorDiaria() << reset;
    // Calcular espaços para alinhamento (aproximado)
    int tamanhoValorDiaria = (quarto.getValorDiaria() >= 1000) ? 8 : ((quarto.getValorDiaria() >= 100) ? 7 : 6);
    espacos = 25 - tamanhoValorDiaria;
    if (espacos < 0) espacos = 0;
    for (int i = 0; i < espacos; i++) cout << " ";
    cout << "│\n";
    cout << "   │" << azul << "   Quantidade:        " << reset << verde << estadia.getQuantidadeDiarias() << " diária(s)" << reset;
    espacos = 25 - (int)to_string(estadia.getQuantidadeDiarias()).length() - 9;
    if (espacos < 0) espacos = 0;
    for (int i = 0; i < espacos; i++) cout << " ";
    cout << "│\n";
    cout << "   │" << roxoBold << "   ───────────────────────────────────────────" << reset << "│\n";
    cout << "   │" << roxoBold << "   TOTAL A PAGAR:     " << reset << verdeBold << "R$ " << fixed << setprecision(2) << valorTotal << reset;
    // Calcular espaços para alinhamento (aproximado)
    int tamanhoValorTotal = (valorTotal >= 10000) ? 9 : ((valorTotal >= 1000) ? 8 : ((valorTotal >= 100) ? 7 : 6));
    espacos = 25 - tamanhoValorTotal;
    if (espacos < 0) espacos = 0;
    for (int i = 0; i < espacos; i++) cout << " ";
    cout << "│\n";
    cout << "   └─────────────────────────────────────────────┘\n\n";

    // Solicitar confirmação do usuário
    cout << amareloBold << "   ⚠ ATENÇÃO: Esta operação irá:" << reset << "\n";
    cout << vermelho << "      • Marcar o quarto " << numQuarto << " como desocupado" << reset << "\n";
    cout << vermelho << "      • Remover a estadia do sistema" << reset << "\n";
    cout << vermelho << "      • Esta ação não pode ser desfeita!" << reset << "\n\n";

    char confirmar;
    cout << amareloBold << "   ➤ Deseja realmente dar baixa nesta estadia? (s/n): " << reset;
    cin >> confirmar;
    cin.ignore(10000, '\n');

    // Verificar confirmação
    if (tolower(confirmar) != 's') {
        limparTela();
        cout << "╔══════════════════════════════════════════════════════╗\n";
        cout << "║" << amareloBold << "        ⚠ OPERAÇÃO CANCELADA ⚠                  " << reset << "║\n";
        cout << "╚══════════════════════════════════════════════════════╝\n\n";
        cout << amarelo << "   A baixa da estadia foi cancelada pelo usuário." << reset << "\n";
        cout << amarelo << "   Nenhuma alteração foi realizada no sistema." << reset << "\n\n";
        cout << amareloBold << "   Pressione ENTER para voltar ao menu principal..." << reset;
        cin.get();
        return;
    }

    // Usuário confirmou - realizar a baixa
    limparTela();
    cout << "╔══════════════════════════════════════════════════════╗\n";
    cout << "║" << verdeBold << "     ✅ PROCESSANDO BAIXA DA ESTADIA ✅           " << reset << "║\n";
    cout << "╚══════════════════════════════════════════════════════╝\n\n";

    // Alterar status do quarto para desocupado
    cout << amareloBold << "   🔄 Alterando status do quarto" << reset;
    for (int i = 0; i < 3; i++) {
        cout << amarelo << ".";
        Sleep(1000);
    }
    cout << reset << "\n";

    if (alterarStatusQuarto(numQuarto, "desocupado")) {
        cout << verdeBold << "   ✔ Quarto " << numQuarto << " marcado como desocupado!" << reset << "\n";
    } else {
        cout << vermelhoBold << "   ✖ Erro ao alterar status do quarto!" << reset << "\n";
    }

    // Adicionar pontos de fidelidade ao cliente (10 pontos por diária)
    int pontosGanhos = estadia.getQuantidadeDiarias() * 10;
    bool pontosAdicionados = false;
    for (auto& cliente : clientes) {
        if (cliente.getCodigo() == estadia.getCodigoCliente()) {
            cliente.adicionarPontosFidelidade(pontosGanhos);
            pontosAdicionados = true;
            cout << verdeBold << "   ✔ " << pontosGanhos << " pontos de fidelidade adicionados ao cliente!" << reset << "\n";
            break;
        }
    }

    if (!pontosAdicionados) {
        cout << amareloBold << "   ⚠ Cliente não encontrado para adicionar pontos de fidelidade." << reset << "\n";
    } else {
        // Salvar clientes atualizados com os novos pontos
        salvarTodosClientes();
    }

    // Remover estadia do vetor
    estadias.erase(estadias.begin() + indiceEstadia);

    // Salvar estadias atualizadas no arquivo
    salvarEstadias();

    // Recarregar estadias do arquivo para garantir sincronização
    carregarEstadias();

    cout << verdeBold << "   ✔ Estadia removida do sistema!" << reset << "\n\n";
    cout << "   ┌─────────────────────────────────────────────┐\n";
    cout << "   │" << verdeBold << "     ✅ BAIXA REALIZADA COM SUCESSO! ✅        "<< reset <<"│\n";
    cout << "   └─────────────────────────────────────────────┘\n\n";
                cout << amareloBold << "   Deseja dar baixa em outra estadia? (s/n): " << reset;
                char continuar;
                cin >> continuar;
                limparBuffer();
                if (tolower(continuar) == 's') {
                    darBaixaEstadia();
                }
}

// Função para inicializar 20 quartos fixos
void inicializarQuartosFixos() {
    // Verificar se já existem quartos no arquivo
    vector<Quarto> quartosExistentes = lerQuartosDoArquivo();
    if (!quartosExistentes.empty()) {
        return; // Já existem quartos, não inicializar novamente
    }

    // Criar 20 quartos fixos
    Quarto quartos[20];

    // Quartos para 2 hóspedes (7 quartos)
    for (int i = 0; i < 7; i++) {
        quartos[i].setNumQuarto(101 + i);
        quartos[i].setQuantidadeHospedes(2);
        quartos[i].setValorDiaria(150.0 + (i * 10));
        quartos[i].setStatus("desocupado");
        string descricoes[] = {
            "Quarto aconchegante com cama de casal, ideal para casais",
            "Quarto standard com vista para o jardim",
            "Quarto confortável com varanda privativa",
            "Quarto com decoração moderna e funcional",
            "Quarto com ar condicionado e TV",
            "Quarto silencioso, perfeito para descanso",
            "Quarto com minibar e cofre"
        };
        quartos[i].setDescricao(descricoes[i]);
        salvarQuartoNoArquivo(quartos[i]);
    }

    // Quartos para 3 hóspedes (8 quartos)
    for (int i = 7; i < 15; i++) {
        quartos[i].setNumQuarto(201 + (i - 7));
        quartos[i].setQuantidadeHospedes(3);
        quartos[i].setValorDiaria(200.0 + ((i - 7) * 15));
        quartos[i].setStatus("desocupado");
        string descricoes[] = {
            "Quarto familiar com cama de casal e beliche",
            "Quarto espaçoso com área de estar",
            "Quarto com varanda e vista panorâmica",
            "Quarto com sofá-cama adicional",
            "Quarto premium com amenities completos",
            "Quarto com cozinha compacta",
            "Quarto com escritório e Wi-Fi rápido",
            "Quarto com banheira de hidromassagem"
        };
        quartos[i].setDescricao(descricoes[i - 7]);
        salvarQuartoNoArquivo(quartos[i]);
    }

    // Quartos para 4 hóspedes (5 quartos)
    for (int i = 15; i < 20; i++) {
        quartos[i].setNumQuarto(301 + (i - 15));
        quartos[i].setQuantidadeHospedes(4);
        quartos[i].setValorDiaria(280.0 + ((i - 15) * 20));
        quartos[i].setStatus("desocupado");
        string descricoes[] = {
            "Suíte familiar com 2 quartos separados",
            "Quarto master com sala de estar integrada",
            "Quarto executivo com espaço para reuniões",
            "Quarto de luxo com varanda ampla",
            "Quarto presidencial com todas as comodidades"
        };
        quartos[i].setDescricao(descricoes[i - 15]);
        salvarQuartoNoArquivo(quartos[i]);
    }
}

// Função para calcular pontos de fidelidade de um cliente
int calcularPontosFidelidade(int codigoCliente) {
    // Buscar o cliente e retornar seus pontos armazenados
    for (auto& cliente : clientes) {
        if (cliente.getCodigo() == codigoCliente) {
            return cliente.getPontosFidelidade();
        }
    }
    return 0; // Cliente não encontrado
}
