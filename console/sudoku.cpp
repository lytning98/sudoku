// sudoku.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "generator.h"

int main(int argc, char** argv)
{
	if (!(argc == 2 && argv[1][1] == 's' || argc == 3 && argv[1][1] == 'c')) {
		printf("Usage :\n[Generate] sudoku.exe -c {count}\n[Solving] soduku.exe -s\n");
	}
	else if(argc == 3){
		int count = 0;
		sscanf_s(argv[2], "%d", &count);
		if(count<1||count>1000000){
			printf("[Generate] sudoku.exe -c {count}\nWhere {count} is integer in range [1, 1000000]\n");
		}
		else {
			printf("Generating ...\n");
			FILE* file;
			fopen_s(&file, "sudoku.txt", "w");
			while(count--){
				generator::generate(file, (bool)(count!=0));
			}
			printf("Done\n");
		}
	}
	else {

	}
    return 0;
}

