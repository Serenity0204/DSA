#ifndef HEAP_H
#define HEAP_H

#include "helper.h"
#include <cassert>  // Provides assert
#include <iomanip>  // Provides std::setw
#include <iostream> // Provides std::cout

namespace DSA
{
    // class
    // ======================================================================================================
    template <class Item>
    class Heap
    {
    private:
        static const int CAPACITY = 1000;
        int _size;   // the space used
        Item* _heap; // dynamic array
        int _cap;
        void print_tree(std::ostream& outs = std::cout) const;
        void print_tree(int root, int level = 0, std::ostream& outs = std::cout) const;

        bool verify(int root) const;

        bool is_root(int i) const { return (i == 0) ? true : false; }
        bool is_leaf(int i) const { return (this->left_child_index(i) >= this->size()) ? true : false; }
        int parent_index(int i) const { return (i == 0) ? -1 : ((i - 1) / 2); }
        int left_child_index(int i) const { return (2 * i + 1); }
        int right_child_index(int i) const { return (2 * i + 2); }
        int first_nonleaf() const { return this->size() / 2 - 1; }
        int big_child_index(int i) const;
        void swap_with_parent(int i);

    public:
        // TYPEDEF
        typedef Item value_type;
        // CONSTRUCTOR
        Heap(const int& cap = CAPACITY);
        Heap(Item* arr, int size, const int& cap = CAPACITY);
        // BIG 3
        Heap(const Heap& x);            // Constructs a container with a copy of each of the elements in x, in the same order.
        ~Heap();                        // Destroys the container object.
        Heap& operator=(const Heap& x); // Assigns new contents to the container, replacing its current contents, and modifying its size accordingly.
        // MODIFICATION MEMBER FUNCTIONS
        void heapify(int i);                     // heapify the new node following a Bottom-up approach
        void heapify(Item*& arr, int n, int i);  // heapify the new node following a Bottom-up approach
        void insert(const Item& insert_me);      // insert into the heap
        void remove();                           // remove top value
        void remove_node(const Item& delete_me); // remove a node from the heap
        Item top();                              // return the value at the top of heap

        // CONST MEMBER FUNCTIONS
        bool is_empty() const { return (this->size() == 0) ? true : false; }
        int size() const { return this->_size; }
        int capacity() const { return this->_cap; }

        bool verify() const; // true if tree is verified to be a heap
        Item* heap_array();  // returns a copy of underlying array:
                             //   don't forget to delete when done
        Item* heap_sort();   // returns a copy of sorted array
        // OVERLOAD OPERATOR FUNCTIONS
        friend std::ostream& operator<<(std::ostream& outs, const Heap<Item>& print_me)
        {
            outs << "============================================================================================================" << std::endl;
            outs << "Heap:" << std::endl;
            print_me.print_tree();
            outs << "============================================================================================================" << std::endl;
            outs << std::endl;
            return outs;
        }
    };
    // ======================================================================================================

    // BIG 3 and constructors
    // ======================================================================================================
    template <class Item>
    const int Heap<Item>::CAPACITY;

    template <class Item>
    Heap<Item>::Heap(const int& cap)
    {
        this->_heap = new Item[cap];
        this->_size = 0;
        this->_cap = cap;
    }
    template <class Item>
    Heap<Item>::Heap(Item* arr, int size, const int& cap)
    {
        this->_size = size;
        this->_heap = HeapUtils::copy_arr<Item>(arr, size, cap);
        this->_cap = cap;
        for (int i = size / 2 - 1; i >= 0; --i) this->heapify(i);
    }

    template <class Item>
    Heap<Item>::Heap(const Heap<Item>& x)
    {
        this->_cap = x._cap;
        this->_size = 0;
        this->_size = x.size();
        this->_heap = HeapUtils::copy_arr<Item>(x._heap, x.size(), x._cap);
        for (int i = this->first_nonleaf(); i >= 0; --i) this->heapify(i);
    }
    template <class Item>
    Heap<Item>::~Heap()
    {
        delete[] this->_heap;
    }
    template <class Item>
    Heap<Item>& Heap<Item>::operator=(const Heap<Item>& x)
    {
        if (this == &x) return *this;
        delete[] this->_heap;
        this->_size = x.size();
        this->_cap = x._cap;
        this->_heap = HeapUtils::copy_arr<Item>(x._heap, x.size(), x._cap);
        for (int i = this->first_nonleaf(); i >= 0; --i) this->heapify(i);
        return *this;
    }
    // ======================================================================================================

    // trivial
    // ======================================================================================================
    template <class Item>
    Item Heap<Item>::top()
    {
        assert(!this->is_empty());
        return this->_heap[0];
    }

    template <class Item>
    Item* Heap<Item>::heap_array()
    {
        Item* heap_arr = HeapUtils::copy_arr<Item>(this->_heap, this->size());
        return heap_arr;
    }

    template <class Item>
    int Heap<Item>::big_child_index(int i) const
    {
        assert(!this->is_leaf(i));
        int left_idx = this->left_child_index(i);
        int right_idx = this->right_child_index(i);
        if (right_idx >= this->size()) return left_idx;

        if (this->_heap[left_idx] > this->_heap[right_idx]) return left_idx;
        return right_idx;
    }

    template <class Item>
    void Heap<Item>::swap_with_parent(int i)
    {
        assert(!this->is_root(i) && i >= 0 && i < this->size());
        Item temp = this->_heap[i];
        int parent_idx = this->parent_index(i);
        this->_heap[i] = this->_heap[parent_idx];
        this->_heap[parent_idx] = temp;
    }

    template <class Item>
    Item* Heap<Item>::heap_sort()
    {
        Item* heap_arr = this->heap_array();
        for (int i = this->size() - 1; i >= 0; --i)
        {
            std::swap(heap_arr[0], heap_arr[i]);
            this->heapify(heap_arr, i, 0);
        }
        return heap_arr;
    }
    // ======================================================================================================

    // heapify
    // ======================================================================================================
    template <class Item>
    void Heap<Item>::heapify(int i)
    {
        this->heapify(this->_heap, this->size(), i);
    }

    template <class Item>
    void Heap<Item>::heapify(Item*& arr, int n, int i)
    {
        int size = n;
        int largest = i;
        int l = this->left_child_index(i);
        int r = this->right_child_index(i);
        if (l < size && arr[l] > arr[largest]) largest = l;
        if (r < size && arr[r] > arr[largest]) largest = r;
        if (largest != i)
        {
            std::swap(arr[i], arr[largest]);
            heapify(arr, n, largest);
        }
    }
    // ======================================================================================================

    // insert remove
    // ======================================================================================================
    template <class Item>
    void Heap<Item>::insert(const Item& insert_me)
    {
        if (this->size() >= this->_cap) return;
        this->_heap[this->size()] = insert_me;
        this->_size++;
        // this->bubble_up(); // or use a for loop to call heapify like this
        for (int i = this->first_nonleaf(); i >= 0; --i) this->heapify(i);
    }

    template <class Item>
    void Heap<Item>::remove()
    {
        if (this->is_empty()) return;
        Item max = this->_heap[0];
        Item end = this->_heap[this->size() - 1];
        if (this->size() == 1)
        {
            this->_size--;
            return;
        }
        this->_heap[0] = end;
        this->_size--;
        // this->sink_down(); // or use a for loop to call heapify
        // if(!this->verify()) this->bubble_up();
        for (int i = this->first_nonleaf(); i >= 0; --i) this->heapify(i);
    }

    template <class Item>
    void Heap<Item>::remove_node(const Item& delete_me)
    {
        int index = -1;
        for (int i = 0; i < this->size(); ++i)
        {
            if (this->_heap[i] == delete_me)
            {
                index = i;
                break;
            }
        }
        if (index == -1) return;
        HeapUtils::shift_left(this->_heap, this->_size, index);
        for (int i = this->first_nonleaf(); i >= 0; --i) this->heapify(i);
    }

    // ======================================================================================================

    // print
    // ======================================================================================================
    template <class Item>
    void Heap<Item>::print_tree(std::ostream& outs) const
    {
        this->print_tree(0, 0, outs);
    }

    template <class Item>
    void Heap<Item>::print_tree(int root, int level, std::ostream& outs) const
    {
        if (this->is_empty())
        {
            outs << "empty" << std::endl;
            return;
        }
        if (root >= this->size()) return;
        print_tree(this->right_child_index(root), level + 1, outs);
        for (int i = 0; i < level; ++i) std::cout << "    ";
        std::cout << this->_heap[root] << std::endl;
        print_tree(this->left_child_index(root), level + 1, outs);
    }
    // ======================================================================================================

    // verify
    // ======================================================================================================
    template <class Item>
    bool Heap<Item>::verify() const
    {
        for (int i = 0; i < this->size(); ++i)
            if (!verify(i)) return false;
        return true;
    }

    // single index
    template <class Item>
    bool Heap<Item>::verify(int root) const
    {
        if (this->is_leaf(root)) return true;
        int left_idx = this->left_child_index(root);
        int right_idx = this->right_child_index(root);
        if (right_idx == this->size()) return (this->_heap[root] >= this->_heap[left_idx]);

        bool check = (this->_heap[root] >= this->_heap[left_idx]) && (this->_heap[root] >= this->_heap[right_idx]);
        return check;
    }
    // ======================================================================================================
};

#endif // HEAP_H