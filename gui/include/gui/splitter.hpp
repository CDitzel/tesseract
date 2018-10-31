#pragma once
#include <QSplitter>

class Splitter : public QSplitter
{
    Splitter(Qt::Orientation orientation) : QSplitter{orientation} {
        vsplit1 = new QSplitter{this};
        vsplit2 = new QSplitter{this};
    }
    QSplitter *vsplit1;
    QSplitter *vsplit2;
};
