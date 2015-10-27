#ifndef LINEGRAPH_H
#define LINEGRAPH_H

#include "graph.h"

class LineGraph : public Graph
{
    Q_OBJECT
private:
    QVector<QPointF> m_points;
    int m_firstIndex = 0;
    int m_numberOfPoints = 0;
    int m_maxNumberOfPoints = 500;

    bool isInValueRange(const QPointF &point, const QRectF &valueRange);
    void cleanupMemory();
public:
    LineGraph();
    ~LineGraph();
    virtual void paint(Figure *figure, QPainter *painter);
signals:

public slots:
    void addPoint(float x, float y);
};

#endif // LINEGRAPH_H
