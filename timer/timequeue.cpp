#include "timequeue.h"
#include <iostream>

void TimeQueue::up(size_t i)
{
    size_t j = (i - 1) / 2;
    while(i != 0 && heap_[i] < heap_[j])
    {
        swapNode(j, i);
        i = j;
        j = (i - 1) / 2;
    }
}

bool TimeQueue::down(size_t index, size_t n)
{
    size_t i = index;
    size_t j1 = 2 * i + 1;
    size_t j2 = 2 * i + 2;
    size_t next = j1;
    while(j1 < n){
        if(j2 >= n) next = j1;
        else next = (heap_[j1] < heap_[j2]) ? j1 : j2;
        if(heap_[i] < heap_[next]) break;
        swapNode(i, next);
        i = next;
        j1 = 2 * i + 1;
        j2 = 2 * i + 2;
    }
    return i > index;
}

void TimeQueue::swapNode(size_t a, size_t b)
{
    std::swap(heap_[a], heap_[b]);
    ref_[heap_[a].id] = a;
    ref_[heap_[b].id] = b;
}

void TimeQueue::add(int id, int timeout, TimeoutCallBack cb)
{
    size_t cur = 0;
    
    if(ref_.count(id) == 0)
    {
        cur = heap_.size();
        ref_[id] = cur;
        heap_.push_back({id, Clock::now() + MS(timeout), cb});
        up(cur);
    }
    else
    {
        cur = ref_[id];
        heap_[cur].expired = Clock::now() + MS(timeout);
        heap_[cur].cb = cb;
        if(!down(cur, heap_.size())){
            up(cur);
        }
    }
}

void TimeQueue::dowork(int id)
{
    if(ref_.count(id) == 0) return;
    TimeNode cur = heap_[ref_[id]];
    cur.cb();
    delete_(ref_[id]);
}

void TimeQueue::delete_(size_t i)
{
    size_t size = heap_.size() - 1;
    if(i < size)
    {
        swapNode(size, i);
        if(!down(i, size)){
            up(i);
        }
    }
    ref_.erase(heap_.back().id);
    heap_.pop_back();
}

void TimeQueue::adjust(int id, int timeout)
{
    heap_[ref_[id]].expired = Clock::now() + MS(timeout);
    down(ref_[id], heap_.size());
}

void TimeQueue::pop()
{
    if(!heap_.empty()) delete_(0);
}

void TimeQueue::tick()
{
    while(!heap_.empty())
    {
        if(std::chrono::duration_cast<MS>(heap_[0].expired - Clock::now()).count() > 0) break;
        heap_[0].cb();
        pop();
    }
}

size_t TimeQueue::nextTick()
{
    tick();
    size_t res = -1;
    if(!heap_.empty()){
        res = std::chrono::duration_cast<MS>(heap_[0].expired - Clock::now()).count();
        if(res < 0) res = 0;
    }
    return res;
}

void TimeQueue::clear()
{
    heap_.clear();
    ref_.clear();
}