/***************** symname.h ****************/
#include <stdlib.h>
#include "sym.h"
char names[symSYMMAX][32];
void symnameinit()
{
  strcpy(names[symEOF],"EOF");
  strcpy(names[symerror],"error");
  strcpy(names[symIDENTIFIER],"IDENTIFIER");
  strcpy(names[symNUMBER],"NUMBER");
  strcpy(names[symPLUS],"PLUS");
  strcpy(names[symMINUS],"MINUS");
  strcpy(names[symMUL],"MUL");
  strcpy(names[symDIV],"DIV");
  strcpy(names[symEQ],"EQ");
  strcpy(names[symNEQ],"NEQ");
  strcpy(names[symLESS],"LESS");
  strcpy(names[symLEQ],"LEQ");
  strcpy(names[symGREATER],"GREATER");
  strcpy(names[symGEQ],"GEQ");
  strcpy(names[symLPAREN],"LPAREN");
  strcpy(names[symRPAREN],"RPAREN");
  strcpy(names[symCOMMA],"COMMA");
  strcpy(names[symSEMI],"SEMI");
  strcpy(names[symPERIOD],"PERIOD");
  strcpy(names[symBEGIN],"BEGIN");
  strcpy(names[symEND],"END");
  strcpy(names[symIF],"IF");
  strcpy(names[symTHEN],"THEN");
  strcpy(names[symWHILE],"WHILE");
  strcpy(names[symDO],"DO");
  strcpy(names[symREAD],"READ");
  strcpy(names[symWRITE],"WRITE");
  strcpy(names[symIDENTIFIERLIST],"IDENTIFIERLIST");
  strcpy(names[symCALL],"CALL");
  strcpy(names[symCONST],"CONST");
  strcpy(names[symINT],"INT");
  strcpy(names[symPROCEDURE],"PROCEDURE");
  strcpy(names[symPROGRAM],"PROGRAM");
  strcpy(names[symSTRING],"STRING");
  strcpy(names[symELSE],"ELSE");
  strcpy(names[symMOD],"MOD");
  strcpy(names[symASSIGN],"ASSIGN");
  strcpy(names[symCOMMENT],"COMMENT");
  strcpy(names[symINC],"INC");
  strcpy(names[symDEC],"DEC");
  strcpy(names[symSHL],"SHL");
  strcpy(names[symSHR],"SHR");
  strcpy(names[symOR],"OR");
  strcpy(names[symLOR],"LOR");
  strcpy(names[symAND],"AND");
  strcpy(names[symLAND],"LAND");
  strcpy(names[symXOR],"XOR");
  strcpy(names[symLBARCE],"LBRACE");
  strcpy(names[symRBARCE],"RBRACE");
  strcpy(names[symHEAD],"symHEAD");
}
