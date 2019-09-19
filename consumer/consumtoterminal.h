#ifndef CONSUMTOTERMINAL_H
#define CONSUMTOTERMINAL_H

#include "ibulkconsumer.h"

#include <string>

class ConsumToTerminal : public IBulkConsumer
{
public:
    ConsumToTerminal();

    // IBulkConsumer interface
    void onCmdReceived(const std::vector<std::string> &vct_str);
    void sharedProcess(std::shared_ptr<BulkHolder>  bulk);


};

#endif // CONSUMTOTERMINAL_H
