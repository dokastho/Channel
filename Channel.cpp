#include "Channel.h"

template <typename T>
size_t Channel<T>::size()
{
    l.lock();
    size_t s = items.size();
    l.unlock();
    return s;
}

template <typename T>
void Channel<T>::add(T item)
{
    l.lock();
    items.push(item);
    l.unlock();
}

template <typename T>
T Channel<T>::get()
{
    l.lock();
    T item = items.front();
    items.pop();
    l.unlock();
    return item;
}
