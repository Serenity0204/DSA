#ifndef QUEUE_H
#define QUEUE_H

#include "dll_functions.h"
#include <iomanip>
#include <iostream>

namespace DSA
{
    template <typename T>
    class Queue
    {
    public:
        class Iterator
        {
        public:
            // Give access to list to access _ptr
            friend class Queue;

            // Default CTOR
            Iterator()
            {
                this->_ptr = nullptr;
            }
            // Point Iterator to where p is pointing to
            Iterator(DLLUtils::node<T>* p)
            {
                this->_ptr = p;
            }

            // Casting operator: true if _ptr not NULL
            // This turned out to be a pain!
            operator bool()
            {
                return (this->_ptr == nullptr) ? false : true;
            }

            // dereference operator
            T& operator*()
            {
                assert(this->_ptr != nullptr);
                return this->_ptr->_item;
            }
            // member access operator
            T* operator->()
            {
                return this->_ptr;
            }
            // true if _ptr is NULL
            bool is_null()
            {
                return (this->_ptr == nullptr) ? true : false;
            }

            // true if left != right
            friend bool operator!=(const Iterator& left, const Iterator& right)
            {
                return left._ptr != right._ptr;
            }
            // true if left == right
            friend bool operator==(const Iterator& left, const Iterator& right)
            {
                return left._ptr == right._ptr;
            }

            // member operator:  ++it; or ++it = new_value
            Iterator& operator++()
            {
                this->_ptr = this->_ptr->_next;
                return *this;
            }
            // friend operator: it++
            friend Iterator operator++(Iterator& it, int unused)
            {
                Iterator hold = it;
                it._ptr = it._ptr->_next;
                return hold;
            }

        private:
            DLLUtils::node<T>* _ptr; // pointer being encapsulated
        };

        // constructor: CTOR
        Queue();

        // BIG 3:
        Queue(const Queue<T>& copyMe);
        ~Queue();
        Queue<T>& operator=(const Queue<T>& RHS);

        // Operations:
        void push(T item); // Enqueue
        T pop();           // Dequeue

        // Accessors:
        Iterator begin() const; // Iterator to the head node
        Iterator end() const;   // Iterator to NULL

        // Checkers:
        int size() const;
        bool empty();
        bool empty() const
        {
            if (this->_rear == nullptr || this->_front == nullptr) return true;
            return false;
        }
        T front();
        T back();

        template <typename U>
        friend std::ostream& operator<<(std::ostream& outs, const Queue<U>& printMe);

    private:
        DLLUtils::node<T>* _front;
        DLLUtils::node<T>* _rear;
        int _size;
    };

    // Definition

    // TODO

    template <typename T>
    Queue<T>::Queue()
    {
        this->_front = nullptr;
        this->_rear = nullptr;
        this->_size = 0;
    }

    template <typename T>
    Queue<T>::Queue(const Queue<T>& copyMe)
    {
        this->_front = nullptr;
        this->_rear = nullptr;
        this->_rear = DLLUtils::_copy_list<T>(this->_front, copyMe._front);
        this->_size = copyMe.size();
    }

    template <typename T>
    Queue<T>::~Queue()
    {
        DLLUtils::_clear_list<T>(this->_front);
        this->_front = nullptr;
        this->_rear = nullptr;
        this->_size = 0;
    }

    template <typename T>
    Queue<T>& Queue<T>::operator=(const Queue<T>& RHS)
    {
        if (this == &RHS) return *this;
        DLLUtils::_clear_list<T>(this->_front);
        this->_front = nullptr;
        this->_rear = nullptr;
        this->_rear = DLLUtils::_copy_list<T>(this->_front, RHS._front);
        this->_size = RHS._size;
        return *this;
    }

    template <typename T>
    void Queue<T>::push(T item)
    {
        this->_size++;
        if (this->empty())
        {
            this->_front = _insert_head(this->_front, item);
            this->_rear = this->_front;
            return;
        }
        this->_rear = DLLUtils::_insert_after<T>(this->_front, DLLUtils::_last_node<T>(this->_front), item);
    }

    template <typename T>
    T Queue<T>::pop()
    {
        if (this->_size > 0) this->_size--;
        return DLLUtils::_delete_node<T>(this->_front, this->_front);
    }

    template <typename T>
    typename Queue<T>::Iterator Queue<T>::begin() const
    {
        Iterator it(this->_front);
        return it;
    }

    template <typename T>
    typename Queue<T>::Iterator Queue<T>::end() const
    {
        Iterator it(_last_node(this->_front)->_next);
        return it;
    }

    template <typename T>
    int Queue<T>::size() const
    {
        return this->_size;
    }

    template <typename T>
    bool Queue<T>::empty()
    {
        if (this->_front == nullptr || this->_rear == nullptr) return true;
        return false;
    }

    template <typename T>
    T Queue<T>::front()
    {
        Iterator it(this->_front);
        return *it;
    }

    template <typename T>
    T Queue<T>::back()
    {
        Iterator it(this->_rear);
        return *it;
    }

    template <typename U>
    std::ostream& operator<<(std::ostream& outs, const Queue<U>& printMe)
    {
        typename Queue<U>::Iterator it;
        if (printMe.empty())
        {
            outs << "|||";
            return outs;
        }

        for (it = printMe.begin(); it != printMe.end(); it++)
        {
            if (it) outs << "[" << *it << "]->";
        }
        outs << "|||" << std::endl;
        return outs;
    }
};

#endif // QUEUE_H