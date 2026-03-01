# Framework de Testes Simples

## ✅ Solução Implementada

Framework de testes **header-only** e **sem dependências externas**.

- ✅ **Compila sem problemas** - não precisa de linkagem externa
- ✅ **Funciona com qualquer versão do MinGW/GCC**
- ✅ **Fácil de usar** - sintaxe similar ao Catch2 substituindo o munit que é utilizado para linguagem C pura.
- ✅ **Sem dependências** - apenas C++ padrão

## 🚀 Como Usar

### Compilar e Executar

```bash
cd testes
.\run_tests.bat
```

O script usa `test_main_simple.cpp` que utiliza o framework simples.

## 📝 Sintaxe

### Criar um teste

```cpp
TEST_CASE("Nome do teste") {
    // Seu código de teste aqui
    REQUIRE(condição);
    REQUIRE_EQ(a, b);
    REQUIRE_NE(a, b);
    REQUIRE_FLOAT_EQ(a, b, 0.01f);
}
```

### Macros Disponíveis

- `REQUIRE(condition)` - Verifica se a condição é verdadeira
- `REQUIRE_EQ(a, b)` - Verifica se a == b
- `REQUIRE_NE(a, b)` - Verifica se a != b
- `REQUIRE_LT(a, b)` - Verifica se a < b
- `REQUIRE_GT(a, b)` - Verifica se a > b
- `REQUIRE_FLOAT_EQ(a, b, epsilon)` - Verifica se floats são iguais (com tolerância)

## 📊 Saída

O framework mostra:
- ✓ para testes que passaram
- ✗ para testes que falharam
- Resumo final com total de testes, passou/falhou
- Detalhes dos erros quando houver falhas

## 🔧 Arquivos

- `simple_test.hpp` - Framework de testes (header-only)
- `test_main_simple.cpp` - Testes 
- `run_tests.bat` - Script para rodar os testes