#include "figure.h"
#include "graph.h"
#include <cmath>
#include <QDebug>
#include <QSvgGenerator>
#include <QFileDialog>

Figure::Figure(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    connect(this, SIGNAL(xMinChanged(double)), this, SLOT(update()));
    connect(this, SIGNAL(xMaxChanged(double)), this, SLOT(update()));
    connect(this, SIGNAL(yMinChanged(double)), this, SLOT(update()));
    connect(this, SIGNAL(yMaxChanged(double)), this, SLOT(update()));
    connect(this, SIGNAL(xLabelChanged(QString)), this, SLOT(update()));
    connect(this, SIGNAL(yLabelChanged(QString)), this, SLOT(update()));
    connect(this, SIGNAL(titleChanged(QString)), this, SLOT(update()));
    m_font = QFont("times", 16);
}

void Figure::paint(QPainter *painter)
{
    if(m_freeze) {
        painter->drawImage(boundingRect(), m_image);
        return;
    }
    if(m_fitData || m_fitX || m_fitY) {
        QList<Graph*> graphs = findChildren<Graph*>();

        double xMin = 1e30;
        double xMax = -1e30;
        double yMin = 1e30;
        double yMax = -1e30;
        for(Graph *graph : graphs) {
            if(graph->isVisible()) {
                graph->bounds(xMin, xMax, yMin, yMax);
            }
        }

        if(m_fitData || m_fitX) {
            if(xMax > 0) xMax *= 1.1;
            else xMax *= 0.9;
            if(xMin > 0) xMin *= 1.1;
            else xMin *= 0.9;

            if(m_fitExact) {
                setXMax(xMax);
                setXMin(xMin);
            } else {
                if(xMax > m_xMax) setXMax(xMax);
                if(xMin < m_xMin) setXMin(xMin);
            }
        }

        if(m_fitData || m_fitY) {
            if(yMax > 0) yMax *= 1.1;
            else yMax *= 0.9;
            if(yMin > 0) yMin *= 1.1;
            else yMin *= 0.9;

            if(m_fitExact) {
                setYMax(yMax);
                setYMin(yMin);
            } else {
                if(yMax > m_yMax) setYMax(yMax);
                if(yMin < m_yMin) setYMin(yMin);
            }
        }
    }
    
    // Calculate how much space we need for titles etc
    float yLabelSpace = 80;
    float titleSpace = 45;
    float xLabelSpace = 60;
    float rightSymmetrySpace = 30;
    m_figureRectangle = QRectF(QPointF(yLabelSpace, titleSpace), QPointF(width() - rightSymmetrySpace, height() - xLabelSpace));
    // m_figureRectangle = scaled(m_figureRectangleNormalized);
    QBrush brush(m_color);
    painter->setBackground(brush);
    painter->setBrush(brush);
    painter->drawRect(QRect(0,0,width(), height()));
    drawAxis(painter);
    drawTicks(painter);
    drawTickText(painter);
    drawLabels(painter);
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

        QPointF p1(x, y1);
        QPointF p2(x, y1-tickLengthHalf);
        lines.push_back(QLineF(p1, p2));
        p1.setY(y2+tickLengthHalf);
        p2.setY(y2);
        lines.push_back(QLineF(p1, p2));
    }

    for(int i=1; i<=numTicksY(); i++) {
        float x1 = m_figureRectangle.left();
        float x2 = m_figureRectangle.right();
        float y = m_figureRectangle.top() + i*deltaY;
        QPointF p1(x1+tickLengthHalf, y);
        QPointF p2(x1, y);
        lines.push_back(QLineF(p1, p2));

        p1.setX(x2);
        p2.setX(x2-tickLengthHalf);
        lines.push_back(QLineF(p1, p2));
    }
    painter->drawLines(lines);
}

void Figure::drawTickText(QPainter *painter) {
    float deltaX = m_figureRectangle.width() / (numTicksX()+1);
    float deltaY = m_figureRectangle.height() / (numTicksY()+1);
    float deltaXValue = (xMax()-xMin()) / (numTicksX()+1);
    float deltaYValue = (yMax()-yMin()) / (numTicksY()+1);
    painter->setFont(m_font);
    QFontMetrics fm(m_font);
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
        if(graph->isVisible()) {
            graph->paint(this, painter);
        }
    }
}

void Figure::drawText(QPointF position, QString text, QPainter *painter) {
    QFontMetrics fm(m_font);
    float textWidthHalf = 0.5*fm.width(text);
    float textHeightHalf = 0.5*fm.height();
    QRectF textBox(QPointF(position.x()-textWidthHalf, position.y()-textHeightHalf), QPointF(position.x()+textWidthHalf, position.y()+textHeightHalf));
    painter->drawText(textBox, text);
}

void Figure::drawLabels(QPainter *painter)
{
    painter->save();
    // X label
    float x = m_figureRectangle.left() + 0.5*m_figureRectangle.width();
    float y = m_figureRectangle.bottom()+40;
    drawText(QPointF(x,y), m_xLabel, painter);

    // Y label
    y = m_figureRectangle.left() - 60;
    x = -m_figureRectangle.top()-0.5*m_figureRectangle.height();
    painter->rotate(-90);
    drawText(QPointF(x,y), m_yLabel, painter);
    painter->rotate(90);

    // Title
    m_font.setBold(true);
    painter->setFont(m_font);
    x = m_figureRectangle.left() + 0.5*m_figureRectangle.width();
    y = m_figureRectangle.top() - 25;
    drawText(QPointF(x,y), m_title, painter);
    m_font.setBold(false);
    painter->restore();
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

QString Figure::xLabel() const
{
    return m_xLabel;
}

QString Figure::yLabel() const
{
    return m_yLabel;
}

QString Figure::title() const
{
    return m_title;
}

QFont Figure::font() const
{
    return m_font;
}

QColor Figure::color() const
{
    return m_color;
}

bool Figure::fitData() const
{
    return m_fitData;
}

void Figure::saveSVG(QString filename)
{
    if (filename.isEmpty())
        return;
    QSvgGenerator generator;
    generator.setFileName(QUrl(filename).toLocalFile());
    generator.setSize(QSize(width(), height()));
    generator.setViewBox(QRect(0, 0, width(), height()));
    generator.setTitle(tr("SVG Generator Example Drawing"));
    generator.setDescription(tr("An SVG drawing created by the SVG Generator "
                             "Example provided with Qt."));
    QPainter painter;
    painter.begin(&generator);
    paint(&painter);
    painter.end();
}

void Figure::savePNG(QString filename)
{
    if (filename.isEmpty())
        return;

    QImage img(width(), height(), QImage::Format_ARGB32);

    QPainter painter;
    painter.begin(&img);
    paint(&painter);
    painter.end();

    img.save(QUrl(filename).toLocalFile());
}

bool Figure::fitExact() const
{
    return m_fitExact;
}

bool Figure::fitX() const
{
    return m_fitX;
}

bool Figure::fitY() const
{
    return m_fitY;
}

bool Figure::freeze() const
{
    return m_freeze;
}

QRectF Figure::scaled(const QRectF &rect) {
    return QRectF(scaled(rect.topLeft()), scaled(rect.bottomRight()));
}

void Figure::setXMin(double xMin)
{
    if (m_xMin == xMin)
        return;

    m_xMin = xMin;
    emit xMinChanged(xMin);
}

void Figure::setXMax(double xMax)
{
    if (m_xMax == xMax)
        return;

    m_xMax = xMax;
    emit xMaxChanged(xMax);
}

void Figure::setYMin(double yMin)
{
    if (m_yMin == yMin) {
        return;
    }

    m_yMin = yMin;
    emit yMinChanged(yMin);
}

void Figure::setYMax(double yMax)
{
    if (m_yMax == yMax)
        return;

    m_yMax = yMax;
    emit yMaxChanged(yMax);
}

void Figure::setXLabel(QString xLabel)
{
    if (m_xLabel == xLabel)
        return;

    m_xLabel = xLabel;
    emit xLabelChanged(xLabel);
}

void Figure::setYLabel(QString yLabel)
{
    if (m_yLabel == yLabel)
        return;

    m_yLabel = yLabel;
    emit yLabelChanged(yLabel);
}

void Figure::setTitle(QString title)
{
    if (m_title == title)
        return;

    m_title = title;
    emit titleChanged(title);
}

void Figure::setFont(QFont font)
{
    if (m_font == font)
        return;

    m_font = font;
    emit fontChanged(font);
}

void Figure::setColor(QColor color)
{
    if (m_color == color)
        return;

    m_color = color;
    emit colorChanged(color);
}

void Figure::setFitData(bool fitData)
{
    if (m_fitData == fitData)
        return;

    m_fitData = fitData;
    emit fitDataChanged(fitData);
}

void Figure::setFreeze(bool freeze)
{
    if (m_freeze == freeze)
        return;

    m_freeze = freeze;
    if(m_freeze) storeCurrentFigure();
    else update();
    emit freezeChanged(freeze);
}

void Figure::storeCurrentFigure()
{
    m_image = QImage(width(), height(), QImage::Format_ARGB32);

    QPainter painter;
    painter.begin(&m_image);
    paint(&painter);
    painter.end();
}

void Figure::setFitX(bool fitX)
{
    if (m_fitX == fitX)
        return;

    m_fitX = fitX;
    emit fitXChanged(fitX);
}

void Figure::setFitY(bool fitY)
{
    if (m_fitY == fitY)
        return;

    m_fitY = fitY;
    emit fitYChanged(fitY);
}

void Figure::setFitExact(bool fitExact)
{
    if (m_fitExact == fitExact)
        return;

    m_fitExact = fitExact;
    emit fitExactChanged(fitExact);
}
