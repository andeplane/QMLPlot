#ifndef GRAPH_H
#define GRAPH_H

#include <QQuickItem>
#include <QPainter>
class Figure;
class Graph : public QQuickItem
{
    Q_OBJECT
protected:
    bool m_dirty = true;

public:
    virtual void paint(Figure *figure, QPainter *painter) = 0;
    virtual void bounds(double &xMin, double &xMax, double &yMin, double &yMax) = 0;

signals:

public slots:
    void setDirty(bool dirty);
    void markDirty();
};

#endif // GRAPH_H
