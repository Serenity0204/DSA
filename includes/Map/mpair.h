#ifndef MPAIR_H
#define MPAIR_H

#include <cstdlib> // Provides std::size_t
#include <iostream>
#include <vector> // Provides std::vector

namespace MapUtils
{
    template <typename K, typename V>
    struct MPair
    {
        K key;
        std::vector<V> value_list;

        //--------------------------------------------------------------------------------
        /*      MPair CTORs:
         *  must have these CTORs:
         *  - default CTOR / CTOR with a key and no value: this will create an empty vector
         *  - CTOR with a key AND a value: pushes the value into the value_list
         *  - CTOR with a key and a vector of values (to replace value_list)
         */
        //--------------------------------------------------------------------------------
        MPair(const K& k = K())
            : key(k), value_list(std::vector<V>())
        {
        }
        MPair(const K& k, const V& v)
            : MPair(k)
        {
            this->value_list.push_back(v);
        }
        MPair(const K& k, const std::vector<V>& vlist)
            : key(k), value_list(vlist)
        {
        }
        //--------------------------------------------------------------------------------

        // Overloaded operators
        friend std::ostream& operator<<(std::ostream& outs, const MPair<K, V>& print_me)
        {
            outs << print_me.key << ":";
            for (int i = 0; i < print_me.value_list.size(); ++i) outs << print_me.value_list[i] << " ";
            outs << std::endl;
            return outs;
        }
        friend bool operator==(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
        {
            return lhs.key == rhs.key;
        }
        friend bool operator<(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
        {
            return lhs.key < rhs.key;
        }
        friend bool operator<=(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
        {
            return lhs.key <= rhs.key;
        }
        friend bool operator>(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
        {
            return lhs.key > rhs.key;
        }
        friend MPair<K, V> operator+(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
        {
            MPair<K, V> mpair(lhs.key, lhs.value_list);
            for (int i = 0; i < rhs.value_list; ++i)
                mpair.value_list.push_back(rhs.value_list[i]);
            return mpair;
        }
        MPair<K, V>& operator+=(const V& value)
        {
            this->value_list.push_back(value);
            return *this;
        }
    };

    template <typename Item>
    std::ostream& operator<<(std::ostream& outs, const std::vector<Item>& vec)
    {
        for (int i = 0; i < vec.size(); ++i) outs << vec[i] << " ";
        outs << std::endl;
        return outs;
    }

    template <typename Item>
    std::vector<Item>& operator+=(std::vector<Item>& vec, const Item& value)
    {
        vec.push_back(value);
        return vec;
    }

};

#endif // MPAIR_H