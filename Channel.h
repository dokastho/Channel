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
    bool alive;

public:
    Channel()
    {
        alive = true;
    }

    ~Channel()
    {
        alive = false;
        cv.notify_all();
    }

    Channel &operator=(const Channel &other)
    {
        if (this == &other)
        {
            return *this;
        }

        this->items = other.items;
        return *this;
    }

    T get()
    {
        std::unique_lock<std::mutex> l(__l);
        while (items.empty())
        {
            cv.wait(l);
            if (!alive)
            {
                #ifndef CHANNEL_EXIT
                throw std::runtime_error("non-empty channel terminated.")
                #endif
                std::exit(0);
            }
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

template <class T>
class priority_channel
{
    struct priority_item
    {
        uint8_t rank;
        T datum;
    };

    struct priority_comp
    {
        bool operator()(priority_item &lhs, priority_item &rhs)
        {
            return lhs.rank < rhs.rank;
        }
    };

    std::priority_queue<priority_item, std::vector<priority_item>, priority_comp> items;
    std::mutex __l;
    std::condition_variable cv;
    bool alive;

public:
    priority_channel()
    {
        alive = true;
    }

    ~priority_channel()
    {
        alive = false;
        cv.notify_all();
    }

    priority_channel &operator=(const priority_channel &other)
    {
        if (this == &other)
        {
            return *this;
        }

        this->items = other.items;
        return *this;
    }

    T get()
    {
        std::unique_lock<std::mutex> l(__l);
        while (items.empty())
        {
            cv.wait(l);
            if (!alive)
            {
                #ifndef CHANNEL_EXIT
                throw std::runtime_error("non-empty channel terminated.")
                #endif
                std::exit(0);
            }
        }

        priority_item item = items.top();
        items.pop();
        l.unlock();
        return item.datum;
    }

    void add(T item, uint8_t priority)
    {
        std::unique_lock<std::mutex> l(__l);
        priority_item p{priority, item};
        items.push(p);
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
