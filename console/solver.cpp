#include "stdafx.h"
#include "solver.hpp"

namespace solver {
	const bool use_multi_thread = true;

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

	std::queue<int*>Q;
	bool going = true;
	bool initial = true;
	void write_result(FILE* fout)
	{
		while (going || !Q.empty()) {
			while (!Q.empty()) {
				initial ? (initial = false) : fprintf_s(fout, "\n");
				int* store = Q.front(); 
				for (int i = 0; i < 9; i++)
					for (int j = 0; j < 9; j++) {
						fputc(store[i * 9 + j] + '0', fout);
						fputc(" \n"[j == 8], fout);
					}
				free(store);
				Q.pop();
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

		if(use_multi_thread){
			int* store = new int[81];
			memcpy(store, map, sizeof(map));
			Q.push(store);
		}
		else {
			for (int i = 0; i < 9; i++)
				for (int j = 0; j < 9; j++){
					fputc(map[i][j] + '0', fout);
					fputc(" \n"[j == 8], fout);
				}
		}
	}

	int solve(FILE* fin, FILE* fout) {
		std::thread IOthread;

		if(use_multi_thread){
			IOthread = std::thread(write_result, fout);
		}

		int count = 0;
		
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
		going = false;
		if(use_multi_thread){
			IOthread.join();
		}
		return count;
	}
}