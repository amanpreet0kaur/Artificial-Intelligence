#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <set>
#include <queue>
#include <climits>

using namespace std;

class Node {
public:
    int nodeNumber;
    int h;
    int g;
    int f;
    Node* parent;

    Node(int hValue, int i) : h(hValue), nodeNumber(i), g(0), f(h + g), parent(nullptr) {}


    bool operator>(const Node& other) const {
        return f > other.f;
    }
};

class graph {
public:
    vector<Node> nodes;

    graph(int size, int hValue[]) {
        for (int i = 0; i < size; i++) {
            nodes.emplace_back(hValue[i], i);
        }
    }
};

void reconstructPath(Node* goal) {
    vector<Node*> path;
    for (Node* node = goal; node != nullptr; node = node->parent) {
        path.push_back(node);
    }
    reverse(path.begin(), path.end());
    cout << "Path: ";
    for (Node* node : path) {
        cout << node->nodeNumber << "\t";
    }
    cout << endl;
}

vector<Node*> getSuccessors(Node* current, int adjMatrix[10][10], graph& g) {
    vector<Node*> successors;

    for (int i = 0; i < 10; i++) {
        if (adjMatrix[current->nodeNumber][i] > 0) {
            Node* successor = &g.nodes[i];
            successors.push_back(successor);
        }
    }
    return successors;
}

void aStar(int adjMatrix[10][10], int startIndex, int goal, graph& g) {
    auto cmp = [](Node* left, Node* right) { return *left > *right; };
    priority_queue<Node*, vector<Node*>, decltype(cmp)> open(cmp);
    set<Node*> closed;
    vector<Node*> allNodes;

    Node* start = &g.nodes[startIndex];
    open.push(start);
    allNodes.push_back(start);

    while (!open.empty()) {
        Node* curr = open.top();
        open.pop();

        if (curr->nodeNumber == goal) {
            reconstructPath(curr);
            return;
        }

        closed.insert(curr);

        vector<Node*> successors = getSuccessors(curr, adjMatrix, g);

        for (Node* successor : successors) {
            if (closed.find(successor) != closed.end()) {
                continue;
            }

            int tentative_g = curr->g + adjMatrix[curr->nodeNumber][successor->nodeNumber];

            if (find(allNodes.begin(), allNodes.end(), successor) == allNodes.end() || tentative_g < successor->g) {
                successor->g = tentative_g;
                successor->f = successor->g + successor->h;
                successor->parent = curr;

                if (find(allNodes.begin(), allNodes.end(), successor) == allNodes.end()) {
                    open.push(successor);
                    allNodes.push_back(successor);
                } else {
                    open.push(successor);  // In case it's already in the queue but with a higher f-value
                }
            }
        }
    }

    cout << "No path found" << endl;
}

int main() {
    int size = 10;

    int adjMatrix[10][10] = {
        {0, 6, 3, 1, 0, 0, 0, 0, 0, 0},
        {6, 0, 2, 6, 3, 4, 0, 0, 0, 0},
        {3, 2, 0, 0, 4, 5, 0, 0, 0, 0},
        {1, 6, 0, 0, 7, 8, 9, 0, 0, 0},
        {0, 3, 4, 7, 0, 6, 9, 9, 0, 0},
        {0, 4, 5, 8, 6, 0, 8, 9, 0, 0},
        {0, 0, 0, 9, 9, 8, 0, 11, 12, 14},
        {0, 0, 0, 0, 9, 9, 11, 0, 14, 15},
        {0, 0, 0, 0, 0, 0, 12, 14, 0, 0},
        {0, 0, 0, 0, 0, 0, 14, 15, 0, 0}
    };

    int hValue[10] = {15, 13, 13, 12, 10, 9, 7, 6, 5, 0};

    graph g(size, hValue);
    int startIndex = 0;
    int goal = 9;
    aStar(adjMatrix, startIndex, goal, g);

    return 0;
}
