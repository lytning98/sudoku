#pragma once
#include "stdafx.h"

namespace solver {
	const int masks[10] = { 1 << 0, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6, 1 << 7, 1 << 8, 1 << 9 };
	const int erase_mask[10] = { 1022, 1021, 1019, 1015, 1007, 991, 959, 895, 767, 511 };

	class bitset {
	private:
		int value;
		
	public:
		void reset() {
			value = 0;
		}

		void set(int p) {
			assert(p >= 1 && p <= 9);
			value |= masks[p];
		}

		void erase(int p) {
			assert(p >= 1 && p <= 9);
			value &= erase_mask[p];
		}

		bool test(int p) {
			assert(p >= 1 && p <= 9);
			return (value & masks[p]) != 0;
		}
	};

	int solve(FILE* fin, FILE* fout);
}