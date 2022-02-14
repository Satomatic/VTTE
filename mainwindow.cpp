#include <mainwindow.h>
#include <iostream>
#include <file.h>
#include <ctype.h>
#include <core/keybind.h>

#include <QPainterPath>
#include <QPainter>
#include <QFileDialog>
#include <fstream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->setWindowTitle("Very tiny text editor");
    this->setGeometry(0,0,500,400);
    this->setStyleSheet("background-color: #161616");
    this->setContentsMargins(10, 10, 10, 10);
}

MainWindow::~MainWindow()
{

}

/*
 *  TODO: Make a better system for keybinds.
 *  TODO: Make scrolling a little nicer.
 *  TODO: Clicking to control the cursor position.
 */

void MainWindow::paintEvent(QPaintEvent *e){
    QWidget::paintEvent(e);
    QPainter painter(this);

    QPainterPath path;
    path.addRect(QRect(10, 10, this->width() - 20, this->height() - 20));
    painter.setPen(Qt::white);
    painter.drawPath(path);
}

void MainWindow::wheelEvent(QWheelEvent *event){
    QPoint numDegrees = event->angleDelta();
    QFontMetrics fm(QFont(editor->fontName.c_str(), editor->fontSize));

    if ((fm.height() * editor->files[editor->fi].FileData.size()) + (editor->files[editor->fi].FileData.size() * editor->linespace) > this->height() - 40){
        int vala = (fm.height() * editor->files[editor->fi].FileData.size()) + (editor->files[editor->fi].FileData.size() * editor->linespace);

        if (0 - editor->scroll >= vala - 40 && numDegrees.y() < 0){
            return;
        }

        editor->scroll += numDegrees.y() / 4;
        if (editor->scroll > 0) editor->scroll = 0;
        editor->repaint();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    std::string currentline = editor->files[editor->fi].FileData[editor->cursory];
    std::string before = currentline.substr(0, editor->cursorx);
    std::string after = currentline.substr(editor->cursorx, editor->cursorx - currentline.size());

    bool overwriteCursorSave = false;

    /*
     *  If a modifier is being pressed, search though the
     *  key binds. Once there is one which matches the key and
     *  modifier, call it's assigned action passing the editor
     *  class as an argument.
     */
    if (event->modifiers() != 0){
        for (int i = 0; i < KeyBinds.size(); i++){
            if (KeyBinds[i].modifiers == event->modifiers() &&
                KeyBinds[i].key == QKeySequence(event->key()).toString().toStdString()){

                KeyBinds[i].function(editor);
                editor->repaint();
                return;
            }
        }
    }

    switch(event->key()){
        case Qt::Key_Right:
            if (editor->cursorx < editor->files[editor->fi].FileData[editor->cursory].size()){
                editor->cursorx ++;
                editor->cursors ++;
            }
            break;

        case Qt::Key_Left:
            if (editor->cursorx > 0){
                editor->cursorx --;
                editor->cursors --;
            }
            break;

        case Qt::Key_Up:
            if (editor->cursory > 0){
                editor->cursorx = editor->cursors;

                /*
                 *  Check if the previous lines shorter than the curX
                 *  and adjust if needed.
                 */
                if (editor->files[editor->fi].FileData[editor->cursory - 1].size() < editor->cursorx){
                    editor->cursorx = editor->files[editor->fi].FileData[editor->cursory - 1].size();
                }

                editor->cursory --;
            }
            break;

        case Qt::Key_Down:
            /*
             *  If cursor position Y is less than the file size, then
             *  we are allowed to move the cursor down.
             */
            if (editor->cursory < editor->files[editor->fi].FileData.size() - 1){
                editor->cursorx = editor->cursors;

                /*
                 *  If the line we are moving to is smaller than curX
                 *  we should alter the curX to prevent faults.
                 */
                if (editor->files[editor->fi].FileData[editor->cursory + 1].size() < editor->cursorx){
                    editor->cursorx = editor->files[editor->fi].FileData[editor->cursory + 1].size();
                }

                editor->cursory ++;
            }
            break;

        case Qt::Key_Tab:
            /*
             *  Ideally the user should be able to set the tab length
             *  but for now just inserting 4 will do nicely.
             */
            editor->files[editor->fi].FileData[editor->cursory] = before + "    " + after;
            editor->files[editor->fi].SyntaxData[editor->cursory] = editor->lexerClass.SyntaxLine(before + "    " + after);
            editor->cursorx += 4;
            overwriteCursorSave = true;
            break;

        case Qt::Key_Backspace:
            if (editor->cursorx > 0){
                std::string before = currentline.substr(0, editor->cursorx - 1);
                std::string after = currentline.substr(editor->cursorx, editor->cursorx - currentline.size());
                std::string newline = before + after;

                editor->files[editor->fi].FileData[editor->cursory] = newline;
                editor->files[editor->fi].SyntaxData[editor->cursory] = editor->lexerClass.SyntaxLine(newline);
                editor->cursorx --;

            } else if (editor->cursorx == 0 && editor->cursory > 0){
                /*
                 *  Concatenate the previous and current line then we
                 *  move that new line to be the new previous line.
                 */
                std::string previousLine = editor->files[editor->fi].FileData[editor->cursory - 1];
                editor->files[editor->fi].FileData[editor->cursory - 1] = previousLine + currentline;
                editor->files[editor->fi].SyntaxData[editor->cursory - 1] = editor->lexerClass.SyntaxLine(previousLine + currentline);

                /*
                 *  Erase the current line from both the FileData and
                 *  the SyntaxData.
                 */
                editor->files[editor->fi].FileData.erase(editor->files[editor->fi].FileData.begin() + editor->cursory);
                editor->files[editor->fi].SyntaxData.erase(editor->files[editor->fi].SyntaxData.begin() + editor->cursory);

                editor->cursory --;
                editor->cursorx = previousLine.size();
            }

            overwriteCursorSave = true;
            break;

        case Qt::Key_Return:
            /*
             *  If cursor is at the end of the current line, insert a
             *  new line below.
             */
            if (editor->cursorx == editor->files[editor->fi].FileData[editor->cursory].size()){
                std::vector <std::pair<std::string, QColor>> temp = {std::make_pair("", QColor(255, 255, 255))};
                editor->files[editor->fi].FileData.insert(editor->files[editor->fi].FileData.begin() + editor->cursory + 1, "");
                editor->files[editor->fi].SyntaxData.insert(editor->files[editor->fi].SyntaxData.begin() + editor->cursory + 1, temp);

            /*
             *  Otherwise we can just split the line, set the current
             *  line to be whatever is before the cursor, and set the
             *  next line to be whatever is after.
             */
            } else {
                editor->files[editor->fi].FileData[editor->cursory] = before;
                editor->files[editor->fi].SyntaxData[editor->cursory] = editor->lexerClass.SyntaxLine((before));
                editor->files[editor->fi].FileData.insert(editor->files[editor->fi].FileData.begin() + editor->cursory + 1, after);
                editor->files[editor->fi].SyntaxData.insert(editor->files[editor->fi].SyntaxData.begin() + editor->cursory + 1, editor->lexerClass.SyntaxLine(after));
            }

            editor->cursorx = 0;
            editor->cursory ++;
            overwriteCursorSave = true;
            break;

        default:
            /*
             *  Make sure the cursor is actually in the bounds of the
             *  line.
             */
            if (editor->cursorx <= editor->files[editor->fi].FileData[editor->cursory].size()){
                char character = event->text().toStdString()[0];

                /*
                 *  Then make sure the character typed isn't special.
                 */
                if (isalnum(character) || ispunct(character) || isspace(character)){
                    std::string newline = before + event->text().toStdString() + after;

                    editor->files[editor->fi].FileData[editor->cursory] = newline;
                    editor->files[editor->fi].SyntaxData[editor->cursory] = editor->lexerClass.SyntaxLine(newline);
                    editor->cursorx ++;
                }
            }
            overwriteCursorSave = true;
            break;
    }

    /*
     *  If the user has made an input which will change the file, the
     *  cursors saved value should be updated.
     */
    if (overwriteCursorSave){
        editor->cursors = editor->cursorx;
        editor->files[editor->fi].edited = true;
        editor->updateTitle();
    }

    int cy = editor->fontHeight * (editor->cursory + 1) - 1 + (editor->linespace * editor->cursory) + editor->scroll;
    if (cy > this->height() - 40){
        int diff = cy - (this->height() - 40);
        editor->scroll -= diff;

    } else if (cy < 10){
        int diff = abs(editor->scroll) - (editor->fontHeight * (editor->cursory + 1) - 1 + (editor->linespace * editor->cursory));
        editor->scroll += (diff + editor->fontHeight);
        if (editor->scroll > 0) editor->scroll = 0;
    }

    editor->repaint();
}
