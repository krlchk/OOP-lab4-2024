#ifndef FREQUENCYANALYSISTHREAD_H
#define FREQUENCYANALYSISTHREAD_H

#include <QThread>
#include <map>

class FrequencyAnalysisThread : public QThread
{
    Q_OBJECT

private:
    char* buff;
    std::map<std::string, size_t> frMap;

public:
    FrequencyAnalysisThread(char* buff = nullptr);
    std::map<std::string, size_t> getFrMap() const;

protected:
    void run();
};

#endif // FREQUENCYANALYSISTHREAD_H
