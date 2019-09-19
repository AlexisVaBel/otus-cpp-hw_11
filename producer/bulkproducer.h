#ifndef BULKPRODUCER_H
#define BULKPRODUCER_H

#include "../datahodor/bulkholder.h"
#include "../processor/cmdacceptor.h"
#include "../context/contextitm.h"

#include <queue>

static const size_t BULK_PROD_MAX_RELAX = 15;
class BulkProducer
{
    std::shared_ptr<BulkHolder>  m_blk;
    std::shared_ptr<BulkHolder>  m_blk2;
    std::shared_ptr<CmdAcceptor> m_cmd;



    std::queue<std::shared_ptr<ContextItm>> m_queueItms;

    bool        m_bRun{true};
    size_t      m_relaxCnt{0};
    std::mutex  m_mutex;
public:
    BulkProducer(std::shared_ptr<BulkHolder> blk,std::shared_ptr<BulkHolder> blk2, std::shared_ptr<CmdAcceptor> cmd);
    void addItm(std::shared_ptr<ContextItm> itm);


    void run();

    void procsCmd();
    void procsCmd(std::string str, size_t szBulk);
    void finish_it();
    int  get_total_lines();
    int  get_total_cmds();
    int  get_total_blok();

};

#endif // BULKPRODUCER_H
