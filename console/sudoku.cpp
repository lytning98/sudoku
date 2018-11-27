// sudoku.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "generator.h"
#include "solver.hpp"

int main(int argc, char** argv)
{
	if(argc == 3 && argv[1][1] == 'c'){
		int count = 0;
		sscanf_s(argv[2], "%d", &count);
		if(count<1||count>1000000){
			printf("[Generate] sudoku.exe -c {count}\nWhere {count} is integer in range [1, 1000000]\n");
		}
		else {
			printf("Generating ...\n");
			FILE* file;
			fopen_s(&file, "sudoku_puzzle.txt", "w");
			while(count--){
				generator::generate(file, (bool)(count!=0));
			}
			fclose(file);
			printf("Generated puzzles into sudoku_puzzle.txt .\n");
		}
	}
	else if (argc == 3 && argv[1][1] == 's'){
		FILE *file, *fout;
		fopen_s(&file, argv[2], "r");
		fopen_s(&fout, "sudoku.txt", "w");
		printf("Solving ...\n");
		solver::solve(file, fout);
		printf("Done. Answers has been saved into sudoku.txt .\n");
		fclose(file);
		fclose(fout);
	}
	else {
		printf("Usage :\n[Generate] sudoku.exe -c {count}\n[Solving] soduku.exe -s\n");
	}
    return 0;
}

