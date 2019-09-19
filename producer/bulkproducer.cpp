#include "bulkproducer.h"

#include <thread>

BulkProducer::BulkProducer(std::shared_ptr<BulkHolder> blk, std::shared_ptr<BulkHolder> blk2, std::shared_ptr<CmdAcceptor> cmd):m_blk(blk),m_blk2(blk2),m_cmd(cmd)
{
}

void BulkProducer::addItm(std::shared_ptr<ContextItm> itm)
{
    std::unique_lock<std::mutex> mlock(m_mutex);
    m_queueItms.push(itm);
}


void BulkProducer::run()
{
    while (m_bRun) {
        if(BULK_PROD_MAX_RELAX < m_relaxCnt) {
            m_bRun = false;
            finish_it();
        }
        auto procsCnt = 0;
        m_mutex.lock();
        if(! m_queueItms.empty()){
            auto t = m_queueItms.front();
            if(t->get_empty() != 0){
                m_queueItms.pop();
                size_t sz = t->get_order();
                while (sz > 0) {
                    --sz;
                    procsCmd(t->get_cmd(), t->get_sizeBulk());
                    ++procsCnt;
                }
            }

        };
        m_mutex.unlock();
        if(procsCnt == 0){
            ++m_relaxCnt;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }


    };
}

void BulkProducer::procsCmd(){
    for(std::string line; std::getline(std::cin, line);){
        if(m_cmd->procsCmd(line)){
            m_blk->push_back(m_cmd->getCmds());
            m_blk2->push_back(m_cmd->getCmds());
            m_cmd->clearProcessd();
        }
    }    
    m_blk->push_back(m_cmd->getCmds());
    m_blk2->push_back(m_cmd->getCmds());
    m_cmd->clearProcessd();
    m_blk->setFinalised();
    m_blk2->setFinalised();
}

void BulkProducer::procsCmd(std::string str, size_t szBulk)
{
    if(m_cmd->procsCmd(str, szBulk)){
        m_blk->push_back(m_cmd->getCmds());
        m_blk2->push_back(m_cmd->getCmds());
        m_cmd->clearProcessd();
    }
}

void BulkProducer::finish_it()
{
    m_blk->push_back(m_cmd->getCmds());
    m_blk2->push_back(m_cmd->getCmds());
    m_cmd->clearProcessd();
    m_blk->setFinalised();
    m_blk2->setFinalised();
}

int BulkProducer::get_total_lines()
{
    return m_cmd->get_lin_cnt();
}

int BulkProducer::get_total_cmds()
{
    return m_cmd->get_cmds_cnt();
}

int BulkProducer::get_total_blok()
{
    return m_cmd->get_blok_cnt();
}
