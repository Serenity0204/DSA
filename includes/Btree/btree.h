#ifndef BTREE_H
#define BTREE_H

#include <cassert>  // Provides assert
#include <cstdlib>  // Provides size_t
#include <iomanip>  // Provides std::setw
#include <iostream> // Provides std::cout
#include <string>   // Provides std::to_string

#include "btree_array_functions.h" // Include the implementation.

namespace DSA
{
    template <class Item>
    class BTree
    {
    public:
        // TYPEDEFS
        typedef Item value_type;
        // CONSTRUCTORS and DESTRUCTOR
        BTree();
        BTree(const BTree& source);
        BTree(Item* a, std::size_t size);
        ~BTree() { clear(); }
        // MODIFICATION MEMBER FUNCTIONS
        BTree& operator=(const BTree& source);
        void clear();
        void copy(const BTree& source);
        bool insert(const Item& entry);
        bool erase(const Item& target);
        // NON-CONSTANT MEMBER FUNCTIONS
        Item* find(const Item& entry);
        bool contains(const Item& entry);
        Item& get(const Item& entry);
        // CONSTANT MEMBER FUNCTIONS
        std::size_t size() const;
        bool empty() const { return (data_count == 0); }
        void print(int indent = 0, std::ostream& outs = std::cout) const;
        bool is_valid() const;
        // OVERLOAD OPERATOR FUNCTIONS
        template <class U>
        friend std::ostream& operator<<(std::ostream& outs, const BTree<U>& btree);
        // SUGGESTED FUNCTION FOR DEBUGGING
        std::string in_order();

    private:
        // MEMBER CONSTANTS
        static const std::size_t MINIMUM = 1;
        static const std::size_t MAXIMUM = 2 * MINIMUM;
        size_t _size;
        // MEMBER VARIABLES
        std::size_t data_count;
        Item data[MAXIMUM + 1];
        std::size_t child_count;
        BTree* subset[MAXIMUM + 2];
        // HELPER MEMBER FUNCTIONS
        bool is_leaf() const { return (child_count == 0); }
        // insert element functions
        bool loose_insert(const Item& entry);
        void fix_excess(std::size_t i);
        // remove element functions
        bool loose_erase(const Item& target);
        void remove_biggest(Item& removed_entry);
        void fix_shortage(std::size_t i);
        void transfer_from_left(std::size_t i);
        void transfer_from_right(std::size_t i);
        void merge_with_next_subset(std::size_t i);
        // helper
    };

    // Implementation MEMBER FUNCTIONS

    // TODO
    // public

    template <class Item>
    bool BTree<Item>::is_valid() const
    {
        bool valid = true;
        // check if the node is empty
        if (empty()) return true;
        // check if the node has too many entries
        if (data_count > MAXIMUM || data_count < MINIMUM)
        {
            std::cout << "node has too many entry" << std::endl;
            return false;
        }
        // check if the node has too many children
        if (child_count > MAXIMUM + 1 || child_count < 0)
        {
            std::cout << "node has too many children" << std::endl;
            return false;
        }
        // check if the data is sorted
        for (size_t i = 0; i < data_count - 1; i++)
        {
            if (data[i] > data[i + 1])
            {
                BtreeUtils::print_array(data, data_count);
                std::cout << "data is not sorted" << std::endl;
                return false;
            }
        }
        if (!is_leaf())
        {
            // check if the child_count is not equal to data_count+1
            if (child_count != data_count + 1)
            {
                std::cout << "child_count is not equal to data_count+1" << std::endl;
                return false;
            }
            // check if data is in range of children
            for (size_t i = 0; i < data_count; i++)
            {
                // check if data[i] is greater than subset[i]
                valid = BtreeUtils::is_gt(subset[i]->data, subset[i]->data_count, data[i]);
                if (!valid)
                {
                    std::cout << "data[i] is NOT greater than subset[i]" << std::endl;
                    return false;
                }
                // check if data[i] is less than subset[i+1]
                valid = BtreeUtils::is_le(subset[i + 1]->data, subset[i + 1]->data_count, data[i]);
                if (!valid)
                {
                    std::cout << "data[i] is NOT less than subset[i + 1]" << std::endl;
                    return false;
                }
                // check if subset[i] is valid
                valid = subset[i]->is_valid();
                if (!valid) return false;
            }
            // check if the last child is valid
            valid = subset[data_count]->is_valid();
            if (!valid) return false;
        }
        return true;
    }

    template <class Item>
    BTree<Item>::BTree()
        : data_count(0), child_count(0), _size(0)
    {
        for (size_t i = 0; i < MAXIMUM + 1; ++i)
            this->data[i] = Item();
        for (size_t i = 0; i < MAXIMUM + 2; ++i)
            this->subset[i] = nullptr;
    }

    template <class Item>
    BTree<Item>::BTree(const BTree<Item>& source)
        : BTree()
    {
        this->_size = source._size;
        this->copy(source);
    }

    template <class Item>
    BTree<Item>::BTree(Item* a, std::size_t size)
        : BTree()
    {
        this->data_count = 0;
        this->child_count = 0;
        for (size_t i = 0; i < size; ++i)
            this->insert(a[i]);
    }

    // MODIFICATION MEMBER FUNCTIONS
    template <class Item>
    BTree<Item>& BTree<Item>::operator=(const BTree<Item>& source)
    {
        if (this == &source) return *this;
        this->clear();
        this->copy(source);
        return *this;
    }

    template <class Item>
    void BTree<Item>::clear()
    {
        for (size_t i = 0; i < this->child_count; ++i)
        {
            if (subset[i] != nullptr) this->subset[i]->clear();
            delete subset[i];
            subset[i] = nullptr;
        }
        this->child_count = 0;
        this->data_count = 0;
    }

    template <class Item>
    void BTree<Item>::copy(const BTree<Item>& source)
    {
        BtreeUtils::copy_array(this->data, source.data, this->data_count, source.data_count);
        this->child_count = source.child_count;
        for (size_t i = 0; i < this->child_count; ++i)
        {
            this->subset[i] = new BTree<Item>();
            this->subset[i]->copy(*(source.subset[i]));
        }
    }

    template <class Item>
    bool BTree<Item>::insert(const Item& entry)
    {
        if (this->contains(entry)) return false;
        if (!this->loose_insert(entry)) return false;
        if (this->data_count > MAXIMUM)
        {
            BTree<Item>* new_root = new BTree();
            BtreeUtils::copy_array(new_root->data, this->data, new_root->data_count, this->data_count);
            BtreeUtils::copy_array(new_root->subset, this->subset, new_root->child_count, this->child_count);

            this->data_count = 0;
            this->subset[0] = new_root;
            this->child_count = 1;

            this->fix_excess(0);
        }
        this->_size++;
        return true;
    }

    template <class Item>
    bool BTree<Item>::erase(const Item& target)
    {
        if (!this->contains(target)) return false;
        if (!this->loose_erase(target)) return false;
        // data_size < MINIMUM && subset_size > 0
        if (this->data_count<MINIMUM&& this->child_count> 0)
        {
            BTree<Item>* child = this->subset[0];
            BtreeUtils::copy_array(this->data, child->data, this->data_count, child->data_count);
            BtreeUtils::copy_array(this->subset, child->subset, this->child_count, child->child_count);
            child->child_count = 0;
            delete child;
        }
        this->_size--;
        return true;
    }

    // NON-CONSTANT MEMBER FUNCTIONS
    template <class Item>
    Item* BTree<Item>::find(const Item& entry)
    {
        std::size_t index = BtreeUtils::first_ge(this->data, this->data_count, entry);

        if ((index < this->data_count) && !(entry < this->data[index]))
            return &this->data[index];

        if (this->child_count == 0)
            return nullptr;

        return this->subset[index]->find(entry);
    }

    template <class Item>
    bool BTree<Item>::contains(const Item& entry)
    {
        Item* found = this->find(entry);
        if (found == nullptr) return false;
        return true;
    }

    template <class Item>
    Item& BTree<Item>::get(const Item& entry)
    {
        Item* found = this->find(entry);
        assert(found != nullptr);
        return *found;
    }

    // CONSTANT MEMBER FUNCTIONS
    template <class Item>
    std::size_t BTree<Item>::size() const
    {
        return this->_size;
    }

    template <class Item>
    void BTree<Item>::print(int indent, std::ostream& outs) const
    {
        static const std::string down_bracket = "\357\271\207"; // ﹇
        static const std::string up_bracket = "\357\271\210";   // ﹈
        if (this->child_count > this->data_count) this->subset[child_count - 1]->print(indent + 1, outs);
        outs << std::setw(indent * 4) << "" << down_bracket << std::endl;
        if (this->data_count != 0)
        {
            for (std::size_t i = this->data_count - 1; i < this->data_count; --i)
            {
                outs << std::setw(indent * 4) << "" << this->data[i] << std::endl;
                if (i == 0) outs << std::setw(indent * 4) << "" << up_bracket << std::endl;
                if (i < this->child_count) this->subset[i]->print(indent + 1, outs);
            }
            return;
        }
        outs << std::setw(indent * 4) << "" << up_bracket << std::endl;
    }

    // OVERLOAD OPERATOR FUNCTIONS
    template <class U>
    std::ostream& operator<<(std::ostream& outs, const BTree<U>& btree)
    {
        btree.print(0, outs);
        return outs;
    }

    // private

    // insert element functions
    template <class Item>
    bool BTree<Item>::loose_insert(const Item& entry)
    {
        size_t index = BtreeUtils::first_ge(this->data, this->data_count, entry);
        bool flag = false;
        if (!this->is_leaf()) flag = this->subset[index]->loose_insert(entry);
        if (this->is_leaf())
        {
            BtreeUtils::insert_item(this->data, index, this->data_count, entry);
            return true;
        }
        if (this->subset[index]->data_count > BTree<Item>::MAXIMUM) this->fix_excess(index);
        return flag;
    }

    template <class Item>
    void BTree<Item>::fix_excess(std::size_t i)
    {
        BTree<Item>* subtree = this->subset[i];
        BTree<Item>* split_tree = new BTree<Item>();
        BtreeUtils::split(subtree->data, subtree->data_count, split_tree->data, split_tree->data_count);
        BtreeUtils::split(subtree->subset, subtree->child_count, split_tree->subset, split_tree->child_count);
        Item item;
        BtreeUtils::detach_item(subtree->data, subtree->data_count, item);
        BtreeUtils::ordered_insert(this->data, this->data_count, item);
        BtreeUtils::insert_item(this->subset, i + 1, this->child_count, split_tree);
    }

    // remove element functions
    template <class Item>
    bool BTree<Item>::loose_erase(const Item& target)
    {
        size_t index = BtreeUtils::first_ge(this->data, this->data_count, target);
        bool flag = false;
        if (this->is_leaf())
        {
            Item item;
            BtreeUtils::delete_item(this->data, index, this->data_count, item);
            return true;
        }
        if (!this->is_leaf())
        {
            bool found = (index < this->data_count) && !(target < this->data[index]);
            if (found)
            {
                Item entry;
                this->subset[index]->remove_biggest(entry);
                this->data[index] = entry;
                flag = true;
            }
            if (!found) flag = this->subset[index]->loose_erase(target);
        }
        if (this->subset[index]->data_count < BTree<Item>::MINIMUM) this->fix_shortage(index);
        return flag;
    }

    template <class Item>
    void BTree<Item>::remove_biggest(Item& removed_entry)
    {
        if (this->is_leaf())
        {
            BtreeUtils::detach_item(this->data, this->data_count, removed_entry);
            return;
        }
        this->subset[this->child_count - 1]->remove_biggest(removed_entry);
        fix_shortage(this->child_count - 1);
    }

    template <class Item>
    void BTree<Item>::fix_shortage(std::size_t i)
    {

        // no shortage, do nothing
        if (this->subset[i]->data_count >= MINIMUM) return;

        // transfer from left to right
        if (i > 0 && this->subset[i - 1]->data_count > MINIMUM)
        {
            this->transfer_from_left(i);
            return;
        }
        // transfer from right to left
        if (i < this->child_count - 1 && this->subset[i + 1]->data_count > MINIMUM)
        {
            this->transfer_from_right(i);
            return;
        }
        if (i > 0)
        {
            this->merge_with_next_subset(i - 1);
            return;
        }
        if (i < this->child_count - 1)
        {
            this->merge_with_next_subset(i);
            return;
        }
    }

    template <class Item>
    void BTree<Item>::transfer_from_left(std::size_t i)
    {
        BTree<Item>* left_subset = this->subset[i - 1];
        BTree<Item>* right_subset = this->subset[i];
        Item item;
        // delete at root
        BtreeUtils::delete_item(this->data, i - 1, this->data_count, item);
        // attach it to right first item
        BtreeUtils::ordered_insert(right_subset->data, right_subset->data_count, item);
        // insert_item(right_subset->data, 0, right_subset->data_count, item);
        //  delete the last element in left subset data
        BtreeUtils::detach_item(left_subset->data, left_subset->data_count, item);
        // insert it at index 0 of root data
        BtreeUtils::ordered_insert(this->data, this->data_count, item);
        // insert_item(this->data, i - 1, this->data_count, item);

        if (left_subset->child_count > 0)
        {
            BTree<Item>* node;
            BtreeUtils::detach_item(left_subset->subset, left_subset->child_count, node);
            BtreeUtils::insert_item(right_subset->subset, 0, right_subset->child_count, node);
        }
    }

    template <class Item>
    void BTree<Item>::transfer_from_right(std::size_t i)
    {
        BTree<Item>* left_subset = this->subset[i];
        BTree<Item>* right_subset = this->subset[i + 1];

        Item item;
        // delete data[i] in root
        BtreeUtils::delete_item(this->data, i, this->data_count, item);
        // attach it to left end
        BtreeUtils::ordered_insert(left_subset->data, left_subset->data_count, item);
        // insert_item(left_subset->data, i, left_subset->data_count, item);
        // delete the first entry in right subset data
        BtreeUtils::delete_item(right_subset->data, 0, right_subset->data_count, item);
        // attach it to root end
        BtreeUtils::ordered_insert(this->data, this->data_count, item);
        // insert_item(this->data, i, this->data_count, item);
        if (right_subset->child_count > 0)
        {
            BTree<Item>* node;
            BtreeUtils::delete_item(right_subset->subset, 0, right_subset->child_count, node);
            BtreeUtils::attach_item(left_subset->subset, left_subset->child_count, node);
        }
    }

    template <class Item>
    void BTree<Item>::merge_with_next_subset(std::size_t i)
    {
        BTree<Item>* left_subset = this->subset[i];
        BTree<Item>* right_subset = this->subset[i + 1];
        Item item;
        BtreeUtils::delete_item(this->data, i, this->data_count, item);
        BtreeUtils::attach_item(left_subset->data, left_subset->data_count, item);
        BtreeUtils::merge(this->subset[i]->data, this->subset[i]->data_count, this->subset[i + 1]->data, this->subset[i + 1]->data_count);
        BtreeUtils::merge(this->subset[i]->subset, this->subset[i]->child_count, this->subset[i + 1]->subset, this->subset[i + 1]->child_count);
        BTree<Item>* node;
        BtreeUtils::delete_item(this->subset, i + 1, this->child_count, node);
        delete node;
    }
};

#endif // BTREE_H
