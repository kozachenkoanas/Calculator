#pragma once
#include "Lexer.h"
#include "PluginManager.h"
#include <memory>
#include <vector>
#include <cmath>

class Node {
public:
    virtual ~Node() = default;
    virtual double Evaluate() const = 0;
};

class NumberNode : public Node {
    double m_value;
public:
    explicit NumberNode(double v) : m_value(v) {}
    double Evaluate() const override { return m_value; }
};

class BinaryOpNode : public Node {
    char m_op;
    std::unique_ptr<Node> m_left;
    std::unique_ptr<Node> m_right;
public:
    BinaryOpNode(char op, std::unique_ptr<Node> l, std::unique_ptr<Node> r)
        : m_op(op), m_left(std::move(l)), m_right(std::move(r)) {
    }

    double Evaluate() const override;
};

class FunctionNode : public Node {
    std::string m_name;
    MathFuncPtr m_func;
    int m_arity;
    std::vector<std::unique_ptr<Node>> m_args;
public:
    FunctionNode(std::string name, const PluginDescriptor* desc, std::vector<std::unique_ptr<Node>> args)
        : m_name(std::move(name)), m_func(desc->func_ptr), m_arity(desc->arity), m_args(std::move(args)) {
    }

    double Evaluate() const override;
};

class Parser {
public:
    Parser(std::vector<Token> tokens, const PluginManager& manager);
    std::unique_ptr<Node> Parse();

private:
    std::vector<Token> m_tokens;
    size_t m_pos = 0;
    const PluginManager& m_manager;

    const Token& peek() const { return m_tokens[m_pos]; }
    const Token& get() { return m_tokens[m_pos++]; }

    std::unique_ptr<Node> ParseExpression();
    std::unique_ptr<Node> ParseTerm();
    std::unique_ptr<Node> ParsePower();
    std::unique_ptr<Node> ParsePrimary();
};