#include "consumtofile.h"
#include <fstream>

ConsumToFile::ConsumToFile()
{

}

void ConsumToFile::onCmdReceived(const std::vector<std::string> &vct_str)
{
    std::string str="";
    for(std::vector<std::string>::const_iterator it = vct_str.cbegin(); it != vct_str.cend(); ++it){
        if( it != vct_str.cbegin() ) str +=", ";
        else str += "bulk: ";
        str+= *it;
    }
    writeLog(str);
}

void ConsumToFile::sharedProcess(std::shared_ptr<BulkHolder> bulk)
{
    while (true) { // welcome to infinity
        if(bulk->is_finalized() && bulk->get_size() == 0 )break;

        auto t = bulk->pop();

        if(t.empty()) continue;
        m_strBulkName = t.back();
        t.pop_back();
        if(t.empty()) continue;

        ++m_iBlokCnt;
        m_iCmdCnt+= t.size();
        onCmdReceived(t);
    }

}

void ConsumToFile::writeLog(std::string &str)
{
    if(str.empty()) return;
    std::ofstream fout(m_strBulkName+".log");
    fout << str;
    fout.close();
}
