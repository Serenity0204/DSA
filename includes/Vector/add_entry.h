#ifndef ADD_ENTRY_H
#define ADD_ENTRY_H

#include "array_functions.h"

// Declaration

// insert an item to an index
// double the capacity when you run out of room
template <typename T>
T* insert_entry(T* list, const T& insert_this, int insert_here, int& size, int& capacity);

// append entry to the end
template<typename T>
T* add_entry(T* list, const T& new_entry, int& size, int& capacity);

// remove the entry by index
// reallocate half the space when the size reaches 1/4 of the capacity
template <typename T>
T* erase_entry(T* list, int index, int& size, int& capacity);

// remove the first ocurred entry from list
template<typename T>
T* remove_entry(T* list, const T& delete_me, int& size, int& capacity);

// remove the last entry from list
template<typename T>
T* remove_last_entry(T* list, T& popped, int& size, int& capacity);

// select item and return pointer of index selected entry, nullptr if out of range
template<typename T>
T* select_entry(T* list, int size, int select_here);



// search for the first 'find_me' and return pointer of it, nullptr if not found
template<typename T>
T* search_entry(T* list, int size, const T& find_me);


// update item with new_update, return the pointer of index updated entry
template<typename T>
T* update_entry(T* list, int size, int update_here, const T& new_update);

// resize helper method
template<typename T>
T* resize(T* arr, int size, int& capacity, bool pushOrPop);



// Definition

template<typename T>
T* resize(T* arr, int size, int& capacity, bool pushOrPop)
{
    
    // expanding logic
    if(size == capacity && pushOrPop == true)
    {
        capacity *= 2;
        arr = reallocate(arr, size, capacity);
        return arr;
    }


    double capCheck = capacity * 0.25;
    //cout << endl << endl << capCheck << endl << endl;
    if((size - 1 <= capCheck) && pushOrPop == false)
    {
        double canShrink = capacity / 2;
        if(canShrink <= MINIMUM_CAPACITY)
        {
            capacity = MINIMUM_CAPACITY;
            arr = reallocate(arr, size, capacity);
            return arr;
        }
        
        capacity /= 2;
        arr = reallocate(arr, size, capacity);
        return arr;      
    }   
    return arr;
}

// insert an item to an index
// double the capacity when you run out of room
template <typename T>
T* insert_entry(T* list, const T& insert_this, int insert_here, int& size, int& capacity)
{
    list = resize(list, size, capacity, true);

    if(size == 0 && insert_here != 0) return list;
    if(size == 0 && insert_here == 0)
    {
        *list = insert_this;
        size++;
        return list;
    }
    
    if(insert_here == size)
    {
        *(list + size) = insert_this;
        size++;
        return list;    
    }

    int oldSize = size;
    shift_right(list, size, insert_here);
    
    // old size == new size means that the array doesn't shift right, which may indicate the insert_here index is invalid
    if(oldSize == size) return list;

    // if the array does shift right at index of insert_here, replace the old value with new value(insert_this)

    *(list + insert_here) = insert_this;
    return list;
}

// append entry to the end
template<typename T>
T* add_entry(T* list, const T& new_entry, int& size, int& capacity)
{
    int last = (size == 0) ? 0 : size;
    list = insert_entry(list, new_entry, last, size, capacity);
    return list;
}

// remove the entry by index
// reallocate half the space when the size reaches 1/4 of the capacity
template <typename T>
T* erase_entry(T* list, int index, int& size, int& capacity)
{
    list = resize(list, size, capacity, false);
    if(size == 0) return list;
    if(size == 1 && index != 0) return list;
    if(size == 1 && index == 0) 
    {
        size--;
        return list;
    }
    shift_left(list, size, index);
    return list;
}

// remove the first ocurred entry from list
template<typename T>
T* remove_entry(T* list, const T& delete_me, int& size, int& capacity)
{
    if(size == 0) return list;
    int removeIdx = search(list, size, delete_me);
    if(removeIdx == -1) return list;

    list = erase_entry(list, removeIdx, size, capacity);
    return list;
}

// remove the last entry from list
template<typename T>
T* remove_last_entry(T* list, T& popped, int& size, int& capacity)
{
    int last = (size <= 1) ? 0 : size - 1;
    popped = *(list + last);
    list = erase_entry(list, last, size ,capacity);
    return list;
}

// select item and return pointer of index selected entry, nullptr if out of range
template<typename T>
T* select_entry(T* list, int size, int select_here)
{
    if(select_here >= size || select_here < 0) return nullptr;

    // select_entry at index i returned an item(value)
    T* entry = list + select_here;
    return entry;
}



// search for the first 'find_me' and return pointer of it, nullptr if not found
template<typename T>
T* search_entry(T* list, int size, const T& find_me)
{
    int idx = search(list, size, find_me);
    if(idx == -1) return nullptr;
    T* ptr = (list + idx);
    return ptr;    
}


// update item with new_update, return the pointer of index updated entry
template<typename T>
T* update_entry(T* list, int size, int update_here, const T& new_update)
{
    T* entry = select_entry(list, size, update_here);
    if(entry == nullptr) return nullptr;

    *entry = new_update;
    return entry;
}



#endif // ADD_ENTRY_H