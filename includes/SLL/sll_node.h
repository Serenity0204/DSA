#ifndef SLL_NODE_H
#define SLL_NODE_H

#include <iostream>
namespace SLLUtils
{
    template <typename T>
    struct node
    {
    public:
        node(const T& item = T(), node* next = nullptr);

        template <typename U>
        friend std::ostream& operator<<(std::ostream& outs, const node<U>& printMe);
        T _item;
        node* _next;
    };

    // Definition

    // TODO
    template <typename T>
    node<T>::node(const T& item, node<T>* next)
    {
        this->_item = item;
        this->_next = next;
    }

    template <typename U>
    std::ostream& operator<<(std::ostream& outs, const node<U>& printMe)
    {
        outs << "[" << printMe._item << "]";
        return outs;
    }
};

#endif // SLL_NODE_H