#pragma once
#include <stdexcept>
#include <string>

class CalcException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class LexerException : public CalcException {
public: using CalcException::CalcException;
};

class ParseException : public CalcException {
public: using CalcException::CalcException;
};

class PluginException : public CalcException {
public: using CalcException::CalcException;
};

class EvalException : public CalcException {
public: using CalcException::CalcException;
};