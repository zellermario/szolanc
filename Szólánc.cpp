#include <algorithm>
#include <iostream>
#include <cassert>
#include <vector>
#include <string>

using namespace std;

/* Checks if two strings are one character away in edit distance,
   i.e. one can be transformed to the other by a single insertion, 
   deletion, or replacement. 
*/
bool areOneAway(string a, string b) {
	if (a.size() < b.size())
		swap(a, b);					// Make sure "a" is the longer one
	if (a.size() - b.size() > 1)
		return false;
	if (a.size() == b.size()) {
		int difference = 0;
		for (size_t i = 0; i < a.size(); ++i)
			if (a[i] != b[i])
				++difference;
		return difference == 1;
	}
	else {
		for (size_t i = 0; i < a.size(); ++i) {
			string copyOfa = a;
			copyOfa.erase(i, 1);
			if (copyOfa == b)			
				return true;		// Deleting character i from "a" yields "b"
		}
		return false;
	}
}

/* Represents a graph using its adjecency matrix */
class Graph {
	int size;
	bool** matrix;
public:
	Graph(size_t size) : size(size) {
		matrix = new bool*[size]{};
		for (size_t i = 0; i < size; ++i) {
			matrix[i] = new bool[size]{};
		}
	}
	bool& at(int i, int j) {
		return matrix[i][j];
	}
	int vertices() { return size; }
};

/* Performs BFS traversal of the graph. */
void BFS(Graph& G, bool visited[], int start = 0) {
	visited[start] = true;
	for (int i = 0; i < G.vertices(); ++i) {
		if (G.at(start, i) && !visited[i]) {
			BFS(G, visited, i);
		}
	}
}

/* Checks if the graph is connected by verifying if all 
   vertices are visited when running a BFS traversal. */
bool isConnected(Graph& G) {
	bool* visited = new bool[G.vertices()]{};
	BFS(G, visited);
	for (int i = 0; i < G.vertices(); ++i)
		if (!visited[i]) return false;
	return true;
}

/* The classical dynamic programming algorithm for findig a 
   Hamiltonian path in the Graph in O(n^2*2^n) running time. 
   Returns an appropriate ordering of the vertices.
*/ 
vector<int> findHamiltonianPath(Graph& G) {
	/* HAM[S][i] stores if the S subset of vertices has
	   a Hamiltonian path ending in vertex i and from where.
	   Vertex i is in subset S if bit i of S is 1.
	*/
	pair<bool, int>** HAM = new pair<bool, int>*[1 << G.vertices()];
	for (int S = 0; S < (1 << G.vertices()); ++S) {
		HAM[S] = new pair<bool, int>[G.vertices()];
	}
	/* Every singleton subset has a Ham. path of length one */
	for (int i = 0; i < G.vertices(); ++i) {
		HAM[1 << i][i].first = true;
	}
	for (int S = 0; S < (1 << G.vertices()); ++S) {
		for (int i = 0; i < G.vertices(); ++i) {
			/* If the target vertex i is in subset S */
			if (S & (1 << i)) {			
				for (int j = 0; j < G.vertices(); ++j) {
					/* There is a HP in S ending in i if there's another vertex j adjacent to 
					   vertex i that already has a Hamiltonian path in the rest of S ending in j. */
					if ((S & (1 << j)) && G.at(i, j) && i != j && HAM[S ^ (1 << i)][j].first) {
						HAM[S][i] = make_pair(true, j);
						break;
					}
				}
			}
		}
	}
	/* Backtracking a valid vertex ordering from the HAM table */
	vector<int> order;
	int S = (1 << G.vertices()) - 1;
	int i = 0;
	while (S > 0 && i < G.vertices()) {
		while (!HAM[S][i].first) ++i;
		order.push_back(i);
		int temp_i = i;
		i = HAM[S][i].second;
		S = S ^ (1 << temp_i);
	}
	return order;
}

inline void fail() { 
	cout << "No solution is possible." << endl;
	exit(0);
}

/* Returns a valid ordering if a word chain is possible.
   Otherwise it returns an empty vector. 
*/
vector<string> wordChain(vector<string> words) {

	vector<string> result;

	/* Creates the graph (with its adjecency matrix) which stores
	   the words that are one character away from each other. */
	Graph oneAwayG(words.size());
	for (int i = 0; i < words.size(); ++i)
		for (int j = 0; j < words.size(); ++j)
			if (areOneAway(words[i], words[j]))
				oneAwayG.at(i, j) = true;

	/* The task is equivalent to finding a Hamiltonian path in the graph,
	   so if the graph is not connected, there cannot be any solution. */
	if (!isConnected(oneAwayG)) return result;

	/* Although this is still an exponential time algorithm, it performs 
	   better than the trivial O(n!) approach for finding Hamiltonian paths. */
	vector<int> order = findHamiltonianPath(oneAwayG);

	/* Reconstructing the world vector from the indices. */
	for (int i : order) {
		result.push_back(words[i]);
	}

	return result;
}

int main() {

	/* Some assertion tests. */

	vector<string> test_1 = { "alma", "korte", "barack" };
	assert(wordChain(test_1).size() == 0);
	vector<string> test_2 = { "coat", "hat", "hot", "dog", "cat", "hog", "cot", "oat" };
	vector<string> result_2 = wordChain(test_2);
	assert(result_2.size() == test_2.size());
	for (auto it = result_2.begin(); it != result_2.end() - 1; ++it) {
		assert(areOneAway(*it, *(it + 1)));
	}

	assert(areOneAway("dog", "hog"));
	assert(areOneAway("coat", "cat"));
	assert(!areOneAway("coat", "hat"));
	assert(!areOneAway("coat", "dog"));

	/* Read words from stdin into a vector. */

	vector<string> words;
	string nextWord;
	while (cin >> nextWord) {
		words.push_back(nextWord);
	}

	vector<string> result = wordChain(words);
	if (result.size() == 0) fail();
	for_each(result.begin(), result.end(), [](string word) {
		cout << word << " ";
	});

}