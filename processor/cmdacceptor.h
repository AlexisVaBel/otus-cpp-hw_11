#ifndef CMDACCEPTOR_H
#define CMDACCEPTOR_H

#include <vector>
#include <stack>
#include <string>
#include <iostream>

static const int MAX_BULK_PER_SECOND = 10;


class CmdAcceptor{
public:
    CmdAcceptor(uint cnt):m_iCnt(cnt), m_iLineCnt(0), m_iCmdsCnt(0), m_iBlokCnt(0),m_iCntHelper(0){
        clearProcessd();
    }

    bool procsCmd(std::string str, size_t szMax){
        if(!str.empty()) ++m_iLineCnt;

        if(str.compare("{") == 0){
            if(m_cmds.empty() && m_prms.empty()) m_strFstCmdArrived = makeTimePrefix();
            m_prms.push(str);
            return !m_cmds.empty();
        }
        if(str.compare("}") == 0){
            if(m_prms.size() > 0 && m_prms.top().compare("{") == 0)
                m_prms.pop();
            if((m_prms.size() == 0 && m_cmds.size() > 0)){
                return true;
            }
            return false;
        }
        if(str.empty()) {
            if((m_cmds.size() > 0 && (m_prms.empty()))){
                return  true;
            }
            return false;
        }

        if(m_cmds.empty()) m_strFstCmdArrived = makeTimePrefix();

        m_cmds.push_back(str);

        if(m_cmds.size() >= szMax && m_prms.empty()){
            return true;
        }
        return false;
    }

    bool procsCmd(std::string str){
        return procsCmd(str, m_iCnt);
    }

    std::vector<std::string> getCmds() const{
        auto tmp = m_cmds;
        tmp.push_back(m_strFstCmdArrived);
        return tmp;
    }

    std::string getFirstCmdArrived(){
        return m_strFstCmdArrived;
    }

    void clearProcessd(){
        if(m_cmds.size() > 0)
        ++m_iBlokCnt;
        m_iCmdsCnt+= m_cmds.size();
        m_cmds.clear();
    }


    int get_lin_cnt(){
        return m_iLineCnt;
    }

    int get_cmds_cnt(){
        return m_iCmdsCnt;
    }

    int get_blok_cnt(){
        return m_iBlokCnt;
    }

private:
    uint        m_iCnt;

    int         m_iLineCnt;
    int         m_iCmdsCnt;
    int         m_iBlokCnt;
    int         m_iCntHelper;



    std::string               m_strFstCmdArrived;
    std::vector<std::string>  m_cmds;
    std::stack<std::string>   m_prms;

    std::string correctStrSuffix(std::string str){
        std::string strSuffix = std::to_string(m_iCntHelper);
        strSuffix.insert(strSuffix.begin(),MAX_BULK_PER_SECOND - strSuffix.length(),'0');
        return str+"_"+strSuffix;
    }

    std::string makeTimePrefix(){
        auto tmp = std::to_string(time(nullptr));

        if(m_strFstCmdArrived.compare(correctStrSuffix(tmp)) == 0){
            ++m_iCntHelper;
        }else m_iCntHelper = 0;

        return correctStrSuffix(tmp);
    }
};


#endif // CMDACCEPTOR_H
