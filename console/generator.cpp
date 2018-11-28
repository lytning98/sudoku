#include "stdafx.h"
#include "generator.h"

namespace generator {
	const int space_per_cell = 4;
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

	void generate(FILE* file, bool empline, bool has_empty) {
		int map[9][9];
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

		if(has_empty){
			for(int i = 0; i < 3; i++){
				for (int j = 0; j < 3; j++){
					shuffle(q, q + 9);
					for (int k = 0; k < 4; k++) {
						map[i * 3 + q[k] / 3][j * 3 + q[k] % 3] = 0;
					}
				}
			}
		}
		
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				fprintf(file, "%d%c", map[i][j], " \n"[j == 8]);
		if (empline)
			fprintf(file, "\n");
	}
}