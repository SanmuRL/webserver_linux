#ifndef TIMEQUEUE_H
#define TIMEQUEUE_H

#include <assert.h> 
#include <unordered_map>
#include <vector>
#include <functional> 
#include <assert.h> 
#include <chrono>
#include <time.h>
#include <utility>

typedef std::function<void()> TimeoutCallBack;
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds MS;
typedef Clock::time_point TimeStamp;

struct TimeNode
{
public:
    int id;
    TimeStamp expired;
    TimeoutCallBack cb;
    bool operator<(const TimeNode& b){
        return this->expired < b.expired;
    }
};

class TimeQueue
{
public:
    TimeQueue(){heap_.reserve(64);}
    ~TimeQueue(){clear();}
    //添加新计时器节点
    void add(int id, int timeout, TimeoutCallBack cb);
    //删除堆顶定时器
    void pop();
    //调整定时器时间
    void adjust(int id, int timeout);
    //上虑操作
    void up(size_t i);
    //下虑操作
    bool down(size_t i, size_t n);
    //定时器堆清空
    void clear();
    //处理超时定时器
    void tick();
    //堆顶触发时刻
    size_t nextTick();
    //交换两个节点
    void swapNode(size_t a, size_t b);
    //删除给定id的计时器
    void dowork(int id);
    //删除定时器
    void delete_(size_t i);

public:
    std::vector<TimeNode> heap_;
    std::unordered_map<int, int> ref_;
};

#endif