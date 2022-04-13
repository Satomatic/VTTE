#ifndef MAINEDITOR_H
#define MAINEDITOR_H

#include "lexer.h"
#include <iostream>
#include <QWidget>
#include <QMouseEvent>
#include <vector>
#include <string>
#include <math.h>

/**
 *  For some reason the font rendering is fine on Linux / Windows,
 *  but on MacOS the font size is much smaller
 */
#if __APPLE__
    #define DF_FONT_SIZE 8
#else
    #define DF_FONT_SIZE 6
#endif

struct File {
    std::string filename;
    std::vector <std::string> FileData;
    std::vector <std::vector<std::pair<std::string, QColor>>> SyntaxData;
    int savedCursorX;
    int savedCursorY;
    int savedCursorS;
    int savedScroll;
    bool newfile;
    bool edited;
};

struct MouseLookupItem {
    int drawY;
    int cursorY;
};

class MainEditor : public QWidget
{
    Q_OBJECT

public:
    MainEditor(QWidget *parent = nullptr);
    ~MainEditor();
    void paintEvent(QPaintEvent *e);
    void updateTitle();
    void mouseClickUpdate(QMouseEvent *e);
    std::vector <std::vector<std::pair<std::string, QColor>>> SyntaxData;
    std::vector <std::string> FileData;
    QWidget *parent;

    std::vector <File> files;
    int fi;

    /**
     *  @note This isn't a great way to implement mouse clicking
     *        but it will work for now.
     */
    std::vector <MouseLookupItem> mouseCursorLookup;

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
