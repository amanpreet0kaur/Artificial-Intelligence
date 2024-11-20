#include <iostream>
#include <unordered_map>
#include <vector>
#include <list>
#include <queue>

using namespace std;

void addEdge(int u, int v, int dir, unordered_map<int, list<int>>& adjList) {
    adjList[u].push_back(v);
    if (dir == 0) {
        adjList[v].push_back(u);
    }
}

void adjMatrix_to_adjList(vector<vector<int>> adjMatrix, unordered_map<int, list<int>>& adjList) {
    int rows = adjMatrix.size();
    int cols = adjMatrix[0].size();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (adjMatrix[i][j] == 1)
                addEdge(i, j, 1, adjList);
        }
    }
}

void printAdj(const unordered_map<int, list<int>>& adjList) {
    for (const auto& i : adjList) {
        cout << i.first << " -> ";
        for (const auto& j : i.second) {
            cout << j << "\t";
        }
        cout << endl;
    }
}

void beam_constant_width_for_level(const unordered_map<int, list<int>>& adjList, unordered_map<int, bool>& visited, vector<int>& ans, int node, int b) {
    queue<int> q;
    q.push(node);
    visited[node] = true;

    while (!q.empty()) {
        int level_size = q.size(); 
        int count = 0;

        while (level_size-- && count < b) {
            int frontNode = q.front();
            q.pop();
            ans.push_back(frontNode);

            if (adjList.find(frontNode) != adjList.end()) {
                for (const auto& i : adjList.at(frontNode)) {
                    if (!visited[i]) {
                        q.push(i);
                        visited[i] = true;
                    }
                }
            }
            count++;
        }

       
    }
}

void beam_constant_width_for_node(const unordered_map<int, list<int>>& adjList, unordered_map<int, bool>& visited, vector<int>& ans, int node, int b) {
    queue<int> q;
    q.push(node);
    visited[node] = true;
    int count = 1;

    while (!q.empty() && count <= b) {
        int frontNode = q.front();
        q.pop();
        ans.push_back(frontNode);

        if (adjList.find(frontNode) != adjList.end()) {
            for (const auto& i : adjList.at(frontNode)) {
                if (!visited[i]) {
                    if (count < b) {
                        q.push(i);
                        visited[i] = true;
                        count++;
                    } else {
                        return;
                    }
                }
            }
        }
    }
}

vector<int> BEAM(int nodes, const unordered_map<int, list<int>>& adjList, int b) {
    vector<int> ans;
    int choice;
    unordered_map<int, bool> visited;
    cout << "Enter 1 for BEAM traversal with constant width at each level and 0 for constant width for nodes: ";
    cin >> choice;
    for (int i = 0; i < nodes; ++i) {
        if (!visited[i]) {
            if (choice == 1)
                beam_constant_width_for_level(adjList, visited, ans, i, b);
            else if (choice == 0)
                beam_constant_width_for_node(adjList, visited, ans, i, b);
            else {
                cout << "Wrong input" << endl;
                exit(1);
            }
        }
    }
    return ans;
}

int main() {
     vector<vector<int>> adjMatrix = {
        {0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0}, 
        {1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0}, 
        {0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}, 
        {0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0}, 
        {0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0}, 
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, 
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1}, 
        {0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0}, 
        {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1}, 
        {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0}
    };
    unordered_map<int, list<int>> adjList;
    adjMatrix_to_adjList(adjMatrix, adjList);
    int b;
    cout << "Enter the beam width: ";
    cin >> b;
    cout << "Printing adjList:" << endl;
    printAdj(adjList);
    cout << endl;

    vector<int> ans = BEAM(adjMatrix.size(), adjList, b);
    cout << "Beam traversal: ";
    for (const auto& node : ans) {
        cout << node << "\t";
    }
    return 0;
}
