
#include "stdafx.h"
#include "generator.h"
#include "solver.hpp"

int main(int argc, char** argv)
{
	if(argc == 3 && (argv[1][1] == 'g' || argv[1][1] == 'c')){
		/*
			Generate ending
		*/
		auto gene_func = (argv[1][1] == 'g') ? generator::puzzle_generate : generator::generate_and_write;
		const char* filename = (argv[1][1] == 'g') ? "sudoku_puzzle.txt" : "sudoku.txt";
		int count = 0;
		sscanf_s(argv[2], "%d", &count);
		if(count<1||count>1000000){
			printf("[Generate] sudoku.exe -c {count}\nWhere {count} is integer in range [1, 1000000]\n");
		}
		else {
			printf("Generating ...\n");
			FILE* file;
			fopen_s(&file, filename, "w");
			while(count--){
				gene_func(file, (bool)(count!=0));
			}
			fclose(file);
			printf("Generated puzzles into [%s] .\n", filename);
		}
	}
	else if (argc == 3 && argv[1][1] == 's'){
		/*
			Solving
		*/
		FILE *file = nullptr, *fout = nullptr;
		fopen_s(&file, argv[2], "r");
		fopen_s(&fout, "sudoku.txt", "w");
		if (!file || !fout) {
			printf("Failed to load [%s] or open [sudoku.txt]. Please ensure the file exists.\n", argv[2]);
			exit(0);
		}
		printf("Solving ...\n");
		auto start = clock();
		int count = solver::solve(file, fout);
		auto end = clock();
		printf("Solved %d puzzles in %.3f seconds.\nAnswers has been saved into sudoku.txt .\n", count, (double)(end-start)/CLOCKS_PER_SEC);
		file ? fclose(file) : 0;
		fout ? fclose(fout) : 0;
	}
	else
	{
		/*
			Illegal commands
		*/
		printf("Usage :\n[Generate] sudoku.exe -c {count}\n[Solving] soduku.exe -s\n");
	}
    return 0;
}

