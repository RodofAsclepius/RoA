
#include "mainwindow.h"
#include "maincontroller.h"
#include "colorandfontmap.h"

#include <QApplication>
#include <QDir>
#include <QStyleFactory>
//#include <QDesktopWidget>

int main(int argc, char* argv[])
 {
    /* App */
    QApplication a(argc, argv);
    QString strStyleSheet;

    /* Install my theme */
    QFile f2(":/style.qss");
    if (!f2.exists())
    {
        qDebug() << "Unable to set my stylesheet, file not found";
    }
    else
    {
        f2.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f2);
        strStyleSheet += ts.readAll();
    }

    a.setStyleSheet(strStyleSheet);

    /* Default font */
    CColorAndFontMap& mapColorsAndFont = CColorAndFontMap::instance();
    QApplication::setFont(mapColorsAndFont.getFont("general/default"));

    /* Main window */
    MainWindow w;
    w.setWindowIcon(QIcon(":view/images/confetti.ico"));
    w.show();

    /* Main controller */
    CMainController::instance().initialize();

    int iCode;
    try
    {
        /* Run */
        iCode = a.exec();
    }
    catch(...)
    {
        std::exception_ptr p = std::current_exception();
        qDebug() << (p ? p.__cxa_exception_type()->name() : "null");
//        std::clog <<(p ? p.__cxa_exception_type()->name() : "null") << std::endl;
    }

    return iCode;
}

