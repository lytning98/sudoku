# Sudoku

2018 年秋季学期《软件工程基础》课程个人项目作业，共分为相互独立的两个项目：可进行数独终局生成和数独求解的控制台应用程序、可生成数独谜题并共用户进行数独游戏的图形用户界面（GUI）应用程序。

更多信息可阅读介绍本项目的 <a href="http://123.206.78.69/sudoku/" target="_blank">Blog</a> 。（*若链接无法访问，请尝试 <a href="https://blog.csdn.net/Lytning/article/details/84853057" target="_blank">CSDN 链接</a>* ）

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

