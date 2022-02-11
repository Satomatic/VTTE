#include <maineditor.h>
#include <mainwindow.h>

#include <QApplication>
#include <QLineEdit>
#include <QVBoxLayout>
#include <iostream>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	MainWindow w;

    MainEditor editor(&w);
	editor.show();

	w.setCentralWidget(&editor);
	w.editor = &editor;
	w.show();

	return a.exec();
}
