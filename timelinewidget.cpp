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
#include <QLabel>

TimelineWidget::TimelineWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimelineWidget),
    rowHeight(40),
    colWidth(40)
{
    ui->setupUi(this);

    // set white background
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::white);
    ui->frame->parentWidget()->setAutoFillBackground(true);
    ui->frame->parentWidget()->setPalette(Pal);

    // list alignment to top
    ui->scrollAreaWidgetContents->layout()->setAlignment(Qt::AlignTop);

    // setup graphicsscene
    QGraphicsView* view = ui->graphicsView;
    QGraphicsScene* s = new QGraphicsScene(0);
    view->setScene(s);
    // TODO: antialiasing??
    view->setRenderHint(QPainter::Antialiasing);

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

    // setup header graphicsscene
    QGraphicsScene* hs = new QGraphicsScene(0);
    ui->header->setScene(hs);
    ui->header->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    hs->addLine(0,0,0,1, QPen(Qt::white));
    for(int i=0; i<15; i++) {
        QGraphicsTextItem* txt = new QGraphicsTextItem(QString::number(i+1));
        int w = txt->boundingRect().width();
        txt->setPos(colCenter(i) - w/2,0);
        hs->addItem(txt);
    }

    // Test Data
    QList<DataObject> objects;
    {
        DataObject auto1("Automobil 1");
        auto1.data.append(DataPoint(1,1));
        auto1.data.append(DataPoint(5,1,1));
        auto1.data.append(DataPoint(10,1));
        auto1.data.append(DataPoint(15,1));
        objects.append(auto1);
        DataObject auto2("Automobil 2");
        auto2.data.append(DataPoint(1,1));
        auto2.data.append(DataPoint(2,1));
        auto2.data.append(DataPoint(3,1));
        auto2.data.append(DataPoint(4,1));
        auto2.data.append(DataPoint(5,1,0));
        auto2.data.append(DataPoint(12,1));
        objects.append(auto2);
    }
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

    // which frames are used
    QList<int> framesToDraw;
    foreach(DataObject obj, objects) {
        foreach(DataPoint p, obj.data) {
            if(!framesToDraw.contains(p.frame)) framesToDraw.append(p.frame);
        }
    }
    qSort(framesToDraw);

    // draw framelines
    QPen framelinePen;
    framelinePen.setColor(Qt::gray);
    framelinePen.setStyle(Qt::DotLine);
    foreach(int f, framesToDraw) {
        s->addLine(colCenter(f-1), 0, colCenter(f-1), objects.length() * rowHeight, framelinePen);
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

        // add data lines
        int pointradius = 5;
        for(int j=0; j<obj.data.length()-1; j++) {
            QPen linePen;
            linePen.setColor(Qt::red);
            linePen.setWidth(2);
            int f1 = obj.data[j].frame-1;
            int f2 = obj.data[j+1].frame-1;
            // test if swap
            if(obj.data[j+1].changeWith != -1) {
                // test if prefill needed
                int fstart = f1;
                if(f2-1 > f1) {
                    fstart = f2-1;
                    s->addLine(colCenter(f1), rowCenter(i), colCenter(fstart), rowCenter(i), linePen);
                }
                int cw = obj.data[j+1].changeWith;
                QPainterPath path;
                path.moveTo(colCenter(fstart),rowCenter(i));
                path.cubicTo(colCenter(f2)-colWidth/2, rowCenter(i), colCenter(fstart)+colWidth/2, rowCenter(cw), colCenter(f2), rowCenter(cw));

                QGraphicsPathItem* pathitem =  s->addPath(path, linePen);
            } else {
                s->addLine(colCenter(f1), rowCenter(i), colCenter(f2), rowCenter(i), linePen);
            }
        }
        // add point handles
        for(int j=0; j<obj.data.length(); j++) {
            int f = obj.data[j].frame-1;
            QGraphicsEllipseItem* handle = s->addEllipse(colCenter(f) - pointradius, rowCenter(i) - pointradius, 2*pointradius, 2*pointradius, QPen(Qt::black), QBrush(Qt::black));
            handle->setZValue(1);
        }
    }


}

TimelineWidget::~TimelineWidget()
{
    delete ui;
}





