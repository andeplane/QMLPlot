#ifndef LINEGRAPH_H
#define LINEGRAPH_H

#include "graph.h"

class LineGraphDataSource : public QObject {
    Q_OBJECT
public:
    QVector<QPointF> m_points;
    int m_firstIndex = 0;
    int m_numberOfPoints = 0;
    int m_maxNumberOfPoints = 500;
    void cleanupMemory();
    Q_INVOKABLE void addPoint(float x, float y);
signals:
    void dataChanged();
};

class LineGraph : public Graph
{
    Q_OBJECT
    Q_PROPERTY(LineGraphDataSource* dataSource READ dataSource WRITE setDataSource NOTIFY dataSourceChanged)
private:
    LineGraphDataSource m_defaultDataSource;
    LineGraphDataSource* m_dataSource = nullptr;
    bool isInValueRange(const QPointF &point, const float &xMin, const float &xMax, const float &yMin, const float &yMax);

public:
    LineGraph();
    ~LineGraph();
    virtual void paint(Figure *figure, QPainter *painter);
    LineGraphDataSource* dataSource() const;

public slots:
    void addPoint(float x, float y);
    void setDataSource(LineGraphDataSource* dataSource);
signals:
    void dataSourceChanged(LineGraphDataSource* dataSource);
};

#endif // LINEGRAPH_H
