#ifndef ARRAY_FUNCTIONS_H
#define ARRAY_FUNCTIONS_H

#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>

const int MINIMUM_CAPACITY = 3;

// Declaration

// allocate 'capacity' elements return array
template <typename T>
T* allocate(int capacity = MINIMUM_CAPACITY);

// take array, resize it return new array delete old array
template <typename T>
T* reallocate(T* a, int size, int capacity);

// delete dynamic array
template <typename T>
T* deallocate(T* a);

// return pointer at index pos
template <typename T>
T* at(T* a, int size, int pos);

// init array with value
template <typename T>
void init_array(T* a, int size, T init_item = T());

// print (size/capacity) for debugging
template <typename T>
void print_array(T* a, int size, int capacity = 0, std::ostream& outs = std::cout);

// print array
template <typename T>
void print(T* a, int how_many, std::ostream& outs = std::cout);

// search for the first 'find_me' and return pointer of it
// template<typename T>
// T* search_entry(T* a, int size, const T& find_me);

// search for the first 'find_me' and return index of it
template <typename T>
int search(T* a, int size, const T& find_me);

// shift left @ pos: erases @ pos
template <typename T>
void shift_left(T* a, int& size, T* shift_here);

// shift left @ pos: erases @ pos
template <typename T>
void shift_left(T* a, int& size, int shift_here);

// shift right: make a hole
template <typename T>
void shift_right(T* a, int& size, T* shift_here);

// shift right: make a hole
template <typename T>
void shift_right(T* a, int& size, int shift_here);

// copy from src to dest
template <typename T>
void copy_array(T* dest, const T* src, int many_to_copy);

// return a copy of src
template <typename T>
T* copy_array(const T* src, int size);

// Definition
// allocate 'capacity' elements return array
template <typename T>
T* allocate(int capacity)
{
    return (capacity < 3) ? new T[MINIMUM_CAPACITY] : new T[capacity];
}

// take array, resize it return new array delete old array
template <typename T>
T* reallocate(T* a, int size, int capacity)
{
    T* newArr = new T[capacity];
    copy_array(newArr, a, size);
    delete[] a;
    return newArr;
}

// init array with value
template <typename T>
void init_array(T* a, int size, T init_item)
{
    T* walker = a;
    for (int i = 0; i < size; ++i)
    {
        *walker = init_item;
        walker++;
    }
}

// print (size/capacity) for debugging
template <typename T>
void print_array(T* a, int size, int capacity, std::ostream& outs)
{
    outs << "(" << size << "/" << capacity << ")";
    print(a, size);
}

// print array
template <typename T>
void print(T* a, int how_many, std::ostream& outs)
{
    T* walker = a;
    outs << "   [";
    for (int i = 0; i < how_many; ++i)
    {
        std::cout << " " << *walker << " ";
        walker++;
    }
    outs << "]" << std::endl;
}

// delete dynamic array
template <typename T>
T* deallocate(T* a)
{
    delete[] a;
    return a;
}

// return pointer at index pos
template <typename T>
T* at(T* a, int size, int pos)
{
    if (pos >= size || pos < 0) return nullptr;
    T* atPtr = (a + pos);
    return atPtr;
}

// search for the first 'find_me' and return index of it
template <typename T>
int search(T* a, int size, const T& find_me)
{
    T* walker = a;
    for (int i = 0; i < size; ++i)
    {
        T val = *walker;
        if (val == find_me) return i;
        walker++;
    }
    return -1;
}

// shift left @ pos: erases @ pos
template <typename T>
void shift_left(T* a, int& size, T* shift_here)
{
    if (shift_here == nullptr) return;
    if (size == 0) return;

    int idx = shift_here - a;
    shift_left(a, size, idx);
}

// shift left @ pos: erases @ pos
template <typename T>
void shift_left(T* a, int& size, int shift_here)
{
    if (size == 0) return;
    if (shift_here >= size || shift_here < 0) return;
    T* walker = a + shift_here;
    T* shifter = walker + 1;
    for (int i = shift_here; i < size - 1; ++i)
    {
        *walker = *shifter;
        walker++;
        shifter++;
    }
    size--;
}

// shift right: make a hole
template <typename T>
void shift_right(T* a, int& size, T* shift_here)
{
    if (shift_here == nullptr) return;
    int idx = shift_here - a;
    shift_right(a, size, idx);
}

// shift right: make a hole
template <typename T>
void shift_right(T* a, int& size, int shift_here)
{
    if (shift_here >= size || shift_here < 0) return;
    T* walker = (a + size);
    T* shifter = (walker - 1);
    for (int i = size; i > shift_here; --i)
    {
        *walker = *shifter;
        walker--;
        shifter--;
    }
    size++;
}

// copy from src to dest
template <typename T>
void copy_array(T* dest, const T* src, int many_to_copy)
{
    const T* walkerSrc = src;
    T* walkerDest = dest;
    for (int i = 0; i < many_to_copy; ++i)
    {
        *walkerDest = *walkerSrc;
        walkerDest++;
        walkerSrc++;
    }
}

// return a copy of src
template <typename T>
T* copy_array(const T* src, int size)
{
    T* dest = new T[size];
    copy_array(dest, src, size);
    return dest;
}

#endif // ARRAY_FUNCTIONS_H