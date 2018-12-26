using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GUI
{
    class Sudoku
    {
        public static Sudoku self = new Sudoku();

        private Random rnd = new Random();
        /// <summary>
        /// Spaces per 3x3 cell
        /// </summary>
        public int base_spaces_cnt, random_spaces_append;

        /// <summary>
        /// Game board
        /// </summary>
        public int[,] board = new int[9, 9];
        public bool[,] isFixed = new bool[9, 9];
        
        public void Clear()
        {
            Array.Clear(board, 0, board.Length);
        }

        /// <summary>
        /// Shift the permutation by w
        /// </summary>
        /// <param name="p">Permutation</param>
        /// <param name="w">Step</param>
        private void Shift(ref int[] p, int w)
        {
            int[] tmp = new int[w];
            for (int i = 0; i < w; i++)
                tmp[i] = p[i];
            for (int i = w; i < 9; i++)
                p[i - w] = p[i];
            for (int i = 0; i < w; i++)
                p[9 - w + i] = tmp[i];
        }

        /// <summary>
        /// Random shuffle a permutation.
        /// </summary>
        /// <param name="p">The permutation</param>
        private void Shuffle(ref int[] p)
        {
            var res = p.OrderBy(c => rnd.Next()).ToList();
            for (int i = 0; i < 9; i++)
                p[i] = res[i];
        }

        /// <summary>
        /// Generate an sudoku puzzle
        /// </summary>
        public void Generate()
        {
            // initial permutation
            int[] p = new int[9] { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
            // random shuffle it
            Shuffle(ref p);
            // arrange the board
            for (int i = 0; i < 3; i++)
                for(int j = 0; j < 3; j++)
                {
                    for (int k = 0; k < 9; k++)
                        board[i * 3 + j, k] = p[k];
                    if (j == 2)
                        Shift(ref p, 1);
                    else
                        Shift(ref p, 3);
                }
            // dig holes to make space 
            for (int i = 0; i < 9; i++)
                for (int j = 0; j < 9; j++)
                    isFixed[i, j] = true;
            for(int i = 0; i < 3; i++)
                for(int j = 0; j < 3; j++)
                {
                    Shuffle(ref p);
                    int spaces = base_spaces_cnt + rnd.Next(0, random_spaces_append + 1);
                    for(int k = 0; k < spaces; k++)
                    { 
                        board[i * 3 + (p[k]-1) / 3, j * 3 + (p[k]-1) % 3] = 0;
                        isFixed[i * 3 + (p[k] - 1) / 3, j * 3 + (p[k] - 1) % 3] = false;
                    }
                }
        }

        /// <summary>
        /// Check whether the puzzle is finished.
        /// </summary>
        /// <returns></returns>
        public bool IsFull()
        {
            for (int i = 0; i < 9; i++)
                for (int j = 0; j < 9; j++)
                    if (board[i, j] == 0)
                        return false;
            return true;
        }

        /// <summary>
        /// Check whether the answer to the puzzle is correct.
        /// </summary>
        /// <returns></returns>
        public bool IsCorrect()
        {
            if (!IsFull()) return false;
            bool[,] rowcnt = new bool[9,9], colcnt = new bool[9,9], blockcnt = new bool[9,9];
            for(int i = 0; i < 9; i++)
                for(int j = 0; j < 9; j++)
                {
                    int block = i / 3 * 3 + j / 3;
                    int color = board[i, j]-1;
                    if (rowcnt[i, color] || colcnt[j, color] || blockcnt[block, color])
                        return false;
                    rowcnt[i, color] = colcnt[j, color] = blockcnt[block, color] = true;
                }
            return true;
        }
    }
}
