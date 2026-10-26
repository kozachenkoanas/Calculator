#include <iostream>
#include <string>
#include "PluginManager.h"
#include "Lexer.h"
#include "Parser.h"
#include "Exceptions.h"

int main() {
    std::ios::sync_with_stdio(false);

    PluginManager manager;
    try {
        manager.LoadAll("./plugins");
    }
    catch (const std::exception& e) {
        std::cerr << "Init error: " << e.what() << "\n";
    }

    std::cout << "Calculator ready. Type 'exit' to quit.\n";

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;
        if (line == "exit" || line == "quit") break;
        if (line.empty()) continue;

        try {
            Lexer lexer(line);
            auto tokens = lexer.Tokenize();

            Parser parser(std::move(tokens), manager);
            auto ast = parser.Parse();

            double result = ast->Evaluate();
            std::cout << "= " << result << "\n";

        }
        catch (const CalcException& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
        catch (const std::exception& e) {
            std::cerr << "Unexpected error: " << e.what() << "\n";
        }
    }

    return 0;
}