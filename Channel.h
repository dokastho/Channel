#ifndef CHANNEL
#define CHANNEL

#include <queue>
#include <mutex>

template <class T>
class Channel
{
private:
    std::queue<T> items;
    std::mutex l;

public:
    T get();

    void add(T item);

    size_t size();
};
#endif
