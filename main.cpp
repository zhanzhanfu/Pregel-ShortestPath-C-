#include "iterator.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

vector<vector<int> > messages(105);   //information carrier
const int src_vertex = 0;    //source vertex


//vertex
template <typename VertexValue, typename EdgeValue, typename MessageValue>
class Vertex {
public:
	virtual void Compute(MessageIterator* msgs) = 0;
	const int& vertex_id() const;
	int superstep() const;
	const VertexValue& GetValue();
	VertexValue* MutableValue();
	OutEdgeIterator GetOutEdgeIterator();
	void SendMessageTo(const int dest_vertex, const MessageValue& message);
	void VoteToHalt();
};

class ShortestPathVertex : public Vertex<int, int, int> {
public:
	//constructor
	ShortestPathVertex(int id_, vector<int>& edges_)
	{
		id = id_;
		d = INT_MAX;
		edges = edges_;
		isHalt = false;
	}
	//compute
	void Compute(MessageIterator* msgs) {
		//judge is source vertex
		int mindist = IsSource(vertex_id()) ? 0 : INT_MAX;
		//traverse msgs, get the minimum value
		for (; !msgs->Done(); msgs->Next())
		{
			mindist = min(mindist, msgs->Value());
		}
		// 1. if the minimum value less than current value, modify the current value
		// 2. send newest message to adjacent vertices
		if (mindist < GetValue()) {
			*MutableValue() = mindist;
			OutEdgeIterator iter = GetOutEdgeIterator();
			for (; !iter.Done(); iter.Next())
			{
				SendMessageTo(iter.Target(), mindist + iter.GetValue());
			}
		}
		//make the vertex halt
		VoteToHalt();
	}
	//return vertex id
	const int& vertex_id() const
	{
		return id;
	}
	//judge is source vertex
	bool IsSource(int id) const
	{
		return id == src_vertex;
	}
	//get current distance
	const int& GetValue()
	{
		return d;
	}
	//modify current distance
	int* MutableValue()
	{
		return &d;
	}
	//return edge iterator
	OutEdgeIterator GetOutEdgeIterator()
	{
		return OutEdgeIterator(edges);
	}
	//send message to adjacent vertices
	void SendMessageTo(const int dest_vertex, const int& message)
	{
		messages[dest_vertex].push_back(message);
	}
	//make the vertex halt
	void VoteToHalt()
	{
		isHalt = true;
	}

private:
	int id;
	int d;
	vector<int> edges;
	bool isHalt;
};

int main() {
	vector<ShortestPathVertex> spv;
	const int num = 100;    //num of vertex

	//input
	for (int i = 0; i < num; i++)
	{
		int id, e;
		vector<int> edges;
		cin >> id;
		char t;
		while ((t = cin.get()) != '\n')
		{
			cin >> e;
			if (id != e)
			{
				edges.push_back(e);
			}
		}
		spv.push_back(ShortestPathVertex(id, edges));
	}
	//output vertex 
	/*for (int i = 0; i < spv.size(); i++)
	{
		cout << spv[i].vertex_id() << " ";
		OutEdgeIterator iter = spv[i].GetOutEdgeIterator();
		for (;!iter.Done(); iter.Next())
		{
			cout << iter.Target() << " ";
		}
		cout << endl;
	}*/
	//compute
	while (true)
	{
		for (int i = 0; i < spv.size(); i++)
		{
			spv[i].Compute(new MessageIterator(messages[i]));
		}
		//judge isOver
		bool flag = false;
		for (int i = 0; i < messages.size(); i++)
		{
			if (messages[i].size() != 0)
			{
				flag = true;
			}
		}
		if (!flag)
			break;
	}
	//result
	for (int i = 0; i < spv.size(); i++)
	{
		cout << spv[i].vertex_id() << " " << spv[i].GetValue() << endl;
	}
	//output messages
	/*for (int i = 0; i < 10; i++)
	{
		cout << i << " ";
		for (int j = 0; j < messages[i].size(); j++)
		{
			cout << messages[i][j] << " ";
		}
		cout << endl;
	}*/

	return 0;
}

/*v=100 e=5
0 16 1 99 92
1 80 97 21 69 40
2 54 22 25 46 47
3 32 17 10 43 77
4 33 18 21 75 13
5 32 82 20 91 63
6 4 7 25 13 62
7 16 35 93 14 31
8 19 20 88 43 11
9 96 40 91 31 15
10 66 70 7 73 75
11 25 74 91 44 62
12 18 35 74 27 75
13 33 52 54 10 11
14 96 32 70 76 15
15 49 4 69 90 43
16 1 37 7 90 31
17 16 17 56 9 58
18 99 67 6 54 93
19 83 4 84 20 23
20 70 56 25 62 79
21 80 3 6 74 45
22 84 38 54 87 11
23 17 8 12 76 15
24 1 33 5 6 47
25 16 23 7 28 78
26 16 17 52 5 55
27 69 71 88 27 60
28 72 89 75 93 15
29 70 57 58 45 15
30 98 84 5 42 15
31 68 87 90 13 95
32 51 59 29 94 63
33 48 97 17 34
34 16 34 39 10 15
35 85 55 56 46 62
36 36 8 56 25 92
37 83 58 26 46 78
38 84 20 85 60 95
39 97 54 73 57 10
40 2 68 4 37 85
41 65 67 25 89 78
42 68 87 71 41 28
43 81 66 83 68 25
44 19 69 85 24 27
45 32 98 19 58 43
46 97 65 83 68 56
47 49 51 5 74 46
48 16 82 35 21 46
49 52 39 56 25 11
50 98 37 24 90 31
51 38 11 12 76 14
52 97 18 26 11 94
53 49 34 84 23 46
54 16 20 89 12 79
55 64 50 28 94 62
56 23 90 62 47
57 33 52 68 37 56
58 84 6 39 72 42
59 17 36 29 62 79
60 64 9 76 46 14
61 65 3 52 4
62 18 11 75 28 30
63 0 24 56 74 77
64 32 3 23 9 31
65 10 74 27 11 13
66 65 97 11 27 30
67 65 18 6 27 95
68 96 23 25 26 91
69 50 35 39 91 47
70 16 64 43 93 46
71 82 8 40 76 93
72 2 83 84 11 12
73 48 67 83 87 47
74 83 51 52 68 7
75 1 34 5 30 31
76 98 34 5 13
77 64 82 22 7 94
78 34 19 84 86 42
79 35 99 37 11 43
80 96 73 9 78
81 50 89 10 94 95
82 72 42 26 29 63
83 19 37 28 45 63
84 1 49 57 41 90
85 64 84 22 56 94
86 98 3 84 69 11
87 32 7 57 42 28
88 3 36 89 10 29
89 51 36 28 12
90 96 82 18 52 87
91 32 20 11 92 13
92 33 55 57 45 15
93 96 2 51 13 93
94 65 49 85 37 92
95 64 35 28 14 47
96 64 83 4 87
97 48 3 56 41 60
98 32 23 72 40 90
99 32 81 51 20 4
*/

/*v=10 e=3
0 0 2 9
1 0 2 3
2 0 5 8
3 1 7 8
4 1 2 7
5 4 8 9
6 3 6 7
7 1 5 9
8 0 2 6
9 1 4 7
*/