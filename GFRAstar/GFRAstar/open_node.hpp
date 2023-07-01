#ifndef __OPEN_NODE_HPP__
#define __OPEN_NODE_HPP__

#include <iostream>
#include <cstring>
using namespace std;

#include "open_node.h"

static inline void swap(POS& left, POS& right)
{
	const POS tmp = left;
	left = right;
	right = tmp;
}

template<int _H, int _W>
inline int OpenNode<_H, _W>::get_f_value(const POS& now)
{
	return gVal[now.row][now.col] + hVal(now);
}

template<int _H, int _W>
inline void OpenNode<_H, _W>::go_up(int i)
{
	const int fVal = get_f_value(heap[i]);

	while (i > 1)
	{
		const int parent_fVal = get_f_value(heap[i / 2]);
		if (parent_fVal < fVal) break;
		if (parent_fVal == fVal &&
			gVal[heap[i].row][heap[i].col] >=
			gVal[heap[i / 2].row][heap[i / 2].col]) break;
		index[heap[i].row][heap[i].col] = i / 2;
		index[heap[i / 2].row][heap[i / 2].col] = i;
		swap(heap[i / 2], heap[i]);
		i /= 2;
	}
}

template<int _H, int _W>
inline void OpenNode<_H, _W>::go_down(int i)
{
	const int fVal = get_f_value(heap[i]);
	while (2 * i < ep)
	{
		int fVals[2] = { INF, INF };
		fVals[0] = get_f_value(heap[2 * i]);
		if (2 * i + 1 < ep)
			fVals[1] = get_f_value(heap[2 * i + 1]);

		if (fVals[0] > fVal && fVals[1] > fVal) break;

		if (fVals[0] < fVals[1])
		{
			if (fVals[0] == fVal &&
				gVal[heap[i].row][heap[i].col] <=
				gVal[heap[2 * i].row][heap[2 * i].col]) break;
			index[heap[i].row][heap[i].col] = 2 * i;
			index[heap[2 * i].row][heap[2 * i].col] = i;
			swap(heap[i], heap[2 * i]);
			i = 2 * i;
		}
		else
		{
			if (fVals[1] == fVal &&
				gVal[heap[i].row][heap[i].col] <=
				gVal[heap[2 * i + 1].row][heap[2 * i + 1].col]) break;
			index[heap[i].row][heap[i].col] = 2 * i + 1;
			index[heap[2 * i + 1].row][heap[2 * i + 1].col] = i;
			swap(heap[i], heap[2 * i + 1]);
			i = 2 * i + 1;
		}
	}
}

template<int _H, int _W>
POS OpenNode<_H, _W>::top()
{
	return heap[1];
}

template<int _H, int _W>
void OpenNode<_H, _W>::pop()
{
	if (ep == 1) return;

	ep--;
	index[heap[ep].row][heap[ep].col] = 1;
	index[heap[1].row][heap[1].col] = 0;
	heap[1] = heap[ep];
	go_down(1);
}

template <int _H, int _W>
void OpenNode<_H, _W>::push(POS now)
{
	if (ep > H * W) return;

	index[now.row][now.col] = ep;
	heap[ep] = move(now);
	go_up(ep);
	ep++;
}

template <int _H, int _W>
void OpenNode<_H, _W>::remove(const POS& now)
{
	const int i = index[now.row][now.col];
	if (i == 0) return;

	ep--;
	heap[i] = heap[ep];
	index[heap[i].row][heap[i].col] = i;
	index[now.row][now.col] = 0;

	go_up(i);
	go_down(i);
}

template<int _H, int _W>
void OpenNode<_H, _W>::update(const POS& now)
{
	const int i = index[now.row][now.col];
	if (i == 0) push(now);
	else
	{
		go_up(i);
		go_down(i);
	}
}

template<int _H, int _W>
inline int OpenNode<_H, _W>::get_index(const POS& now)
{
	return index[now.row][now.col];
}

template<int _H, int _W>
inline bool OpenNode<_H, _W>::empty()
{
	return ep == 1;
}

template<int _H, int _W>
void OpenNode<_H, _W>::reorder()
{
	for (int i = (ep - 1) / 2; i >= 1; --i)
		go_down(i);
}

#endif




