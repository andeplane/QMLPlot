#include "figure.h"
#include "graph.h"

Figure::Figure(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    connect(this, SIGNAL(xMinChanged(float)), this, SLOT(update()));
    connect(this, SIGNAL(xMaxChanged(float)), this, SLOT(update()));
    connect(this, SIGNAL(yMinChanged(float)), this, SLOT(update()));
    connect(this, SIGNAL(yMaxChanged(float)), this, SLOT(update()));
    QPointF ul(0.15, 0.1);
    QPointF dr(0.9, 0.9);
    m_figureRectangleNormalized = QRectF(ul, dr);
    m_figureRectangle = scaled(m_figureRectangleNormalized);
}

void Figure::paint(QPainter *painter)
{
    m_figureRectangle = scaled(m_figureRectangleNormalized);
    drawAxis(painter);
    drawTicks(painter);
    drawTickText(painter);
    drawGraphs(painter);
    painter->setRenderHint(QPainter::Antialiasing);
}

void Figure::drawAxis(QPainter *painter) {
    QVector<QLineF> lines;
    lines.push_back(QLineF(m_figureRectangle.bottomLeft(),m_figureRectangle.bottomRight()));
    lines.push_back(QLineF(m_figureRectangle.bottomRight(),m_figureRectangle.topRight()));
    lines.push_back(QLineF(m_figureRectangle.topRight(),m_figureRectangle.topLeft()));
    lines.push_back(QLineF(m_figureRectangle.topLeft(),m_figureRectangle.bottomLeft()));
    painter->drawLines(lines);
}

void Figure::drawTicks(QPainter *painter) {
    float deltaX = m_figureRectangle.width() / (numTicksX()+1);
    float deltaY = m_figureRectangle.height() / (numTicksY()+1);
    float tickLengthHalf = 5;
    QVector<QLineF> lines;
    for(int i=1; i<=numTicksX(); i++) {
        float x = m_figureRectangle.left() + i*deltaX;
        float y1 = m_figureRectangle.bottom();
        float y2 = m_figureRectangle.top();

        QPointF p1(x, y1+tickLengthHalf);
        QPointF p2(x, y1-tickLengthHalf);
        lines.push_back(QLineF(p1, p2));
        p1.setY(y2+tickLengthHalf);
        p2.setY(y2-tickLengthHalf);
        lines.push_back(QLineF(p1, p2));
    }

    for(int i=1; i<=numTicksY(); i++) {
        float x1 = m_figureRectangle.left();
        float x2 = m_figureRectangle.right();
        float y = m_figureRectangle.top() + i*deltaY;
        QPointF p1(x1+tickLengthHalf, y);
        QPointF p2(x1-tickLengthHalf, y);
        lines.push_back(QLineF(p1, p2));

        p1.setX(x2+tickLengthHalf);
        p2.setX(x2-tickLengthHalf);
        lines.push_back(QLineF(p1, p2));
    }
    painter->drawLines(lines);
}

void Figure::drawTickText(QPainter *painter) {
    float deltaX = m_figureRectangle.width() / (numTicksX()+1);
    float deltaY = m_figureRectangle.height() / (numTicksY()+1);
    QRectF range = valueRange();
    float deltaXValue = range.width() / (numTicksX()+1);
    float deltaYValue = range.height() / (numTicksY()+1);
    QFont font("times", 16);
    painter->setFont(font);
    QFontMetrics fm(font);
    for(int i=0; i<numTicksX()+2; i++) {
        float xValue = xMin() + i*deltaXValue;
        QString text = QString::number(xValue, 'f', 2);
        int textWidthHalf = 0.5*fm.width(text);
        int textHeightHalf = 0.5*fm.height();

        float x = m_figureRectangle.left() + i*deltaX;
        float y = m_figureRectangle.bottom() + 2.0*textHeightHalf;

        QRectF textBox = QRectF(QPointF(x-textWidthHalf, y-textHeightHalf), QPointF(x+textWidthHalf, y+textHeightHalf));
        painter->drawText(textBox, text);
    }

    for(int i=0; i<numTicksY()+2; i++) {
        float yValue = yMax() - i*deltaYValue;
        QString text = QString::number(yValue, 'f', 2);
        int textWidthHalf = 0.5*fm.width(text);
        int textHeightHalf = 0.5*fm.height();

        float x = m_figureRectangle.left() - 1.8*textWidthHalf;
        float y = m_figureRectangle.top() + i*deltaY;

        QRectF textBox = QRectF(QPointF(x-textWidthHalf, y-textHeightHalf), QPointF(x+textWidthHalf, y+textHeightHalf));
        painter->drawText(textBox, text);
    }
}

void Figure::drawGraphs(QPainter *painter) {
    QList<Graph*> graphs = findChildren<Graph*>();
    for(Graph *graph : graphs) {
        graph->paint(this, painter);
    }

//    QVector<QPointF> scaledPoints;
//    float rangeX = xMax() - xMin();
//    float rangeY = yMax() - yMin();

//    for(int i=startValueIndex; i<m_points.size()-1; i++) {
//        const QPointF &p1 = m_points[i];
//        const QPointF &p2 = m_points[i+1];
//        if(!isInValueRange(p1) || !isInValueRange(p2)) {
//            continue;
//        }

//        float x1 = (p1.x()-xMin())/rangeX;
//        float y1 = (p1.y()-yMin())/rangeY;
//        float x2 = (p2.x()-xMin())/rangeX;
//        float y2 = (p2.y()-yMin())/rangeY;
//        scaledPoints.push_back(scaled(QPointF(x1,y1), rect, rect.topLeft()));
//        scaledPoints.push_back(scaled(QPointF(x2,y2), rect, rect.topLeft()));
//    }

//    painter->drawLines(scaledPoints);
}

QLineF Figure::scaled(const QLineF &line) {
    return QLineF(scaled(line.p1()), scaled(line.p2()));
}

QPointF Figure::scaled(const QPointF &p) {
    return QPointF(p.x()*width(), p.y()*height());
}

QPointF Figure::scaled(const QPointF &p, const QRectF &rect, const QPointF delta) {
    return QPointF(p.x()*rect.width() + delta.x(), p.y()*rect.height() + delta.y());
}

QRectF Figure::scaled(const QRectF &rect) {
    return QRectF(scaled(rect.topLeft()), scaled(rect.bottomRight()));
}

void Figure::setXMin(float xMin)
{
    if (m_xMin == xMin)
        return;

    m_xMin = xMin;
    emit xMinChanged(xMin);
}

void Figure::setXMax(float xMax)
{
    if (m_xMax == xMax)
        return;

    m_xMax = xMax;
    emit xMaxChanged(xMax);
}

void Figure::setYMin(float yMin)
{
    if (m_yMin == yMin)
        return;

    m_yMin = yMin;
    emit yMinChanged(yMin);
}

void Figure::setYMax(float yMax)
{
    if (m_yMax == yMax)
        return;

    m_yMax = yMax;
    emit yMaxChanged(yMax);
}
