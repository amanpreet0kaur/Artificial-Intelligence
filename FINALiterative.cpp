#include <iostream>
#include <unordered_map>
#include <vector>
#include <list>
#include <stack>

using namespace std;

// Function to add an edge to the adjacency list
void addEdge(int u, int v, int dir, unordered_map<int, list<int>>& adjList) {
    adjList[u].push_back(v);
    if (dir == 0) {
        adjList[v].push_back(u);
    }
}

// Function to convert adjacency matrix to adjacency list
void adjMatrix_to_adjList(const vector<vector<int>>& adjMatrix, unordered_map<int, list<int>>& adjList) {
    int rows = adjMatrix.size();
    int cols = adjMatrix[0].size();
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (adjMatrix[i][j] == 1) {
                addEdge(i, j, 1, adjList);
            }
        }
    }
}

// Function to print the adjacency list
void printAdj(const unordered_map<int, list<int>>& adjList) {
    for (const auto& i : adjList) {
        cout << i.first << " -> ";
        for (const auto& j : i.second) {
            cout << j << "\t";
        }
        cout << endl;
    }
}

// Iterative Deepening Search (IDS) function
void ids(const unordered_map<int, list<int>>& adjList, unordered_map<int, bool>& visited, vector<int>& ans, int node, int depthLimit) {
    stack<pair<int, int>> s;
    s.push({node, 0});

    while (!s.empty()) {
        int currentNode = s.top().first;
        int currentDepth = s.top().second;
        s.pop();

        if (currentDepth <= depthLimit && !visited[currentNode]) {
            visited[currentNode] = true;
            ans.push_back(currentNode);

            if (currentDepth < depthLimit) {
                if (adjList.find(currentNode) != adjList.end()) {
                    for (const auto& neighbor : adjList.at(currentNode)) {
                        if (!visited[neighbor]) {
                            s.push({neighbor, currentDepth + 1});
                        }
                    }
                }
            }
        }
    }
}

// Function to perform IDS for all depths from 1 to maxDepth
void IDS(int nodes, const unordered_map<int, list<int>>& adjList, int maxDepth) {
    if (maxDepth == 0) {
        cout << "Depth zero returns no nodes" << endl;
        return;
    }

    for (int depth = 1; depth <= maxDepth; ++depth) {
        unordered_map<int, bool> visited;
        vector<int> ans;
        ids(adjList, visited, ans, 0, depth - 1); // Start from node 0 with depth - 1
        cout << "IDS traversal of depth: " << depth << endl;
        for (const auto& node : ans) {
            cout << node << "\t";
        }
        cout << endl;
    }
}

int main() {
    int nodes, edges;
    cout << "Enter number of nodes: ";
    cin >> nodes;
    cout << "Enter number of edges: ";
    cin >> edges;

    unordered_map<int, list<int>> adjList;
    cout << "Enter edges (format: node1 node2):" << endl;
    for (int i = 0; i < edges; ++i) {
        int node1, node2;
        cin >> node1 >> node2;
        addEdge(node1, node2, 1, adjList); // Directed edge by default
    }

    int depth;
    cout << "Enter the maximum depth: ";
    cin >> depth;

    cout << "Printing adjacency list:" << endl;
    printAdj(adjList);
    cout << endl;

    IDS(nodes, adjList, depth);

    return 0;
}
