#include <iostream>
#include <thread>
#include <string>

#include "open_node.h"

#define ROWSIZE 10
#define COLSIZE 15

POS start;
POS newStart;
POS goal;
POS newGoal;

struct TRASH
{
	POS list[ROWSIZE * COLSIZE];
	int ep = 0;

	void push(const POS& now)
	{
		list[ep] = now;
		ep++;
	}
	void clear()
	{
		ep = 0;
	}
};

int hval(const POS& now)
{
	int r = now.row - goal.row;
	int c = now.col - goal.col;
	if (r < 0) r = -r;
	if (c < 0) c = -c;
	return r + c;
}

int gval[ROWSIZE][COLSIZE];
POS parent[ROWSIZE][COLSIZE];
OpenNode<ROWSIZE, COLSIZE> pq(gval, hval);
TRASH trash;
const int dir[4][2] = { 0,1, -1,0, 0,-1, 1,0 };

char obstacle[ROWSIZE][COLSIZE];

void debug()
{
	char map[ROWSIZE][COLSIZE];
	memset(map, '.', sizeof(map));
	for (int i = 0; i < ROWSIZE; ++i)
	{
		for (int j = 0; j < COLSIZE; ++j)
		{
			if (parent[i][j].row != -1)
				map[i][j] = '#';
			if (pq.get_index({ i,j }) != 0)
				map[i][j] = 'o';
			if (obstacle[i][j] == 'w')
				map[i][j] = '@';
		}
	}
	POS path = goal;
	while (parent[path.row][path.col].row != -1)
	{
		map[path.row][path.col] = '*';
		path = parent[path.row][path.col];
	}
	map[start.row][start.col] = 'S';
	map[goal.row][goal.col] = 'G';

	cout << "\n-----------------------------------------\n";
	for (int i = 0; i < ROWSIZE; ++i)
	{
		for (int j = 0; j < COLSIZE; ++j)
			cout << map[i][j];
		cout << '\n';
	}
	cout << "-----------------------------------------\n";
}

inline void init()
{
	for (int i = 0; i < ROWSIZE; ++i)
	{
		for (int j = 0; j < COLSIZE; ++j)
		{
			gval[i][j] = INF;
			parent[i][j].row = -1;
		}
	}
	gval[start.row][start.col] = 0;
	pq.push(start);

	obstacle[2][5] = 'w';
	obstacle[3][5] = 'w';
	obstacle[4][5] = 'w';
	obstacle[5][5] = 'w';
	obstacle[6][5] = 'w';

	obstacle[6][4] = 'w';
	obstacle[6][3] = 'w';
	obstacle[6][2] = 'w';
	obstacle[6][1] = 'w';

	obstacle[4][9] = 'w';
	obstacle[5][9] = 'w';
	obstacle[4][10] = 'w';
	obstacle[5][10] = 'w';
}

inline bool is_closed(const POS& now)
{
	if (now == start) return true;
	if (pq.get_index(now) == 0 && parent[now.row][now.col].row != -1)
		return true;
	return false;
}

bool search_path()
{
	while (!pq.empty())
	{
		POS now = pq.top();
		pq.pop();

		for (int i = 0; i < 4; ++i)
		{
			const int nr = now.row + dir[i][0];
			const int nc = now.col + dir[i][1];
			if (nr < 0 || nc < 0 || nr >= ROWSIZE || nc >= COLSIZE) continue;
			if (obstacle[nr][nc] == 'w') continue;

			const POS succ = { nr,nc };
			if (is_closed(succ)) continue;

			const int cost = gval[now.row][now.col] + 1;
			if (gval[succ.row][succ.col] <= cost) continue;

			parent[succ.row][succ.col] = now;
			gval[succ.row][succ.col] = cost;
			pq.update(succ);
		}

		if (now == goal) return true;
	}
	return false;
}

void dfs(const POS now, const POS& newStart)
{
	if (now == newStart) return;

	trash.push(now);
	gval[now.row][now.col] = INF;
	parent[now.row][now.col].row = -1;
	if (pq.get_index(now) != 0)
		pq.remove(now);

	for (int i = 0; i < 4; ++i)
	{
		const int nr = now.row + dir[i][0];
		const int nc = now.col + dir[i][1];
		if (nr < 0 || nc < 0 || nr >= ROWSIZE || nc >= COLSIZE) continue;
		if (obstacle[nr][nc] == 'w') continue;
		if (parent[nr][nc] != now) continue;

		dfs({ nr,nc }, newStart);
	}
}

void delete_node(const POS newStart)
{
	if (!is_closed(newStart))
	{
		gval[newStart.row][newStart.col] = 0;
		pq.push(newStart);
	}
	parent[newStart.row][newStart.col].row = -1;
	dfs(start, newStart);
}

void connect_node()
{
	for (int i = 0; i < trash.ep; ++i)
	{
		const POS now = trash.list[i];
		bool flag = 0;
		for (int j = 0; j < 4; ++j)
		{
			const int nr = now.row + dir[j][0];
			const int nc = now.col + dir[j][1];
			if (nr < 0 || nc < 0 || nr >= ROWSIZE || nc >= COLSIZE) continue;
			if (obstacle[nr][nc] == 'w') continue;

			const POS pred = { nr,nc };
			if (!is_closed(pred)) continue;
			flag = 1;

			const int cost = gval[pred.row][pred.col] + 1;
			if (gval[now.row][now.col] <= cost) continue;
			gval[now.row][now.col] = cost;
			parent[now.row][now.col] = pred;
		}
		if (flag)
			pq.update(now);
	}
	trash.clear();
}

void foo()
{
	init();

	while (1)
	{
		while (!search_path())
			cout << "fail\n";
		debug();

		bool flag = 0;
		while (is_closed(goal))
		{
			if (start != newStart)
			{
				delete_node(newStart);
				start = newStart;
				flag = 1;
				debug();
			}
			if (goal != newGoal)
			{
				goal = newGoal;
				pq.reorder();
				debug();
			}
		}

		if (flag)
		{
			connect_node();
			debug();
		}
	}
}

void input(POS& whom)
{
	while (1)
	{
		char d;
		cin >> d;

		switch (d)
		{
		case 'w':
			if (whom.row > 0)
				whom.row--;
			break;
		case 's':
			if (whom.row < ROWSIZE - 1)
				whom.row++;
			break;
		case 'a':
			if (whom.col > 0)
				whom.col--;
			break;
		case 'd':
			if (whom.col < COLSIZE - 1)
				whom.col++;
			break;
		default:
			return;
		}
	}
}

int main()
{
	thread tid(foo);

	char cmd = 'G';
	while (1)
	{
		cin >> cmd;
		if (cmd == 'G')
			input(newGoal);
		else if (cmd == 'S')
			input(newStart);
	}

	return 0;
}

/*
	while (1)
	{
		cout << "\nCOMMAND >> ";
		string cmd;
		cin >> cmd;
		if (cmd == "start")
		{
			cout << "   POS >> ";
			int row, col;
			cin >> row >> col;

			newStart.row = row;
			newStart.col = col;
		}
		else if (cmd == "goal")
		{
			cout << "   POS >> ";
			int row, col;
			cin >> row >> col;

			newGoal.row = row;
			newGoal.col = col;
		}
	}
*/