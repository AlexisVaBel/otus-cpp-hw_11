#ifndef CONSUMTOFILE_H
#define CONSUMTOFILE_H

#include "ibulkconsumer.h"

#include <string>

class ConsumToFile : public IBulkConsumer
{
public:
    ConsumToFile();

    // IBulkConsumer interface
    void onCmdReceived(const std::vector<std::string> &vct_str);
    void sharedProcess(std::shared_ptr<BulkHolder>  bulk);

private:
    std::string m_strBulkName;

    void writeLog(std::string &str);
};

#endif // CONSUMTOFILE_H
