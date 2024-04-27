#include "calculationthread.h"

#include <QString>

#include <vector>
#include <fstream>

CalculationThread::CalculationThread(QString path, bool singleTreadMode)
    : path(path), singleTreadMode(singleTreadMode)
{

}

std::map<std::string, size_t> CalculationThread::getFrMap() const
{
    return frMap;
}

void CalculationThread::run()
{
    std::ifstream in(path.toStdString().c_str(), std::ios::in | std::ios::binary);
    qDebug() << QString(path.toStdString().c_str()) << "\n";
    if (in.is_open())
    {
        qDebug() << "File is open!\n";
        if (singleTreadMode)
        {
            size_t blockSize = 1024 * 100;
            while (!in.eof())
            {
                char* buff = new char[blockSize];
                in.read(buff, blockSize);
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
        }
        else
        {
            size_t blockSize = 1024 * 100;
            std::vector<FrequencyAnalysisThread*> threads;
            int i = 0;
            char is[10];
            while (!in.eof())
            {
                char* buff = new char[blockSize];
                in.read(buff, blockSize);
                itoa(++i, is, 10);
                qDebug() << is;
                threads.push_back(new FrequencyAnalysisThread(buff));
                threads.back()->start();
            }
            for (FrequencyAnalysisThread* thread : threads)
            {
                thread->wait();
            }
            for (FrequencyAnalysisThread* thread : threads)
            {
                auto temp = thread->getFrMap();
                for (auto& word : temp)
                {
                    frMap[word.first] += word.second;
                }
            }
            for (FrequencyAnalysisThread* thread : threads)
            {
                thread->terminate();
                delete thread;
            }
        }
    }
    else
        qDebug() << "File is not open!\n";
}
