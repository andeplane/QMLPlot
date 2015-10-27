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
    void setDirty(bool dirty);

signals:

public slots:
};

#endif // GRAPH_H
