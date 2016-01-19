#ifndef FIGURE_H
#define FIGURE_H

#include <QQuickPaintedItem>
#include <QPainter>

class Figure : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(double xMin READ xMin WRITE setXMin NOTIFY xMinChanged)
    Q_PROPERTY(double xMax READ xMax WRITE setXMax NOTIFY xMaxChanged)
    Q_PROPERTY(double yMin READ yMin WRITE setYMin NOTIFY yMinChanged)
    Q_PROPERTY(double yMax READ yMax WRITE setYMax NOTIFY yMaxChanged)
    Q_PROPERTY(QString xLabel READ xLabel WRITE setXLabel NOTIFY xLabelChanged)
    Q_PROPERTY(QString yLabel READ yLabel WRITE setYLabel NOTIFY yLabelChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY fontChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(bool fitData READ fitData WRITE setFitData NOTIFY fitDataChanged)
    Q_PROPERTY(bool fitX READ fitX WRITE setFitX NOTIFY fitXChanged)
    Q_PROPERTY(bool fitY READ fitY WRITE setFitY NOTIFY fitYChanged)
    Q_PROPERTY(bool freeze READ freeze WRITE setFreeze NOTIFY freezeChanged)
private:
    double m_xMin = 0;
    double m_xMax = 0;
    double m_yMin = 0;
    double m_yMax = 0;
    QRectF m_figureRectangle;
    QImage m_image;
    int numTicksX() { return width() / 100; }
    int numTicksY() { return height() / 100; }

    void drawAxis(QPainter *painter);
    void drawTicks(QPainter *painter);
    void drawTickText(QPainter *painter);
    void drawGraphs(QPainter *painter);
    void drawLabels(QPainter *painter);
    QString m_xLabel;
    QString m_yLabel;
    QString m_title;
    QFont m_font;
    void drawText(QPointF position, QString text, QPainter *painter);
    QColor m_color = "white";
    bool m_fitData = true;
    bool m_freeze = false;
    bool m_fitX = false;
    bool m_fitY = false;

public:
    Figure(QQuickItem *parent = 0);
    virtual void paint(QPainter *painter);

    QRectF figureRectangle() { return m_figureRectangle; }
    QPointF scaled(const QPointF &p);
    QRectF scaled(const QRectF &rect);
    QLineF scaled(const QLineF &line);
    QPointF scaled(const QPointF &p, const QRectF &rect, const QPointF delta = QPointF(0,0));

    double xMin() const { return m_xMin; }
    double xMax() const { return m_xMax; }
    double yMin() const { return m_yMin; }
    double yMax() const { return m_yMax; }

    QString xLabel() const;
    QString yLabel() const;
    QString title() const;
    QFont font() const;
    QColor color() const;
    bool fitData() const;
    bool freeze() const;
    bool fitX() const;
    bool fitY() const;

    Q_INVOKABLE void saveSVG(QString filename);
    Q_INVOKABLE void savePNG(QString filename);



public slots:
    void setXMin(double xMin);
    void setXMax(double xMax);
    void setYMin(double yMin);
    void setYMax(double yMax);
    void setXLabel(QString xLabel);
    void setYLabel(QString yLabel);
    void setTitle(QString title);
    void setFont(QFont font);
    void setColor(QColor color);
    void setFitData(bool fitData);
    void setFreeze(bool freeze);
    void storeCurrentFigure();
    void setFitX(bool fitX);
    void setFitY(bool fitY);

signals:
    void xMinChanged(double xMin);
    void xMaxChanged(double xMax);
    void yMinChanged(double yMin);
    void yMaxChanged(double yMax);
    void xLabelChanged(QString xLabel);
    void yLabelChanged(QString yLabel);
    void titleChanged(QString title);
    void fontChanged(QFont font);
    void colorChanged(QColor color);
    void fitDataChanged(bool fitData);
    void freezeChanged(bool freeze);
    void fitXChanged(bool fitX);
    void fitYChanged(bool fitY);
};

#endif // FIGURE_H
