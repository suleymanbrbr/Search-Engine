#ifndef AVL_Tree_h
#define AVL_Tree_h

#include <string>
#include <cstddef>
#include <vector>
#include <iostream>

using namespace std;

template <class key, class value>
class AvlTree;

template <class key, class value>
class AvlNode {
public:
    
    key word;
    value details;
    AvlNode *left;
    AvlNode *right;
    int height;

    // Constructor
    AvlNode(const key & theWord, const value & theDetail, AvlNode *lt = nullptr, AvlNode *rt = nullptr, int h = 0 )
            : word( theWord ), details(theDetail), left( lt ), right( rt ), height( h ) { }

    friend class AvlTree<key, value>;
};

template <class key, class value>
class AvlTree {
    
public:
    // Constructor
    explicit AvlTree(const key & notFound);
    AvlTree(const AvlTree & rhs); // Copy constructor
    ~AvlTree( ); // Destructor

    const key & findMin( ) const;
    const key & findMax( ) const;
    const key & find(const key & x) const;
    AvlNode<key, value> * update(const key & x);
    bool isEmpty( ) const;
    void printTree( ) const;
    int getBalance(AvlNode<key, value> * node);
    void makeEmpty( );
    void insert(const key & x, const value & y);
    void remove(const key & x);

    const AvlTree & operator=(const AvlTree & rhs);

private:
    
    AvlNode<key, value> *root;
    const key ITEM_NOT_FOUND;

    const key & elementAt(AvlNode<key, value> *t ) const;
    void insert(const key & x, const value & y, AvlNode<key, value> * & t) const;
    void remove(const key & x, AvlNode<key, value> * & t);
    void printTree( AvlNode<key, value> *t ) const;
    AvlNode<key, value> * findMin(AvlNode<key, value> *t) const;
    AvlNode<key, value> * findMax(AvlNode<key, value> *t) const;
    AvlNode<key, value> * find(const key & x, AvlNode<key, value> *t ) const;
    void makeEmpty(AvlNode<key, value> * & t) const;

    // AVL tree balancing functions
    int height(AvlNode<key, value> *t) const;
    int max(int lhs, int rhs) const;
    void rotateWithLeftChild(AvlNode<key, value> * & k2) const;
    void rotateWithRightChild(AvlNode<key, value> * & k1) const;
    void doubleWithLeftChild(AvlNode<key, value> * & k3) const;
    void doubleWithRightChild(AvlNode<key, value> * & k1) const;
};

// Constructor
template <class key, class value>
AvlTree<key, value>::AvlTree(const key & notFound)
: ITEM_NOT_FOUND( notFound ), root( nullptr ) {}

// Get the height of a node
template <class key, class value>
int AvlTree<key, value>::height(AvlNode<key, value> *t) const {
    
    if (t == nullptr)
        return -1;
    return t->height;
}

// Get the maximum of two integers
template <class key, class value>
int AvlTree<key, value>::max(int lhs, int rhs) const {
    
    if (lhs > rhs)
        return lhs;
    return rhs;
}

// Insert a node into the AVL tree
template <class key, class value>
void AvlTree<key, value>::insert(const key & x, const value & y){
    insert( x, y, root);
}

// Internal method to insert into a subtree
template <class key, class value>
void AvlTree<key, value>::insert(const key & x, const value & y, AvlNode<key, value> * & t) const{
    
    if (t == nullptr)
        t = new AvlNode<key, value>(x, y, nullptr, nullptr, 0);
    
    else if (x < t->word) {
        insert(x, y, t->left);
        if ( height(t->left) - height( t->right ) == 2 ){
            if (x < t->left->word)  // X was inserted to the left-left subtree!
                rotateWithLeftChild(t);
            else                 // X was inserted to the left-right subtree!
                doubleWithLeftChild(t);
        }
        
    } else if(t->word < x) {    // Otherwise X is inserted to the right subtree
        
        insert(x, y, t->right);
        if (height(t->right) - height(t->left) == 2){ // height of the right subtree increased
            if (t->right->word < x) // X was inserted to right-right subtree
                rotateWithRightChild(t);
            else // X was inserted to right-left subtree
                doubleWithRightChild(t);
        }
    }
    else
        ;
    t->height = max(height(t->left), height(t->right)) + 1;
}

// Rotate binary tree node with left child
template <class key, class value>
void AvlTree<key, value>::rotateWithLeftChild(AvlNode<key, value> * & k2) const{ // rotate_right
    
    AvlNode<key, value> *k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = max(height(k2->left), height(k2->right)) + 1;
    k1->height = max( height( k1->left ), k2->height ) + 1;
    k2 = k1;
}

// Rotate binary tree node with right child
template <class key, class value>
void AvlTree<key, value>::rotateWithRightChild(AvlNode<key, value> * & k1) const{ // rotate_left
    
    AvlNode<key, value> *k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1->height = max(height(k1->left), height(k1->right)) + 1;
    k2->height = max(height(k2->right), k1->height) + 1;
    k1 = k2;
}

// Double rotate binary tree node: first left child
// with its right child; then node k3 with new left child
template <class key, class value>
void AvlTree<key, value>::doubleWithLeftChild(AvlNode<key, value> * & k3) const{
    
    rotateWithRightChild(k3->left);
    rotateWithLeftChild(k3);
}

// Double rotate binary tree node: first right child
// with its left child; then node k1 with new right child
template <class key, class value>
void AvlTree<key, value>::doubleWithRightChild(AvlNode<key, value> * & k1) const{
    
    rotateWithLeftChild(k1->right);
    rotateWithRightChild(k1);
}

// Find the element at the given node
template <class key, class value>
const key & AvlTree<key, value>::elementAt(AvlNode<key, value> *t) const{
    return t == nullptr ? ITEM_NOT_FOUND : t->word;
}

// Update the details of a node with the given key
template <class key, class value>
AvlNode<key, value> *  AvlTree<key, value>::update(const key & x){
    return find(x, root);
}

// Print the AVL tree
template <class key, class value>
void AvlTree<key, value>::printTree() const {
    printTree(root);
}

// Internal method to print a subtree rooted at t
template <class key, class value>
void AvlTree<key, value>::printTree( AvlNode<key, value> * t ) const {
    
    if (t != nullptr){
        printTree( t->left );
        cout << t->word << endl;
        printTree( t->right );
    }
}

// Find a given key in the AVL tree
template <class key, class value>
const key & AvlTree<key, value>::find( const key & x ) const {
    return elementAt( find( x, root ) );
}

// Internal method to find an element in a subtree
template <class key, class value>
AvlNode<key, value> * AvlTree<key, value>::find( const key & x, AvlNode<key, value> *t ) const {
    
    if (t == nullptr)
        return nullptr;
    else if(x < t->word)
        return find(x, t->left);
    else if(t->word < x)
        return find(x, t->right);
    else
        return t;    // Match
}

// Find the minimum element in the AVL tree
template <class key, class value>
const key & AvlTree<key, value>::findMin( ) const {
    return elementAt( findMin( root ) );
}

// Find the minimum element in a subtree
template <class key, class value>
AvlNode<key, value> * AvlTree<key, value>::findMin( AvlNode<key, value> *t ) const {
    
    if( t == nullptr )
        return nullptr;
    if( t->left == nullptr )
        return t;
    return findMin( t->left );
}

// Find the maximum element in the AVL tree
template <class key, class value>
const key & AvlTree<key, value>::findMax( ) const {
    return elementAt( findMax( root ) );
}

// Find the maximum element in a subtree
template <class key, class value>
AvlNode<key, value> * AvlTree<key, value>::findMax( AvlNode<key, value> *t ) const {
    
    if(t != nullptr){
        while(t->right != nullptr){
            t = t->right;
        }
    }
    return t;
}

// Check if the AVL tree is empty
template <class key, class value>
bool AvlTree<key, value>::isEmpty( ) const {
    return root == nullptr;
}

// Make the AVL tree empty
template <class key, class value>
void AvlTree<key, value>::makeEmpty( ) {
    makeEmpty(root);
}

// Internal method to make a subtree empty
template <class key, class value>
void AvlTree<key, value>::makeEmpty(AvlNode<key, value> * & t ) const {
    
    if(t != nullptr){
        makeEmpty( t->left );
        makeEmpty( t->right );
        delete t;
    }
    t = nullptr;
}

// Destructor
template <class key, class value>
AvlTree<key, value>::~AvlTree() {
    makeEmpty(root);
}

// Remove a node from the AVL tree
template <class key, class value>
void AvlTree<key, value>::remove(const key & x){
    remove(x, root);
}

// Get the balance factor of a node
template <class key, class value>
int AvlTree<key, value>::getBalance(AvlNode<key, value> * node){
    
    if (node == nullptr)
        return 0;
    return height(node->left) - height(node->right);
}

// Internal method to remove a node from a subtree
template <class key, class value>
void AvlTree<key, value>::remove(const key & x, AvlNode<key, value> * & t) {
    
    if (t == nullptr)
        return; // Element not found or tree is empty

    if (x < t->word)
        remove(x, t->left);
    
    else if (x > t->word)
        remove(x, t->right);
    
    else if (t->left != nullptr && t->right != nullptr) {
        t->word = findMin(t->right)->word;
        remove(t->word, t->right);
        
    } 
    else {
        
        AvlNode<key, value> * temp = t;
        t = (t->left != nullptr) ? t->left : t->right;
        delete temp;
    }

    if (t == nullptr)
        return;

    t->height = max(height(t->left), height(t->right)) + 1;

    if (height(t->left) - height(t->right) == 2) {
        
        if (height(t->left->left) >= height(t->left->right))
            rotateWithLeftChild(t);
        else
            doubleWithLeftChild(t);
    } 
    else if (height(t->right) - height(t->left) == 2) {
        
        if (height(t->right->right) >= height(t->right->left))
            rotateWithRightChild(t);
        else
            doubleWithRightChild(t);
    }
}

#endif /* AVL_Tree_h */
