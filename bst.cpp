#include <iostream>
#include <algorithm>
#include <iterator>
#include <utility>


template <typename ValType>
struct BstNode
{
    ValType value;
    BstNode* right;
    BstNode* left;
    BstNode* parent;

    BstNode(const ValType& val, BstNode* rn, BstNode* ln, BstNode* par): 
        value(val), right(rn), left(ln), parent(par) {}

    BstNode(ValType&& val, BstNode* rn, BstNode* ln, BstNode* par):
        value(std::move(val)), right(rn), left(ln), parent(par) {}
};

template <typename ValType>
class Bst
{
    public:
        class BstIterator: public std::iterator<std::bidirectional_iterator_tag, ValType>
        {
            public:
                using value_type = ValType;

                BstIterator();
                
                // comparison operators for node pointers
                bool operator==(const BstIterator& other) const;
                bool operator!=(const BstIterator& other) const;
                
                // derefrence operator, return a ref to the
                // value pointed by current node
                const value_type& operator*() const;
                
                // postincrement and preincrement
                BstIterator& operator++();
                BstIterator operator++(int);
                BstIterator& operator--();
                BstIterator operator--(int);

                private:
                    // current is the current location in the tree.
                    // tree is address of stree object associate with this iterator
                    const BstNode<value_type>* current;
                    const Bst<value_type>* tree;
                    
                    // used to construct an iterator return value from a node pointer
                    BstIterator(const BstNode<value_type>* p, const Bst<value_type>* t);

                    friend class Bst<value_type>;
            };
            

            // we just want const_iterator so that
            // iterators cant modify the tree
            using const_iterator = BstIterator;
            using iterator = const_iterator;
            using value_type = ValType;

            Bst(): root{nullptr} {}

            Bst(const Bst& other): root{nullptr} { root = clone(other.root); }

            Bst(const Bst&& other): root{other.root} { other.root = nullptr; }

            ~Bst() { clear(); }

            Bst& operator=(const Bst& other)
            {
                Bst copy = other;
                std::swap(*this, copy);
                return *this;
            }

            Bst& operator=(const Bst&& other)
            {
                std::swap(root, other.root);
                return *this;
            }
            
            // search for x, if found return an iterator
            // to it, otherwise return end()
            const_iterator find(const value_type& x) const;
            
            // return an iterator pointing to the first item (inorder)
            const_iterator begin() const;
            
            // return an iterator poiting just past the end of tree
            const_iterator end() const;

            const value_type& findMin() const
            {
                if (isEmpty())
                    throw std::underflow_error("Null Tree!");
                return findMin(root)->value;
            }

            const value_type& findMax() const
            {
            if(isEmpty())
                    throw std::underflow_error("Null Tree!");
            return findMax(root)->value;
        }
        
        // return true if x is found in the tree
        bool contains(const value_type& x) const
        {
            return contains(x, root);
        }

        bool isEmpty() const { return root == nullptr; }

        void print(std::ostream& out = std::cout) const
        {
            if(isEmpty())
                out << "Null Tree!" << std::endl;
            else
                print(root, out);
        }

        void clear() { clear(root); }

        const_iterator insert(const value_type& x)
        {
            auto t = insert(x, root, nullptr);
            if(t == nullptr) return end();
            else return const_iterator(t, this);
        }

        void remove(const value_type& x) { remove(x, root); }

    private:
        BstNode<value_type>* root;

        BstNode<value_type>* insert(const value_type& x, BstNode<value_type>*& t, BstNode<value_type>* par)
        {
            if(t == nullptr)
            {
                t = new BstNode<value_type>{x, nullptr, nullptr, par};
                return t;
            }
            else if(x < t->value) return insert(x, t->left, t);
            else if(x > t->value) return insert(x, t->right, t);
            else return nullptr;
        }

        bool remove(const value_type& x, BstNode<value_type>*& t)
        {
            if(t == nullptr) return false;
            if(x < t->value) return remove(x, t->left);
            else if(x > t->value) return remove(x, t->right);
            else if((t->left != nullptr) && (t->right != nullptr))
            {
                t->value = findMin(t->right)->value;
                remove(t->value, t->right);
                return true;
            }
            else
            {
                BstNode<value_type>* holder = t;
                t = (t->left != nullptr) ? t->left : t->right;
                delete holder;
                return true;
            }
        }

        BstNode<value_type>* findMin(BstNode<value_type>* t) const
        {
            if(t == nullptr) return nullptr;
            if(t->left == nullptr) return t;
            return findMin(t->left);
        }

        BstNode<value_type>* findMax(BstNode<value_type>* t) const
        {
            if(t == nullptr) return nullptr;
            if(t->right == nullptr) return t;
            return findMax(t->right);
        }

        bool contains(const value_type& x, BstNode<value_type>* t) const
        {
            if(t == nullptr) return false;
            else if(x < t->value) return contains(x, t->left);
            else if(x > t->value) return contains(x, t->right);
            else return true;
        }

        void clear(BstNode<value_type>*& t)
        {
            if(t != nullptr)
            {
                clear(t->left);
                clear(t->right);
                delete t;
            }
            t = nullptr;
        }

        void print(BstNode<value_type>* t, std::ostream& out) const
        {
            if( t != nullptr )
            {
                print(t->left, out);
                out << t->value <<std:: endl;
                print(t->right, out);
            }
        }

        BstNode<value_type>* clone(BstNode<value_type>* t) const
        {
            if(t == nullptr) return nullptr;
            else return new BstNode<value_type>{t->value, clone(t->right), clone(t->left), t->parent};
        }
};

template <typename ValType>
typename Bst<ValType>::const_iterator
Bst<ValType>::find(const ValType& x) const
{
    auto t = root;
    while(t != nullptr && !(t->value == x))
        t = (x < t->value) ? t->left : t->right;
    return BstIterator(t, this);
}

template <typename ValType>
typename Bst<ValType>::const_iterator
Bst<ValType>::begin() const
{
    return const_iterator(findMin(root), this);
}

template <typename ValType>
typename Bst<ValType>::const_iterator
Bst<ValType>::end() const
{
    return BstIterator(nullptr, this);
}

template <typename ValType>
Bst<ValType>::BstIterator::BstIterator(): current(nullptr), tree(nullptr) {}

template <typename ValType>
bool Bst<ValType>::BstIterator::operator!=(const BstIterator& other) const
{
    return tree != other.tree || current != other.current;
}

template <typename ValType>
const ValType& Bst<ValType>::BstIterator::operator*() const
{
    if(current == nullptr)
        throw std::underflow_error("Null Tree!");
    return current->value;
}

template <typename ValType>
typename Bst<ValType>::BstIterator&
Bst<ValType>::BstIterator::operator++()
{
    BstNode<ValType>* p;
    if( current == nullptr)
    {
        current = tree->root;
        if(current = nullptr)
            throw std::underflow_error("Null Tree!");
        while(current->left != nullptr)
            current = current->left;
    }
    else
    {
        if(current->right != nullptr)
        {
            current = current->right;
            while(current->left != nullptr)
                current = current->left;
        }
        else
        {
            p = current->parent;
            while((p != nullptr) && (current == p->right))
            {
                current = p;
                p = p->parent;
            }
            current = p;
        }
    }

    return *this;
}

template <typename ValType>
typename Bst<ValType>::BstIterator
Bst<ValType>::BstIterator::operator++(int)
{
    auto holder = *this;
    operator++();
    return holder;
}

template <typename ValType>
typename Bst<ValType>::BstIterator&
Bst<ValType>::BstIterator::operator--()
{
    BstNode<ValType>* p;
    if(current == nullptr)
    {
        current = tree->root;
        if(current == nullptr)
            throw std::underflow_error("Null Tree!");
        while(current->right != nullptr)
            current = current->right;
    }
    else if(current->left != nullptr)
    {
        while(current->right != nullptr)
            current = current->right;
    }
    else
    {
        p = current->parent;
        while((p != nullptr) && (current == p->left))
        {
            current = p;
            p = p->parent;
        }
        current = p;
    }

    return *this;
}

template <typename ValType>
typename Bst<ValType>::BstIterator
Bst<ValType>::BstIterator::operator--(int)
{
    auto holder = *this;
    operator--();
    return holder;
}

template <typename ValType>
Bst<ValType>::BstIterator::BstIterator(const BstNode<ValType>* p, const Bst<ValType>* t):
    current(p), tree(t) {}


//int main() {}
