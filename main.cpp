#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "linegraph.h"
#include "figure.h"
#include "linegraph.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    // qmlRegisterType<LineGraph>("LineGraph", 1,0,"LineGraph");
    // qmlRegisterType<Figure>("Figure", 1,0,"QPlot");
    qmlRegisterType<Figure>("QMLPlot", 1,0,"Figure");
    qmlRegisterType<LineGraph>("QMLPlot", 1,0,"LineGraph");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

