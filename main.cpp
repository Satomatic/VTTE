#include <maineditor.h>
#include <mainwindow.h>

#include <QApplication>
#include <QLineEdit>
#include <QVBoxLayout>

#include <core/keybind.h>
#include <global.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

    MainWindow _MainWindow;

    MainEditor editor(&_MainWindow);
	editor.show();

    _MainWindow.setCentralWidget(&editor);
    _MainWindow.editor = &editor;
    _MainWindow.show();

    InitKeybinds();

	return a.exec();
}
