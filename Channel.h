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
    std::mutex __l;
    std::condition_variable cv;

public:
    T get()
    {
        std::unique_lock<std::mutex> l(__l);
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
        std::unique_lock<std::mutex> l(__l);
        items.push(item);
        cv.notify_one();
        l.unlock();
    }

    size_t size()
    {
        std::unique_lock<std::mutex> l(__l);
        size_t s = items.size();
        l.unlock();
        return s;
    }

    bool empty()
    {
        std::unique_lock<std::mutex> l(__l);
        bool b = items.empty();
        l.unlock();
        return b;
    }
};
#endif
