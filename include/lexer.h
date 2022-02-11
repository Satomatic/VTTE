#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>
#include <QColor>

typedef std::vector <std::string> tStringVector;

class lexer
{
public:
    lexer();
    std::vector <std::pair <std::string, QColor>> SyntaxLine(std::string line);
};

#endif // LEXER_H
