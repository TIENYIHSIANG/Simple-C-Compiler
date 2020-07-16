# Simple-C-Compiler

## 一、支援語法規則
1. Program ::= [ProgramHead] MainFunction <br>
2. ProgramHead ::= #inculde String <br>
3. MainFunction ::= int main() ‘{‘ Block return 0; ‘}'<br> 
4. Block ::= [IntDeclaration] CompoundStatement  <br>
5. IntDeclaration ::= int IdentifierList ; <br>
6. Statement ::= [AssignmentStatement | WriteStatement] ;  <br>           
7. AssignmentStatement ::= Identifier = Expression<br>
8. CompoundStatement ::= {Statement| IfStatement | WhileStatement | ReadStatement} <br>
9. IfStatement ::= if (Condition) ’{‘ {Statement} ’}’<br>
10. WhileStatement ::= while( Condition) ’{‘ {Statement} ’}’<br>
11. ReadStatement ::= scanf ( “%d”,Identifier );     <br>
12. WriteStatement ::= printf ( “%d”,Identifier )<br>
13. Condition ::= Expression \<|<=|=|<>|>|>=\ Expression<br>
14. Expression ::= [+|-] Term { \+|-\ Term }<br>
15. Term :: = Factor { \ *|/ \ Factor }<br>
16. Factor ::= Identifier|Number| (Expression)<br>
17. Identifier ::= Alpha { Alpha | Digit }<br>
18. Number ::= Digit { Digit }<br>
19. Alpha ::= A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z|a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z<br>
20. Digit ::= 0|1|2|3|4|5|6|7|8|9<br>
21. String ::= " 任何非雙引號的字元集合 "<br>




## 二、	製作成果
### (a)	Lexer
  ![image](https://github.com/TIENYIHSIANG/Simple-C-Compiler/blob/master/images/1.png)
  ![image](https://github.com/TIENYIHSIANG/Simple-C-Compiler/blob/master/images/2.png)
  ![image](https://github.com/TIENYIHSIANG/Simple-C-Compiler/blob/master/images/3.png)
  ![image](https://github.com/TIENYIHSIANG/Simple-C-Compiler/blob/master/images/4.png)
### (b)	Parser
  ![image](https://github.com/TIENYIHSIANG/Simple-C-Compiler/blob/master/images/5.png)
 #### 錯誤
  ![image](https://github.com/TIENYIHSIANG/Simple-C-Compiler/blob/master/images/6.png)
### (c)	Code Generator
;************** test_r.asm ****************<br>
;<br>
	ORG	100H<br>
	JMP	_start1<br>
_intstr	DB	'     ','$'<br>
_buf	TIMES 256 DB ' '<br>
	DB 13,10,'$'<br>
%include	"dispstr.mac"<br>
%include	"itostr.mac"<br>
%include	"readstr.mac"<br>
%include	"strtoi.mac"<br>
%include	"newline.mac"<br>
a	DW	0<br>
b	DW	0<br>
_start1:<br>
	readstr	_buf<br>
	strtoi	_buf, '$', a<br>
	newline<br>
	PUSH		WORD [a]<br>
	PUSH		10<br>
	POP		BX<br>
	POP		AX<br>
	CMP		AX, BX<br>
	JG		_go2<br>
	JMP		_go3<br>
_go2:<br>
	PUSH		WORD [a]<br>
	PUSH		2<br>
	POP		BX<br>
	POP		AX<br>
	SUB		AX, BX<br>
	PUSH		AX<br>
	PUSH		3<br>
	POP		BX<br>
	POP		AX<br>
	MUL		BX<br>
	PUSH		<br>AX
	POP		AX<br>
	MOV		[b], AX<br>
_go3:<br>
	PUSH		WORD [a]<br>
	PUSH		10<br>
	POP		BX<br>
	POP		AX<br>
	CMP		AX, BX<br>
	JLE		_go4<br>
	JMP		_go5<br>
_go4:<br>
	PUSH		WORD [a]<br>
	PUSH		2<br>
	POP		BX<br>
	POP		AX<br>
	ADD		AX, BX<br>
	PUSH		AX<br>
	PUSH		3<br>
	POP		BX<br>
	POP		AX<br>
	MUL		BX<br>
	PUSH		AX<br>
	POP		AX<br>
	MOV		[b], AX<br>
_go5:<br>
_go6:<br>
	PUSH		WORD [a]<br>
	PUSH		0<br>
	POP		BX<br>
	POP		AX<br>
	CMP		AX, BX<br>
	JGE		_go7<br>
	JMP		_go8<br>
_go7:<br>
	PUSH		WORD [b]<br>
	PUSH		1<br>
	POP		BX<br>
	POP		AX<br>
	ADD		AX, BX<br>
	PUSH		AX<br>
	POP		AX<br>
	MOV		[b], AX<br>
	PUSH		WORD [a]<br>
	PUSH		1<br>
	POP		BX<br>
	POP		AX<br>
	SUB		AX, BX<br>
	PUSH		AX<br>
	POP		AX<br>
	MOV		[a], AX<br>
	JMP		_go6<br>
_go8:<br>
	itostr	a, _intstr, '$'<br>
	MOV		DX, _intstr<br>
	MOV		AH, 09H<br>
	INT		21H<br>
	newline<br>
	itostr	b, _intstr, '$'<br>
	MOV		DX, _intstr<br>
	MOV		AH, 09H<br>
	INT		21H<br>
	newline<br>
	MOV		AX, 4C00H  <br>
	INT		21H  <br>
### (d)	Dosbox執行.com檔
![image](https://github.com/TIENYIHSIANG/Simple-C-Compiler/blob/master/images/7.png)
