#ifndef CHANNEL
#define CHANNEL

#include <queue>
#include <mutex>
#include <condition_variable>

template <class T>
class Channel
{
private:
    std::queue<T> items;
    std::unique_lock<std::mutex> l;
    std::condition_variable cv;

public:
    T get()
    {
        l.lock();
        while (items.empty())
        {
            cv.wait(l);
        }

        T item = items.front();
        items.pop();
        l.unlock();
        return item;
    }

    void add(T item)
    {
        l.lock();
        items.push(item);
        cv.notify_one();
        l.unlock();
    }

    size_t size()
    {
        l.lock();
        size_t s = items.size();
        l.unlock();
        return s;
    }
};
#endif
