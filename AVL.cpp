
#include "AVL.hpp"

// ------------------- PUBLIC ------------------- :

void AVL::print() {
    print(root, 0);
}

void AVL::insertInterval(int start, int end) {
    root = insertInterval(root, start, end);
}

void AVL::deleteInterval(int start, int end) {
    root = deleteInterval(root, start, end);
}

bool AVL::overlaps(int start, int end) {
    return overlaps(root, start, end);
}

bool AVL::contains(int key) {
    return contains(root, key);
}

// ------------------- PRIVATE ------------------- :

void AVL::print(TreeNode* cur, int spaces) {
    if (!cur) {
        return;
    }
    print(cur->right, spaces + 3);
    cout << string(spaces, ' ') << "[" << cur->interval.start << ", " << cur->interval.end << "]\n";
    print(cur->left, spaces + 3);
}


int AVL::getBalance(TreeNode* node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}


AVL::TreeNode* AVL::insertInterval(TreeNode* node, int start, int end) {
    if (!node) {
        return new TreeNode(start, end);
    }

    if (start < node->interval.start) {
        node->left = insertInterval(node->left, start, end);
    }
    else if (start > node->interval.start) {
        node->right = insertInterval(node->right, start, end);
    }
    else return node;

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    int balance = getBalance(node);

    if (balance > 1 && start < node->left->interval.start) {
        return rotateRight(node);
    }

    if (balance < -1 && start > node->right->interval.start) {
        return rotateLeft(node);
    }

    if (balance > 1 && start > node->left->interval.start) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && start < node->right->interval.start) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}


AVL::TreeNode* AVL::rotateLeft(TreeNode* node) {
    if (node->right) {
        TreeNode* tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        tmp->height = 1 + max(getHeight(tmp->left), node->height);
        return tmp;
    }
    return node;
}


AVL::TreeNode* AVL::rotateRight(TreeNode* node) {
    if (node->left) {
        TreeNode* tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;

        node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
        tmp->height = max(getHeight(tmp->left), node->height) + 1;
        return tmp;
    }
    return node;
}


int AVL::getHeight(TreeNode* node) {
    return node ? node->height : 0;
}

bool inside(int x, int start, int end) {
    return start <= x && x <= end;
}

bool AVL::overlaps(TreeNode* node, int start, int end) {
    if (!node) {
        return false;
    }

    if (inside(start, node->interval.start, node->interval.end) || inside(end, node->interval.start, node->interval.end)) {
        return true;
    }

    if (node->interval.start >= end) {
        return overlaps(node->left, start, end);
    }
    return overlaps(node->right, start, end);
}


bool AVL::contains(TreeNode* cur, int key) {
    if (!cur) {
        return false;
    }
    if (cur->interval.start >= key && key <= cur->interval.end) {
        return true;
    }
    return contains((cur->interval.start > key) ? cur->left : cur->right, key);
}


AVL::TreeNode* AVL::minValueNode(TreeNode* node) {
    TreeNode* cur = node;
    while (cur->left)
        cur = cur->left;
    return cur;
}


AVL::TreeNode* AVL::deleteInterval(TreeNode* node, int start, int end) {
    if (!node)
        return node;

    if (start < node->interval.start) {
        node->left = deleteInterval(node->left, start, end);
    }
    else if (start > node->interval.start) {
        node->right = deleteInterval(node->right, start, end);
    }

    else {
        if (!node->left || !node->right) {
            TreeNode* tmp = (node->left) ? node->left : node->right;

            if (!tmp) {
                tmp = node;
                node = NULL;
            }
            else *node = *tmp;
            free(tmp);
        }
        else {
            TreeNode* tmp = minValueNode(node->right);
            node->interval = tmp->interval;
            node->right = deleteInterval(node->right, tmp->interval.start, tmp->interval.end);
        }
    }


    if (!node)
        return node;

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    int balance = getBalance(node);
    if (balance > 1 && getBalance(node->left) >= 0) {
        return rotateRight(node);
    }

    if (balance < -1 && getBalance(node->right) <= 0) {
        return rotateLeft(node);
    }

    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}