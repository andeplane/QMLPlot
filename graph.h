#ifndef GRAPH_H
#define GRAPH_H

#include <QQuickItem>
#include <QPainter>
class Figure;
class Graph : public QQuickItem
{
    Q_OBJECT
public:
    virtual void paint(Figure *figure, QPainter *painter) = 0;
signals:

public slots:
};

#endif // GRAPH_H
