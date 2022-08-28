#include "convolution.h"
#include "ui_convolution.h"

Convolution::Convolution(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Convolution)
{
    ui->setupUi(this);

    this->setWindowIcon(QIcon(":/new/icon/icon.png"));
    pal = window()->palette();
    //pal.setColor(QPalette::Window, QRgb(0x40434a));
    //pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
    pal.setColor(QPalette::Window, QRgb(0xcee7f0));
    pal.setColor(QPalette::WindowText, QRgb(0x404044));
    this->setPalette(pal);

    setButton();
    setChart();

    *t=100;
    tmr = new QTimer(this);
    connect(tmr, SIGNAL(timeout()), this, SLOT(startTime()));
    tmr->start(*t);
}

Convolution::~Convolution()
{
    delete ui;
}

void Convolution::setButton()
{

    style = " QPushButton { background: rgb(38,133,191); "
            "color: rgb(255,255,255); "
            "border-radius: 5px;"
            "padding: 6px } "
            "QPushButton:hover { background: rgb(94,174,223);}"
            "QPushButton:pressed { background: rgb(31,106,152);}";

    ui->start->setEnabled(false);
    ui->stop->setEnabled(false);
    ui->resume->setEnabled(false);

    b1.setText("Прямоугольный импульс");
    b2.setText("Треугольник");
    b3.setText("|sinc|");
    ui->start->setText("старт");

    b1.setStyleSheet(style);
    b2.setStyleSheet(style);
    b3.setStyleSheet(style);
    ui->help->setStyleSheet(style);
    ui->start->setStyleSheet(style);
    ui->stop->setStyleSheet("QPushButton { background: rgb(206,231,240); "
                            "border-radius: 5px} ");
    ui->resume->setStyleSheet("QPushButton { background: rgb(206,231,240); "
                              "border-radius: 5px} ");

    this->connect( &b1, SIGNAL( clicked() ), this, SLOT( Rectange() ) );
    this->connect( &b2, SIGNAL( clicked() ), this, SLOT( Triangle() ) );
    this->connect( &b3, SIGNAL( clicked() ), this, SLOT( Sinc() ) );
    this->connect( ui->start, SIGNAL( clicked() ), this, SLOT( Start() ) );
    this->connect( ui->stop, SIGNAL( clicked() ), this, SLOT( Stop() ) );
    this->connect( ui->resume, SIGNAL( clicked() ), this, SLOT( Resume() ) );
    this->connect( ui->help, SIGNAL( clicked() ), this, SLOT( Help() ) );

    ui->LayoutButtom->addWidget( &b1 );
    ui->LayoutButtom->addWidget( &b2 );
    ui->LayoutButtom->addWidget( &b3 );

}

void Convolution::setChart()
{
    QChart::ChartTheme theme = QChart::ChartThemeBlueIcy;

    //сигнал
    ui->LayoutChart->addWidget(chartViewSignal);

    //установка темы для графика
    chartViewSignal->setPalette(pal);
    chartSignal->setPalette(pal);
    chartSignal->setTheme(theme);

    chartSignal->createDefaultAxes();

    QAreaSeries *area = new QAreaSeries(upperSeries);
    area->setName("сигнал");
    QAreaSeries *area2 = new QAreaSeries(upperSeries2);
    area2->setName("дублированный сигнал");

    chartSignal->addAxis(SignalAxisX, Qt::AlignBottom);
    chartSignal->addAxis(SignalAxisY, Qt::AlignLeft);

    chartViewSignal->setRenderHint(QPainter::Antialiasing, true); //сглаживание графика
    SignalAxisX->setRange(-10, 10);
    SignalAxisY->setRange(0, 10);

    chartSignal->addSeries(area);
    chartSignal->addSeries(area2);
    area->attachAxis(SignalAxisX);
    area->attachAxis(SignalAxisY);
    area2->attachAxis(SignalAxisX);
    area2->attachAxis(SignalAxisY);

    chartViewSignal->setChart(chartSignal);

    //акф
    ui->LayoutChart->addWidget(chartViewAKF);

    //установка темы для графика
    chartViewAKF->setPalette(pal);
    chartAKF->setPalette(pal);
    chartAKF->setTheme(theme);

    chartAKF->createDefaultAxes();

    //установка осей
    QAreaSeries *areaAKF = new QAreaSeries(upperSeriesAKF);
    areaAKF->setName("АКФ");

    chartAKF->addAxis(akfAxisX, Qt::AlignBottom);
    chartAKF->addAxis(akfAxisY, Qt::AlignLeft);

    chartViewAKF->setRenderHint(QPainter::Antialiasing, true); //сглаживание графика

    akfAxisX->setRange(-10, 10);
    akfAxisY->setRange(0, 10);

    chartAKF->addSeries(areaAKF);
    areaAKF->attachAxis(akfAxisX);
    areaAKF->attachAxis(akfAxisY);

    chartViewAKF->setChart(chartAKF);
}

void Convolution::Rectange()
{
    length=100; height=50;
    clean(layotL,layotH);
    clean(layotE,layotMax);
    setData(length, height);
    setRectange();
    flag=0;
    startFlag=0;
    k=0;
    ui->start->setEnabled(true);
    ui->resume->setEnabled(false);
}

void Convolution::Triangle()
{
    length=100;
    clean(layotL,layotH);
    clean(layotE,layotMax);
    setData(length);
    setTriangle();
    flag=1;
    startFlag=0;
    k=0;
    ui->start->setEnabled(true);
    ui->resume->setEnabled(false);
}

void Convolution::Sinc()
{
    length=50;
    clean(layotL,layotH);
    clean(layotE,layotMax);
    setData(length);
    setSinc();
    flag=2;
    startFlag=0;
    k=0;
    ui->start->setEnabled(true);
    ui->resume->setEnabled(false);
}

void Convolution::Start()
{
    ui->stop->setEnabled(true);
    ui->resume->setEnabled(false);
    switch (flag)
    {
    case 0:
        clean(layotE,layotMax);
        setRectange();
        break;
    case 1:
        clean(layotE,layotMax);
        setTriangle();
        break;
    case 2:
        clean(layotE,layotMax);
        setSinc();
    }

    *t=10000/length;
    signalPoint2 = signalPoint;
    k=0;
    if(signal.size()) signal.clear();
    rectangeMethod();
    startFlag=1;
    AKF();
}

void Convolution::Stop()
{
    startFlag=0;
    ui->resume->setEnabled(true);
    ui->stop->setEnabled(false);
    setDataAKF();
}

void Convolution::Resume()
{
    startFlag=1;
    ui->resume->setEnabled(false);
    ui->stop->setEnabled(true);
}

void Convolution::clean(QHBoxLayout* layot1, QHBoxLayout* layot2)
{
    if(signalPoint.size()) signalPoint.clear();
    upperSeries->clear();
    upperSeries2->clear();

    if(akfPoint.size()) akfPoint.clear();
    upperSeriesAKF->clear();

    if(akf.size()) akf.clear();

    cleanLayout(layot1);
    cleanLayout(layot2);
}

void Convolution::cleanLayout(QLayout *layout)
{
    for(int i=0; layout->count(); ++i)
    {
        QLayoutItem * item = layout->itemAt((i));

        while(item=layout->itemAt((0)))
        {
            layout->removeItem(item);
            layout->removeWidget((item->widget()));
            delete item->widget();
            delete item;
            layout->update();
        }
    }
}

void Convolution::setData(int Length, int Height)
{
    QString styleLine = "QLineEdit { background: rgb(206,231,240); "
                        "color: rgb(64,64,68);"
                        "border: rgb(64,64,68) }";
    if(Length)
    {
        QLabel* lL = new QLabel("Длина: " );
        lL->setFixedSize(QSize(50,22));

        lineL = new QLineEdit();
        lineL->setStyleSheet(styleLine);
        lineL->setText(QString::number(Length));
        lineL->setFixedSize(QSize(100,22));
        lineL->setReadOnly(true);

        layotL->addWidget(lL);
        layotL->addWidget(lineL);

        ui->LayoutAfterData->addLayout(layotL);
    }
    if(Height)
    {
        QLabel* lH = new QLabel("Высота: ");
        lH->setFixedSize(QSize(50,22));

        lineH = new QLineEdit();
        lineH->setStyleSheet(styleLine);
        lineH->setText(QString::number(Height));
        lineH->setFixedSize(QSize(100,22));
        lineH->setReadOnly(true);

        layotH->addWidget(lH);
        layotH->addWidget(lineH);

        ui->LayoutAfterData->addLayout(layotH);
    }
}

void Convolution::rectangeMethod()
{
    coordinates temp;
    for (double i=0;i<length*2;i+=2)
    {
        temp.first=i; temp.second=(signalPoint[i+1].second+signalPoint[i].second)/2;
        signal.push_back(temp);
    }
}

void Convolution::setRectange()
{
    //составим список точек функции
    coordinates temp;
    for (double i=-length/2;i<=length/2;i+=count)
    {
        temp.first=i; temp.second=height;
        signalPoint.push_back(temp);
        *upperSeries << QPointF(temp.first, temp.second);
    }
    SignalAxisY->setRange(0, 2*height);
    SignalAxisX->setRange(-length, length);
    akfAxisY->setRange(0,height*height*length);
}

void Convolution::setTriangle()
{
    //составим список точек функции
    coordinates temp;
    for (double i=-length/2, j=0 ;i<=length/2 ;i+=count, j+=count)
    {
        temp.first=i; temp.second=j;
        signalPoint.push_back(temp);
        *upperSeries << QPointF(temp.first, temp.second);
    }
    SignalAxisY->setRange(0, length);
    SignalAxisX->setRange(-length, length);
    akfAxisY->setRange(0,length*length*length/2);
}

void Convolution::setSinc()
{
    //составим список точек функции
    coordinates temp;
    for (double i=-length/2;i<=length/2;i+=count)
    {
        temp.first=i;
        if(i==0) temp.second=1;
        else temp.second=abs(sin(i)/i);
        //temp.first=i;
        signalPoint.push_back(temp);
        *upperSeries << QPointF(temp.first, temp.second);
    }
    SignalAxisY->setRange(0, 1);
    SignalAxisX->setRange(-length, length);
    akfAxisY->setRange(0,3);
}

void Convolution::AKF()
{
    tempList = signal;
    coordinates t;
    int Ssize = signal.size();
    maxY=0;
    for (double i=0 ; i < Ssize ;i++)
    {
        t.second=0; t.first=0;
        tempList.push_front(t);
        tempList.push_back(t);
    }

    akfAxisX->setRange(-length, length);
}

void Convolution::startTime()
{
    coordinates temp;
    int Ssize = signal.size();
    int Tsize = tempList.size();
    double E=0;

    if(startFlag)
    {
        if(k<signalPoint2.size())
        {
            upperSeries2->clear();
            for(int i=0; i<signalPoint2.size(); i++)
            {
                *upperSeries2 << QPointF(signalPoint2[i].first+k-length, signalPoint2[i].second);
            }

            for(double q=0; q<Ssize; q++)
            {
                double a=signal[q].second;
                double b=tempList[2*length-k+q].second;
                E+=a*b;
            }
            if(E>maxY)maxY=E;
            temp.first=k-length; temp.second=E;
            akf.push_back(temp);
            *upperSeriesAKF << QPointF(temp.first, temp.second);

            k++;
        }
        else
        {
            k=0;
            startFlag=0;
            upperSeries2->clear();
            ui->stop->setEnabled(false);
            setDataAKF();
        }
    }
}

void Convolution::setDataAKF()
{
    cleanLayout(layotE);
    cleanLayout(layotMax);
    QString styleLine = "QLineEdit { background: rgb(206,231,240); "
                        "color: rgb(64,64,68);"
                        "border: rgb(64,64,68) }";

    QLabel* lMax = new QLabel("Max: " );
    lMax->setFixedSize(QSize(50,22));

    lineMax = new QLineEdit();
    lineMax->setStyleSheet(styleLine);
    lineMax->setText(QString::number(maxY));
    lineMax->setFixedSize(QSize(100,22));
    lineMax->setReadOnly(true);

    layotMax->addWidget(lMax);
    layotMax->addWidget(lineMax);

    ui->LayoutBeforData->addLayout(layotMax);
}

void Convolution::Help()
{
    QString About= "Автокорреляционная функция(АКФ) - зависимость взаимосвязи между функцией(сингалом) и её сдвинутой копией от величины временного сдвига.\n";
            About+= "Для постройки АКФ выберете один из трех представленных сигналов (прямоугольник, треугольник, |sinc|);\n";
            About+= "При необходимости скорректируйте параметры сигнала;\n";
            About+= "Нажмите кнопку 'старт';\n";
    QMessageBox MB(("О программе"),About,QMessageBox::NoIcon,QMessageBox::Ok,Qt::NoButton,Qt::NoButton);

    MB.setWindowIcon(QIcon(":/new/icons/icon_converter.png"));

    MB.setButtonText(QMessageBox::Ok,("ОК"));
    MB.setModal(true);

    MB.exec();
}







