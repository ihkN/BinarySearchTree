#include <iostream>
#include <utility>
#include <algorithm>

template <typename ValType>
class Bst
{
    public:
        Bst(): root{nullptr} {}
        
        // copy constructor
        Bst(const Bst& other): root{nullptr} { root = clone(other.root); }
        
        // move constructor
        Bst(const Bst&& other): root{other.root} { other.root=nullptr; }

        // copy assignment
        // TODO: does the original one needs deletion?
        Bst& operator=(const Bst& other)
        {
            if(this == &other) return *this;
            Bst copy = other;
            std::swap(*this, copy);
            return *this;
        }

        // move assignment 
        Bst& operator=(const Bst&& other)
        {
            if(this == &other) return *this;
            std::swap(root, other.root);
            return *this;
        }

        ~Bst() { clear(); }

        const ValType& findMin() const
        {
            if(isEmpty())
                throw std::underflow_error("Null Tree!");
            return findMin(root)->value;
        }

        const ValType& findMax() const
        {
            if(isEmpty())
                throw std::underflow_error("Null Tree!");
            return findMax(root)->value;
        }

        bool find(const ValType& x) const { return find(x, root); }

        bool isEmpty() const { return root==nullptr; }

        void print(std::ostream& out = std::cout) const
        {
            if(isEmpty()) out << "Null tree!" << std::endl;
            else print(root, out);
        }

        void clear() { clear(root); }

        void insert(ValType& x) { insert(x, root); }

        void insert(ValType&& x) { insert(std::move(x), root); }

        void remove(const ValType& x) { remove(x, root); }

    private:
        struct BNode
        {
            ValType value;
            BNode* right;
            BNode* left;
            // copy constructor
            BNode(const ValType& val, BNode* lt, BNode* rt): 
                value{val}, left{lt}, right{rt} {}
            // move constructor
            BNode(const ValType&& val, BNode* lt, BNode* rt): 
                value{std::move(val)}, left{lt}, right{rt} {}
        };

        BNode* root;

        // internal methodes
        void insert(const ValType& x, BNode*& t)
        {
            if(t == nullptr) t = new BNode{x, nullptr, nullptr};
            else if(x < t->value) insert(x, t->left);
            else if(x > t->value) insert(x, t->right);
            else {} ; // is duplicate
        }

        void insert(ValType&& x, BNode*& t)
        {
            if(t == nullptr) t = new BNode{std::move(x), nullptr, nullptr};
            else if(x < t->value) insert(std::move(x), t->left);
            else if(x > t->value) insert(std::move(x), t->right);
            else {}; // is duplicate
        }

        void remove(const ValType& x, BNode*& t)
        {
            if(t == nullptr) return;
            if(x < t->value) remove(x, t->left);
            else if(x > t->value) remove(x, t->right);
            // two children case:
            // find minimum node on right subtree,
            // replace current node with that, then delete it
            else if((t->left != nullptr) && (t->right != nullptr))
            {
                t->value = findMin(t->right)->value;
                remove(t->value, t->right);
            }
            // leaf case and a node with one children:
            // replace current node with that children,
            // if there is none it will be nullptr
            else
            {
                BNode* current = t;
                t = (t->left != nullptr) ? t->left : t->right;
                delete current;
            }
        }

        BNode* findMin(BNode* t) const
        {
            if(t == nullptr) return nullptr;
            if(t->left == nullptr) return t;
            return findMin(t->left);
        }

        BNode* findMax(BNode* t) const
        {
            if(t == nullptr) return nullptr;
            if(t->right == nullptr) return t;
            return findMax(t->right);
            // non-recursive version
            /*
             *if(t != nullptr)
             *    while(t->right != nullptr)
             *        t = t->right;
             *return t;
             */
        }

        bool find(const ValType& x, BNode* t) const
        {
            if(t == nullptr) return false;
            if(x < t->value) return find(x, t->left);
            if(x > t->value) return find(x, t->right);
            return true;
            // non-recursive version
            /*
             *while(t != nullptr)
             *    if(x < t->value)
             *        t = t->left;
             *    else if(x > t->value)
             *        t = t->right;
             *    else
             *        return true;
             *return false;
             */
        }

        void clear(BNode* t)
        {
            if(t != nullptr)
            {
                clear(t->left);
                clear(t->right);
            }
            t= nullptr;
        }

        void print(BNode* t, std::ofstream& out) const
        {
            if( t != nullptr )
            {
                print(t->left, out);
                out << t->value << std::endl;
                print(t->right, out);
            }
        }

        BNode* clone(BNode* t) const
        {
            if(t == nullptr) return nullptr;
            return new BNode(t->value, clone(t->left), clone(t->right));
        }
};

int main()
{
    Bst<int> tree;
    tree.insert(5);
    tree.insert(6);
    tree.insert(4);
    tree.print();
    
}
