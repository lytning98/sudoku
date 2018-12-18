#include "stdafx.h"
#include "generator.h"

namespace generator {
	const int space_per_cell = 4;
	int map[9][9];

	/*
		Write current map to file.
	*/
	inline void write_map(FILE* file, bool empline) {
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++){
				fputc(map[i][j] + '0', file);
				fputc(" \n"[j == 8], file);
			}
		if (empline)
			fputc('\n', file);
	}

	/*
		Enumerate permutations.
	*/
	void step_forward(int* p, int shift[3][3]) {
		using namespace std;

		if (!next_permutation(shift[2], shift[2] + 3)) {
			if (!next_permutation(shift[1], shift[1] + 3)) {
				next_permutation(p + 1, p + 9);
				memcpy(p + 10, p + 1, sizeof(int) * 8);
				for (int i = 0; i < 3; i++)
					shift[1][i] = shift[2][i] = i;
			}
			for (int i = 0; i < 3; i++)
				shift[2][i] = i;
		}
	}

	/*
		Generate an ending.
	*/
	void generate() {
		static int p[18] = { 8, 1, 2, 3, 4, 5, 6, 7, 9, 8, 1, 2, 3, 4, 5, 6, 7, 9 };
		static int shift[3][3] = {
			{ 0, 1, 2 },
			{ 0, 1, 2 },
			{ 0, 1, 2 }
		};

		step_forward(p, shift);
		int offset = 0;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++) {
				memcpy(map[i * 3 + shift[i][j]], p + offset, sizeof(int) * 9);
				if (j == 2)
					offset = (offset + 1) % 9;
				else
					offset = (offset + 3) % 9;
			}
	}

	void generate_and_write(FILE* file, bool empline) {
		generate();
		write_map(file, empline);
	}
	//////////////////////////////////////
	/*
		Code below is for generating random sudoku puzzles.
		This function is used to generate data for solver.
	*/
	//////////////////////////////////////
	std::default_random_engine random_engine((unsigned)std::chrono::system_clock::now().time_since_epoch().count());

	void forward(int* p, int step) {
		int tmp[9];
		for (int i = 0; i < step; i++)
			tmp[i] = p[i];
		for (int i = step; i < 9; i++)
			p[i - step] = p[i];
		for (int i = 0; i < step; i++)
			p[9 - step + i] = tmp[i];
	}

	inline void shuffle(int* first, int* last) {
		std::shuffle(first, last, random_engine);
	}

	void puzzle_generate(FILE* file, bool empline) {
		int p[9] = { 8, 1, 2, 3, 4, 5, 6, 7, 9 }, q[9];

		memcpy(q, p, sizeof(q));
		std::for_each(q, q + 9, [](int& x) {x--;});

		shuffle(p + 1, p + 9);
		for (int i = 0; i < 3; i++){
			for (int j = 0; j < 3; j++){
				memcpy(map[i * 3 + j], p, sizeof(p));
				if (j == 2)
					forward(p, 1);
				else
					forward(p, 3);
			}
		}

		for(int i = 0; i < 3; i++){
			for (int j = 0; j < 3; j++){
				shuffle(q, q + 9);
				for (int k = 0; k < space_per_cell; k++) {
					map[i * 3 + q[k] / 3][j * 3 + q[k] % 3] = 0;
				}
			}
		}
		
		write_map(file, empline);
	}
}