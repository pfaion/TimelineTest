#include "timelinewidget.h"
#include "ui_timelinewidget.h"
#include <QPalette>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QGraphicsView>
#include <QHeaderView>
#include <QGraphicsPathItem>

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

    QGraphicsView* view = ui->graphicsView;

    QGraphicsScene* s = new QGraphicsScene();
    view->setScene(s);




}

TimelineWidget::~TimelineWidget()
{
    delete ui;
}





