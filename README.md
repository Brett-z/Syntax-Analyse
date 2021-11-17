# 语法分析_语法检测及AST生成

## 介绍&使用说明

本次实验在词法分析的基础上，修改了 .l 文件来完成文件的读取，参照 SysY 语言的语法规则来编写了递归下降程序rdcheck.c来对测试用例的语法进行分析，并在此基础上完成了rdparser.c来生成语法树。参照如下语法规则：

![sysy语法](https://images.gitee.com/uploads/images/2021/1117/165328_aa4cae8e_9865699.png "RW]X@(~OTOF}4JZOZ9EJGY2.png")

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

## 测试说明

1. rdcheck.c 测试为：编译运行 rdcheck.c 后在 console端输入一句 SysY 代码。预期结果为：若输入的代码符合 SysY 的语法规则，则输出“compile successfully!”字样，否则会卡顿无输出，此时输入 ctrl z 会打印“Some errors happened”字样。
![rdcheck.c测试](https://images.gitee.com/uploads/images/2021/1117/165758_41622995_9865699.png "LAW)QP5GEA~IWQQUN@3BBIW.png")

2. rdparser.c 测试为：编译 rdparser.c 后，在 console端输入命令示例：

```
.\test expr.txt
```
其中，test 为编译 rdparser.c 生成的.exe 文件名，expr.txt 为目标测试样例的文件名。预期结果为：若输入的代码符合 SysY 的语法规则，则输出对应代码的语法抽象树(AST)，否则不输出 AST。

测试用例为：

```
int a = 1, b = 2;
int array[3] = {1,2,3};
```
测试预期：由于 CompUnit 有两个子节点(均为 Decl)，此处将两个 Decl 节点(Decl 1 & Decl 2)分开表示。
![节点图](https://images.gitee.com/uploads/images/2021/1117/165926_a5ec4847_9865699.png "N1RRV7%Q]042%A{S7KKGGSC.png")
graph by 王云青

测试结果：
![测试结果](https://images.gitee.com/uploads/images/2021/1117/170039_17380bd2_9865699.png "%PE{S%$[}N((_FJMKW[DU}M.png")

## 作者
曾彬芮 王云青 2021/11/17 于四川成都

( 对代码有疑问可联系： e-mail： brettemail@163.com || 392184316@qq.com )