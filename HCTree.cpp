#include "HCTree.hpp"
#include "Helper.hpp"
#include <queue>
#include <vector>

//destructor helper
void deleteTree(HCNode* node) {
    if (node == nullptr) {
        return;
    }
    deleteTree(node->c0);
    deleteTree(node->c1);
    delete node;
}
//destructor
HCTree::~HCTree() {
    deleteTree(root);
    root = nullptr;
}

//build
void HCTree::build(const vector<int>& freqs) {
    
    //clears any exisiting tree preventing leaks
    deleteTree(root);
    root = nullptr;

    for(int i = 0; i < 256; i++) {
        leaves[i] = nullptr;
    }

    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;

    for (int i = 0; i < 256; i++) {
        if (freqs[i] > 0) {
            leaves[i] = new HCNode(freqs[i], (unsigned char)i);
            pq.push(leaves[i]); 
        }
    }
    while (pq.size() > 1) {
        HCNode* firstNode = pq.top(); pq.pop();
        HCNode* secondNode = pq.top(); pq.pop();

        HCNode* parentNode = new HCNode(firstNode->count + secondNode->count, 0);

        parentNode->c0 = firstNode;
        parentNode->c1 = secondNode;
        firstNode->p = parentNode;
        secondNode->p = parentNode;

        pq.push(parentNode);
    }
    if (!pq.empty()) {
        root = pq.top();
    }
}

//encode
void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const {
    HCNode* currentNode = leaves[symbol];
    if (!currentNode) return;

    vector<int> path;

    while (currentNode != root) {
        HCNode* parentNode = currentNode->p; 
        if (parentNode->c0 == currentNode) {
            path.push_back(0);
        } else {
            path.push_back(1);
        }
        currentNode = parentNode;
    }
    for (int i = path.size() - 1; i >= 0; i--) {
        out.write_bit(path[i]);
    }
}

//decode
unsigned char HCTree::decode(FancyInputStream & in) const {
    if (root == nullptr) {
        return -1;
    }
    HCNode* currentNode = root;

    while (currentNode->c0 != nullptr || currentNode->c1 != nullptr) {
        int bit = in.read_bit();
        if (bit == -1) return -1;

        if (bit == 0) {
            currentNode = currentNode->c0;
        } else {
            currentNode = currentNode->c1;
        }
    }
    return (int)currentNode->symbol;
}
