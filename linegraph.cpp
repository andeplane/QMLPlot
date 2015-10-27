#include "linegraph.h"
#include "figure.h"

LineGraph::LineGraph()
{

}

LineGraph::~LineGraph()
{
    m_points.clear();
}

bool LineGraph::isInValueRange(const QPointF &point, const QRectF &valueRange) {
    return point.x()>=valueRange.left() && point.x() <= valueRange.right() && point.y() >= valueRange.top() && point.y() <= valueRange.bottom();
}


void LineGraph::paint(Figure *figure, QPainter *painter)
{
    QVector<QPointF> scaledPoints;
    QRectF valueRange = figure->valueRange();
    float rangeX = valueRange.width();
    float rangeY = valueRange.height();
    float xMin = valueRange.left();
    float yMin = valueRange.top();

    for(int i=m_firstIndex; i<m_points.size()-1; i++) {
        const QPointF &p1 = m_points[i];
        const QPointF &p2 = m_points[i+1];
        if(!isInValueRange(p1, valueRange) || !isInValueRange(p2, valueRange)) {
            continue;
        }

        float x1 = (p1.x()-xMin)/rangeX;
        float y1 = (p1.y()-yMin)/rangeY;
        float x2 = (p2.x()-xMin)/rangeX;
        float y2 = (p2.y()-yMin)/rangeY;
        scaledPoints.push_back(figure->scaled(QPointF(x1,y1), figure->figureRectangle(), figure->figureRectangle().topLeft()));
        scaledPoints.push_back(figure->scaled(QPointF(x2,y2), figure->figureRectangle(), figure->figureRectangle().topLeft()));
    }

    painter->drawLines(scaledPoints);
    setDirty(false);
}

void LineGraph::cleanupMemory() {
    m_points.erase(m_points.begin(), m_points.begin()+m_firstIndex-1);
        m_firstIndex = 0;
        m_numberOfPoints = m_points.size();
}

void LineGraph::addPoint(float x, float y)
{
    m_points.push_back(QPointF(x,y));
    if(++m_numberOfPoints > m_maxNumberOfPoints) {
        m_numberOfPoints = m_maxNumberOfPoints;
        m_firstIndex++;
    }

    if(m_points.size() > 2*m_maxNumberOfPoints) {
        cleanupMemory();
    }
    setDirty(true);
}
