#pragma one
#include <memory>
#include <stack>

namespace ariel
{

    //--------------------------------------- Node class -------------------------------------//
    template <typename T>
    class BinaryTree
    {
        //----------------- Node --------------//
        class Node
        {
            friend class BinaryTree;
            T data;
            Node *left;
            Node *right;
            Node *parent;

        public:
            Node(T val) : data(val), right(NULL), left(NULL), parent(NULL){};
             bool operator==(Node other) {
                return this->data == other->data && this->left == other->left &&
                       this->right == other.right && this->parent == other.parent;
            }
        };
        //----------------- Tree objects --------------//

        Node *root;
        enum Order
        {
            preorder,
            inorder,
            postorder
        };
        //----------------- Iterator --------------//

        class Iterator
        {
            Node *curr;
            Order order_By;
            std::stack<Node *> stack;
        public:
            // constrctor
            Iterator(Node *node, const Order &orderBy) : order_By(orderBy)
            {
                init_Stack(node);

                curr = stack.top();
                stack.pop();
            }
            Iterator() : curr(nullptr){};
            
            Node* get_curr()
            {
                return curr;
            }

            //destrctor
            ~Iterator(){};

            void init_Stack(Node *node)
            {
                //Base
                if (node == nullptr)
                {
                    return;
                }
                if (order_By == inorder)
                {
                    init_Stack(node->right);
                    stack.push(node);
                    init_Stack(node->left);
                }
                else if (order_By == preorder)
                {
                    init_Stack(node->right);
                    init_Stack(node->left);
                    stack.push(node);
                }
                else //order_By==postorder
                {
                    stack.push(node);
                    init_Stack(node->right);
                    init_Stack(node->left);
                }
            }
            //++i
            Iterator &operator++()
            {
                if (stack.empty())
                {
                    curr = nullptr;
                    return *this;
                }
                curr = stack.top();
                stack.pop();
                return *this;
            }
            //i++
            const Iterator operator++(int)
            {
                if (stack.empty())
                {
                    curr = nullptr;
                    return *this;
                }
                Node *t = curr;
                curr = stack.top();
                stack.pop();
                return const Iterator(t, inorder);

                // Iterator t=*this;
                // curr=stack.top();
                // stack.pop();
                // return t;

            }

            T &operator*() const
            {
                return curr->data;
            }

            T *operator->() const
            {
                return &(curr->data);
            }
            bool operator==(const Iterator &other) const
            {
                return curr == other.curr;
                // need to check
            }

            bool operator!=(const Iterator &other) const
            {
                return !(*this == other);
            }
        };
        //--------------- Tree ----------------//
    public:
        BinaryTree() : root(nullptr) {}

        //--------------//

 void copy_ctor(Node *nd, const Node *other_nd) {
            if (other_nd->left != nullptr) {
                nd->left = new Node(other_nd->left->data);
                copy_ctor(nd->left, other_nd->left);
            }
            if (other_nd->right != nullptr) {
                nd->right = new Node(other_nd->right->data);
                copy_ctor(nd->right, other_nd->right);
            }
        }

        BinaryTree &operator=(const BinaryTree<T> &bt) {
            if (this == &bt) {
                return *this;
            }
            if (root != nullptr) {
                delete root;
            }
            if (bt.root != nullptr) {
                root = new Node{bt.root->data};
                copy_ctor(root, bt.root);
            }
            return *this;
        }

        BinaryTree(const BinaryTree &bt) { //copy ctor
            if (bt.root != nullptr) {
                this->root = new Node(bt.root->data);
                copy_ctor(root, bt.root);
            }
        }
        BinaryTree& operator=(BinaryTree<T>&& bt) noexcept {
            if (root){ delete root;}
            root  = bt.root;
            bt.root = nullptr;
            return *this;
        }

        BinaryTree(BinaryTree &&bt)  noexcept {
            this->root = bt.root;
            bt.root = nullptr;
        }
        //-------------//

        ~BinaryTree<T>()
        {
            if (root)
            {
                 for (auto it = (*this).begin_postorder(); it != (*this).end_postorder(); ++it) {
                    delete it.get_curr();
                }
            }
        }

    
        friend std::ostream &operator<<(std::ostream &os, const BinaryTree<T> &tree)
        {
            return os;
        }
        BinaryTree<T> &add_root(T data)
        {
            if (root != nullptr) // There is already a root just need to update a value
            {
                root->data = data;
            }
            else // We will create a new one
            {
                this->root = new Node(data);
            }
            return *this;
        }

        Node *found_node(Node *node, T value)
        {
            if (node == nullptr) //Basic
            {
                return nullptr;
            }
            if (node->data == value) // we found the value !!!!!
            {
                return node;
            }
            Node *right_node = nullptr;
            Node *left_node = nullptr;
            if (node->left != nullptr) // if we can move left
            {
                left_node = found_node(node->left, value);
            }
            if (node->right != nullptr) // if we can move right
            {
                right_node = found_node(node->right, value);
            }
            // If the tree is not symmetrical
            if (left_node != nullptr)
            {
                return left_node;
            }
            else
            {
                return right_node;
            }
        };

        // Node *find_node(T value){
        //     if(root->value==value)
        //     {
        //         return root;
        //     }
        //     return find_node(root,val);
        // };

        BinaryTree<T> &add_left(T parent_val, T child_val)
        {
            Node *found = found_node(this->root, parent_val);
            if (found == nullptr) // if we didnt found
            {
                throw std::invalid_argument("No value found");
            }
            else
            {
                if (found->left == nullptr) //If he has no left son
                {
                    found->left = new Node(child_val);
                    found->left->parent = found; // Updating his parent
                }
                else // If he has a son
                {
                    found->left->data = child_val;
                }
            }
            return *this;
        }

        BinaryTree<T> &add_right(T parent_val, T child_val)
        {
            Node *found = found_node(this->root, parent_val);
            if (found == nullptr) // if we didnt found
            {
                throw std::invalid_argument("No value found");
            }
            else
            {
                if (found->right == nullptr) //If he has no right son
                {
                    found->right = new Node(child_val);
                    found->right->parent = found; // Updating his parent
                }
                else // If he has a son
                {
                    found->right->data = child_val;
                }
            }
            return *this;
        }

        Iterator begin()
        {
            return Iterator{root, inorder};
        };
        Iterator end()
        {
            return Iterator{};
        };
        Iterator end_preorder()
        {
            return Iterator{};
        }
        Iterator begin_preorder()
        {
            return Iterator{root, preorder};
        }
        Iterator end_inorder()
        {
            return Iterator{};
        }
        Iterator begin_inorder()
        {
            return Iterator{root, inorder};
        }
        Iterator end_postorder()
        {
            return Iterator{};
        }
        Iterator begin_postorder()
        {
            return Iterator{root, postorder};
        }
    };
}