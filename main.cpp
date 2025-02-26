#include "cmainwindow.h"
#include <QApplication>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
///    int w_ecran = app.desktop()->width();
///    int h_ecran = app.desktop()->height();

///    CMainWindow w(w_ecran, h_ecran);
/// Modifs qt6
    CMainWindow w;
    w.show();

    return app.exec();
}
