#pragma once
#include <vector>
using std::vector;

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
