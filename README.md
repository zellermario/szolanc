<h1>Szólánc</h1>

<p>
This C++ program finds, if possible, a permutation of a list of words such that each neighboring pair will have an edit distance of one.
This is achieved by first finding each pair which are one character-edit away from each other, then creating a graph from these 
(every input word is a node of the graph, and two nodes are adjacent if the word they represent are one character-edit away).
Then the task is to find a path in the graph such that each vertex is visited exactly once, i.e. a Hamiltonian path.
</p>
<p>
The Hamiltonian path problem is known to be NP-complete, and the naive approach of trying each permutation costs <img src="https://render.githubusercontent.com/render/math?math=O(n!)"> trials in an n-vertex graph. 
There is however a dynamic programming approach to finding a Hamiltonian path which can be run in <img src="https://render.githubusercontent.com/render/math?math=O({n}^{2}{2}^{n})"> iterations. My solution uses this approach.
</p>
<p>
There might be some shortcut to this problem I did not realize just yet, with which the problem does not fall back on the general Hamiltonian path problem, but as per now, this is the best
I could come up with.

<h2>Expected input</h2>
Whitespace separated words on the standard input. </br>

<h2>Output</h2>
A correct one-edit-distance ordering of the words. </br>
Or, if none is possible, a message indicating it.

<h2>Compilation and running</h2>
<pre>
g++ szolanc.cpp -o szolanc -std=c++11
./szolanc
</pre>
