# 语法分析_语法检测及AST生成

## 介绍&使用说明

本次实验在词法分析的基础上，修改了 .l 文件来完成文件的读取，参照 SysY 语言的语法规则来编写了递归下降程序rdcheck.c来对测试用例的语法进行分析，并在此基础上完成了rdparser.c来生成语法树。

1. rdcheck.c 文件为不生成AST的语法分析器；
2. rdparser.c 文件为生成AST的语法分析器；
3. expr.txt 为测试用例；
4. 编译 rdcheck.c 与 rdparser.c 时需要与 lex.yy.c 联合编译：

~~~ 
gcc -o test rdcheck.c lex.yy.c
~~~

5. lex.yy.c 为 sysy_lex.l 经过flex工具生成的词法分析器，具体使用可见词法分析器章节：https://gitee.com/Brett-Tseng/Lexical_Analysis

### 注意：在进行rdparser.c的测试时：

1. 运行程序默认打开自带的expr.txt；
2. 若需检查自定义文件，需要在命令行输入文件名；如：.\rdparser.exe  expr2.txt
3. 终端最多输入一个文件名，否则将会报 argcs too many 并退出！

