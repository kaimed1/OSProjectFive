#include <iostream>
#include <vector>
#include <pthread.h>
#include <sstream>
#include <fstream>
using namespace std;

pthread_mutex_t print_mutex;  // Global mutex for synchronizing output

// Tree node structure built entirely from input.
struct TreeNode {
    int index;
    int level;
    int position;
    vector<int> values;  // For leaf nodes, holds the chunk of input.
    int sum;
    pthread_t tid;
    TreeNode* left;
    TreeNode* right;
    
    TreeNode(int idx, int lvl, int pos, const vector<int>& vals)
        : index(idx), level(lvl), position(pos), values(vals), sum(0),
          left(nullptr), right(nullptr) {}
};

void* computeSum(void* arg) {
    TreeNode* node = reinterpret_cast<TreeNode*>(arg);
    node->tid = pthread_self(); // Assign the correct thread ID
    node->sum = 0;
    for (int v : node->values)
        node->sum += v;
    pthread_mutex_lock(&print_mutex);
    cout << "[Thread Index " << node->index << "] [Level " << node->level
         << ", Position " << node->position << "] [TID " << node->tid
         << "] computed leaf sum: " << node->sum << endl;
    pthread_mutex_unlock(&print_mutex);
    pthread_exit(nullptr);
}

void* computeParentSum(void* arg) {
    TreeNode* node = reinterpret_cast<TreeNode*>(arg);
    node->tid = pthread_self(); // Assign the correct thread ID

    // Wait for child threads to complete
    if (node->left) pthread_join(node->left->tid, nullptr);
    if (node->right) pthread_join(node->right->tid, nullptr);

    // Compute the sum from child nodes
    node->sum = 0; // Initialize sum
    if (node->left) node->sum += node->left->sum; // Add left child's sum
    if (node->right) node->sum += node->right->sum; // Add right child's sum

    pthread_mutex_lock(&print_mutex);
    cout << "[Thread Index " << node->index << "] [Level " << node->level
         << ", Position " << node->position << "] [TID " << node->tid
         << "] received: " << endl;
    if (node->left) {
        cout << "Left Child [Index " << node->left->index << ", Level " << node->left->level
             << ", Position " << node->left->position << ", TID " << node->left->tid
             << "]: " << node->left->sum << endl;
    }
    if (node->right) {
        cout << "Right Child [Index " << node->right->index << ", Level " << node->right->level
             << ", Position " << node->right->position << ", TID " << node->right->tid
             << "]: " << node->right->sum << endl;
    }
    cout << "[Thread Index " << node->index << "] computed sum: " << node->sum << endl;
    pthread_mutex_unlock(&print_mutex);

    pthread_exit(nullptr);
}

void createThreads(TreeNode* node) {
    if (!node) return;

    // Create threads for child nodes first (bottom-up approach)
    createThreads(node->left);
    createThreads(node->right);

    // Create thread for the current node
    if (node->left || node->right) { // Parent nodes
        pthread_create(&node->tid, nullptr, computeParentSum, node);
    } else { // Leaf nodes
        pthread_create(&node->tid, nullptr, computeSum, node);
    }
}

void cleanupTreeAdjusted(TreeNode* node) {
    if (!node)
        return;
    cleanupTreeAdjusted(node->left);
    cleanupTreeAdjusted(node->right);
    pthread_join(node->tid, nullptr); // Ensure all threads are joined
    pthread_mutex_lock(&print_mutex);
    cout << "[Thread Index " << node->index << "] [Level " << node->level
         << ", Position " << node->position << "] terminated." << endl;
    pthread_mutex_unlock(&print_mutex);
    delete node;
}

int main(int argc, char* argv[]) { 
    pthread_mutex_init(&print_mutex, nullptr);

    int levels, num_values;
    cin >> levels >> num_values;
    vector<int> values;

    // Read num_values numbers from input
    for (int i = 0; i < num_values; ++i) {
        int value;
        cin >> value;
        values.push_back(value);
    }

    // Step 2: Compute tree parameters
    int N = 1 << (levels - 1); // Number of leaf nodes
    int Mprime = values.size();
    if (Mprime % N != 0) {
        int pad = N - (Mprime % N);
        for (int i = 0; i < pad; i++)
            values.push_back(0); // Pad with zeros to make divisible
        Mprime = values.size();
    }
    int chunkSize = Mprime / N; // Size of each chunk for leaf nodes
    int totalThreads = (1 << levels) - 1; // Total number of nodes in the tree

    // Step 3: Build the tree and allocate node data.
    vector<TreeNode*> nodes(totalThreads); // Preallocate space for all nodes
    int index = 0;
    for (int level = 1; level <= levels; ++level) {
        int numNodes = 1 << (level - 1); // Number of nodes at this level
        for (int i = 0; i < numNodes; ++i) {
            vector<int> nodeValues;
            if (level == levels) { // Leaf nodes
                int start = i * chunkSize;
                int end = (i + 1) * chunkSize - 1;
                for (int j = start; j <= end; ++j)
                    nodeValues.push_back(values[j]);
            }
            nodes[index] = new TreeNode(index, level, i, nodeValues); // Create node
            index++;
        }
    }

    // Step 4: Link parent nodes to children.
    for (int i = 0; i < (1 << (levels - 1)) - 1; ++i) { // Only iterate over non-leaf nodes
        nodes[i]->left = nodes[2 * i + 1];
        nodes[i]->right = nodes[2 * i + 2];
    }

    // Step 5: Create all threads in bottom-up order.
    createThreads(nodes[0]);

    // Wait for root's computation to finish.
    pthread_join(nodes[0]->tid, nullptr);

    pthread_mutex_lock(&print_mutex);
    cout << endl << "[Thread Index 0] now initiates tree cleanup." << endl << endl;
    pthread_mutex_unlock(&print_mutex);

    // Cleanup tree
    cleanupTreeAdjusted(nodes[0]); // Ensure proper cleanup of the entire tree

    // Destroy the mutex.
    pthread_mutex_destroy(&print_mutex);
    return 0;
}
