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
    T get();

    void add(T item);

    size_t size();
};
#endif
