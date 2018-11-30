#include "stdafx.h"
#include "solver.hpp"

namespace solver {
#define multi_thread 2018

	bitset row_bit[9], col_bit[9], cell_bit[9];
	std::vector<int> valid[9][9];
	const int cell_ids[9][9] = {
		{ 0, 0, 0, 1, 1, 1, 2, 2, 2 },
		{ 0, 0, 0, 1, 1, 1, 2, 2, 2 },
		{ 0, 0, 0, 1, 1, 1, 2, 2, 2 },
		{ 3, 3, 3, 4, 4, 4, 5, 5, 5 },
		{ 3, 3, 3, 4, 4, 4, 5, 5, 5 },
		{ 3, 3, 3, 4, 4, 4, 5, 5, 5 },
		{ 6, 6, 6, 7 ,7, 7, 8, 8, 8 },
		{ 6, 6, 6, 7 ,7, 7, 8, 8, 8 },
		{ 6, 6, 6, 7 ,7, 7, 8, 8, 8 },
	};
	int map[9][9];
	bool emp[9][9];

	bool dfs(int x, int y) {
		if (y == 9) {
			y = 0;
			x++;
			if (x == 9) {
				return true;
			}
		}
		if (!emp[x][y])	return dfs(x, y + 1);

		int cell_id = cell_ids[x][y];
		for(int i : valid[x][y])
		{
			if (!(row_bit[x].test(i) || col_bit[y].test(i) || cell_bit[cell_id].test(i)))
			{
				map[x][y] = i;
				row_bit[x].set(i);
				col_bit[y].set(i);
				cell_bit[cell_id].set(i);
				if (dfs(x, y + 1))	
					return true;
				row_bit[x].erase(i);
				col_bit[y].erase(i);
				cell_bit[cell_id].erase(i);
			}
		}
		return false;
	}
	
	bool going = true;
	bool initial = true;
	std::queue<int*>Q;
	std::mutex qMutex;
	std::condition_variable cond_var;

	void write_result(FILE* fout) {
		char buf[300];
		while (going) {
			std::unique_lock<std::mutex> lk(qMutex);
			cond_var.wait(lk);
			while (!Q.empty()) {
				initial ? (initial = false) : fputc('\n', fout);
				int* store = Q.front(); Q.pop();
				int index = 0;
				for (int i = 0; i < 81; i++) {
					buf[index++] = store[i] + '0';
					buf[index++] = " \n"[i % 9 == 8];
				}
				buf[index] = 0; //replace the last '\n' with '\0'
				fputs(buf, fout);
				delete[] store;
			}
		}
		printf("IO finished.\n");
	}
	
	void solve(FILE* fout) {
		for (int i = 0; i < 9; i++)
			row_bit[i].reset(), col_bit[i].reset(), cell_bit[i].reset();

		for(int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
			{
				if (map[i][j] == 0) {
					emp[i][j] = true;
				}
				else {
					emp[i][j] = false;
					row_bit[i].set(map[i][j]);
					col_bit[j].set(map[i][j]);
					cell_bit[cell_ids[i][j]].set(map[i][j]);
					int mask = 1 << map[i][j];
				}
			}
		for(int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
			{
				if (!emp[i][j])	continue;
				valid[i][j].clear();
				for (int k = 1; k <= 9; k++)
					if (!(row_bit[i].test(k) || col_bit[j].test(k) || cell_bit[cell_ids[i][j]].test(k)))
						valid[i][j].push_back(k);
			}

		dfs(0, 0);

#ifdef multi_thread
		int* store = new int[81];
		memcpy(store, map, sizeof(map));
		std::unique_lock<std::mutex> lk(qMutex);
		Q.push(store);
		cond_var.notify_all();
#else
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++){
				fputc(map[i][j] + '0', fout);
				fputc(" \n"[j == 8], fout);
			}
#endif
	}

	int solve(FILE* fin, FILE* fout) {
#ifdef multi_thread
		std::thread IOthread(write_result, fout);
#endif

		int count = 0;
		going = true;
		
		while ((map[0][0] = fgetc(fin))!= EOF) {
			map[0][0] -= '0'; fgetc(fin);
			for (int i = 0; i < 9; i++)
				for (int j = 0; j < 9; j++)
					if (i != 0 || j != 0) {
						map[i][j] = fgetc(fin) - '0';
						fgetc(fin);
					}
			fgetc(fin); // read '\n'
			solve(fout);
			count++;
		}

		printf("Solving finished.\n");
		
#ifdef multi_thread
		going = false;
		cond_var.notify_all();
		IOthread.join();
#endif
		return count;
	}
}