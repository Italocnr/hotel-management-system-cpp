// Framework de testes simples e funcional
// Header-only, sem dependências externas

#ifndef SIMPLE_TEST_HPP
#define SIMPLE_TEST_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cassert>

namespace SimpleTest {

struct TestResult {
    std::string name;
    bool passed;
    std::string error;
    int line;
    std::string file;
};

class TestRunner {
private:
    std::vector<TestResult> results;
    int totalTests = 0;
    int passedTests = 0;
    int failedTests = 0;
    std::string currentTestName;
    bool currentTestFailed = false;
    std::string currentTestError;

public:
    static TestRunner& getInstance() {
        static TestRunner instance;
        return instance;
    }

    void startTest(const std::string& name) {
        currentTestName = name;
        currentTestFailed = false;
        currentTestError = "";
        totalTests++;
    }

    void recordFailure(const std::string& error, int line, const char* file) {
        currentTestFailed = true;
        std::ostringstream oss;
        oss << file << ":" << line << " - " << error;
        currentTestError = oss.str();
    }

    void endTest() {
        TestResult result;
        result.name = currentTestName;
        result.passed = !currentTestFailed;
        result.error = currentTestError;
        
        results.push_back(result);
        
        if (result.passed) {
            passedTests++;
            std::cout << "  ✓ " << result.name << std::endl;
        } else {
            failedTests++;
            std::cout << "  ✗ " << result.name << std::endl;
            if (!result.error.empty()) {
                std::cout << "    " << result.error << std::endl;
            }
        }
    }

    void printSummary() {
        std::cout << "\n========================================" << std::endl;
        std::cout << "  RESUMO DOS TESTES" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Total: " << totalTests << std::endl;
        std::cout << "Passou: " << passedTests << " ✓" << std::endl;
        std::cout << "Falhou: " << failedTests << " ✗" << std::endl;
        
        if (failedTests > 0) {
            std::cout << "\nTestes que falharam:" << std::endl;
            for (const auto& result : results) {
                if (!result.passed) {
                    std::cout << "  - " << result.name << std::endl;
                    if (!result.error.empty()) {
                        std::cout << "    " << result.error << std::endl;
                    }
                }
            }
        }
        
        std::cout << "========================================" << std::endl;
        
        if (failedTests == 0) {
            std::cout << "TODOS OS TESTES PASSARAM! ✓" << std::endl;
        } else {
            std::cout << "ALGUNS TESTES FALHARAM! ✗" << std::endl;
        }
    }

    int getExitCode() const {
        return failedTests > 0 ? 1 : 0;
    }
};

// Helper para gerar nomes únicos usando __LINE__
#define CONCAT_INNER(a, b) a##b
#define CONCAT(a, b) CONCAT_INNER(a, b)

// Macros para testes
#define TEST_CASE(name) \
    TEST_CASE_IMPL(name, __LINE__)

#define TEST_CASE_IMPL(name, line) \
    static void CONCAT(test_func_, line)(); \
    static struct CONCAT(TestRegistrar_, line) { \
        CONCAT(TestRegistrar_, line)() { \
            SimpleTest::TestRunner::getInstance().startTest(name); \
            try { \
                CONCAT(test_func_, line)(); \
            } catch (const std::exception& e) { \
                SimpleTest::TestRunner::getInstance().recordFailure( \
                    std::string("Exception: ") + e.what(), __LINE__, __FILE__); \
            } catch (...) { \
                SimpleTest::TestRunner::getInstance().recordFailure( \
                    "Unknown exception", __LINE__, __FILE__); \
            } \
            SimpleTest::TestRunner::getInstance().endTest(); \
        } \
    } CONCAT(test_registrar_instance_, line); \
    static void CONCAT(test_func_, line)()

// Macros para asserções
#define REQUIRE(condition) \
    do { \
        if (!(condition)) { \
            std::ostringstream oss; \
            oss << "REQUIRE failed: " << #condition; \
            SimpleTest::TestRunner::getInstance().recordFailure( \
                oss.str(), __LINE__, __FILE__); \
            return; \
        } \
    } while(0)

#define REQUIRE_EQ(a, b) \
    do { \
        if ((a) != (b)) { \
            std::ostringstream oss; \
            oss << "REQUIRE_EQ failed: " << #a << " (" << (a) << ") != " << #b << " (" << (b) << ")"; \
            SimpleTest::TestRunner::getInstance().recordFailure( \
                oss.str(), __LINE__, __FILE__); \
            return; \
        } \
    } while(0)

#define REQUIRE_NE(a, b) \
    do { \
        if ((a) == (b)) { \
            std::ostringstream oss; \
            oss << "REQUIRE_NE failed: " << #a << " (" << (a) << ") == " << #b << " (" << (b) << ")"; \
            SimpleTest::TestRunner::getInstance().recordFailure( \
                oss.str(), __LINE__, __FILE__); \
            return; \
        } \
    } while(0)

#define REQUIRE_LT(a, b) \
    do { \
        if (!((a) < (b))) { \
            std::ostringstream oss; \
            oss << "REQUIRE_LT failed: " << #a << " (" << (a) << ") >= " << #b << " (" << (b) << ")"; \
            SimpleTest::TestRunner::getInstance().recordFailure( \
                oss.str(), __LINE__, __FILE__); \
            return; \
        } \
    } while(0)

#define REQUIRE_GT(a, b) \
    do { \
        if (!((a) > (b))) { \
            std::ostringstream oss; \
            oss << "REQUIRE_GT failed: " << #a << " (" << (a) << ") <= " << #b << " (" << (b) << ")"; \
            SimpleTest::TestRunner::getInstance().recordFailure( \
                oss.str(), __LINE__, __FILE__); \
            return; \
        } \
    } while(0)

#define REQUIRE_FLOAT_EQ(a, b, epsilon) \
    do { \
        double diff = (a) > (b) ? (a) - (b) : (b) - (a); \
        if (diff > epsilon) { \
            std::ostringstream oss; \
            oss << "REQUIRE_FLOAT_EQ failed: " << #a << " (" << (a) << ") != " << #b << " (" << (b) << ")"; \
            SimpleTest::TestRunner::getInstance().recordFailure( \
                oss.str(), __LINE__, __FILE__); \
            return; \
        } \
    } while(0)

} // namespace SimpleTest

#endif // SIMPLE_TEST_HPP

