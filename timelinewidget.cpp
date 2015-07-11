#include "timelinewidget.h"
#include "ui_timelinewidget.h"
#include <QPalette>
#include <QDebug>
#include <QGraphicsTextItem>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QGraphicsView>
#include <QHeaderView>
#include <QGraphicsPathItem>
#include <QGraphicsScene>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDir>

TimelineWidget::TimelineWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimelineWidget)
{
    ui->setupUi(this);

    // set white background
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::white);
    ui->frame->parentWidget()->setAutoFillBackground(true);
    ui->frame->parentWidget()->setPalette(Pal);

    // list alignment to top
    ui->scrollAreaWidgetContents->layout()->setAlignment(Qt::AlignTop);

    // add a clean scene
    QGraphicsView* view = ui->graphicsView;
    QGraphicsScene* s = new QGraphicsScene(0);
    view->setScene(s);

    // align origins of scene and view
    // TODO: only workaround. is there a better way?
    view->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    s->addLine(0,0,1,0,QPen(Qt::white));

    // synch scroll of buttonlist and graphicsview
    connect(view->verticalScrollBar(), SIGNAL(valueChanged(int)), ui->scrollArea->verticalScrollBar(), SLOT(setValue(int)));
    connect(ui->scrollArea->verticalScrollBar(), SIGNAL(valueChanged(int)), view->verticalScrollBar(), SLOT(setValue(int)));

    // synch scroll of header and graphicsview
    connect(view->horizontalScrollBar(), SIGNAL(valueChanged(int)), ui->header->horizontalScrollBar(), SLOT(setValue(int)));
    connect(ui->header->horizontalScrollBar(), SIGNAL(valueChanged(int)), view->horizontalScrollBar(), SLOT(setValue(int)));

    QGraphicsScene* hs = new QGraphicsScene(0);
    ui->header->setScene(hs);
    ui->header->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    hs->addLine(0,0,0,1, QPen(Qt::white));
    for(int i=0; i<15; i++) {
        QGraphicsTextItem* txt = new QGraphicsTextItem(QString::number(i+1));
        int w = txt->boundingRect().width();
        txt->setPos(40*i+20 -w/2,0);
        hs->addItem(txt);
    }

    // Test Data
    QList<DataObject> objects;
    for(int i=0; i<5; i++) {
        DataObject auto1("Auto "+QString::number(2*i));
        auto1.data.append(DataPoint(1,1));
        auto1.data.append(DataPoint(5,1));
        auto1.data.append(DataPoint(10,1));
        auto1.data.append(DataPoint(15,1));
        objects.append(auto1);
        DataObject auto2("Auto "+QString::number(2*i+1));
        auto2.data.append(DataPoint(1,1));
        auto2.data.append(DataPoint(2,1));
        auto2.data.append(DataPoint(3,1));
        auto2.data.append(DataPoint(4,1));
        auto2.data.append(DataPoint(5,1));
        auto2.data.append(DataPoint(12,1));
        objects.append(auto2);
    }

    // parse data
    for(int i=0; i<objects.length(); i++) {
        DataObject obj = objects[i];

        // add left header
        QFrame* frame = new QFrame;

        QHBoxLayout* layout = new QHBoxLayout;
        frame->setLayout(layout);

        QLabel* nameLabel = new QLabel;
        nameLabel->setText(obj.name);
        layout->addWidget(nameLabel);

        ui->scrollAreaWidgetContents->layout()->addWidget(frame);

        // add data
        for(int j=0; j<obj.data.length()-1; j++) {
            int f1 = obj.data[j].frame-1;
            int f2 = obj.data[j+1].frame-1;
            s->addLine(40*f1+20, 40*i+20, 40*f2+20, 40*i+20, QPen(Qt::red));
            s->addEllipse(40*f1+15, 40*i+15, 10, 10, QPen(Qt::black));
        }
        // last index
        int f = obj.data.last().frame-1;
        s->addEllipse(40*f+15, 40*i+15, 10, 10, QPen(Qt::black));
    }


}

TimelineWidget::~TimelineWidget()
{
    delete ui;
}





