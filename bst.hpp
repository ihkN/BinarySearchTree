#ifndef BST_H
#define BST_H

#include <iostream>
#include <algorithm>
#include <iterator>
#include <utility>
#include <memory>

template <typename Key, typename Val>
struct BstNode
{
    using Pair = std::pair<Key, Val>;
    using Uptr = std::unique_ptr<BstNode>;
    
    Pair data;
    Uptr right, left; 
    BstNode* parent;

    explicit BstNode(const Pair& dat, Uptr rn, Uptr ln, BstNode* par)
    {
        data = dat;
        if(rn) right = std::make_unique<BstNode>(rn);
        else right = rn;
        if(ln) left = std::make_unique<BstNode>(ln);
        else left = ln;
        parent = par;
    } 

    explicit BstNode(Pair&& dat, Uptr rn, Uptr ln, BstNode* par)
    {
        data = std::move(dat);
        if(rn) right = std::make_unique<BstNode>(rn);
        else right = rn;
        if(ln) left = std::make_unique<BstNode>(ln);
        else left = ln;
        parent = par;
    }
};

template <typename Key, typename Val, typename Cmp=std::less<std::pair<Key, Val>>>
class Bst
{
    public:
        class BstIterator: public std::iterator<std::bidirectional_iterator_tag, Key, Val>
        {
            public:
                using Pair = std::pair<Key, Val>;
                using Node = BstNode<Key, Val>;

                BstIterator();
                
                // comparison operators for node pointers
                bool operator==(const BstIterator& other) const;
                bool operator!=(const BstIterator& other) const;
                
                // derefrence operator, return a ref to the
                // value pointed by current node
                const Pair& operator*() const;
                
                // post and pre increment and decrement
                BstIterator& operator++();
                BstIterator& operator--();
                BstIterator operator++(int);
                BstIterator operator--(int);

                private:
                    // current is the current location in the tree.
                    // tree is address of stree object associate with this iterator
                    const Node* current;
                    const Bst<Key, Val, Cmp>* tree;
                    
                    // used to construct an iterator return value from a node pointer
                    BstIterator(const Node* p, const Bst<Key, Val, Cmp>* t);

                    friend class Bst;
            };

            class BstConstIterator: public BstIterator
            {
                public:
                    using Pair = std::pair<Key, Val>;
                    using BstIterator::BstIterator;

                    const Pair& operator*() const { return BstIterator::operator*(); }

                private:
                    friend class Bst;
            };

            

            // we just want const_iterator so that
            // iterators cant modify the tree
            using Pair = std::pair<Key, Val>;
            using Node = BstNode<Key, Val>;
            using Uptr = std::unique_ptr<Node>;
            using iterator = BstIterator;
            using const_iterator = BstConstIterator;

            Bst(): root{Uptr(nullptr)} {}

            explicit Bst(const Bst& other): root{std::make_unique<Node>(clone(other.root))} {}

            explicit Bst(Bst&& other) noexcept: root{std::move(other.root)} {}

            ~Bst() { clear(); }

            Bst& operator=(const Bst& other)
            {
                clear();
                Bst copy = other;
                *this = std::move(copy);
                return *this;
            }

            Bst& operator=(Bst&& other) noexcept
            {
                root = std::move(other.root);
                return *this;
            }
            
            // search for x, if found return an iterator
            // to it, otherwise return end()
            iterator find(const Pair& x);
            const_iterator find(const Pair& x) const;
            
            // return an iterator pointing to the first item (inorder)
            // begin will return iterator or const_iterator depending 
            // on the const-qulification of the object it is called upon,
            // cbegin will return const iterator unconditionally
            iterator begin();
            const_iterator begin() const;
            const_iterator cbegin() const;
            
            // return an iterator poiting just past the end of tree
            iterator end();
            const_iterator end() const;
            const_iterator cend() const;

            const Pair& findMin() const
            {
                if (isEmpty())
                    throw std::underflow_error("Null Tree!");
                return findMin(root)->data;
            }

            const Pair& findMax() const
            {
            if(isEmpty())
                    throw std::underflow_error("Null Tree!");
            return findMax(root.get())->data;
        }
        
        // return true if x is found in the tree
        bool contains(const Pair& x) const
        {
            return contains(x, root);
        }

        bool isEmpty() const { return root == nullptr; }

        void clear() { clear(root); }

        iterator insert(const Pair& x)
        {
            auto t = insert(x, root, nullptr);
            if(t == nullptr) return end();
            else return iterator(t, this);
        }

        void remove(const Pair& x) { remove(x, root); }

    private:
        std::unique_ptr<Node> root;
        Cmp compare;

        Node* insert(const Pair& x, Uptr& t, Node* par)
        {
            if(t == nullptr)
            {
                t = std::make_unique<Node>(x, nullptr, nullptr, par);
                return t.get();
            }
            else if(compare(x, t->data)) return insert(x, t->left, t);
            else if(compare(t->data, x)) return insert(x, t->right, t);
            else return nullptr;
        }

        bool remove(const Pair& x, Uptr& t)
        {
            if(t == nullptr) return false;
            if(compare(x, t->data)) return remove(x, t->left);
            else if(compare(t->data, x)) return remove(x, t->right);
            else if((t->left != nullptr) && (t->right != nullptr))
            {
                t->data = findMin(t->right.get())->data;
                remove(t->data, t->right);
                return true;
            }
            else
            {
                if(t->left != nullptr) t = std::move(t->left);
                else t = std::move(t->right);
                return true;
            }
        }

        Node* findMin(Node* t) const
        {
            if(t == nullptr) return nullptr;
            if(t->left == nullptr) return t.get();
            return findMin(t->left.get());
        }

        Node* findMax(Node* t) const
        {
            if(t == nullptr) return nullptr;
            if(t->right == nullptr) return t.get();
            return findMax(t->right.get());
        }

        bool contains(const Pair& x, Uptr t) const
        {
            if(t == nullptr) return false;
            else if(compare(x, t->data)) return contains(x, t->left);
            else if(compare(t->data, x)) return contains(x, t->right);
            else return true;
        }

        void clear(Uptr& t) { t.reset(); }

        Node* clone(Node* t) const
        {
            if(t == nullptr) return nullptr;
            else return (t->data, clone(t->right), clone(t->left), t->parent);
        }
        
        friend std::ostream& operator<<(std::ostream& os, const Bst& tr)
        {
            if(tr.isEmpty())
                std::cout << "Null Tree!" << std::endl;
            else for(auto& p: tr)
                os << "(" << p.first << "," << p.second << ")" << std::endl;
           return os; 
        };
};

template <typename Key, typename Val, typename Cmp>
typename Bst<Key, Val, Cmp>::iterator
Bst<Key, Val, Cmp>::find(const std::pair<Key, Val>& x)
{
    auto t = std::move(root);
    while(t != nullptr && !(t->data == x))
        t = std::move((compare(x, t->data)) ? t->left : t->right);
    return iterator(t, this);
}

template <typename Key, typename Val, typename Cmp>
typename Bst<Key, Val, Cmp>::const_iterator
Bst<Key, Val, Cmp>::find(const std::pair<Key, Val>& x) const
{
    auto t = root;
    while(t != nullptr && !(t->data == x))
        t = (compare(x, t->data)) ? t->left : t->right;
    return const_iterator(t, this);
}

template <typename Key, typename Val, typename Cmp>
typename Bst<Key, Val, Cmp>::iterator
Bst<Key, Val, Cmp>::begin()
{
    return iterator(findMin(root.get()), this);
}

template <typename Key, typename Val, typename Cmp>
typename Bst<Key, Val, Cmp>::const_iterator
Bst<Key, Val, Cmp>::begin() const
{
    return const_iterator(findMin(root.get()), this);
}

template <typename Key, typename Val, typename Cmp>
typename Bst<Key, Val, Cmp>::const_iterator
Bst<Key, Val, Cmp>::cbegin() const
{
    return const_iterator(findMin(root.get()), this);
}

template <typename Key, typename Val, typename Cmp>
typename Bst<Key, Val, Cmp>::iterator
Bst<Key, Val, Cmp>::end()
{
    return BstIterator(nullptr, this);
}

template <typename Key, typename Val, typename Cmp>
typename Bst<Key, Val, Cmp>::const_iterator
Bst<Key, Val, Cmp>::end() const
{
    return BstConstIterator(nullptr, this);
}

template <typename Key, typename Val, typename Cmp>
typename Bst<Key, Val, Cmp>::const_iterator
Bst<Key, Val, Cmp>::cend() const
{
    return BstConstIterator(nullptr, this);
}

template <typename Key, typename Val, typename Cmp>
Bst<Key, Val, Cmp>::BstIterator::BstIterator(): current(nullptr), tree(nullptr) {}

template <typename Key, typename Val, typename Cmp>
bool Bst<Key, Val, Cmp>::BstIterator::operator!=(const BstIterator& other) const
{
    return tree != other.tree || current != other.current;
}

template <typename Key, typename Val, typename Cmp>
const std::pair<Key, Val>& Bst<Key, Val, Cmp>::BstIterator::operator*() const
{
    if(current == nullptr)
        throw std::underflow_error("Null Tree!");
    return current->data;
}

template <typename Key, typename Val, typename Cmp>
typename Bst<Key, Val, Cmp>::BstIterator&
Bst<Key, Val, Cmp>::BstIterator::operator++()
{
    BstNode<Key, Val>* p;
    if( current == nullptr)
    {
        current = tree->root.get();
        if(current == nullptr)
            throw std::underflow_error("Null Tree!");
        while(current->left != nullptr)
            current = current->left.get();
    }
    else
    {
        if(current->right != nullptr)
        {
            current = current->right.get();
            while(current->left != nullptr)
                current = current->left.get();
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

template <typename Key, typename Val, typename Cmp>
typename Bst<Key, Val, Cmp>::BstIterator
Bst<Key, Val, Cmp>::BstIterator::operator++(int)
{
    auto holder = *this;
    operator++();
    return holder;
}

template <typename Key, typename Val, typename Cmp>
typename Bst<Key, Val, Cmp>::BstIterator&
Bst<Key, Val, Cmp>::BstIterator::operator--()
{
    BstNode<Key, Val>* p;
    if(current == nullptr)
    {
        current = std::move(tree->root);
        if(current == nullptr)
            throw std::underflow_error("Null Tree!");
        while(current->right != nullptr)
            current = std::move(current->right);
    }
    else if(current->left != nullptr)
    {
        while(current->right != nullptr)
            current = std::move(current->right);
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

template <typename Key, typename Val, typename Cmp>
typename Bst<Key, Val, Cmp>::BstIterator
Bst<Key, Val, Cmp>::BstIterator::operator--(int)
{
    auto holder = *this;
    operator--();
    return holder;
}

template <typename Key, typename Val, typename Cmp>
Bst<Key, Val, Cmp>::BstIterator::BstIterator(const BstNode<Key, Val>* p, const Bst<Key, Val, Cmp>* t):
    current(p), tree(t) {}

#endif
