#ifndef IBULKCONSUMER_H
#define IBULKCONSUMER_H

#include "../datahodor/bulkholder.h"

class IBulkConsumer{
public:
    virtual ~IBulkConsumer(){}

    virtual void onCmdReceived(const std::vector<std::string> &vct_str) = 0;

    virtual void sharedProcess(std::shared_ptr<BulkHolder>  bulk) = 0;

    virtual int  get_blok_cnt(){
        return m_iBlokCnt;
    }
    virtual int  get_cmd_cnt(){
        return m_iCmdCnt;
    }
protected:
    int         m_iBlokCnt;
    int         m_iCmdCnt;
};



#endif // IBULKCONSUMER_H
