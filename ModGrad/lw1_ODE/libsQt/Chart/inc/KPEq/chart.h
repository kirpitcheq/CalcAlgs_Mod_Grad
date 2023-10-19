#pragma once

#include <QChart>
#include <QScatterSeries>
#include <QLineSeries>

namespace KPEq{

class Chart : public QChart
{
    Q_OBJECT
    template <typename T>
    int stdVectToQSeries(QLineSeries& outsers, const std::vector<std::pair<T,T>> & vectin){
        for(auto [x,y] : vectin){
            outsers.append(QPointF(x,y));
        }
        return 1;
    }
public:
    Chart();
    void updateSeries(QAbstractSeries *series);
    void addPointToNewSeries(QPointF &point, qreal markersize = 10);
    template <typename T>
    void updateSeries(const std::vector<std::pair<T,T>> & vectin){
        auto series = new QLineSeries;
        this->stdVectToQSeries(*series, vectin);
        updateSeries(series);
    }
    template <typename T>
    void addVectSeries(const std::vector<std::pair<T,T>> & vectin){
        auto series = new QLineSeries;
        stdVectToQSeries(*series, vectin);
        addSeries(series);
    }
};

#ifdef ADD_TO_SERIES_WITHOUT_MARK
    series->append(xtarget, desValueY.value()); //HOW set BOLD this point?
    auto points = series->points();
    std::sort(points.begin(), points.end(), [](const auto & p1, const auto & p2){return p1.x() > p2.x();}); //do no back line diagram
    series->replace(points);
    //need two charts for compare
#endif

}
