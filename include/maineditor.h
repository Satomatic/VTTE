#ifndef MAINEDITOR_H
#define MAINEDITOR_H

#include "lexer.h"
#include <QWidget>
#include <vector>
#include <string>

struct File{
    std::string filename;
    std::vector <std::string> FileData;
    std::vector <std::vector<std::pair<std::string, QColor>>> SyntaxData;
    int savedCursorX;
    int savedCursorY;
    int savedCursorS;
    int savedScroll;
    bool newfile;
};

class MainEditor : public QWidget
{
    Q_OBJECT

public:
    MainEditor(QWidget *parent = nullptr);
    ~MainEditor();
    void paintEvent(QPaintEvent *e);
    std::vector <std::vector<std::pair<std::string, QColor>>> SyntaxData;
    std::vector <std::string> FileData;
    QWidget *parent;

    std::vector <File> files;
    int fi;

    std::string fontName;
    int fontSize;
    int fontHeight;

    int cursorx;
    int cursory;
    int cursors;
    int scroll;

    int linespace;

    lexer lexerClass;
};

#endif // MAINEDITOR_H
