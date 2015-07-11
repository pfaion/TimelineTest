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

    int rowHeight;
    int colWidth;

    inline int rowCenter(int row) { return rowHeight * row + rowHeight/2; };
    inline int colCenter(int col) { return colWidth * col + colWidth/2; };

    struct DataPoint {
        int frame;
        int type;
        int changeWith;
        DataPoint(int f, int t, int cw = -1): frame(f), type(t), changeWith(cw) {}
    };

    struct DataObject {
        QString name;
        QList<DataPoint> data;
        DataObject(QString n): name(n) {}
    };

};

#endif // TIMELINEWIDGET_H
