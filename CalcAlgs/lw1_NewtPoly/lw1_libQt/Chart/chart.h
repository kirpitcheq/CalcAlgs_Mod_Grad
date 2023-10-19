#pragma once

#include <QChart>
#include <QScatterSeries>

class Chart : public QChart
{
    Q_OBJECT
public:
    Chart();
    void updateSeries(QAbstractSeries *series);
    void addPointToNewSeries(QPointF &point, qreal markersize = 10);
};

#ifdef ADD_TO_SERIES_WITHOUT_MARK
    series->append(xtarget, desValueY.value()); //HOW set BOLD this point?
    auto points = series->points();
    std::sort(points.begin(), points.end(), [](const auto & p1, const auto & p2){return p1.x() > p2.x();}); //do no back line diagram
    series->replace(points);
    //need two charts for compare
#endif
