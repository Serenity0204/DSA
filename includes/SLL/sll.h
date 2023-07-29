#ifndef SLL_H
#define SLL_H

#include "sll_functions.h"
#include <cassert>
#include <iostream>

namespace DSA
{
    template <typename T>
    class SLL
    {
    public:
        class Iterator
        {
        public:
            // give access to list to access _ptr
            friend class SLL;

            // default ctor
            Iterator()
            {
                this->_ptr = nullptr;
            }
            // Point Iterator to where p is pointing to
            Iterator(SLLUtils::node<T>* p)
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
                return left._ptr == left._ptr;
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
            SLLUtils::node<T>* _ptr; // pointer being encapsulated
        };

        // constructor: CTOR
        SLL();

        // BIG 3:
        SLL(const SLL<T>& copyThis);
        ~SLL();
        SLL& operator=(const SLL& RHS);

        // Operations:
        Iterator insert_head(T i);                     // insert at the head of list
        Iterator insert_after(T i, Iterator iMarker);  // insert after iMarker
        Iterator insert_before(T i, Iterator iMarker); // insert before iMarker
        T Delete(SLL<T>::Iterator iMarker);            // delete node at iMarker
        Iterator search(const T& key) const;           // return Iterator to node [key], nullptr if not there

        // Accessors:
        const T& operator[](int index) const; // const version of the operator []
        T& operator[](int index);             // return item at index
        Iterator prev(Iterator iMarker);      // previous node: iMarker
        Iterator begin() const;               // Iterator to head node
        Iterator end() const;                 // Iterator to nullptr
        Iterator last_node() const;           // Iterator to last node

        // Checkers:
        int size() const { return _size; }
        bool empty() const { return _head == nullptr; }

        void Print() const;          // print the list
        void PrintBackwards() const; // print the list backwards

        template <typename U> // insertion operator, Note template arg U
        friend std::ostream& operator<<(std::ostream& outs, const SLL<U>& l);

    private:
        SLLUtils::node<T>* _head;
        int _size;
    };

    // Definition of List Class
    // TODO

    template <typename T>
    SLL<T>::SLL()
    {
        this->_size = 0;
        this->_head = nullptr;
    }

    template <typename T>
    SLL<T>::SLL(const SLL<T>& copyThis)
    {
        this->_head = SLLUtils::_copy_list<T>(copyThis._head);
        this->_size = copyThis._size;
    }

    template <typename T>
    SLL<T>::~SLL()
    {
        SLLUtils::_clear_list<T>(this->_head);
        this->_size = 0;
    }

    template <typename T>
    SLL<T>& SLL<T>::operator=(const SLL<T>& RHS)
    {
        if (this == &RHS) return *this;
        SLLUtils::_clear_list<T>(this->_head);
        this->_head = SLLUtils::_copy_list<T>(RHS._head);
        this->_size = RHS._size;
        return *this;
    }

    template <typename T>
    typename SLL<T>::Iterator SLL<T>::insert_head(T i)
    {
        this->_size++;
        Iterator it(SLLUtils::_insert_head<T>(this->_head, i));
        return it;
    }

    template <typename T>
    typename SLL<T>::Iterator SLL<T>::insert_after(T i, typename SLL<T>::Iterator iMarker)
    {
        this->_size++;
        Iterator it(SLLUtils::_insert_after<T>(this->_head, iMarker._ptr, i));
        return it;
    }

    template <typename T>
    typename SLL<T>::Iterator SLL<T>::insert_before(T i, typename SLL<T>::Iterator iMarker)
    {
        this->_size++;
        Iterator it(SLLUtils::_insert_before<T>(this->_head, iMarker._ptr, i));
        return it;
    }

    template <typename T>
    T SLL<T>::Delete(typename SLL<T>::Iterator iMarker)
    {
        if (this->_size > 0) this->_size--;
        return SLLUtils::_delete_node<T>(this->_head, iMarker._ptr);
    }

    template <typename T>
    typename SLL<T>::Iterator SLL<T>::search(const T& key) const
    {
        Iterator it(SLLUtils::_search_list<T>(this->_head, key));
        return it;
    }

    template <typename T>
    const T& SLL<T>::operator[](int index) const
    {
        return SLLUtils::_at<T>(this->_head, index);
    }

    template <typename T>
    T& SLL<T>::operator[](int index)
    {
        return SLLUtils::_at<T>(this->_head, index);
    }

    template <typename T>
    typename SLL<T>::Iterator SLL<T>::prev(typename SLL<T>::Iterator iMarker)
    {
        Iterator it(SLLUtils::_previous_node<T>(this->_head, iMarker._ptr));
        return it;
    }

    template <typename T>
    typename SLL<T>::Iterator SLL<T>::begin() const
    {
        Iterator it(this->_head);
        return it;
    }

    template <typename T>
    typename SLL<T>::Iterator SLL<T>::end() const
    {
        Iterator it(_last_node(this->_head)->_next);
        return it;
    }

    template <typename T>
    typename SLL<T>::Iterator SLL<T>::last_node() const
    {
        Iterator it(_last_node(this->_head));
        return it;
    }

    template <typename T>
    void SLL<T>::Print() const
    {
        SLLUtils::_print_list<T>(this->_head);
    }

    template <typename T>
    void SLL<T>::PrintBackwards() const
    {
        SLLUtils::_print_list_backwards<T>(this->_head);
    }

    template <typename U>
    std::ostream& operator<<(std::ostream& outs, const SLL<U>& l)
    {
        typename SLL<U>::Iterator it;
        for (it = l.begin(); it != l.end(); it++)
        {
            outs << "[" << *it << "]->";
        }
        outs << "|||" << std::endl;
        return outs;
    }
};

#endif // LIST_ITERATED_H