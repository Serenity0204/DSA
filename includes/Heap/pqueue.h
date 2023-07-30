#ifndef PQUEUE_H
#define PQUEUE_H

#include "heap.h"
#include "info.h"
#include <cassert>  // Provides assert
#include <iomanip>  // Provides std::setw
#include <iostream> // Provides std::cout
#include <vector>

namespace DSA
{
    template <typename T>
    class PriorityQueue
    {
    public:
        // CONSTRUCTOR
        PriorityQueue();
        // MODIFICATION MEMBER FUNCTIONS
        void push(const T& value, int p);
        void pop();
        T top();
        // CONST MEMBER FUNCTIONS
        bool empty() const;
        int size() const;
        void print_tree() const;
        bool verify() const;
        // OVERLOAD OPERATOR FUNCTIONS
        friend std::ostream& operator<<(std::ostream& outs, const PriorityQueue<T>& print_me)
        {
            outs << print_me._heap;
            return outs;
        }

    private:
        Heap<info<T>> _heap;
    };

    template <typename T>
    PriorityQueue<T>::PriorityQueue()
    {
        this->_heap = Heap<info<T>>();
    }

    template <typename T>
    void PriorityQueue<T>::push(const T& value, int p)
    {
        this->_heap.insert(info<T>(value, p));
    }

    template <typename T>
    void PriorityQueue<T>::pop()
    {
        this->_heap.remove();
    }

    template <typename T>
    T PriorityQueue<T>::top()
    {
        return this->_heap.top().get_item();
    }

    // CONST MEMBER FUNCTIONS
    template <typename T>
    bool PriorityQueue<T>::empty() const
    {
        return this->_heap.is_empty();
    }

    template <typename T>
    int PriorityQueue<T>::size() const
    {
        return this->_heap.size();
    }

    template <typename T>
    void PriorityQueue<T>::print_tree() const
    {
        std::cout << this->_heap << std::endl;
    }

    template <typename T>
    bool PriorityQueue<T>::verify() const
    {
        return this->_heap.verify();
    }
};

#endif