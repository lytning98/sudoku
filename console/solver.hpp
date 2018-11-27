#pragma once
#include "stdafx.h"

namespace solver {
	class bitset {
	private:
		int value;
		const int masks[10] = { 1 << 0, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6, 1 << 7, 1 << 8, 1 << 9 };
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
			if (value & masks[p])
				value ^= masks[p];
		}

		bool test(int p) {
			assert(p >= 1 && p <= 9);
			return (value & masks[p]) != 0;
		}
	};

	void solve(FILE* fin, FILE* fout);
}