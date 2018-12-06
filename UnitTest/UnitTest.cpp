#include "stdafx.h"
#include "CppUnitTest.h"
#include "../console/generator.cpp"
#include "../console/solver.hpp"
#include "../console/solver.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(GeneratorTest)
	{
	public:
		
		/*
			Check if number confliction exists in generated endings.
		*/
		TEST_METHOD(GeneratorConflictionTest)
		{
			using namespace generator;

			int times = rand() % 10000 + 1;

			while(times--)
				generate();

			bool cnt_row[10][10] = { 0 }, cnt_col[10][10] = { 0 }, cnt_cell[10][10] = { 0 };
			for(int i = 0; i < 9; i++)
				for (int j = 0; j < 9; j++)
				{
					int d = map[i][j], cell_id = i/3*3+j/3;
					if (!cnt_row[i][d] && !cnt_col[j][d] && !cnt_cell[cell_id][d]) {
						cnt_row[i][d] = cnt_col[j][d] = cnt_cell[cell_id][d] = true;
					}
					else {
						Assert::Fail();
						break;
					}
				}
			
		}
		
		/*
			Check if step_forward() enumerates permutation correctly.
		*/
		typedef std::vector<int> arr;

		TEST_METHOD(EnumerationTest1)
		{
			int p[18] = { 1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9 };
			int shift[3][3] = {
				{0, 1, 2},
				{2, 1, 0},
				{2, 1, 0}
			};
			arr before(p, p + 18);
			generator::step_forward(p, shift);
			arr after(p, p + 18);
			Assert::IsFalse(before == after);
		}

		TEST_METHOD(EnumerationTest2)
		{
			int p[18] = { 1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9 };
			int shift[3][3] = {
				{ 0, 1, 2 },
				{ 0, 1, 2 },
				{ 2, 1, 0 }
			};
			std::pair<arr, arr> before = { arr(shift[1], shift[1] + 3), arr(shift[2], shift[2] + 3) };
			generator::step_forward(p, shift);
			std::pair<arr, arr> after = { arr(shift[1], shift[1] + 3), arr(shift[2], shift[2] + 3) };
			Assert::IsTrue(before.first != after.first && before.second != after.second);
		}

		TEST_METHOD(EnumerationTest3)
		{
			int p[18] = { 1,2,3,4,5,6,7,8,9,1,2,3,4,5,6,7,8,9 };
			int shift[3][3] = {
				{ 0, 1, 2 },
				{ 0, 1, 2 },
				{ 0, 2, 1 }
			};
			
			std::tuple<arr, arr, arr> before = { arr(p, p + 18), arr(shift[0], shift[0] + 3), arr(shift[1], shift[1] + 3) };
			arr before_v(shift[2], shift[2] + 3);
			generator::step_forward(p, shift);
			std::tuple<arr, arr, arr> after = { arr(p, p + 18), arr(shift[0], shift[0] + 3), arr(shift[1], shift[1] + 3) };
			arr after_v(shift[2], shift[2] + 3);
			Assert::IsTrue(before == after);
			Assert::IsFalse(before_v == after_v);
		}

	};

	/*
		Test basic operations (set, erase, reset) of solver::bitset.
	*/
	TEST_CLASS(SolverBitsetTest)
	{
	public:
		TEST_METHOD(BitsetSetTest)
		{
			solver::bitset bitset;
			bitset.set(5);
			Assert::IsFalse(bitset.test(1));
			Assert::IsTrue(bitset.test(5));
		}

		TEST_METHOD(BitsetEraseTest)
		{
			solver::bitset bitset;
			for (int i = 1; i <= 9; i++)
				bitset.set(i);
			for (int i = 1; i <= 9; i++)
				Assert::IsTrue(bitset.test(i));
			bitset.erase(9);
			for (int i = 1; i < 9; i++)
				Assert::IsTrue(bitset.test(i));
			Assert::IsFalse(bitset.test(9));
		}

		TEST_METHOD(BitsetResetTest)
		{
			solver::bitset bitset;
			for (int i = 1; i <= 9; i++)
				if(i%2==1)
					bitset.set(i);
			bitset.reset();
			for (int i = 1; i <= 9; i++)
				Assert::IsFalse(bitset.test(i));
		}
	};

	TEST_CLASS(SolverTest)
	{
	public:
		/*
			Check if IO thread of solver can start, work and exit properly.
		*/
		TEST_METHOD(ThreadTest)
		{
			solver::IO::start_IO(nullptr);
			Assert::IsTrue(solver::IO::going);
			Assert::IsNotNull(solver::IO::IO_thread);
			solver::IO::join_IO();
		}

		// Test Data for solver
		const int map[9][9] = {
			{ 8,4,1,5,7,0,0,3,0 },
			{ 5,0,2,6,0,9,0,4,1 },
			{ 0,0,0,0,4,0,5,0,2 },
			{ 3,0,8,0,1,0,0,0,6 },
			{ 0,0,5,0,2,6,0,9,0 },
			{ 7,2,0,0,9,8,4,1,5 },
			{ 0,6,0,0,0,4,0,5,7 },
			{ 0,8,0,0,5,7,2,0,0 },
			{ 1,5,7,0,6,3,9,0,4 }
		};

		/*
			Check if solver's initialization works properly.
		*/
		TEST_METHOD(SolverInitTest)
		{
			memcpy(solver::map, map, sizeof(map));
			solver::DFS::initialize();
			for (int i = 0; i < 9; i++)
				for (int j = 0; j < 9; j++) {
					Assert::IsTrue(solver::DFS::emp[i][j] == (map[i][j] == 0));
					if(map[i][j]){
						Assert::IsTrue(solver::DFS::row_bit[i].test(map[i][j]));
						Assert::IsTrue(solver::DFS::col_bit[j].test(map[i][j]));
						Assert::IsTrue(solver::DFS::cell_bit[i/3*3+j/3].test(map[i][j]));
					}
				}

		}

		/*
			Check solver's answer.
		*/
		TEST_METHOD(SolverAnswerTest)
		{
			bool row[10][10] = { 0 }, col[10][10] = { 0 }, cell[10][10] = { 0 };
			memcpy(solver::map, map, sizeof(map));
			solver::DFS::solve();
			for (int i = 0; i < 9; i++)
				for (int j = 0; j < 9; j++) {
					bool &rowb = row[i][solver::map[i][j]];
					bool &colb = col[j][solver::map[i][j]];
					bool &cellb = cell[i / 3 * 3 + j / 3][solver::map[i][j]];
					Assert::IsFalse(rowb | colb | cellb);
					rowb = colb = cellb = true;
					if (map[i][j]) {
						Assert::IsTrue(map[i][j] == solver::map[i][j]);
					}
					else {
						Assert::IsTrue(solver::map[i][j] != 0);
					}
				}
		}
	};

}