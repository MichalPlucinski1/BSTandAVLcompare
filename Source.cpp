// AVL tree implementation in C++

#include <iostream>

#include<vector>
#include<algorithm>
#include<cstdlib>
#include<ctime>
#include <chrono>
#include <fstream>
#include <iomanip>

using namespace std;
using namespace std::chrono;



void fillVector(vector<long>& numbers, long size, long max_size)
{
    long random = rand() % (max_size)+1;

    for (int i = numbers.size(); i < size; i++)
    {
        while (find(numbers.begin(), numbers.end(), random) != numbers.end()) 
            random = rand() % (max_size * 3) + 1;
        numbers.push_back(random);
    }
}

void prlongVector(vector<long>& numbers) {
    for (int i = 0; i < numbers.size(); i++) {
        cout << "vec[" << i << "]= " << numbers[i] << endl;
    }
}


// ------------------------ AVL ----------------------------

class Node {
public:
    int key;
    Node* left;
    Node* right;
    int height;
};

int max(int a, int b);

// Calculate height
int height(Node* N) {
    if (N == NULL)
        return 0;
    return N->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

// New node creation
Node* newNode(int key) {
    Node* node = new Node();
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return (node);
}

// Rotate right
Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left),
        height(y->right)) +
        1;
    x->height = max(height(x->left),
        height(x->right)) +
        1;
    return x;
}

// Rotate left
Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left),
        height(x->right)) +
        1;
    y->height = max(height(y->left),
        height(y->right)) +
        1;
    return y;
}

// Get the balance factor of each node
int getBalanceFactor(Node* N) {
    if (N == NULL)
        return 0;
    return height(N->left) -
        height(N->right);
}

// Insert a node
Node* insertNode(Node* node, int key) {
    // Find the correct postion and insert the node
    if (node == NULL)
        return (newNode(key));
    if (key < node->key)
        node->left = insertNode(node->left, key);
    else if (key > node->key)
        node->right = insertNode(node->right, key);
    else
        return node;

    // Update the balance factor of each node and
    // balance the tree
    node->height = 1 + max(height(node->left),
        height(node->right));
    int balanceFactor = getBalanceFactor(node);
    if (balanceFactor > 1) {
        if (key < node->left->key) {
            return rightRotate(node);
        }
        else if (key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
    }
    if (balanceFactor < -1) {
        if (key > node->right->key) {
            return leftRotate(node);
        }
        else if (key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
    }
    return node;
}

// Node with minimum value
Node* nodeWithMimumValue(Node* node) {
    Node* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

// Delete a node
Node* deleteNode(Node* root, int key) {
    // Find the node and delete it
    if (root == NULL)
        return root;
    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        if ((root->left == NULL) ||
            (root->right == NULL)) {
            Node* temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            }
            else
                *root = *temp;
            free(temp);
        }
        else {
            Node* temp = nodeWithMimumValue(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right,
                temp->key);
        }
    }

    if (root == NULL)
        return root;

    // Update the balance factor of each node and
    // balance the tree
    root->height = 1 + max(height(root->left),
        height(root->right));
    int balanceFactor = getBalanceFactor(root);
    if (balanceFactor > 1) {
        if (getBalanceFactor(root->left) >= 0) {
            return rightRotate(root);
        }
        else {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
    }
    if (balanceFactor < -1) {
        if (getBalanceFactor(root->right) <= 0) {
            return leftRotate(root);
        }
        else {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }
    }
    return root;
}

// Print the tree
void printTree(Node* root, string indent, bool last) {
    if (root != nullptr) {
        cout << indent;
        if (last) {
            cout << "R----";
            indent += "   ";
        }
        else {
            cout << "L----";
            indent += "|  ";
        }
        cout << root->key << endl;
        printTree(root->left, indent, false);
        printTree(root->right, indent, true);
    }
}


// ------------------------ BST ----------------------------

class BST {
public:
    long data, height;
    BST* left, * right;
    static long max_height;

    BST(long value, long _height) {
        data = value;
        height = _height;
        left = right = NULL;
    }

    BST(long value) {
        data = value;
        height = 0;
        left = right = NULL;
    }

        BST() {
            data = 0;
            height = 0;
            left = right = NULL;
        }
};
long BST::max_height = 0;

BST* BSTinsert(BST* root, long value, long _height = 0)
{
    cout << "h: " << _height << " mh: " <<BST::max_height << endl;
    if (_height > BST::max_height)
        BST::max_height = _height;

    if (!root) {
        BST *temp = new BST(value);
        
        // Insert the first node, if root is NULL.
        return temp;
    }
    // Insert data.
    if (value > root->data) {
        root->right = BSTinsert(root->right, value, _height+=1);
    }
    else if (value < root->data) {
        root->left = BSTinsert(root->left, value, _height+=1);
    }

    // Return 'root' node, after insertion.
    //root->height = _height;
    
    return root;
}

void inOrder(BST* root)
{
    if (!root) {
        return;
    }
    inOrder(root->left);
    cout << root->data << endl;
    inOrder(root->right);
}


BST* makeBSTFromVector(BST* root, vector<long>& num) {

    root = BSTinsert(root, num[0]);
    for (long i = 1; i < num.size(); i++) {
        root = BSTinsert(root, num[i]);
        cout << "inserting " << num[i] << endl;
    }

    return root;

}

Node* makeAVLFromVector(Node * root, vector<long>& num) {

    //root = b.Insert(root, num[0]);  
    for (long i = 0; i < num.size(); i++) {
        root = insertNode(root, num[i]);
        cout << "inserting " << num[i] << endl;
    }

    return root;

}
int main() {
    
    srand(time(NULL));
    vector <long> num;
    fillVector(num, 11, 10);


    prlongVector(num);


    BST* BSTroot = NULL;
    BSTroot = BSTinsert(BSTroot, num[0]);
    BSTroot = makeBSTFromVector(BSTroot, num);
    inOrder(BSTroot);

    cout << "wysokosc: " << BST::max_height;


    /*Node* root = NULL;
    root = insertNode(root, 33);
    root = insertNode(root, 13);
    root = insertNode(root, 53);
    root = insertNode(root, 9);
    root = insertNode(root, 21);
    root = insertNode(root, 61);
    root = insertNode(root, 8);
    root = insertNode(root, 11);

    
    root = makeAVLFromVector(root, num);

    cout <<"root height" << root->height << endl;

    printTree(root, "", true);*/





    //root = deleteNode(root, 13);
    //cout << "After deleting " << endl;
    //printTree(root, "", true);
}