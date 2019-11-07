#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

vector<vector<int> > messages(105);
const int src_vertex = 0;


class Iterator {
public:
	Iterator() {};
	virtual ~Iterator() {};
	virtual void First() = 0;
	virtual void Next() = 0;
	virtual bool Done() = 0;
};

class MessageIterator :public Iterator {
public:
	MessageIterator(vector<int>& v) :val(v) {
		index = 0;
		v.clear();
	}
	~MessageIterator() {}

	virtual void First() {
		index = 0;
	}
	virtual void Next() {
		if (index < val.size())
			index++;
	}
	virtual bool Done() {
		return index == val.size();
	}
	int Value() {
		return val[index];
	}

private:
	int index;
	vector<int> val;
};

class OutEdgeIterator :public Iterator {
public:
	OutEdgeIterator(vector<int>& v) : val(v)
	{
		index = 0;
	}
	~OutEdgeIterator() {}

	virtual void First()
	{
		index = 0;
	}
	virtual void Next()
	{
		if (index < val.size())
			index++;
	}
	virtual bool Done()
	{
		return index == val.size();
	}
	int GetValue()
	{
		return 1;
	}
	int Target()
	{
		return val[index];
	}
private:
	int index;
	vector<int> val;
};

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
	ShortestPathVertex(int id_, vector<int>& edges_)
	{
		id = id_;
		d = INT_MAX;
		edges = edges_;
		isHalt = false;
	}

	void Compute(MessageIterator* msgs) {
		int mindist = IsSource(vertex_id()) ? 0 : INT_MAX;
		for (; !msgs->Done(); msgs->Next())
		{
			mindist = min(mindist, msgs->Value());
		}
		if (mindist < GetValue()) {
			*MutableValue() = mindist;
			OutEdgeIterator iter = GetOutEdgeIterator();
			for (; !iter.Done(); iter.Next())
			{
				SendMessageTo(iter.Target(), mindist + iter.GetValue());
			}
		}
		VoteToHalt();
	}

	const int& vertex_id() const
	{
		return id;
	}
	bool IsSource(int id) const
	{
		return id == src_vertex;
	}
	const int& GetValue()
	{
		return d;
	}
	int* MutableValue()
	{
		return &d;
	}
	OutEdgeIterator GetOutEdgeIterator()
	{
		return OutEdgeIterator(edges);
	}
	void SendMessageTo(const int dest_vertex, const int& message)
	{
		messages[dest_vertex].push_back(message);
	}
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
	//num of vertex
	const int num = 10;

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
	//debug code
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
	//debug code
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

/*v=100 e=10
0 81 65 35 68 8 43 28 78
1 48 0 17 52 55 25 59 63 31 15
2 0 49 67 54 55 39 8 10 60 62
3 0 83 68 36 58 27 28 45 62
4 32 49 33 67 52 54 23 89 76 13
5 0 67 21 57 89 27 91 11 45 29
6 17 67 21 41 73 9 74 26 29 31
7 33 66 19 23 88 56 77 62 78 95
8 32 19 99 36 37 85 22 42 26 15
9 0 65 83 99 5 7 57 27 92
10 49 66 3 69 42 90 77 93 78 15
11 67 37 87 39 8 73 9 44 61 46
12 16 64 19 38 24 75 11 45 78 62
13 0 1 2 67 38 42 11 78 79 47
14 33 19 68 22 58 92 44 13 30
15 96 50 66 36 6 23 73 58 30 15
16 97 4 53 69 54 7 73 76 79
17 48 97 50 37 6 73 58 75 45
18 65 52 6 56 60 45 30 94 63 95
19 65 82 18 35 68 52 41 60 62
20 0 37 86 70 88 89 25 42 58
21 17 68 39 71 89 42 59 27 76 77
22 81 97 51 85 38 72 41 26 28
23 3 38 87 43 77 46 15 47
24 66 50 53 7 25 89 26 45 78 15
25 32 33 81 2 21 69 59 44 12 62
26 97 4 69 22 72 41 58 42 75 62
27 1 97 34 84 39 71 7 89 91 78
28 99 51 6 23 87 27 62 31
29 32 51 68 53 55 7 90 59 28
30 96 36 53 89 26 10 27 13 93 14
31 97 17 51 71 24 88 57 91 59 92
32 54 23 39 88 40 28 92 61 13 47
33 48 1 98 88 41 73 59 29
34 64 65 67 35 4 85 40 42 46 95
35 2 85 22 88 91 12 92 93 95 63
36 96 80 99 19 86 6 90 92 28
37 17 52 6 24 59 29 78 30 15
38 49 83 20 52 21 26 75 43 92 76
39 34 68 53 21 23 41 28 13 47
40 64 80 48 19 54 87 59 77 30
41 67 53 22 23 10 58 11 45 31
42 16 64 81 49 34 83 22 92 30
43 33 18 51 67 22 72 74 30
44 64 81 66 82 52 4 57 94 95 15
45 16 81 2 3 52 5 72 57 14 47
46 81 23 71 8 26 11 29 93 63
47 50 20 72 24 41 74 59 94 46 47
48 0 18 67 99 69 41 57 13 47
49 96 81 97 98 99 6 55 9 59 45
50 49 97 66 20 85 22 42 12 93 78
51 0 50 99 3 21 7 87 41 91 95
52 16 33 67 37 7 9 10 62 79
53 80 82 38 23 39 25 59 61 15
54 64 98 51 37 53 6 89 58 28 61
55 80 16 64 0 97 22 56 90 76 62
56 65 85 89 74 75 59 76 93 78 15
57 0 17 2 67 19 39 40 91 30
58 16 2 83 3 67 24 89 42 14 79
59 52 6 71 90 42 91 44 92 46 79
60 17 82 18 85 21 5 38 7 46 63
61 32 65 18 2 36 21 71 30 79
62 17 51 85 86 38 93 79
63 98 54 70 25 9 41 10 26 29 47
64 97 2 20 5 86 7 26 13 45 14
65 96 33 82 52 6 10 58 91 75 47
66 97 98 2 70 55 87 88 24 73 62
67 16 1 68 52 37 56 40 9
68 49 52 20 54 11 76 13 14 62 78
69 82 3 36 52 56 42 60 93 78 15
70 81 20 85 87 91 27 28 45 62
71 35 19 6 86 25 89 11 60 13
72 80 17 2 34 70 8 10 43 62 78
73 0 1 65 98 82 99 8 40 60 63
74 0 48 97 2 3 83 85 25 11
75 16 35 84 6 87 25 26 29 46 30
76 64 81 17 18 19 83 26 44 47
77 96 16 65 69 6 70 24 44 46
78 16 0 48 66 4 85 54 72 59
79 1 2 52 86 39 56 40 57 41 74
80 16 37 21 6 86 92 13 15 47
81 96 64 33 35 67 22 74 44 28 95
82 32 83 36 69 85 90 42 76 79
83 51 35 73 58 42 11 76 14 95 15
84 17 21 74 58 76 13 62 30 47 95
85 32 48 65 18 85 21 37 8 93 77
86 64 85 21 87 43 12 60 13 14 47
87 32 16 18 98 51 52 22 56 76 14
88 4 22 8 88 57 90 59 91 61
89 18 34 51 35 83 70 10 62 47
90 81 66 98 4 20 69 8 73 92 76
91 64 49 36 37 54 9 75 76 31 79
92 0 98 19 4 86 41 78 47 79
93 0 97 19 20 53 69 9 28 12 60
94 32 68 69 85 22 9 73 43 11 60
95 49 1 50 6 87 24 41 91 29 95
96 96 33 18 82 84 85 21 6 23 87
97 16 98 50 19 4 22 9 26 63
98 16 32 50 36 37 8 9 41 60 29
99 17 98 2 83 52 36 7 58 62 79
*/

/*v=20 e=3
0 2 6 15
1 11 15
2 18 10 12
3 16 12 13
4 18 2 4
5 18 11 15
6 17 4 9
7 17 2 8
8 7 10 11
9 19 4 14
10 5 7 10
11 1 3 7
12 16 1 6
13 1 2 9
14 17 7 10
15 0 5
16 0 17 5
17 18 7 14
18 16 1
19 17 2
*/

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