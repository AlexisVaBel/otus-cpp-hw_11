#include "consumtoterminal.h"

#include <iostream>

ConsumToTerminal::ConsumToTerminal()
{

}

void ConsumToTerminal::onCmdReceived(const std::vector<std::string> &vct_str)
{
    for(std::vector<std::string>::const_iterator it = vct_str.cbegin(); it != vct_str.cend(); ++it){
        if( it != vct_str.cbegin() ) std::cout << ", ";
        else std::cout << "bulk: ";
        std::cout << *it;
    }
    std::cout << std::endl;
}

void ConsumToTerminal::sharedProcess(std::shared_ptr<BulkHolder> bulk)
{
    while (true) { // welcome to infinity
        if(bulk->is_finalized() && bulk->get_size() == 0 )break;

        auto t = bulk->pop();

        if(t.empty()) continue;
        t.pop_back();
        if(t.empty()) continue;

        ++m_iBlokCnt;
        m_iCmdCnt+= t.size();
        onCmdReceived(t);
    }
}


