#include <iostream>
#include <algorithm>
#include <iterator>
#include <utility>


template <typename Key, typename Val>
struct BstNode
{
    using Pair = std::pair<Key, Val>;
    
    Pair data;
    BstNode* right;
    BstNode* left;
    BstNode* parent;

    BstNode(const Pair& dat, BstNode* rn, BstNode* ln, BstNode* par): 
        data(dat), right(rn), left(ln), parent(par) {}

    BstNode(Pair&& dat, BstNode* rn, BstNode* ln, BstNode* par):
        data(std::move(dat)), right(rn), left(ln), parent(par) {}
};

template <typename Key, typename Val>
class Bst
{
    public:
        class BstIterator: public std::iterator<std::bidirectional_iterator_tag, Key, Val>
        {
            public:
                using Pair = std::pair<Key, Val>;

                BstIterator();
                
                // comparison operators for node pointers
                bool operator==(const BstIterator& other) const;
                bool operator!=(const BstIterator& other) const;
                
                // derefrence operator, return a ref to the
                // value pointed by current node
                const Pair& operator*() const;
                
                // postincrement and preincrement
                BstIterator& operator++();
                BstIterator operator++(int);
                BstIterator& operator--();
                BstIterator operator--(int);

                private:
                    // current is the current location in the tree.
                    // tree is address of stree object associate with this iterator
                    const BstNode<Key, Val>* current;
                    const Bst<Key, Val>* tree;
                    
                    // used to construct an iterator return value from a node pointer
                    BstIterator(const BstNode<Key, Val>* p, const Bst<Key, Val>* t);

                    friend class Bst<Key, Val>;
            };
            

            // we just want const_iterator so that
            // iterators cant modify the tree
            using const_iterator = BstIterator;
            using iterator = const_iterator;
            using Pair = std::pair<Key, Val>;

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
            const_iterator find(const Pair& x) const;
            
            // return an iterator pointing to the first item (inorder)
            const_iterator begin() const;
            
            // return an iterator poiting just past the end of tree
            const_iterator end() const;

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
            return findMax(root)->data;
        }
        
        // return true if x is found in the tree
        bool contains(const Pair& x) const
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

        const_iterator insert(const Pair& x)
        {
            auto t = insert(x, root, nullptr);
            if(t == nullptr) return end();
            else return const_iterator(t, this);
        }

        void remove(const Pair& x) { remove(x, root); }

    private:
        BstNode<Key, Val>* root;

        BstNode<Key, Val>* insert(const Pair& x, BstNode<Key, Val>*& t, BstNode<Key, Val>* par)
        {
            if(t == nullptr)
            {
                t = new BstNode<Key, Val>{x, nullptr, nullptr, par};
                return t;
            }
            else if(x.first < t->data.first) return insert(x, t->left, t);
            else if(x.first > t->data.first) return insert(x, t->right, t);
            else return nullptr;
        }

        bool remove(const Pair& x, BstNode<Key, Val>*& t)
        {
            if(t == nullptr) return false;
            if(x < t->data.first) return remove(x, t->left);
            else if(x > t->data.first) return remove(x, t->right);
            else if((t->left != nullptr) && (t->right != nullptr))
            {
                t->data = findMin(t->right)->data;
                remove(t->data, t->right);
                return true;
            }
            else
            {
                BstNode<Key, Val>* holder = t;
                t = (t->left != nullptr) ? t->left : t->right;
                delete holder;
                return true;
            }
        }

        BstNode<Key, Val>* findMin(BstNode<Key, Val>* t) const
        {
            if(t == nullptr) return nullptr;
            if(t->left == nullptr) return t;
            return findMin(t->left);
        }

        BstNode<Key, Val>* findMax(BstNode<Key, Val>* t) const
        {
            if(t == nullptr) return nullptr;
            if(t->right == nullptr) return t;
            return findMax(t->right);
        }

        bool contains(const Pair& x, BstNode<Key, Val>* t) const
        {
            if(t == nullptr) return false;
            else if(x < t->data.first) return contains(x, t->left);
            else if(x > t->data.first) return contains(x, t->right);
            else return true;
        }

        void clear(BstNode<Key, Val>*& t)
        {
            if(t != nullptr)
            {
                clear(t->left);
                clear(t->right);
                delete t;
            }
            t = nullptr;
        }

        void print(BstNode<Key, Val>* t, std::ostream& out) const
        {
            if( t != nullptr )
            {
                print(t->left, out);
                out << t->data.first << ", " << t->data.second <<std:: endl;
                print(t->right, out);
            }
        }

        BstNode<Key, Val>* clone(BstNode<Key, Val>* t) const
        {
            if(t == nullptr) return nullptr;
            else return new BstNode<Key, Val>{t->data, clone(t->right), clone(t->left), t->parent};
        }
};

template <typename Key, typename Val>
typename Bst<Key, Val>::const_iterator
Bst<Key, Val>::find(const std::pair<Key, Val>& x) const
{
    auto t = root;
    while(t != nullptr && !(t->value == x))
        t = (x < t->data.first) ? t->left : t->right;
    return BstIterator(t, this);
}

template <typename Key, typename Val>
typename Bst<Key, Val>::const_iterator
Bst<Key, Val>::begin() const
{
    return const_iterator(findMin(root), this);
}

template <typename Key, typename Val>
typename Bst<Key, Val>::const_iterator
Bst<Key, Val>::end() const
{
    return BstIterator(nullptr, this);
}

template <typename Key, typename Val>
Bst<Key, Val>::BstIterator::BstIterator(): current(nullptr), tree(nullptr) {}

template <typename Key, typename Val>
bool Bst<Key, Val>::BstIterator::operator!=(const BstIterator& other) const
{
    return tree != other.tree || current != other.current;
}

template <typename Key, typename Val>
const std::pair<Key, Val>& Bst<Key, Val>::BstIterator::operator*() const
{
    if(current == nullptr)
        throw std::underflow_error("Null Tree!");
    return current->data;
}

template <typename Key, typename Val>
typename Bst<Key, Val>::BstIterator&
Bst<Key, Val>::BstIterator::operator++()
{
    BstNode<Key, Val>* p;
    if( current == nullptr)
    {
        current = tree->root;
        if(current == nullptr)
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

template <typename Key, typename Val>
typename Bst<Key, Val>::BstIterator
Bst<Key, Val>::BstIterator::operator++(int)
{
    auto holder = *this;
    operator++();
    return holder;
}

template <typename Key, typename Val>
typename Bst<Key, Val>::BstIterator&
Bst<Key, Val>::BstIterator::operator--()
{
    BstNode<Key, Val>* p;
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

template <typename Key, typename Val>
typename Bst<Key, Val>::BstIterator
Bst<Key, Val>::BstIterator::operator--(int)
{
    auto holder = *this;
    operator--();
    return holder;
}

template <typename Key, typename Val>
Bst<Key, Val>::BstIterator::BstIterator(const BstNode<Key, Val>* p, const Bst<Key, Val>* t):
    current(p), tree(t) {}

int main()
{
    Bst<int, std::string> tree;
    tree.insert(std::make_pair(1, "AB"));
    tree.insert(std::make_pair(3, "GR"));
    tree.insert(std::make_pair(2, "DM"));
    tree.insert(std::make_pair(6, "RE"));
    tree.print();
    //std::cout << "********" << std::endl;
    //tree.remove(std::pair<int, std::string>(3, "GR"));
}
