#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <QMainWindow>

#include <qchart.h>
#include <cmath>

#include <QMainWindow>
#include <QWidget>
#include <QLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>
#include <QTimer>

#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QValueAxis>
#include <QtCharts/QAreaSeries>

QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
class Ui_ThemeWidgetForm;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui { class Convolution; }
QT_END_NAMESPACE

class Convolution : public QMainWindow
{
    Q_OBJECT

public:
    Convolution(QWidget *parent = nullptr);
    ~Convolution();

    int length, height;

private:
    Ui::Convolution *ui;

    void setButton();// установка кнопок
    void setChart();//установка графиков
    void clean(QHBoxLayout* layot1, QHBoxLayout* layot2);
    void cleanLayout(QLayout *layout);
    void setData(int Length = 0, int Height = 0 );
    void setDataAKF();
    void rectangeMethod();
    void setRectange();
    void setTriangle();
    void setSinc();
    void AKF();

    QPalette pal;//палитра
    QString style;

    QPushButton b1;
    QPushButton b2;
    QPushButton b3;

    QChart *chartSignal = new QChart();
    QValueAxis *SignalAxisX = new QValueAxis();
    QValueAxis *SignalAxisY = new QValueAxis();
    QLineSeries *upperSeries = new QLineSeries(chartSignal);
    QLineSeries *upperSeries2 = new QLineSeries(chartSignal);
    QChartView *chartViewSignal = new QChartView(this);

    QChart *chartAKF = new QChart();
    QValueAxis *akfAxisX = new QValueAxis();
    QValueAxis *akfAxisY = new QValueAxis();
    QLineSeries *upperSeriesAKF = new QLineSeries(chartAKF);
    QChartView *chartViewAKF = new QChartView(this);

    typedef QPair<double, double> coordinates;
    QList<coordinates> signalPoint;
    QList<coordinates> signalPoint2;
    QList<coordinates> akfPoint;
    QList<coordinates> signal;
    QList<coordinates> akf;
    QList<coordinates> tempList;

    QLineEdit* lineL;
    QLineEdit* lineH;

    QHBoxLayout* layotL = new QHBoxLayout();
    QHBoxLayout* layotH = new QHBoxLayout();

    QLineEdit* lineE;
    QLineEdit* lineMax;

    QHBoxLayout* layotE = new QHBoxLayout();
    QHBoxLayout* layotMax = new QHBoxLayout();

    int flag;
    double count=0.5;
    int *t;
    QTimer * tmr;
    bool startFlag=0;
    int k=0;
    double maxY;

private slots:
    void  Rectange();
    void  Triangle();
    void  Sinc();
    void  Start();
    void  Stop();
    void  Resume();
    void  Help();
    void  startTime();
};
#endif // CONVOLUTION_H
