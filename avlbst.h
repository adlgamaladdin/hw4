#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rightRotation(AVLNode<Key, Value>* current);
    void leftRotation(AVLNode<Key, Value>* current);
    void leftRight(AVLNode<Key, Value>* current);
    void rightLeft(AVLNode<Key, Value>* current);
    void removeTwoChildNode(AVLNode<Key, Value>* current);



};

template<class Key, class Value>
void AVLTree<Key, Value>::rightRotation(AVLNode<Key, Value>* current){
    if(current == nullptr) return;

    AVLNode<Key, Value>* temp = current->getLeft();
    current->setLeft(temp->getRight());

    if(temp->getRight() != nullptr){
        temp->getRight()->setParent(current);
    }

    temp->setParent(current->getParent());

    if(current == this->root_){
        this->root_ = temp;
    }

    else if(current->getParent() != nullptr && current == current->getParent()->getLeft()){
        current->getParent()->setLeft(temp);
    }

    else{
        current->getParent()->setRight(temp);
    }

    temp->setRight(current);
    current->setParent(temp);

    if (temp->getBalance() == -1) {
        current->setBalance(0);
        temp->setBalance(0);
    }
    else if (temp->getBalance() == 0) { 
        current->setBalance(-1);
        temp->setBalance(1);
    }
    else { 
        
    }

}

template<class Key, class Value>
void AVLTree<Key, Value>::leftRotation(AVLNode<Key, Value>* current){
    if(current == nullptr) return;
    AVLNode<Key, Value>* temp = current->getRight();
    current->setRight(temp->getLeft());

    if(temp->getLeft() != nullptr){
        temp->getLeft()->setParent(current);
    }
    
    temp->setParent(current->getParent());

    if(current == this->root_){
        this->root_ = temp;
    }

    else if(current->getParent() != nullptr && current == current->getParent()->getRight()){
        current->getParent()->setRight(temp);
    }

    else{
        current->getParent()->setLeft(temp);
    }

    temp->setLeft(current);
    current->setParent(temp);

    if (temp->getBalance() == 1) {
        current->setBalance(0);
        temp->setBalance(0);
    } 
    
    else if (temp->getBalance() == 0) {
        current->setBalance(1);
        temp->setBalance(-1);
    }
    
}

template<class Key, class Value>
void AVLTree<Key, Value>::leftRight(AVLNode<Key, Value>* current){
    leftRotation(current->getLeft());
    rightRotation(current);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rightLeft(AVLNode<Key, Value>* current){
    rightRotation(current->getRight());
    leftRotation(current);
}


/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if(this->root_ == nullptr){ // case where tree is empty
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        return;
    }
    AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parent = nullptr;

    while(true){ // we are going to loop through the tree to find the proper place to insert
        parent = current;

        if(current->getKey() == new_item.first){
            current->setValue(new_item.second); // overwrites the current value if key already exists
            return;
        }

        else if(current->getKey() > new_item.first){ // The key to be inserted is less than the current node
            if(current->getLeft()== nullptr){ //if the current node has no left child we can insert it here
                AVLNode<Key, Value>* temp = new AVLNode<Key, Value>(new_item.first, new_item.second, current);
                current->setLeft(temp);
                current = current->getLeft();
                break;
            }
            else{
                current = current->getLeft();
            }
        }

        else if(current->getKey() < new_item.first){ // The key to be inserted is greater than the current node
            if(current->getRight()== nullptr){ //if the current node has no right child we can insert it here
                AVLNode<Key, Value>* temp = new AVLNode<Key, Value>(new_item.first, new_item.second, current);
                current->setRight(temp);
                current = current->getRight();
                break;
            }
            else{
                current = current->getRight();
            }
        }
    }
    parent = current->getParent();
    AVLNode<Key, Value>* child = current;
    while(parent != nullptr) {
        if(child == parent->getLeft()) {
            parent->updateBalance(1);
        }
        else {
            parent->updateBalance(-1);
        }
        if(parent->getBalance() == 0) {
            break; 
        }
        else if(abs(parent->getBalance()) == 1) {
            child = parent;
            parent = parent->getParent();
        }
        else { 
            if(parent->getBalance() == 2) {
                if(child->getBalance() == 1) {
                    rightRotation(parent);
                }
                else {
                    leftRight(parent);
                }
            }
            else {
                if(child->getBalance() == -1) {
                    leftRotation(parent); 
                }
                else {
                    rightLeft(parent); 
                }
            }
            break; 
        }
    }

}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->internalFind(key)); //utilizes helper function to find the node to be removed

    if(temp == nullptr){ // check if the tree is empty or key is not found, if so then nothing to be removed
        return;
    }
    AVLNode<Key, Value>* parent = temp->getParent();

    if(temp->getLeft() == nullptr && temp->getRight() == nullptr){ //if no child exists delete temp and update parent
        if(temp->getParent() != nullptr){
            AVLNode<Key, Value>* tempParent = temp->getParent();

            if(tempParent->getRight() == temp){
                tempParent->setRight(nullptr);
            }

            else{
                tempParent->setLeft(nullptr);
            }
        }
        else{

            this->root_ = nullptr;
        } 

        delete temp;
    }

    else if(temp->getLeft() != nullptr && temp->getRight() != nullptr){ //if both children exist then we swap the predecessor and the node to be removed
        removeTwoChildNode(temp);
    }

    else{
        if(temp->getLeft() == nullptr && temp->getRight() != nullptr){ //if right child exists but not left swap right and temp
            if(temp->getParent() != nullptr){
                if(temp->getParent()->getLeft() == temp){
                    temp->getParent()->setLeft(temp->getRight());
                }
                
                else{
                    temp->getParent()->setRight(temp->getRight());
                }

                temp->getRight()->setParent(temp->getParent());
            }

            else{
                this->root_ = temp->getRight();
                this->root_->setParent(nullptr);
            }

            delete temp;
        }

        else if(temp->getLeft() != nullptr && temp->getRight() == nullptr){ //if left child exists but not right swap left and temp
            if(temp->getParent() != nullptr){
                if(temp->getParent()->getLeft() == temp){
                    temp->getParent()->setLeft(temp->getLeft());
                }
                else{
                    temp->getParent()->setRight(temp->getLeft());
                }
                temp->getLeft()->setParent(temp->getParent());
            }
            else{
                this->root_ = temp->getLeft();
                this->root_->setParent(nullptr);
            }
            delete temp;
        }
    }

    while(parent != nullptr){
        int left, right;

        if(parent->getLeft() != nullptr){
            left = parent->getLeft()->getBalance();
        }

        else left = 0;

        if(parent->getRight() != nullptr){
            right = parent->getRight()->getBalance();
        }

        else right = 0;
        
        parent->setBalance(std::max(left, right) + 1);
        int diff = left - right;

        if(diff > 1 && (parent->getLeft() != nullptr && (parent->getLeft()->getLeft() != nullptr))){ //left left case so we need to do a right rotation
            rightRotation(parent);
            break;
        }

        if(diff < -1 && (parent->getRight() != nullptr && parent->getRight()->getRight() != nullptr)){ //right right case so we need to do a left rotation
            leftRotation(parent);
            break;
        }

        if(diff < -1 && (parent->getRight() != nullptr && parent->getRight()->getLeft() != nullptr)){ //right left case
            rightLeft(parent);
            break;
        }

        if(diff > 1 && (parent->getLeft() != nullptr && (parent->getLeft()->getRight() != nullptr))){ //left right case
            leftRight(parent);
            break;
        }

        parent = parent->getParent();
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeTwoChildNode(AVLNode<Key, Value>* current){
    if(current == nullptr){
        return;
    }

    AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(current));
    nodeSwap(current, pred);
    AVLNode<Key, Value>* parent = current->getParent();

    if (current->getLeft() != nullptr) {
        if (parent != nullptr) {
            if (parent->getLeft() == current) {
                parent->setLeft(current->getLeft());
            } 
            
            else {
                parent->setRight(current->getLeft());
            }

        } 
        
        else {
            this->root_ = current->getLeft();
        }

        current->getLeft()->setParent(parent);
    } 

    else{
        if (parent != nullptr) {
            if (parent->getLeft() == current) {
                parent->setLeft(nullptr);
            } 
            
            else {
                parent->setRight(nullptr);
            }
        } 
        
        else {
            this->root_ = nullptr;
        }
    }

    delete current;

    while(parent != nullptr){
        int left, right;

        if(parent->getLeft() != nullptr){
            left = parent->getLeft()->getBalance();
        }

        else left = 0;

        if(parent->getRight() != nullptr){
            right = parent->getRight()->getBalance();
        }

        else right = 0;
        
        parent->setBalance(std::max(left, right) + 1);
        int diff = left - right;

        if(diff > 1 && (parent->getLeft() != nullptr && (parent->getLeft()->getLeft() != nullptr))){ //left left case so we need to do a right rotation
            rightRotation(parent);
            break;
        }
        if(diff < -1 && (parent->getRight() != nullptr && parent->getRight()->getRight() != nullptr)){ //right right case so we need to do a left rotation
            leftRotation(parent);
            break;
        }
        if(diff < -1 && (parent->getRight() != nullptr && parent->getRight()->getLeft() != nullptr)){ //right left case
            rightLeft(parent);
            break;
        }
        if(diff > 1 && (parent->getLeft() != nullptr && (parent->getLeft()->getRight() != nullptr))){ //left right case
            leftRight(parent);
            break;
        }
        parent = parent->getParent();
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
