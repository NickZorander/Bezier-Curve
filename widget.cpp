#include "widget.h"
#include <QApplication>
#include<QPainter>
#include<QPaintEvent>
#include<QtWidgets>
#include<cstdlib>
#include <QtDebug>



void MyWidget::paintEvent(QPaintEvent* e){

    QPainter painter(this);
    int i;

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black));

    QVector<QPointF>::iterator it=SplinePoints.begin();
    painter.drawPolyline(it, SplinePoints.size());

    painter.setPen(QPen(Qt::yellow, 6));
    painter.drawPoint(cursor);

    for (int i=0; i<=N-1; ++i)
    {
        painter.drawPoint(StartPoints[i]);
    }
}





void MyWidget::mouseReleaseEvent(QMouseEvent* pe)
{
    QPointF Q(pe->x(),pe->y());
    qDebug() << "new point " <<Q;

    if (change_point_flag)
        {
            StartPoints[change_point_index]=Q;
            BuildSpline();
            repaint();
        }
    change_point_flag=false;
}



qreal distance (const QPointF& p1, const QPointF& p2)
{
    return qSqrt(  (p2.x()-p1.x())*(p2.x()-p1.x()) + (p2.y()-p1.y())*(p2.y()-p1.y()) );
}





void MyWidget::mousePressEvent(QMouseEvent *pe)
{
    change_point_flag=false;

    QPointF Q(pe->x(),pe->y());


    for (int i=0; i<=N-1; ++i)
        {
            if (distance(Q, StartPoints[i]) <=7)
                {
                    change_point_index=i;
                    change_point_flag=true;
                    break;
                }
        }
}


void MyWidget::mouseMoveEvent(QMouseEvent *pe)
{
    cursor=QPointF(pe->x(), pe->y());
    repaint();
}


void MyWidget::wheelEvent (QWheelEvent* pe)
{
    QPoint nAngle=pe->angleDelta(); //  на сколько крутанули
    QPointF Q(pe->x(),pe->y()); //где курсор

    for (int i=0; i<=N-1; ++i)
        {
            if (distance(Q, StartPoints[i]) <=7)
                {
                    if (Weights[i]+(nAngle.y()/8)>=1)
                        Weights[i]+= (nAngle.y()/8);
                    break;
                }
        }
    BuildSpline();
    repaint();
}


void MyWidget::InitData()
{
    QFile inputfile("D:\\QT\\Bezier_Curve\\points.txt");

    if (!inputfile.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::information(this,"Error", "path not found!");
        return;
    } //проверили на открытие

    QTextStream stream(&inputfile);

    QString A =stream.readAll(); //сделали содержимое файла строчкой

    QStringList list = A.split(' ', QString::SkipEmptyParts); //распилили строку в список где каждый элемент - одна координата точки
    bool ok;

    N=list[0].toInt(&ok); //первым числом передается число точек

    StartPoints.resize(N);
    SplinePoints.resize(1000);


    for (int i=0; i<=N-1; ++i)
        {
            QPointF p(list[i*2+1].toInt(&ok), list[i*2+2].toInt(&ok));
            StartPoints[i]=p;
        } //создали массив точек


    change_point_flag=false;

    Weights.resize(N);

    for (int i=0; i<=N-1; ++i)
        {
           Weights[i]=1;
        } //создали массив весов
}


//______________________________________________________________________________________________________________________________________________________________________________

int factorial (int x)
{
    if (x==0)
        return 1;

    int res=1;
    for (int i=1; i<=x;++i)
        res*=i;
    return res;
}

qreal topower(qreal x, int n)
{
    if (n==0)
        return 1;

    qreal res=1;
    for (int i=1; i<=n; ++i)
        res*=x;
    return res;
}


qreal Bernstein (qreal t, int n, int i)
{
    return (factorial(n)*topower(t,i)*topower(1-t, n-i) )/(factorial(i)*factorial(n-i));
}


qreal MyWidget::W(qreal t, int n)
{
    qreal res=0;
    for (int i=0; i<=n; i++)
        {
            res+= (Bernstein(t, n, i) * Weights[i]);
        }
    return res;
}


QPointF MyWidget::R(qreal t)
{
    QPointF res(0,0);
    for (int i=0; i<=N-1; i++)
        {
            res+= (Bernstein(t, N-1, i) * StartPoints[i]*Weights[i]);
        }
    res/=W(t, N-1);
    return res;
}



void MyWidget::BuildSpline()
{
    SplinePointsIterator=0;
    qreal t=0;

    for (int i=1; i<=1000; ++i, t+=0.001)
         {
            SplinePoints[SplinePointsIterator]=R(t);
             ++SplinePointsIterator;
         }
}


void MyWidget::PrintInfo()
{
    qDebug()<< "Points: ";
    for (int i=0; i<=N-1;++i)
        qDebug()<<StartPoints[i];
}




















