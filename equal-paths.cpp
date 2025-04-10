#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

bool depthFinder(Node* node, int depth, int& leafDepth);


bool equalPaths(Node * root)
{
    // Add your code below
    if (root == nullptr){
        return true;
    }

    int leafDepth = -1; // This will track our first leaf nodes depth which we will use to compare with every leaf afterwards
    return depthFinder(root, 0, leafDepth);
}

bool depthFinder(Node* node, int depth, int& leafDepth){
    if (node == nullptr) return true;

    if(node->left == nullptr && node->right == nullptr){ // leaf node
        if(leafDepth == -1){
            leafDepth = depth;
        }
        return leafDepth == depth;
    }

    return depthFinder(node->left, depth + 1, leafDepth) && depthFinder(node->right, depth + 1, leafDepth);

}

