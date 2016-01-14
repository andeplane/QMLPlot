#ifndef LINEGRAPH_H
#define LINEGRAPH_H
#include <functional>
#include "graph.h"

class LineGraphDataSource : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool saveMemory READ saveMemory WRITE setSaveMemory NOTIFY saveMemoryChanged)
private:
    QVector<QPointF> m_points;
    int m_firstIndex = 0;
    int m_numberOfPoints = 0;
    bool m_saveMemory = false;
    int m_maxNumberOfPoints = 1000;
    void cleanupMemory();

public:
    Q_INVOKABLE void addPoint(float x, float y);
    QPointF operator[](int index) { return m_points[index+m_firstIndex*m_saveMemory]; }
    QPointF get(int index) { return m_points[index+m_firstIndex*m_saveMemory]; }
    Q_INVOKABLE void setPoints(QVector<QPointF> points);
    void iterate(std::function<void(int i, QPointF point)> action);
    int size();
    void clear();
    bool saveMemory() const;

public slots:
    void setSaveMemory(bool saveMemory);

signals:
    void dataChanged();
    void saveMemoryChanged(bool saveMemory);
};

class LineGraph : public Graph
{
    Q_OBJECT
    Q_PROPERTY(LineGraphDataSource* dataSource READ dataSource WRITE setDataSource NOTIFY dataSourceChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(Qt::PenStyle style READ style WRITE setStyle NOTIFY styleChanged)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
private:
    LineGraphDataSource m_defaultDataSource;
    LineGraphDataSource* m_dataSource = nullptr;
    bool isInValueRange(const QPointF &point, const float &xMin, const float &xMax, const float &yMin, const float &yMax);
    QColor m_color = "black";
    Qt::PenStyle m_style = Qt::SolidLine;
    int m_width = 1;

public:
    LineGraph();
    ~LineGraph();
    virtual void paint(Figure *figure, QPainter *painter) override;
    LineGraphDataSource* dataSource() const;
    QColor color() const;
    Qt::PenStyle style() const;
    int width() const;

public slots:
    void addPoint(float x, float y);
    void setDataSource(LineGraphDataSource* dataSource);
    void setColor(QColor color);
    void setStyle(Qt::PenStyle style);
    void setWidth(int width);

signals:
    void dataSourceChanged(LineGraphDataSource* dataSource);
    void colorChanged(QColor color);
    void styleChanged(Qt::PenStyle style);
    void widthChanged(int width);

    // Graph interface
public:
    virtual void bounds(double &xMin, double &xMax, double &yMin, double &yMax) override;
};

#endif // LINEGRAPH_H
