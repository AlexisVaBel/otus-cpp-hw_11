#ifndef BULKHOLDER_H
#define BULKHOLDER_H
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <vector>

#include <condition_variable>
#include <chrono>

using namespace std::chrono_literals;

struct BulkHolder{

    BulkHolder():m_bFinalized(false){

    }

    bool is_finalized(){
        std::unique_lock<std::mutex> mlock(m_mutex);
        return  m_bFinalized;
    }

    int get_size(){return m_queue.size();}

    void push_back(std::vector<std::string> str){
        std::unique_lock<std::mutex> mlock(m_mutex);
        m_queue.push(str);
        m_cond.notify_one();
    }

    std::vector<std::string> pop(){
        std::unique_lock<std::mutex> mlock(m_mutex);
        if(!m_cond.wait_for(mlock, 100ms,[this]{return ( (get_size() > 0) );})){
            return std::vector<std::string>();
        }
        auto last = m_queue.front();
        m_queue.pop();
        return last;
    }

    void setFinalised(){
        std::unique_lock<std::mutex> mlock(m_mutex);
        m_bFinalized = true;
        m_cond.notify_all();
    }


    std::mutex m_mutex;
    std::queue<std::vector<std::string>> m_queue;
    std::condition_variable m_cond;

    bool    m_bFinalized;
};

#endif // BULKHOLDER_H
