#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <set>
#include <queue>
#include <stack>
#include <climits>
#include <list>

using namespace std;

class Node {
public:
    int nodeNumber;
    int h; // Heuristic value
    int g; // Cost from start
    int f; // Total cost (g + h)
    Node* parent;

    Node(int hValue, int i) : h(hValue), nodeNumber(i), g(0), f(h + g), parent(nullptr) {}

    bool operator>(const Node& other) const {
        return f > other.f;
    }
};

class Graph {
public:
    vector<Node> nodes;

    Graph(int size, int hValue[]) {
        for (int i = 0; i < size; i++) {
            nodes.emplace_back(hValue[i], i);
        }
    }
};

void reconstructPath(Node* goal, vector<char>& nodeMapping) {
    vector<Node*> path;
    for (Node* node = goal; node != nullptr; node = node->parent) {
        path.push_back(node);
    }
    reverse(path.begin(), path.end());
    cout << "Path: ";
    for (Node* node : path) {
        cout << nodeMapping[node->nodeNumber] << "\t";
    }
    cout << endl;
}

vector<Node*> getSuccessors(Node* current, int adjMatrix[10][10], Graph& g) {
    vector<Node*> successors;
    for (int i = 0; i < 10; i++) {
        if (adjMatrix[current->nodeNumber][i] > 0) {
            Node* successor = &g.nodes[i];
            successors.push_back(successor);
        }
    }
    return successors;
}

void aStar(int adjMatrix[10][10], int startIndex, int goal, Graph& g, vector<char>& nodeMapping) {
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
            reconstructPath(curr, nodeMapping);
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
                    open.push(successor);
                }
            }
        }
    }

    cout << "No path found" << endl;
}

void bfs(const vector<vector<int>>& graph, int start, vector<char>& nodeMapping) {
    vector<bool> visited(graph.size(), false);
    queue<int> q;
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        cout << nodeMapping[node] << " ";

        for (int neighbor : graph[node]) {
            if (!visited[neighbor]) {
                q.push(neighbor);
                visited[neighbor] = true;
            }
        }
    }
    cout << endl;
}

void dfs(const vector<vector<int>>& graph, int start, vector<char>& nodeMapping) {
    vector<bool> visited(graph.size(), false);
    stack<int> s;
    s.push(start);

    while (!s.empty()) {
        int node = s.top();
        s.pop();
        if (!visited[node]) {
            cout << nodeMapping[node] << " ";
            visited[node] = true;
            for (int neighbor : graph[node]) {
                if (!visited[neighbor]) {
                    s.push(neighbor);
                }
            }
        }
    }
    cout << endl;
}

void ids(const unordered_map<int, list<int>>& adjList, unordered_map<int, bool>& visited,
         vector<int>& ans, int node, int depthLimit) {
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

void IDS(int nodes, const unordered_map<int, list<int>>& adjList, int maxDepth, vector<char>& nodeMapping) {
    if (maxDepth == 0) {
        cout << "Depth zero returns no nodes" << endl;
        return;
    }

    for (int depth = 1; depth <= maxDepth; ++depth) {
        unordered_map<int, bool> visited;
        vector<int> ans;
        ids(adjList, visited, ans, 0, depth - 1);
        cout << "IDS traversal of depth: " << depth << endl;
        for (const auto& node : ans) {
            cout << nodeMapping[node] << "\t";
        }
        cout << endl;
    }
}

void beam_constant_width_for_level(const unordered_map<int, list<int>>& adjList, unordered_map<int, bool>& visited, vector<int>& ans, int startNode, int b, const vector<Node>& nodes) {
    queue<int> q;
    q.push(startNode);
    visited[startNode] = true;

    while (!q.empty()) {
        int level_size = q.size();
        vector<Node*> levelNodes;  // Nodes at the current level

        while (level_size--) {
            int frontNode = q.front();
            q.pop();
            levelNodes.push_back(const_cast<Node*>(&nodes[frontNode]));  // Fixing the type mismatch

            if (adjList.find(frontNode) != adjList.end()) {
                for (const auto& neighbor : adjList.at(frontNode)) {
                    if (!visited[neighbor]) {
                        q.push(neighbor);
                        visited[neighbor] = true;
                    }
                }
            }
        }

        // Sort level nodes based on their g value
        sort(levelNodes.begin(), levelNodes.end(), [](Node* a, Node* b) {
            return a->g < b->g;
        });

        // Add nodes to the answer, keeping only `b` nodes
        for (int i = 0; i < min(b, (int)levelNodes.size()); ++i) {
            ans.push_back(levelNodes[i]->nodeNumber);
        }
    }
}

void beam_constant_width_for_node(const unordered_map<int, list<int>>& adjList, unordered_map<int, bool>& visited, vector<int>& ans, int startNode, int b, const vector<Node>& nodes) {
    auto cmp = [](Node* left, Node* right) { return left->g > right->g; };
    priority_queue<Node*, vector<Node*>, decltype(cmp)> pq(cmp);
    pq.push(const_cast<Node*>(&nodes[startNode]));  // Fixing the type mismatch
    visited[startNode] = true;

    while (!pq.empty() && ans.size() < b) {
        Node* curr = pq.top();
        pq.pop();
        ans.push_back(curr->nodeNumber);

        if (adjList.find(curr->nodeNumber) != adjList.end()) {
            for (const auto& neighbor : adjList.at(curr->nodeNumber)) {
                if (!visited[neighbor]) {
                    Node* successor = const_cast<Node*>(&nodes[neighbor]);  // Fixing the type mismatch
                    pq.push(successor);
                    visited[neighbor] = true;
                }
            }
        }
    }
}

vector<int> BEAM(int nodes, const unordered_map<int, list<int>>& adjList, int b, const vector<Node>& nodeList) {
    vector<int> ans;
    int choice;
    unordered_map<int, bool> visited;
    cout << "Enter 1 for BEAM traversal with constant width at each level and 0 for constant width for nodes: ";
    cin >> choice;

    for (int i = 0; i < nodes; ++i) {
        if (!visited[i]) {
            if (choice == 1)
                beam_constant_width_for_level(adjList, visited, ans, i, b, nodeList);
            else if (choice == 0)
                beam_constant_width_for_node(adjList, visited, ans, i, b, nodeList);
            else {
                cout << "Wrong input" << endl;
                exit(1);
            }
        }
    }
    return ans;
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

    // Create a mapping from node indices to characters
    vector<char> nodeMapping(size);
    for (int i = 0; i < size; ++i) {
        nodeMapping[i] = 'a' + i;
    }

    Graph g(size, hValue);
    int startIndex = 0;
    int goal = 9;

    cout << "Running A* algorithm:" << endl;
    aStar(adjMatrix, startIndex, goal, g, nodeMapping);

    cout << "Running BFS algorithm:" << endl;
    vector<vector<int>> graph(size);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (adjMatrix[i][j] > 0) {
                graph[i].push_back(j);
            }
        }
    }
    bfs(graph, startIndex, nodeMapping);

    cout << "Running DFS algorithm:" << endl;
    dfs(graph, startIndex, nodeMapping);

    unordered_map<int, list<int>> adjList;
    for (int i = 0; i < size; ++i) {
        list<int> neighbors;
        for (int j = 0; j < size; ++j) {
            if (adjMatrix[i][j] > 0) {
                neighbors.push_back(j);
            }
        }
        adjList[i] = neighbors;
    }

    cout << "Running IDS algorithm:" << endl;
    IDS(size, adjList, 3, nodeMapping);

    cout << "Running BEAM algorithm:" << endl;
    int b = 2;
    vector<int> beamResult = BEAM(size, adjList, b, g.nodes);
    for (int node : beamResult) {
        cout << nodeMapping[node] << " ";
    }
    cout << endl;

    return 0;
}
