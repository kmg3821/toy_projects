#ifndef __OPEN_NODE_H__
#define __OPEN_NODE_H__

#include <cstring>

#define INF (int)2e9

struct POS
{
	int row, col;

	bool operator==(const POS& right) const
	{
		return (row == right.row) && (col == right.col);
	}
	bool operator!=(const POS& right) const
	{
		return row != right.row || col != right.col;
	}
};

template <int _H, int _W>
class OpenNode
{
	static_assert(_H * _W <= 1000000, "SIZE IS TOO BIG");

private:
	const int H = _H;
	const int W = _W;
	POS heap[_H * _W + 1];
	int index[_H][_W];
	int ep = 1;
	int(*const gVal)[_W];
	int(*hVal)(const POS&);

	int get_f_value(const POS& now);
	void go_up(int);
	void go_down(int);

public:
	explicit OpenNode(void *_gVal, int(*_hVal)(const POS&)) : gVal((int(*const)[_W])_gVal), hVal(_hVal)
	{
		memset(index, 0, sizeof(index));
	};
	~OpenNode() {};

	POS top();
	void pop();
	void push(POS);
	void remove(const POS&);
	void update(const POS&);
	int get_index(const POS&);
	bool empty();
	void reorder();
};

#include "open_node.hpp"

#endif
