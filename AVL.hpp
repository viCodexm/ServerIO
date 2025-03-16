
#pragma once
#include <iostream>

using namespace std;


struct Interval {
    int start;
    int end;

    bool operator<(const Interval& other) const {
        return this->end < other.start;
    }
};


struct AVL {

    struct TreeNode {
        Interval interval;
        int height;
        TreeNode* left;
        TreeNode* right;

        TreeNode(int start, int end) : interval({start, end}), height(1), left(nullptr), right(nullptr) {}
    };
    TreeNode* root;


    AVL() { root = nullptr; }

    void print();
    void insertInterval(int start, int end);
    void deleteInterval(int start, int end);
    bool overlaps(int start, int end);
    bool contains(int key);


private:
    void print(TreeNode* cur, int spaces);
    TreeNode* insertInterval(TreeNode* node, int start, int end);
    TreeNode* deleteInterval(TreeNode* node, int start, int end);
    bool overlaps(TreeNode* node, int start, int end);
    bool contains(TreeNode* cur, int key);

    TreeNode* rotateLeft(TreeNode* node);
    TreeNode* rotateRight(TreeNode* node);

    int getHeight(TreeNode* node);
    int getBalance(TreeNode* node);
    
    TreeNode* minValueNode(TreeNode* node);
};