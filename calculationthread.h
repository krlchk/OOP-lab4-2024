#ifndef CALCULATIONTHREAD_H
#define CALCULATIONTHREAD_H

#include <QMainWindow>
#include <QThread>
#include <QLabel>

#include <map>
#include <string>

#include "frequencyanalysisthread.h"

class CalculationThread : public QThread
{
    Q_OBJECT

private:
    QString path;
    bool singleTreadMode;
    std::map<std::string, size_t> frMap;

public:
    CalculationThread(QString path = "", bool singleTreadMode = false);
    std::map<std::string, size_t> getFrMap() const;

protected:
    void run();
};

#endif // CALCULATIONTHREAD_H
