#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QWidget>

namespace Ui {
class TimelineWidget;
}



class TimelineWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TimelineWidget(QWidget *parent = 0);
    ~TimelineWidget();



private:
    Ui::TimelineWidget *ui;

    struct DataPoint {
        int frame;
        int type;
        DataPoint(int f, int t): frame(f), type(t) {}
    };

    struct DataObject {
        QString name;
        QList<DataPoint> data;
        DataObject(QString n): name(n) {}
    };

};

#endif // TIMELINEWIDGET_H
