@echo off
echo ========================================
echo   SISTEMA DE TESTES - HOTEL
echo ========================================
echo.

echo [1/3] Verificando framework de testes...
if not exist "simple_test.hpp" (
    echo ERRO: Arquivo simple_test.hpp nao encontrado!
    pause
    exit /b 1
)
echo OK - Framework de testes simples encontrado
echo.

echo [2/3] Compilando testes...
REM Usar framework de testes simples (sem dependências externas)
g++ -std=c++17 -I. -I../codigo test_main_simple.cpp -o test_runner.exe 2>compile_errors.txt
if %errorlevel% neq 0 (
    echo ERRO na compilacao!
    type compile_errors.txt
    del compile_errors.txt 2>nul
    pause
    exit /b 1
)
del compile_errors.txt 2>nul
echo OK - Compilacao concluida
echo.

echo [3/3] Executando testes...
echo.
test_runner.exe
echo.

echo ========================================
echo   Testes concluidos!
echo ========================================
pause

