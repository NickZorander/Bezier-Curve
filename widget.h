#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class MyWidget : public QWidget
{
    Q_OBJECT
public:
    QVector <QPointF> SplinePoints;
    QVector <QPointF> StartPoints;
    QVector <qreal> Weights;

    int SplinePointsIterator=0;
    int N;
    bool change_point_flag=false;
    int change_point_index=0;

    bool change_weight_flag=false;
    int change_weight_index=0;

    QPointF cursor;

    qreal W(qreal t, int n);
    QPointF R (qreal);
    void BuildSpline(); //заполняет массив SplinePoints достаточным для отрисовки количеством точек
    void InitData(); //инициализация данных
    void PrintInfo();

    MyWidget(QWidget* p=0) {}
    virtual ~MyWidget() {}
protected:
     void paintEvent(QPaintEvent*e);
     void mouseReleaseEvent(QMouseEvent* pe);
     void mousePressEvent(QMouseEvent *pe);
     void mouseMoveEvent(QMouseEvent *pe);
     void wheelEvent (QWheelEvent* pe);
};
#endif // WIDGET_H
