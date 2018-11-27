#include "stdafx.h"
#include "solver.hpp"

namespace solver {
	bitset row_bit[9], col_bit[9], cell_bit[9];
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

		int cell_id = x / 3 * 3 + y / 3;
		for(int i = 1; i <= 9; i++)
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
		return false;
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
					cell_bit[i / 3 * 3 + j / 3].set(map[i][j]);
				}
			}

		dfs(0, 0);

		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				fprintf(fout, "%d%c", map[i][j], " \n"[j == 8]);
	}

	void solve(FILE* fin, FILE* fout) {
		bool initial = true;
		while (fscanf_s(fin, "%1d", &map[0][0]) != EOF) {
			initial ? (initial = false) : fprintf_s(fout, "\n");

			for (int i = 0; i < 9; i++)
				for (int j = 0; j < 9; j++)
					(i == 0 && j == 0) ? 0 : fscanf_s(fin, "%1d", &map[i][j]);

			solve(fout);
		}
	}
}