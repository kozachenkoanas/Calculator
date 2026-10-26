#include "Parser.h"
#include "Exceptions.h"

// Реализация Evaluate для узлов
double BinaryOpNode::Evaluate() const {
    double l = m_left->Evaluate();
    double r = m_right->Evaluate();
    switch (m_op) {
    case '+': return l + r;
    case '-': return l - r;
    case '*': return l * r;
    case '/':
        if (r == 0) throw EvalException("Division by zero");
        return l / r;
    case '^': return std::pow(l, r);
    default: throw EvalException("Unknown operator");
    }
}

double FunctionNode::Evaluate() const {
    if (m_args.size() != static_cast<size_t>(m_arity)) {
        throw EvalException("Wrong number of arguments for " + m_name);
    }
    double a = m_args[0]->Evaluate();
    double b = (m_arity == 2) ? m_args[1]->Evaluate() : 0.0;
    try {
        return m_func(a, b);
    }
    catch (const std::exception& e) {
        throw EvalException("Error in plugin '" + m_name + "': " + e.what());
    }
    catch (...) {
        throw EvalException("Unknown error in plugin '" + m_name + "'");
    }
}

// Реализация Parser
Parser::Parser(std::vector<Token> tokens, const PluginManager& manager)
    : m_tokens(std::move(tokens)), m_manager(manager) {
}

std::unique_ptr<Node> Parser::Parse() {
    auto result = ParseExpression();
    if (peek().type != 'E') {
        throw ParseException("Unexpected token after expression");
    }
    return result;
}

std::unique_ptr<Node> Parser::ParseExpression() {
    auto left = ParseTerm();
    while (peek().type == '+' || peek().type == '-') {
        char op = get().type;
        auto right = ParseTerm();
        left = std::make_unique<BinaryOpNode>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<Node> Parser::ParseTerm() {
    auto left = ParsePower();
    while (peek().type == '*' || peek().type == '/') {
        char op = get().type;
        auto right = ParsePower();
        left = std::make_unique<BinaryOpNode>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<Node> Parser::ParsePower() {
    auto base = ParsePrimary();
    if (peek().type == '^') {
        get();
        auto exp = ParsePower();
        return std::make_unique<BinaryOpNode>('^', std::move(base), std::move(exp));
    }
    return base;
}

std::unique_ptr<Node> Parser::ParsePrimary() {
    if (peek().type == 'N') {
        double val = get().value;
        return std::make_unique<NumberNode>(val);
    }
    else if (peek().type == '(') {
        get();
        auto node = ParseExpression();
        if (peek().type != ')') throw ParseException("Expected ')'");
        get();
        return node;
    }
    else if (peek().type == 'I') {
        std::string name = get().text;
        const PluginDescriptor* desc = m_manager.FindFunction(name);
        if (!desc) throw ParseException("Unknown function: " + name);

        if (peek().type != '(') throw ParseException("Expected '(' after function name");
        get();

        std::vector<std::unique_ptr<Node>> args;
        if (peek().type != ')') {
            args.push_back(ParseExpression());
            while (peek().type == ',') {
                get();
                args.push_back(ParseExpression());
            }
        }

        if (peek().type != ')') throw ParseException("Expected ')'");
        get();

        return std::make_unique<FunctionNode>(name, desc, std::move(args));
    }

    throw ParseException("Unexpected token in expression");
}