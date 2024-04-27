#include "frequencyanalysisthread.h"

#include <QDebug>

FrequencyAnalysisThread::FrequencyAnalysisThread(char* buff)
    : buff(buff)
{

}

std::map<std::string, size_t> FrequencyAnalysisThread::getFrMap() const
{
    return frMap;
}

void FrequencyAnalysisThread::run()
{
    if (buff)
    {
        const char delim[] = " \n\r\t.,!?:;\'\"-*+()[]{}<>~=/\\“”‘’#$&%—";
        char *token;
        char *context;
        token = strtok_s(buff, delim, &context);
        while (token != nullptr) {
            frMap[token] += 1;
            token = strtok_s(nullptr, delim, &context);
        }
    }
}
