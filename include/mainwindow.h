#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "maineditor.h"

#include <QMainWindow>
#include <QLineEdit>
#include <QKeyEvent>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    MainEditor *editor;
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    void paintEvent(QPaintEvent *e);
};

#endif // MAINWINDOW_H
