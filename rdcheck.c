/**
	*******************************************************************************************************************
	*******************************************************************************************************************
	*	作者： 		曾彬芮 王云青
	* 
	*	日期： 		2021-11-17
	*
	*	版本： 		V1.0
	*
	*	说明： 		语法分析器（纯语法检测，不生成AST）
	*******************************************************************************************************************
	*******************************************************************************************************************
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum yytokentype
{
	IntConst = 258,
	EOL,
	Ident,
	INT,		 // int
	VOID,		 // void
	CONST,		 // const
	IF,			 // if
	ELSE,		 // else
	WHILE,		 // while
	BREAK,		 // break
	CONTINUE,	 // continue
	RETURN,		 // return
	LESSEQ,		 // <=
	GREATEQ,	 // >=
	NOTEQ,		 // !=
	EQ,		 	 // ==
	AND, 		 // &&
	OR,			 // ||
};

void myUnput(char* yytext, int len);
extern int yylex();
extern int yylval;
extern int yyleng;
extern char *yytext;
extern FILE *yyin;

int tok;

#pragma region functionDecl
int CompUnit();
int Decl();
int ConstDecl();
int BType();
int ConstDef();
int ConstInitVal();
int VarDecl();
int VarDef();
int InitVal();
int FuncDef();
int FuncType();
int FuncFParams();
int FuncFParam();
int Block();
int BlockItem();
int Stmt();
int Exp();
int Cond();
int LVal();
int PrimaryExp();
int Number();
int UnaryExp();
int UnaryOp();
int FuncRParams();
int MulExp();
int AddExp();
int RelExp();
int EqExp();
int LAndExp();
int LOrExp();
int ConstExp();
#pragma endregion

void advance()
{
	tok = yylex();
	// printf("tok: %s\n", yytext);
}

#pragma region recursiveDescent
// CompUnit:  [ CompUnit ] ( Decl | FuncDef )
int CompUnit()
{
	int result;
	// Decl -> const || int   int -> Ident
	// FuncDef -> void || int  int -> Ident Ident -> (
	if(tok == 'q') exit(0);
	while(tok == CONST || tok == VOID || tok == INT){
		if(tok == CONST) result = Decl();
		else if(tok == VOID) result = FuncDef();
		else if(tok == INT){
			char* s1 = strdup(yytext);
			int l1 = yyleng;
			advance();
			if(tok == Ident){
				char* s2 = strdup(yytext);
				int l2 = yyleng;
				advance();
				if(tok == '('){
					myUnput(yytext,yyleng);
					myUnput(s2,l2);
					myUnput(s1,l1);
					advance();
					result = FuncDef();
				}else{
					myUnput(yytext,yyleng);
					myUnput(s2,l2);
					myUnput(s1,l1);
					advance();
					result = Decl();
				}
			}else return -1;
		}
		if(result!=1){
			return -1;
		}
	}
	return result;
}

//Decl: ConstDecl | VarDecl
int Decl()
{
	if(tok == CONST){
		return ConstDecl();
	}
	else return VarDecl();
}

//ConstDecl: 'const' BType ConstDef {','ConstDef} ';'
int ConstDecl()
{
	if(tok == CONST){
		advance();
		if(BType()){
			if(ConstDef()){
				while(tok == ','){
					advance();
					if(!ConstDef()) return -1;
				}
				if(tok == ';'){
					// 不需要步进
					advance();
					return 1;
				}else{
					return -1;
				}
			}else return -1;
		}else return -1;
	}else return -1;
	return -1;
}

//BType: 'int'
int BType()
{
	if(tok == INT)
	{
		advance();
		return 1;
	}
	else return 0;
}

//ConstDef: Ident { '[' ConstExp ']' } '=' ConstInitVal
int ConstDef()
{
	if(tok == Ident){
		advance();
		while(tok == '['){
			advance();
			if(!ConstExp()) return -1;
			if(tok != ']'){
				return -1;
			}else advance();
		}
		if(tok == '='){
			advance();
			return ConstInitVal();
		}
	}
	return -1;
}

//ConstInitVal: ConstExp |  '{' [ ConstInitVal { ',' ConstInitVal } ] '}'
int ConstInitVal()
{
	if(tok == '{'){
		advance();
		if(ConstInitVal()){
			while(tok == ','){
				advance();
				if(!ConstInitVal()) return -1;
			}
			if(tok == '}') return 1;
			else{
				return -1;
			}
		}
		else if(tok == '}') return 1;
		else return -1;
	}
	else{
		return ConstExp();
	}
}

//VarDecl: BType VarDef {',' VarDef}';'
int VarDecl()
{
	if(BType()){
		if(VarDef()){
			while(tok == ','){
				advance();
				if(!VarDef()) return -1;
			}
			if(tok == ';'){
//				advance();
				return 1;
			}else return -1;
		}else return -1;
	}else return -1;

	return -1;
}

//VarDef: Ident {'['ConstExp']'} | Ident {'['ConstExp']'} '=' InitVal
int VarDef()
{
	int l = -1;
	if(tok == Ident){
		advance();
		while(tok == '['){
			advance();
			if(!ConstExp()) return -1;
			if(tok == ']'){
				advance();
				l = 1;
			}
		}
		if(tok == '='){
			advance();
			l = InitVal();
		}
	}
	return l;
}

//InitVal: Exp | '{' [ InitVal { ',' InitVal } ] '}'
int InitVal()
{
	// Exp()
	// if(tok==Ident||tok==IntConst||\
	//    tok=='+'||tok=='-'||tok=='!'||tok=='('){
	// 	   return Exp();
	// }
	if(tok == '{'){
		advance();
		if(InitVal()){
			while(tok == ','){
				if(InitVal()) return -1;
			}
			if(tok == '}'){
				advance();
				return 1;
			}else{				
				return -1;
			}
		}else if(tok == '}'){
			advance();
			return 1;
		}
	}else{
		return Exp();
	}
	return -1;
}

//FuncDef: FuncType Ident '(' [FuncFParams] ')' Block
int FuncDef()
{
	if(FuncType()){
		if(tok == Ident){
			advance();
			if(tok == '('){
				advance();
				if(FuncFParams()){
					if(tok == ')'){
						advance();
						return Block();
					}else return -1;
				}else if(tok==')'){
					advance();
					return Block();
				}else return -1;
			}else return -1;
		}else return -1;
	}else return -1;
}

//FuncType:  'void' | 'int'
int FuncType()
{
	if(tok == VOID || tok == INT){
		advance(); 
		return 1;
	}
	else return -1;
}

//FuncFParams: FuncFParam { ',' FuncFParam }
int FuncFParams()
{
	if(FuncFParam()){
		while(tok == ','){
			advance();
			if(!FuncFParam()) return -1;
		}
		return 1;
	}
	return -1;
}

//FuncFParam: BType Ident ['[' ']' { '[' Exp ']' }]
int FuncFParam()
{
	int l = 0;
	if(BType()){
		if(tok == Ident){
			advance();
			if(tok == '['){
				advance();
				if(tok == ']'){
					advance(); l = 1;
					while(tok == '['){
						advance();
						if(!Exp()) return -1;
						if(tok == ']'){
							advance();
							l = 1;
						}else return -1;
					}
					return l;
				}else{
					return -1;
				}
			} 
			return 1;  
		}else return -1;
	}
	return -1;
}

//Block:  '{' { BlockItem } '}'
int Block()
{
	if(tok == '{')
	{
		advance();
		while(tok!='}')
		{
			if(BlockItem())
			{
				if(tok == '}')
				return 1;
			}
			else return -1;
		}
		if(tok=='}') return 1;
		else return -1;
	}
}

//BlockItem: Decl | Stmt
int BlockItem()
{
	if(tok==CONST || tok == INT){
		return Decl();
	}else{
		return Stmt();
	}
}

//Stmt: LVal '=' Exp ';'
//      | [Exp] ';'
//      | Block
//      | 'if' '( Cond ')' Stmt [ 'else' Stmt ]
//      | 'while' '(' Cond ')' Stmt
//      | 'break' ';'
//      | 'continue' ';'
//      | 'return' [Exp] ';'
int Stmt()
{
    // Stmt: 'if' '(' Cond ')' Stmt [ 'else' Stmt ]
    if(tok == IF){
       advance();
		if(tok == '('){
			advance();
			if(Cond()){
				if(tok == ')'){
					advance();
					if(Stmt()){
						if(tok == ELSE){
							advance();
							return Stmt();
						}else return 1;
					}else return -1;
				}else return -1;
			}else return -1;
		}else return -1;
    }
     // Stmt:'while' '(' Cond ')' Stmt
    else if(tok == WHILE){
        advance();
		if(tok == '('){
			advance();
			if(Cond()){
				if(tok == ')'){
					advance();
					if(Stmt())  return 1; 
					else        return -1;					
				}return -1;
			}return -1;
		}return -1;
    }
     // Stmt:'break' ';'
    else if(tok == BREAK){
        advance();
		if(tok == ';'){
			advance();
			return 1;
		}else return -1;
    }
     // Stmt:'continue' ';'
    else if(tok == CONTINUE){
        advance();
		if(tok == ';'){
			advance();
			return 1;
		}else return -1;
    }
     // Stmt:'return' [Exp] ';'
    else if(tok == RETURN){
		advance();
		if(tok==';'){
			advance();
			return 1;
		}
		else if(Exp()){
			if(tok == ';'){
				advance();
				return 1;
			}else return -1;
		}
		return -1;
    }
     // Stmt: ;
    else if(tok == ';'){
        return 1;
    }
    // Stmt: LVal '=' Exp ';'
    else if(tok == Ident){
        if(LVal()){
            if(tok == '=')
            advance();
            if(Exp()){
                if(tok == ';'){
                	advance();
                return 1;
                }
            }   
            else return -1;
        }
    }
    // Stmt: Block
    else if(tok == '{'){
        return Block();
    }
    // Stmt: Exp ';'
    else{
        if(Exp()){
            if(tok == ';'){
                advance();
                return 1;
            }else return -1;
        }
		else return -1;
    }
}

//Exp: AddExp
int Exp()
{
	return AddExp();
}

//Cond: LOrExp
int Cond()
{
	return LOrExp();
}

//LVal: Ident {'[' Exp ']'}
int LVal()
{
	if(tok == Ident)
	{
		advance();
		while(tok == '[')
		{
			advance();
			int l = Exp();
			if(tok == ']')
			{
				advance();
				return l;
			}
			else return -1;
		}
		return tok;
	}
	else return -1;
}

//PrimaryExp: '(' Exp ')' | LVal | Number
int PrimaryExp()
{
	if(tok == '('){
		advance();
		int l = Exp();
		if(tok == ')'){
			advance();
			return l;
		}else{
			exit(0);
		}
	}
	else if(tok == IntConst){
		return Number();
	}
	else{
		return LVal();
	}
}

//Number: IntConst
int Number()
{
	if (tok == IntConst)
	{
		advance();
		// return yylval;
		return 1;
	}
	return -1;
}

//UnaryExp: PrimaryExp | Ident '(' [FuncRParams] ')' | UnaryOp UnaryExp
int UnaryExp(){
	int u=0;
	// UnaryExp -> UnaryOp UnaryExp
	if(tok == '+' || tok == '-' || tok == '!'){
		advance();
		while(tok == '+' || tok == '-' || tok == '!')
		{
			int oper = UnaryOp();
			advance();
			u = UnaryExp();
			if(oper == '+') {}
			else if(oper == '-') {u = -u;}	
			// else if(oper == '!') {u = 0 ? 0 : 1;}
			advance();
		}
		return u;
	}
	else if(tok == Ident){
		advance();
		if(tok == '('){
			advance(); 
		    if(FuncRParams()){
				if(tok == ')'){
					advance();
					return 1;
				}else return -1;
			}
			else if(tok == ')'){
				advance();
				return 1;
			}
		}
		else return -1;
	}else return PrimaryExp();	
}

//UnaryOp:  '+' | '-' | '!'
int UnaryOp()
{
	if (tok == '+' || tok == '-' || tok == '!')
	{
		advance();
		return tok;
	}else{
		return -1;
	}
}

//FuncRParams: Exp { ',' Exp }
int FuncRParams()
{
	int e_count = 0,c_count = 0;
	while(Exp())
	{
		e_count++;
		if(tok == ',')
		{
			advance();
			c_count++;
		}
	}
	int i = e_count - c_count;
	return i ==1 ? 1 : 0;
}

//MulExp: UnaryExp | MulExp ('*' | '/' | '%') UnaryExp
int MulExp()
{
	int l = UnaryExp();
	while (tok == '*' || tok == '/' || tok == '%')
	{
		int oper = tok;
		advance();
		int r = UnaryExp();
		if (oper == '*')
			l *= r;
		else if (oper == '/')
			l /= r;
		else
			l %= r;
		l=1;
	}
	return l;
}

//AddExp: MulExp | AddExp ('+' | '-') MulExp
int AddExp()
{
	int l = MulExp();
	while (tok == '+' || tok == '-')
	{
		int oper = tok;
		advance();
		int r = MulExp();
		if (oper == '+')
			l += r;
		else
			l -= r;
		l=1;
	}
	return l;
}

//RelExp: AddExp | RelExp ('<' | '>' | '<=' | '>=') AddExp
int RelExp()
{
	int l = AddExp();
	while (tok == '<' || tok == '>' || tok == LESSEQ || tok == GREATEQ)
	{
		int oper = tok;
		advance();
		int r = AddExp();
		if (oper == '<')
			l = (l < r);
		else if (oper == '>')
			l = (l > r);
		else if (oper == LESSEQ)
			l = (l <= r);
		else
			l = (l >= r);
		l=1;
	}
	return l;
}

//EqExp: RelExp | EqExp ('==' | '!=') RelExp
int EqExp()
{
	int l = RelExp();
	while (tok == EQ || tok == NOTEQ)
	{
		int oper = tok;
		advance();
		int r = RelExp();
		if (oper == EQ)
			l = (l == r);
		else
			l = (l != r);
		l=1;
	}
	return l;
}

//LAndExp: EqExp | LAndExp '&&' EqExp
int LAndExp()
{
	int l = EqExp();
	while (tok == AND)
	{
		int oper = tok;
		advance();
		int r = EqExp();
		if (oper == AND)
			l = (l && r);
		l=1;
	}
	return l;
}

//LOrExp: LAndExp | LOrExp '||' LAndExp
int LOrExp()
{
	int l = LAndExp();
	while (tok == OR)
	{
		int oper = tok;
		advance();
		int r = LAndExp();
		if (oper == OR)
			l = (l || r);
		l=1;
	}
	return l;
}

//ConstExp: AddExp
int ConstExp()
{
	return AddExp();
}

#pragma endregion

int main(int argc, char **argv)
{
	while(1){
		printf("input expression, 'q' to exit>");
		advance();
		int r = CompUnit();
		if(r==1) printf("compile successfully!\n");
		else printf("Some errors happend\n");
	}
	return 0;
}	
