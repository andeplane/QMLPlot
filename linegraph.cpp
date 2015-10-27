#include "linegraph.h"
#include "figure.h"

LineGraph::LineGraph()
{
    setDataSource(&m_defaultDataSource);
}

LineGraph::~LineGraph()
{

}

bool LineGraph::isInValueRange(const QPointF &point, const float &xMin, const float &xMax, const float &yMin, const float &yMax) {
    return point.x()>=xMin && point.x() <= xMax && point.y() >= yMin && point.y() <= yMax;
}

void LineGraph::paint(Figure *figure, QPainter *painter)
{
    QVector<QPointF> scaledPoints;
    float rangeX = figure->xMax()-figure->xMin();
    float rangeY = figure->yMax()-figure->yMin();
    float xMin = figure->xMin();
    float yMin = figure->yMin();
    float xMax = figure->xMax();
    float yMax = figure->yMax();
    qDebug() << "yMin: " << yMin << " yMax: " << yMax;

    for(int i=m_dataSource->m_firstIndex; i<m_dataSource->m_points.size()-1; i++) {
        const QPointF &p1 = m_dataSource->m_points[i];
        const QPointF &p2 = m_dataSource->m_points[i+1];
        if(!isInValueRange(p1, xMin, xMax, yMin, yMax) || !isInValueRange(p2, xMin, xMax, yMin, yMax)) {
            continue;
        }

        float x1 = (p1.x()-xMin)/rangeX;
        float x2 = (p2.x()-xMin)/rangeX;

        float y1 = 1.0 - (p1.y()-yMin)/rangeY; // y is flipped
        float y2 = 1.0 - (p2.y()-yMin)/rangeY;
        scaledPoints.push_back(figure->scaled(QPointF(x1,y1), figure->figureRectangle(), figure->figureRectangle().topLeft()));
        scaledPoints.push_back(figure->scaled(QPointF(x2,y2), figure->figureRectangle(), figure->figureRectangle().topLeft()));
    }

    painter->drawLines(scaledPoints);
    scaledPoints.clear();
    setDirty(false);
}

LineGraphDataSource *LineGraph::dataSource() const
{
    return m_dataSource;
}

void LineGraphDataSource::cleanupMemory() {
    m_points.erase(m_points.begin(), m_points.begin()+m_firstIndex-1);
        m_firstIndex = 0;
        m_numberOfPoints = m_points.size();
}

void LineGraph::addPoint(float x, float y)
{
    m_dataSource->addPoint(x,y);
}

void LineGraph::setDataSource(LineGraphDataSource *dataSource)
{
    if (m_dataSource == dataSource)
        return;

    if(m_dataSource) {
        m_dataSource->disconnect(this);
    }

    m_dataSource = dataSource;
    connect(m_dataSource, SIGNAL(dataChanged()), this, SLOT(markDirty()));
    emit dataSourceChanged(dataSource);
}

void LineGraphDataSource::addPoint(float x, float y)
{
    m_points.push_back(QPointF(x,y));
    if(++m_numberOfPoints > m_maxNumberOfPoints) {
        m_numberOfPoints = m_maxNumberOfPoints;
        m_firstIndex++;
    }

    if(m_points.size() > 2*m_maxNumberOfPoints) {
        cleanupMemory();
    }

    emit dataChanged();
}
