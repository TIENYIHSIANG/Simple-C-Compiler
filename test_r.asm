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
	POP	BX
	POP	AX
	CMP	AX, BX
	JG	_go2
	JMP	_go3
_go2:
	PUSH	WORD [a]
	PUSH	2
	POP	BX
	POP	AX
	SUB	AX, BX
	PUSH	AX
	PUSH	3
	POP	BX
	POP	AX
	MUL	BX
	PUSH	AX
	POP	AX
	MOV	[b], AX
_go3:
	PUSH	WORD [a]
	PUSH	10
	POP	BX
	POP	AX
	CMP	AX, BX
	JLE	_go4
	JMP	_go5
_go4:
	PUSH	WORD [a]
	PUSH	2
	POP	BX
	POP	AX
	ADD	AX, BX
	PUSH	AX
	PUSH	3
	POP	BX
	POP	AX
	MUL	BX
	PUSH	AX
	POP	AX
	MOV	[b], AX
_go5:
_go6:
	PUSH	WORD [a]
	PUSH	0
	POP	BX
	POP	AX
	CMP	AX, BX
	JGE	_go7
	JMP	_go8
_go7:
	PUSH	WORD [b]
	PUSH	1
	POP	BX
	POP	AX
	ADD	AX, BX
	PUSH	AX
	POP	AX
	MOV	[b], AX
	PUSH	WORD [a]
	PUSH	1
	POP	BX
	POP	AX
	SUB	AX, BX
	PUSH	AX
	POP	AX
	MOV	[a], AX
	JMP	_go6
_go8:
	itostr	a, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	itostr	b, _intstr, '$'
	MOV	DX, _intstr
	MOV	AH, 09H
	INT	21H
	newline
	MOV	AX, 4C00H
	INT	21H
