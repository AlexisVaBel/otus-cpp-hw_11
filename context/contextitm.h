#pragma once

#include <cstddef>
#include <mutex>
#include <queue>

#include <iostream>

class ContextItm {

public:
  ContextItm(std::size_t bulkSize):m_szBulk(bulkSize) {

  }

  ~ContextItm() {

  }

  void process(const char* data, std::size_t size) {
      std::unique_lock<std::mutex> mlock(m_mutex);

      size_t szTaskCnt = 0;
      size_t leftPos = 0, rightPos = 0;
      std::string str;
      while(rightPos < size){
          if(data[rightPos] == '\n'){
              str = std::string(data, leftPos, rightPos - leftPos);
              m_queuTasks.push(str);
              ++rightPos;
              leftPos = rightPos;
              ++szTaskCnt;
          }else
              ++rightPos;
      }
      if(leftPos != rightPos){
          str = std::string(data, leftPos,  rightPos - leftPos);
          ++szTaskCnt;
          m_queuTasks.push(str);
      }
      m_queuPackInOrder.push(szTaskCnt);
  }

  size_t   get_empty(){
      std::unique_lock<std::mutex> mlock(m_mutex);
      return m_queuTasks.size();
  }

  std::string get_cmd(){
      std::unique_lock<std::mutex> mlock(m_mutex);
      auto t = m_queuTasks.front();
      m_queuTasks.pop();
      return t;
  }

  size_t get_order(){
      std::unique_lock<std::mutex> mlock(m_mutex);
      auto t = m_queuPackInOrder.front();
      m_queuPackInOrder.pop();
      return t;
  }


  size_t    get_sizeBulk() {return m_szBulk;}

private:
  std::mutex m_mutex;
  size_t     m_szBulk;

  std::queue<std::string> m_queuTasks;
  std::queue<size_t>      m_queuPackInOrder; // keeps how many task should be processed
};
