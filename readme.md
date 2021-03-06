# Sudoku

2018 年秋季学期《软件工程基础》课程个人项目作业，共分为相互独立的两个项目：可进行数独终局生成和数独求解的控制台应用程序、可生成数独谜题并供用户进行数独游戏的图形用户界面（GUI）应用程序。

更多信息可阅读介绍本项目的相关 Blog：

- 介绍控制台版本程序的 <a href="http://123.206.78.69/sudoku/" target="_blank">Blog</a> 。（*若链接无法访问，请尝试 <a href="https://blog.csdn.net/Lytning/article/details/84853057" target="_blank">CSDN 链接</a>* ）
- 简单介绍 GUI 版本 <a href="http://123.206.78.69/sudoku/blog_GUI.html" target="_blank">Blog</a> 。（ *<a href="https://blog.csdn.net/Lytning/article/details/85269947" target="_blank">CSDN 链接</a>* ）

### 目录结构

```
/ bin					可执行文件等二进制文件目录
	/ sudoku.exe				控制台程序可执行文件
	/ GUI.exe				GUI 可执行文件
/ console				控制台程序工程目录
	/ generator.cpp				生成器实现
	/ generator.h				生成器头文件
	/ solver.cpp				求解器实现
	/ solver.hpp				求解器头文件
	/ sudoku.cpp				主函数实现文件
	/ ...					其他
/ UnitTest				控制台程序单元测试工程目录
	/ UnitTest.cpp				单元测试实现
	/ ...					其他
/ GUI					GUI 程序工程目录
	/ Form1.cs				主窗体实现文件
	/ sudoku.cs				数独逻辑实现文件
	/ ...
/ blog					博客作业目录
	/ img					Markdown 文件引用的图片目录
	/ blog.md				Markdown 格式的博客
	/ blog.pdf				博客的 PDF 导出
```

​      

### 控制台程序调用参数

#### 生成数独终局


```shell
sudoku.exe -c count
```

其中 `count` 为生成数量，终局生成至同目录下的 `sudoku.txt` 中。

#### 求解数独

```shell
sudoku.exe -s path
```

其中 `{path}` 为数独题目的路径，解保存至同目录下的 `sudoku.txt` 中。

`{path}` 指定的文件应为文本文件，连续的以空格、Tab 或换行符分隔的 81 个数字 0~9 被视为一组数独题目，其中零值代表待填方格，非零值代表给定值。一个示例如下：

```
9 0 8 0 6 0 1 2 4
2 3 7 4 5 1 9 6 8
1 4 6 0 2 0 3 5 7
0 1 2 0 7 0 5 9 3
0 7 3 0 1 0 4 8 2
4 8 0 0 0 5 6 0 1
7 0 4 5 9 0 8 1 6
8 9 0 7 4 6 2 0 0
3 0 5 0 8 0 7 0 9
9 0 0 8 0 0 4 0 0
```

​      

### GUI 效果演示

详细的操作说明请查阅介绍 GUI 的 Blog 文章。


![demo](https://raw.githubusercontent.com/lytning98/sudoku/master/blog/img/DEMO.gif)

