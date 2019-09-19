#pragma once


#include <map>
#include <memory>
#include <condition_variable>

#include "contextitm.h"
#include "../datahodor/bulkholder.h"
#include "../consumer/consumtofile.h"
#include "../consumer/consumtoterminal.h"
#include "../producer/bulkproducer.h"
#include "../processor/cmdacceptor.h"




#include <thread>
class ContextHolder{
public:
    static ContextHolder& get_instance()
    {
        static ContextHolder m_contextHolder;
        return m_contextHolder;
    }

    ~ContextHolder() {
        m_thrProd.join();
        m_thrFile.join();
        m_thrTerm.join();
    }


    void * do_connect(size_t szBulk){

        std::shared_ptr<ContextItm> itm(new ContextItm(szBulk));
        void *res = itm.get();

        std::unique_lock<std::mutex> mlock(m_mutex);
        m_mapItms.insert(std::pair<void*,std::shared_ptr<ContextItm>>(res,itm));

        return res;
    }

    void do_receive(void *id, const char *data, size_t szData){
        std::unique_lock<std::mutex> mlock(m_mutex);
        if(id == nullptr || m_mapItms.find(id) == m_mapItms.end())return;
        m_mapItms.at(id)->process(data,szData);

        m_producer->addItm(m_mapItms.at(id));
    }

    void disconnect(void *id){
        std::unique_lock<std::mutex> mlock(m_mutex);
        m_mapItms.erase(id);
        m_cond.notify_all();
    }

private:
    // magic number 5 - just magic number
    ContextHolder():
        m_cmd(std::shared_ptr<CmdAcceptor>          (new CmdAcceptor(10))),
        m_consFile(std::unique_ptr<ConsumToFile>    (new ConsumToFile())),
        m_consTerm(std::unique_ptr<ConsumToTerminal>(new ConsumToTerminal())),
        m_bulkTerm(std::shared_ptr<BulkHolder>      (new BulkHolder())),
        m_bulkFile(std::shared_ptr<BulkHolder>      (new BulkHolder())),
        m_producer(std::shared_ptr<BulkProducer>    (new BulkProducer(m_bulkTerm, m_bulkFile, m_cmd)))
    {
        m_thrFile = std::thread(&IBulkConsumer::sharedProcess, m_consFile.get(), m_bulkFile);
        m_thrTerm = std::thread(&IBulkConsumer::sharedProcess, m_consTerm.get(), m_bulkTerm);
        m_thrProd = std::thread(&BulkProducer::run, m_producer.get());
    }

    // on create contextholder, create bulkconsumers, one shared place for commands
    // each contextitm would have it`s oun command
    std::shared_ptr<CmdAcceptor>        m_cmd;

    std::unique_ptr<ConsumToFile>       m_consFile;
    std::unique_ptr<ConsumToTerminal>   m_consTerm;
    std::shared_ptr<BulkHolder>         m_bulkTerm;
    std::shared_ptr<BulkHolder>         m_bulkFile;
    std::shared_ptr<BulkProducer>       m_producer;



    std::thread m_thrFile;
    std::thread m_thrTerm;
    std::thread m_thrProd;

    std::map<void*, std::shared_ptr<ContextItm>> m_mapItms;
    std::mutex                                   m_mutex;
    std::condition_variable                      m_cond;
};
