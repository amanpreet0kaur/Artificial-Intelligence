#include <iostream>
#include <vector>
#include <queue>
#include <stack>


using namespace std;

void bfs(const vector<vector<int>>& graph, int start) {
    vector<bool> visited(graph.size(), false);
    queue<int> q;
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        cout << node << " ";

        for (int neighbor : graph[node]) {
            if (!visited[neighbor]) {
                q.push(neighbor);
                visited[neighbor] = true;
            }
        }
    }
    cout << endl;
}

void dfs(const vector<vector<int>>& graph, int start) {
    vector<bool> visited(graph.size(), false);
    stack<int> s;
    s.push(start);

    while (!s.empty()) {
        int node = s.top();
        s.pop();
        if (!visited[node]) {
            cout << node << " ";
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

int main() {
    int n, edges;
    cout << "Enter the number of nodes: ";
    cin >> n;

    vector<vector<int>> graph(n);

    cout << "Enter the number of edges: ";
    cin >> edges;

    cout << "Enter the edges (node1 node2):\n";
    for (int i = 0; i < edges; ++i) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u); 
    }

    int start;
    cout << "Enter the starting node: ";
    cin >> start;

    char option;
    cout << "Enter 'B' for Breadth-First Search or 'D' for Depth-First Search: ";
    cin >> option;

    if (option == 'B' || option == 'b') {
        cout << "BFS Order: ";
        bfs(graph, start);
    } else if (option == 'D' || option == 'd') {
        cout << "DFS Order: ";
        dfs(graph, start);
    } else {
        cout << "Invalid option" << endl;
    }

    return 0;
}
