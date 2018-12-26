using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GUI
{
    public enum State
    {
        Welcome, Going, WaitingForAnswer, WaitingForMenu,Finished
    };

    public partial class Form1 : Form
    {
        State GameState = State.Welcome;

        Button[] btns = new Button[81];

        int[,] values = new int[9, 9];
        int cx, cy;

        public Form1()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Reset the board UI for welcome scene
        /// </summary>
        private void ResetBoard()
        {
            foreach (Button btn in btns)
                btn.Text = "";
            btns[30].Text = "选"; btns[30].ForeColor = Color.Black;
            btns[32].Text = "择"; btns[32].ForeColor = Color.Black;
            btns[48].Text = "难"; btns[48].ForeColor = Color.Black;
            btns[50].Text = "度"; btns[50].ForeColor = Color.Black;
            for (int i = 0; i < 5; i++)
            {
                btns[38 + i].Text = (i + 1).ToString();
                btns[38 + i].ForeColor = Color.Black;
                // Gradient Color (100, 255, 70) -> (255, 100, 70)
                btns[38 + i].BackColor =
                    Color.FromArgb(100 + (255 - 100) / 5 * (i + 1),
                        255 + (100 - 255) / 5 * (i + 1), 70);
            }
            Notice.Text = "点击数字方格选择难度以开始游戏";
        }

        /// <summary>
        /// Re-paint the board
        /// </summary>
        private void PaintBoard()
        {
            for (int i = 0; i < 9; i++)
                for (int j = 0; j < 9; j++)
                { 
                    if (Sudoku.self.board[i, j] != 0)
                        btns[i * 9 + j].Text = Sudoku.self.board[i, j].ToString();
                    else
                        btns[i * 9 + j].Text = "";
                    if (Sudoku.self.isFixed[i, j])
                        btns[i * 9 + j].ForeColor = Color.Black;
                    else
                        btns[i * 9 + j].ForeColor = Color.Blue;

                    int blockid = i / 3 * 3 + j / 3;
                    if (blockid % 2 == 0)
                    {
                        btns[i * 9 + j].BackColor = Color.FromArgb(230, 230, 230);
                    }
                    else
                    {
                        btns[i * 9 + j].BackColor = Color.FromArgb(195, 195, 195);
                    }
                }
            if (Sudoku.self.IsFull())
            {
                Notice.Text = "数独已填满，但不符合要求！";
                foreach (Button btn in btns)
                    btn.BackColor = Color.MistyRose;
            }
            else
                Notice.Text = "左键单击空白方格以填入数字，右键单击呼出菜单";
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // positioning the buttons
            foreach(Control Ctrl in this.Controls)
            {
                if(Ctrl.Name.Substring(0, 6) == "button")
                {
                    string number = Ctrl.Name.Substring(6);
                    int id = int.Parse(number);
                    id--;
                    btns[id] = (Button)Ctrl;
                    int row = id / 9, col = id % 9;
                    Ctrl.Width = 47;
                    Ctrl.Height = 47;
                    Ctrl.Top = 11 + 48 * row + row / 3;
                    Ctrl.Left = 21 + 48 * col + col / 3;
                    if(Ctrl != button1) {
                        Ctrl.MouseUp += button1_Click;
                    }
                }
            }
            PaintBoard();
            ResetBoard();
        }

        /// <summary>
        /// Get adjacent 3 rows/columns without crossing the boundaries.
        /// </summary>
        /// <param name="posi"></param>
        /// <returns></returns>
        private int[] GetAdjacent(int posi)
        {
            int[] ret = new int[3];
            if (posi == 0)
                ret = new int[3] { 0, 1, 2 };
            else if (posi == 8)
                ret = new int[3] { 6, 7, 8 };
            else
                ret = new int[3] { posi - 1, posi, posi + 1 };
            return ret;
        }

        /// <summary>
        /// Show context menu on the board
        /// </summary>
        /// <param name="row">Row number</param>
        /// <param name="col">Colomn number</param>
        private void ShowContextMenu(int row, int col)
        {
            Notice.Text = "点击文字方格选择功能";
            int[] rows = GetAdjacent(row), cols = GetAdjacent(col);
            foreach (int i in rows)
                foreach (int j in cols)
                {
                    btns[i * 9 + j].Text = "";
                    btns[i * 9 + j].BackColor = Color.LightYellow;
                }
            Array.Clear(values, 0, values.Length);

            btns[rows[0] * 9 + cols[0]].Text = "重";
            btns[rows[0] * 9 + cols[0]].BackColor = Color.LightBlue;
            btns[rows[0] * 9 + cols[0]].ForeColor = Color.Black;
            btns[rows[0] * 9 + cols[1]].Text = "开";
            btns[rows[0] * 9 + cols[1]].BackColor = Color.LightBlue;
            btns[rows[0] * 9 + cols[1]].ForeColor = Color.Black;
            values[rows[0], cols[0]] = values[rows[0], cols[1]] = 1;

            btns[rows[2] * 9 + cols[1]].Text = "首";
            btns[rows[2] * 9 + cols[1]].BackColor = Color.LightGreen;
            btns[rows[2] * 9 + cols[1]].ForeColor = Color.Black;
            btns[rows[2] * 9 + cols[2]].Text = "页";
            btns[rows[2] * 9 + cols[2]].BackColor = Color.LightGreen;
            btns[rows[2] * 9 + cols[2]].ForeColor = Color.Black;
            values[rows[2], cols[1]] = values[rows[2], cols[2]] = 2;
        }

        /// <summary>
        /// Show input menu on the board
        /// </summary>
        /// <param name="row">Row number</param>
        /// <param name="col">Colomn number</param>
        private void ShowInputMenu(int row, int col)
        {
            Array.Clear(values, 0, values.Length);
            Notice.Text = "点击黄色方格以填入一个答案，点击无关方格清空答案";
            int[] rows = GetAdjacent(row), cols = GetAdjacent(col);
            cx = row; cy = col;
            int old_value = Sudoku.self.board[row, col];
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                {
                    values[rows[i], cols[j]] = (i * 3 + j + 1);
                    int btn_id = rows[i] * 9 + cols[j];
                    btns[btn_id].Text = (i * 3 + j + 1).ToString();

                    if (rows[i] != cx || cols[j] != cy)
                        btns[btn_id].BackColor = Color.FromArgb(255, 255, 128);
                    else
                        btns[btn_id].BackColor = Color.FromArgb(249, 200, 64);
                    if (i * 3 + j + 1 == old_value)
                        btns[btn_id].ForeColor = Color.Blue;
                    else
                        btns[btn_id].ForeColor = Color.Green;
                }
        }

        private void ShowWelcomeScene()
        {
            Sudoku.self.Clear();
            PaintBoard();
            ResetBoard();
            GameState = State.Welcome;
        }

        private void button1_Click(object sender, MouseEventArgs e)
        {
            System.Diagnostics.Debug.WriteLine((e).Clicks);
            bool rightclick = e.Button == MouseButtons.Right;

            int id = int.Parse(((Button)sender).Name.Substring(6)) - 1;
            System.Diagnostics.Debug.WriteLine("Clicked Btn." + id);
            // Click to restart game
            if(GameState == State.Finished)
            {
                ShowWelcomeScene();
                return;
            }
            // Click to start game
            if (id >= 38 && id <= 42 && GameState == State.Welcome)
            {
                Notice.Text = "Loading...";
                GameState = State.Going;
                Sudoku.self.base_spaces_cnt = id - 38 + 1;
                Sudoku.self.random_spaces_append = (id - 38 + 1) / 2;
                Sudoku.self.Generate();
                PaintBoard();
                return;
            }
            int row = id / 9, col = id % 9;
            // long click to call contex menu
            if(GameState == State.Going && rightclick)
            {
                GameState = State.WaitingForMenu;
                ShowContextMenu(row, col);
                return;
            }
            // selected a menu item
            if(GameState == State.WaitingForMenu)
            {
                // Irrelative item
                if(values[row, col] == 0)
                {
                    GameState = State.Going;
                    PaintBoard();
                }
                // Remake
                else if(values[row, col] == 1)
                {
                    GameState = State.Going;
                    Sudoku.self.Generate();
                    PaintBoard();
                }
                // Back to home page
                else
                {
                    ShowWelcomeScene();
                }
                return;
            }
            // Click to input answer
            if (GameState == State.Going && !rightclick)
            {
                if (Sudoku.self.isFixed[row, col]) return;
                GameState = State.WaitingForAnswer;
                ShowInputMenu(row, col);
                return;
            }
            // Selected an candidate answer
            if(GameState == State.WaitingForAnswer)
            {
                // Clicked irrelative cells
                Sudoku.self.board[cx, cy] = values[row, col];
                if (Sudoku.self.IsCorrect())
                {
                    PaintBoard();
                    foreach (Button btn in btns)
                        btn.BackColor = Color.PaleGreen;
                    Notice.Text = "恭喜完成题目！点击任何方格以回到首页";
                    GameState = State.Finished;
                    return;
                }
                PaintBoard();
                GameState = State.Going;
                return;
            }
        }

    }
}
