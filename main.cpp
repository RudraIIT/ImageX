#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle("ImageX");
    window.resize(400,300);

    window.show();

    return app.exec();
}