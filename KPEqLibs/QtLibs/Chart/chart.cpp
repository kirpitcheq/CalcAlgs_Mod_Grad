#include <KPEq/chart.h>
#define DARK_THEME_SET

using namespace KPEq;
Chart::Chart() {
    this->legend()->hide();
    this->setAnimationOptions(QChart::SeriesAnimations);
    this->createDefaultAxes();
#ifdef linux
#ifdef DARK_THEME_SET
    this->setTheme(QChart::ChartThemeDark);
#endif
#endif
    this->show();
}

void Chart::updateSeries(QAbstractSeries * series){
    this->removeAllSeries();
    this->createDefaultAxes();
    this->addSeries(series);
    this->createDefaultAxes();
}

void Chart::addPointToNewSeries(QPointF & point, qreal markersize){
    auto onepointseries = new QScatterSeries(this);
    onepointseries->append(point);
    onepointseries->setMarkerSize(markersize);
    this->addSeries(onepointseries);
    this->createDefaultAxes();
}
