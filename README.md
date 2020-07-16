# Simple-C-Compiler

一、支援語法規則
1. <Program> ::= [<ProgramHead>]<MainFunction> 
2. <ProgramHead> ::= #inculde <String>
3. <MainFunction> ::= int main() ‘{‘ <Block> return 0; ‘}' 
4. <Block> ::= [<IntDeclaration>] <CompoundStatement>  
5. <IntDeclaration> ::= int <IdentifierList> ; 
6. <Statement> ::= [<AssignmentStatement> | <WriteStatement>] ;             
7. <AssignmentStatement> ::= <Identifier> = <Expression>
8. <CompoundStatement> ::= {<Statement>| <IfStatement> | <WhileStatement> | <ReadStatement>} 
9. <IfStatement> ::= if (<Condition>) ’{‘ {<Statement>} ’}’
10. <WhileStatement> ::= while( <Condition>) ’{‘ {<Statement>} ’}’
11. <ReadStatement> ::= scanf ( “%d”,<Identifier> );     
12. <WriteStatement> ::= printf ( “%d”,<Identifier> )
13. <Condition> ::= <Expression> \<|<=|=|<>|>|>=\<Expression>
14. <Expression> ::= [+|-] <Term> { \+|-\ <Term> }
15. <Term> :: = <Factor> { \ *|/ \ <Factor> }
16. <Factor> ::= <Identifier>|<Number>| (<Expression>)
17. <Identifier> ::= <Alpha> { <Alpha> | <Digit> }
18. <Number> ::= <Digit> { <Digit> }
19. <Alpha> ::= A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z|a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z
20. <Digit> ::= 0|1|2|3|4|5|6|7|8|9
21. <String> ::= " 任何非雙引號的字元集合 "




二、	製作成果
(a)	Lexer
  ![image](https://github.com/TIENYIHSIANG/Simple-C-Compiler/blob/master/images/1.png)
  ![image](https://github.com/TIENYIHSIANG/Simple-C-Compiler/blob/master/images/2.png)
  ![image](https://github.com/TIENYIHSIANG/Simple-C-Compiler/blob/master/images/3.png)
  ![image](https://github.com/TIENYIHSIANG/Simple-C-Compiler/blob/master/images/4.png)
(b)	Parser
  ![image](https://github.com/TIENYIHSIANG/Simple-C-Compiler/blob/master/images/5.png)
 錯誤
  ![image](https://github.com/TIENYIHSIANG/Simple-C-Compiler/blob/master/images/6.png)
(c)	Code Generator
;************** test_r.asm ****************
;
	ORG	100H
	JMP	_start1
_intstr	DB	'     ','$'
_buf	TIMES 256 DB ' '
	DB 13,10,'$'
%include	"dispstr.mac"
%include	"itostr.mac"
%include	"readstr.mac"
%include	"strtoi.mac"
%include	"newline.mac"
a	DW	0
b	DW	0
_start1:
	readstr	_buf
	strtoi	_buf, '$', a
	newline
	PUSH	WORD [a]
	PUSH	10
	POP		BX
	POP		AX
	CMP		AX, BX
	JG		_go2
	JMP		_go3
_go2:
	PUSH	WORD [a]
	PUSH	2
	POP		BX
	POP		AX
	SUB		AX, BX
	PUSH	AX
	PUSH	3
	POP		BX
	POP		AX
	MUL		BX
	PUSH	AX
	POP		AX
	MOV		[b], AX
_go3:
	PUSH	WORD [a]
	PUSH	10
	POP		BX
	POP		AX
	CMP		AX, BX
	JLE		_go4
	JMP		_go5
_go4:
	PUSH	WORD [a]
	PUSH	2
	POP		BX
	POP		AX
	ADD		AX, BX
	PUSH	AX
	PUSH	3
	POP		BX
	POP		AX
	MUL		BX
	PUSH	AX
	POP		AX
	MOV		[b], AX
_go5:
_go6:
	PUSH	WORD [a]
	PUSH	0
	POP		BX
	POP		AX
	CMP		AX, BX
	JGE		_go7
	JMP		_go8
_go7:
	PUSH	WORD [b]
	PUSH	1
	POP		BX
	POP		AX
	ADD		AX, BX
	PUSH	AX
	POP		AX
	MOV		[b], AX
	PUSH	WORD [a]
	PUSH	1
	POP		BX
	POP		AX
	SUB		AX, BX
	PUSH	AX
	POP		AX
	MOV		[a], AX
	JMP		_go6
_go8:
	itostr	a, _intstr, '$'
	MOV		DX, _intstr
	MOV		AH, 09H
	INT		21H
	newline
	itostr	b, _intstr, '$'
	MOV		DX, _intstr
	MOV		AH, 09H
	INT		21H
	newline
	MOV		AX, 4C00H
INT		21H
(d)	Dosbox執行.com檔
    ![image](https://github.com/TIENYIHSIANG/Simple-C-Compiler/blob/master/images/7.png)
