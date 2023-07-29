#ifndef VECTOR_H
#define VECTOR_H

#include "add_entry.h"
#include "array_functions.h"
#include <iostream>

namespace DSA
{
    template <typename T>
    class Vector
    {
    public:
        // Constructor: CTOR
        Vector(int capacity = 100);
        Vector(T* arr, int size);

        // BIG 3
        Vector(const Vector& x);            // Constructs a container with a copy of each of the elements in x, in the same order.
        ~Vector();                          // Destroys the container object.
        Vector& operator=(const Vector& x); // Assigns new contents to the container, replacing its current contents, and modifying its size accordingly.

        // Element access:
        T& operator[](int n); // Returns a reference to the element at position n in the vector container.
        const T& operator[](int n) const;
        T& at(int n); // Returns a reference to the element at position n in the vector container.
        const T& at(int n) const;
        T& front(); // Returns a reference to the first element in the vector.
        T& back();  // Returns a reference to the last element in the vector.

        // Modifiers:
        void push_back(const T& val);            // Adds a new element at the end of the vector
        void pop_back();                         // Removes the last element in the vector, effectively reducing the container size by one.
        void insert(int position, const T& val); // Insert elements val at position
        void erase(int position);                // Removes from the vector a single element at position
        int index_of(const T& val);              // Search for val. return index.

        // Capacity:
        int size() const { return _size; }         // return _size
        int capacity() const { return _capacity; } // return _capacity

        void resize(int n, T val = T()); // Resizes the container so that it contains n elements.
        // If n is smaller than the current container size, the content is reduced to its first n elements, removing those beyond (and destroying them).
        // If n is greater than the current container size, the content is expanded by inserting at the end as many elements as needed to reach a size of n. If val is specified, the new elements are initialized as copies of val, otherwise, they are value-initialized.
        // If n is also greater than the current container capacity, an automatic reallocation of the allocated storage space takes place.

        void reserve(int n); // Requests that the vector capacity be at least enough to contain n elements.
        // If n is greater than the current vector capacity, the function causes the container to reallocate its storage increasing its capacity to n
        // In all other cases, the function call does not cause a reallocation and the vector capacity is not affected.
        // This function has no effect on the vector size and cannot alter its elements.

        bool empty() const; // Returns whether the vector is empty

        // Output:
        template <class U>
        friend std::ostream& operator<<(std::ostream& outs, const Vector<U>& _a);

    private:
        int _capacity; // the space allocated
        int _size;     // the space used
        T* _arr;       // pointer of dynamic array
    };

    // implementation

    template <typename T>
    Vector<T>::Vector(int capacity)
    {
        if (capacity < 0) capacity = 100;
        this->_capacity = capacity;
        this->_size = 0;
        this->_arr = allocate<T>(capacity);
    }

    template <typename T>
    Vector<T>::Vector(T* arr, int size)
    {
        this->_arr = allocate<T>(size);
        copy_array<T>(this->_arr, arr, size);
        this->_size = size;
        this->_capacity = size;
    }

    template <typename T>
    Vector<T>::Vector(const Vector<T>& x)
    {
        this->_arr = allocate<T>(x.capacity());
        this->_size = x.size();
        this->_capacity = x.capacity();
        copy_array<T>(this->_arr, x._arr, x.size());
    }

    template <typename T>
    Vector<T>::~Vector()
    {
        this->_arr = deallocate<T>(this->_arr);
        this->_capacity = 0;
        this->_size = 0;
    }

    template <typename T>
    Vector<T>& Vector<T>::operator=(const Vector<T>& x)
    {
        if (this == &x) return *this;
        this->_size = x.size();
        this->_capacity = x.capacity();
        this->_arr = deallocate<T>(this->_arr);
        this->_arr = allocate<T>(this->capacity());
        copy_array<T>(this->_arr, x._arr, this->size());
        return *this;
    }

    template <typename T>
    T& Vector<T>::operator[](int n)
    {
        return this->at(n);
    }

    template <typename T>
    const T& Vector<T>::operator[](int n) const
    {
        return this->at(n);
    }

    template <typename T>
    T& Vector<T>::at(int n)
    {
        return *select_entry(this->_arr, this->size(), n);
    }

    template <typename T>
    const T& Vector<T>::at(int n) const
    {
        return this->at(n);
    }

    template <typename T>
    T& Vector<T>::front()
    {
        return this->at(0);
    }

    template <typename T>
    T& Vector<T>::back()
    {
        return this->at(this->size() - 1);
    }

    template <typename T>
    void Vector<T>::push_back(const T& val)
    {
        this->_arr = add_entry<T>(this->_arr, val, this->_size, this->_capacity);
    }

    template <typename T>
    void Vector<T>::pop_back()
    {
        T val = 0;
        this->_arr = remove_last_entry<T>(this->_arr, val, this->_size, this->_capacity);
    }

    template <typename T>
    void Vector<T>::insert(int position, const T& val)
    {
        this->_arr = insert_entry<T>(this->_arr, val, position, this->_size, this->_capacity);
    }

    template <typename T>
    void Vector<T>::erase(int position)
    {
        this->_arr = erase_entry<T>(this->_arr, position, this->_size, this->_capacity);
    }

    template <typename T>
    int Vector<T>::index_of(const T& val)
    {
        int idx = search(this->_arr, this->size(), val);
        return idx;
    }

    template <typename T>
    void Vector<T>::resize(int n, T val)
    {
        if (n == this->size()) return;
        if (n > this->capacity())
        {
            this->_arr = reallocate<T>(this->_arr, this->size(), n);
            this->_capacity = n;
        }

        int old_size = this->size();
        int diff = abs(n - old_size);

        if (n > old_size)
            for (int i = 0; i < diff; ++i) this->push_back(val);
        if (n < old_size)
            for (int i = 0; i < diff; ++i) this->pop_back();
    }

    template <typename T>
    void Vector<T>::reserve(int n)
    {
        if (n <= this->capacity()) return;
        this->_arr = reallocate<T>(this->_arr, this->size(), n);
        this->_capacity = n;
    }

    template <typename T>
    bool Vector<T>::empty() const
    {
        return (this->_size == 0) ? true : false;
    }

    template <class U>
    std::ostream& operator<<(std::ostream& outs, const Vector<U>& _a)
    {
        print_array(_a._arr, _a.size(), _a.capacity(), outs);
        return outs;
    }
};

#endif // VECTOR_H
