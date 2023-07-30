#ifndef INFO_H
#define INFO_H

template <typename T>
struct info
{
    T item;
    int priority;

    info()
    {
        this->item = T();
        this->priority = 0;
    }
    info(T i, int p)
    {
        this->item = i;
        this->priority = p;
    }

    friend std::ostream& operator<<(std::ostream& outs, const info<T>& print_me)
    {
        const bool debug = true;
        outs << print_me.item;
        if (!debug) outs << " priority: " << print_me.priority << std::endl;
        return outs;
    }

    const T& get_item() const { return this->item; }

    friend bool operator<(const info<T>& lhs, const info<T>& rhs)
    {
        if (lhs.priority == rhs.priority) return lhs.item < rhs.item;
        return lhs.priority < rhs.priority;
    }

    friend bool operator>(const info<T>& lhs, const info<T>& rhs)
    {
        if (lhs.priority == rhs.priority) return lhs.item > rhs.item;
        return lhs.priority > rhs.priority;
    }
};

#endif