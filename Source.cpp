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

void printVector(vector<long>& numbers) {
    cout << "------------vector:--------------" << endl;
    for (int i = 0; i < numbers.size(); i++) {
        cout << "vec[" << i << "]= " << numbers[i] << endl;
    }
}

void printVector(vector<double>& numbers) {
    cout << "------------vector:--------------" << endl;
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

    ~Node() {

        if (right != NULL) {
            delete this->right;
        }
        if (left != NULL) {
            delete this->left;
        }
    }
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

    BST(long value, long _height = 0) {
        data = value;
        height = _height;
        left = right = NULL;
    }


        BST() {
            data = 0;
            height = 0;
            left = right = NULL;
        }

        ~BST() {

            if (right != NULL) {
                delete right;
            }
            if (left != NULL) {
                delete left;
            }
        }
};
long BST::max_height = 0;

BST* BSTinsert(BST* root, long value, long _height = 0)
{
    //cout << "h: " << _height << " mh: " <<BST::max_height << endl;
    if (_height > BST::max_height)
        BST::max_height = _height;

    if (!root) {
        BST *temp = new BST(value, _height+1);
        
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
    root->height = _height;
    return root;
}

void inOrder(BST* root)
{
    if (!root) {
        return;
    }
    inOrder(root->left);
    cout << root->data <<" h: " << root->height << endl;
    inOrder(root->right);
}

void inOrder(Node* root)
{
    if (!root) {
        return;
    }
    inOrder(root->left);
    cout << root->key << " h: " << root->height << endl;
    inOrder(root->right);
}


BST* makeBSTFromVector(BST* root, vector<long>& num) {

    root = BSTinsert(root, num[0]);
    for (long i = 1; i < num.size(); i++) {
        root = BSTinsert(root, num[i]);
        //cout << "inserting " << num[i] << endl;
    }

    return root;

}

Node* makeAVLFromVector(Node * root, vector<long>& num) {

    //root = b.Insert(root, num[0]);  
    for (long i = 0; i < num.size(); i++) {
        root = insertNode(root, num[i]);
        //cout << "inserting " << num[i] << endl;
    }

    return root;

}
void BSTsearchElement(BST* root, long s) {
    if (!root)
        return;
    if (root->data == s)
    {
        // cout << "found " << s << endl;
        return;
    }
    if (s < root->data) {
        BSTsearchElement(root->left, s);
    }
    else if (s > root->data) { BSTsearchElement(root->right, s); }

    return;

}

void BSTsearchElement(Node* root, long s) {
    if (!root)
        return;
    if (root->key == s)
    {
        // cout << "found " << s << endl;
        return;
    }
    if (s < root->key) {
        BSTsearchElement(root->left, s);
    }
    else if (s > root->key) { BSTsearchElement(root->right, s); }

    return;

}
void BSTsearchList(BST* b, vector<long> num) {
    for (long i = 0; i < num.size(); i++) {
        BSTsearchElement(b, num[i]);
    }
}

void BSTsearchList(Node* b, vector<long> num) {
    for (long i = 0; i < num.size(); i++) {
        BSTsearchElement(b, num[i]);
    }
}

void BSTSeeChild(BST* b) {
    if (!b)
        return;
    
    if ((b->left && b->right)) {
        cout << "curr: " << b->data << " h: " << b->height << " l: " << b->left->data << " r: " << b->right->data << endl;
    }
    if (!b->left && b->right) {
        cout << "curr: " << b->data << " h: " << b->height << " l: NULL " << " r: " << b->right->data << endl;
    }
    if (b->left && !b->right) {
        cout << "curr: " << b->data << " h: " << b->height << " l: " << b->left->data << " r: NULL" << endl;
    }
    if (!b->left && !b->right) {
        cout << "curr: " << b->data << " h: " << b->height << " l: NULL " << " r: NULL" << endl;
    }
    BSTSeeChild(b->left);
    BSTSeeChild(b->right);
    return;
}

void AVLSeeChild(Node* b) {
    if (!b)
        return;

    if ((b->left && b->right)) {
        cout << "curr: " << b->key << " childrens: " << b->height << " l: " << b->left->key << " r: " << b->right->key << endl;
    }
    if (!b->left && b->right) {
        cout << "curr: " << b->key << " childrens: " << b->height << " l: NULL " << " r: " << b->right->key << endl;
    }
    if (b->left && !b->right) {
        cout << "curr: " << b->key << " childrens: " << b->height << " l: " << b->left->key << " r: NULL" << endl;
    }
    if (!b->left && !b->right) {
        cout << "curr: " << b->key << " childrens: " << b->height << " l: NULL " << " r: NULL" << endl;
    }
    AVLSeeChild(b->left);
    AVLSeeChild(b->right);
    return;
}

void saveHeightsToTXT(vector<long> objects, vector<double>heightBST, vector<long>heightAVL) {
    fstream file;
    file.open("data3.txt");
    if (file.good())
    {
        file << "NR\tobjects\theightBST\theightAVL\n";
        for (int i = 0; i < objects.size(); i++)
        {
            file << i + 1 << "\t" << objects[i] << "\t" << heightBST[i] << "\t" << heightAVL[i] << "\n";
        }
        file.close();
    }
    else
    {
        file.close();
        cout << "Not good file\n";
        return;
    }
}

void saveTimesToTXT(vector<long> objects, vector<long> timesCreateBST, vector<long> timesCreateAVL, vector<long> timesSearchBST, vector<long> timesSearchAVL) {
    fstream file;
    file.open("data4_BST_and_AVL.txt");
    if (file.good())
    {
        file << "NR\tobjects\tCreateBST\tCreateAVL\tSearchBST\tSearchAVL\tSummaryBST\ttSummaryAVL\n";
        for (int i = 0; i < objects.size(); i++)
        {
            file << i + 1 << "\t" << objects[i] << "\t" << timesCreateBST[i] << "\t" << timesCreateAVL[i] << "\t" << timesSearchBST[i] << "\t" << timesSearchAVL[i] << "\t" << timesCreateBST[i] + timesSearchBST[i] << "\t" << timesCreateAVL[i] + timesSearchAVL[i] <<"\n";
        }
        file.close();
    }
    else
    {
        file.close();
        cout << "Not good file\n";
        return;
    }
}

int main() {
    
    srand(time(NULL));

    long numOfStartElements = 10;
    long numOfEndElements = 10;
    long step = 1;
    long numOfReps = (numOfEndElements - numOfStartElements) / step;
    cout << "num of reps: " << numOfReps + 1 << endl;

    vector<long> numberOfObjects;

    vector<double> heightBST;
    vector<long> heightAVL;

    vector<long> timesCreateBST;
    vector<long> timesCreateAVL;
    
    vector<long> timesSearchBST;
    vector<long> timesSearchAVL;

    vector <long> num;

    long time_taken = 0;
    //double tempheight = 0;


    for (long i = 0; i <= numOfReps; i++)
    {
        
        
        numberOfObjects.push_back(numOfStartElements);
        cout << "------------  BST --------" << endl;
        //for (int i = 0; i < 5; i++)
        //{
            fillVector(num, numOfStartElements, numOfEndElements);

            // ------ creating BST -----------
            auto start = chrono::steady_clock::now();
            BST* BSTroot = NULL;
            BSTroot = BSTinsert(BSTroot, num[0]);
            BSTroot = makeBSTFromVector(BSTroot, num);
            auto end = chrono::steady_clock::now();
            time_taken = chrono::duration_cast<chrono::microseconds>(end - start).count();
            timesCreateBST.push_back(time_taken);
            //tempheight += double(BST::max_height);
            // ------- searching BST ----------
             start = chrono::steady_clock::now();
            BSTsearchList(BSTroot, num);
             end = chrono::steady_clock::now();
            time_taken = chrono::duration_cast<chrono::microseconds>(end - start).count();

            timesSearchBST.push_back(time_taken);
            inOrder(BSTroot);
            cout << "BST see child\n";
            BSTSeeChild(BSTroot);
            //delete BSTroot;
        //}
        

        //tempheight /= 5;
        //heightBST.push_back(tempheight);
        heightBST.push_back(BST::max_height);
        delete BSTroot;
        //tempheight = 0;



        cout << "------------  AVL --------" << endl;
         start = chrono::steady_clock::now();
        Node* root = NULL;
        root = makeAVLFromVector(root, num);
         end = chrono::steady_clock::now();
        time_taken = chrono::duration_cast<chrono::microseconds>(end - start).count();
        timesCreateAVL.push_back(time_taken);

        // ---------- searching AVL ---------
         start = chrono::steady_clock::now();
        BSTsearchList(root, num);
         end = chrono::steady_clock::now();
        time_taken = chrono::duration_cast<chrono::microseconds>(end - start).count();

        timesSearchAVL.push_back(time_taken);

        heightAVL.push_back(root->height);
        //cout << "root height" << root->height << endl;
        inOrder(root);
        cout << "AVL see child\n";
        AVLSeeChild(root);
        delete root;

        numOfStartElements += step;
    }
    cout << "\n-----heightBST-----\n";
    printVector(heightBST);
    cout << "\n-----heightAVL-----\n";
    printVector(heightAVL);\

    /*
    cout << "\n-----CreationBST-----\n";
    printVector(timesCreateBST);

    cout << "\n-----CreationAVL-----\n";
    printVector(timesCreateAVL);
    cout << "\n-----SearchBST-----\n";
    printVector(timesSearchBST);
    cout << "\n-----SearchAVL-----\n";
    printVector(timesSearchAVL);
    */


    //saveTimesToTXT(numberOfObjects, timesCreateBST, timesCreateAVL, timesSearchBST, timesSearchAVL);
    //saveHeightsToTXT(numberOfObjects, heightBST, heightAVL);
   


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