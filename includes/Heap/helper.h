#ifndef HELPER_H
#define HELPER_H

#include <cassert>  // Provides assert
#include <iomanip>  // Provides std::setw
#include <iostream> // Provides std::cout

namespace HeapUtils
{
    const int CAPACITY = 1000;

    template <class Item>
    Item* copy_arr(Item* src, int size, const int& cap = CAPACITY)
    {
        Item* arr = new Item[cap];
        for (int i = 0; i < size; ++i) arr[i] = src[i];
        return arr;
    }

    template <class Item>
    void reverse_arr(Item* arr, int size)
    {
        int left = 0, right = size - 1;
        while (left < right)
        {
            std::swap(arr[left], arr[right]);
            right--;
            left++;
        }
    }

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

};

#endif // HELPER_H