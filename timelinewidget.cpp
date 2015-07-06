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

    for(int i=0; i<10; i++) {
        s->addLine(0,40*i,100,40*i,QPen(Qt::red));
        s->addLine(0,40*i+39,100,40*i+39,QPen(Qt::black));
    }
    for(int i=0; i<10; i++) {
        s->addLine(40*i+20,0,40*i+20, 400, QPen(Qt::green));
    }

    QGraphicsScene* hs = new QGraphicsScene(0);
    ui->header->setScene(hs);
    ui->header->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    hs->addLine(0,0,0,1, QPen(Qt::white));
    for(int i=0; i<10; i++) {
        QGraphicsTextItem* txt = new QGraphicsTextItem(QString::number(i+1));
        int w = txt->boundingRect().width();
        txt->setPos(40*i+20 -w/2,0);
        hs->addItem(txt);
    }
}

TimelineWidget::~TimelineWidget()
{
    delete ui;
}





