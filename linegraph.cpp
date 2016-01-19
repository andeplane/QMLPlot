#include "linegraph.h"
#include "figure.h"
#include <algorithm>

void LineGraphDataSource::cleanupMemory() {
    m_points.erase(m_points.begin(), m_points.begin()+m_firstIndex-1);
        m_firstIndex = 0;
        m_numberOfPoints = m_points.size();
}

void LineGraphDataSource::addPoint(float x, float y)
{
    m_points.push_back(QPointF(x,y));
    if(m_saveMemory) {
        if(++m_numberOfPoints > m_maxNumberOfPoints) {
            m_numberOfPoints = m_maxNumberOfPoints;
            m_firstIndex++;
        }

        if(m_points.size() > 2*m_maxNumberOfPoints) {
            cleanupMemory();
        }
    }

    emit dataChanged();
}

void LineGraphDataSource::setPoints(QVector<QPointF> points)
{
    m_points = points;
    m_numberOfPoints = m_points.size();
    m_firstIndex = 0;
    qDebug() << "I sat points. Here they are: " << m_points;
    emit dataChanged();
}

void LineGraphDataSource::iterate(std::function<void(int i, QPointF point)> action)
{
    for(int i=0; i<size(); i++) {
        action(i, get(i));
    }
}

int LineGraphDataSource::size()
{
    if(m_saveMemory) return m_numberOfPoints;
    else return m_points.size();
}

void LineGraphDataSource::clear()
{
    m_points.clear();
    m_numberOfPoints = 0;
    m_firstIndex = 0;
    emit dataChanged();
}

bool LineGraphDataSource::saveMemory() const
{
    return m_saveMemory;
}

void LineGraphDataSource::update()
{
    emit dataChanged();
}

void LineGraphDataSource::setSaveMemory(bool saveMemory)
{
    if (m_saveMemory == saveMemory)
        return;

    m_saveMemory = saveMemory;
    emit saveMemoryChanged(saveMemory);
}

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

void LineGraph::save(QString filename)
{
    if(!m_dataSource) return;
    QFile file(QUrl(filename).toLocalFile());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Could not open file "+filename;
        return;
    }

    QTextStream out(&file);
    m_dataSource->iterate([&](int i, QPointF point) {
        out << point.x() << " " << point.y() << "\n";
    });
    file.close();
}

void LineGraph::load(QString filename)
{
    if(!m_dataSource) return;
    QFile file(QUrl(filename).toLocalFile());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open file "+filename;
        return;
    }

    m_dataSource->clear();
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList words = line.split(" ");
        if(words.length() != 2) continue;

        bool castOk;
        float pointX = QString(words[0]).toFloat(&castOk);
        if(!castOk) continue;
        float pointY = QString(words[1]).toFloat(&castOk);
        if(!castOk) continue;
        m_dataSource->addPoint(pointX,pointY);
    }
    file.close();
}

void LineGraph::paint(Figure *figure, QPainter *painter)
{
    painter->save();
    QPen pen(m_style);
    pen.setColor(m_color);
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(m_width);
    painter->setPen(pen);

    QVector<QPointF> scaledPoints;
    float rangeX = figure->xMax()-figure->xMin();
    float rangeY = figure->yMax()-figure->yMin();
    float xMin = figure->xMin();
    float yMin = figure->yMin();
    float xMax = figure->xMax();
    float yMax = figure->yMax();

    for(int i=0; i< m_dataSource->size()-1; i++) {
        const QPointF &p1 = m_dataSource->get(i);
        const QPointF &p2 = m_dataSource->get(i+1);
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
    painter->restore();
}

LineGraphDataSource *LineGraph::dataSource() const
{
    return m_dataSource;
}

void LineGraph::addPoint(float x, float y)
{
    m_dataSource->addPoint(x,y);
}

QColor LineGraph::color() const
{
    return m_color;
}

Qt::PenStyle LineGraph::style() const
{
    return m_style;
}

int LineGraph::width() const
{
    return m_width;
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

void LineGraph::setColor(QColor color)
{
    if (m_color == color)
        return;

    m_color = color;
    emit colorChanged(color);
}

void LineGraph::setStyle(Qt::PenStyle style)
{
    if (m_style == style)
        return;

    m_style = style;
    emit styleChanged(style);
}

void LineGraph::setWidth(int width)
{
    if (m_width == width)
        return;

    m_width = width;
    emit widthChanged(width);
}

void LineGraph::bounds(double &xMin, double &xMax, double &yMin, double &yMax)
{
    m_dataSource->iterate([&](int i, QPointF point) {
        Q_UNUSED(i);
        xMin = std::min(xMin, point.x());
        xMax = std::max(xMax, point.x());
        yMin = std::min(yMin, point.y());
        yMax = std::max(yMax, point.y());
    });
}
