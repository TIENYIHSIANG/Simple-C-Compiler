/*********************** parser.c **********************/
#include <stdio.h>
#include <stdlib.h>
/*
** �ۭq���Y��
*/
  #include "scanner.h"
  #include "resword.h"
  #include "err.h"
  #include "followsym.h"
  #include "idobj.h"
  #include "idobjstack.h"
/*
** �ۭq�`��
*/
  #define IDLEN      50
  #define BUFSIZE   512
  #define PROCDEPTH  36
/*
** �ۭq�禡�쫬
*/
  void Program();
  void ProgramHead();
  void MainFunction();
  void Block();
  void IntDeclaration();
  void Statement();
  void AssignmentStatement();
  void CompoundStatement();
  void IfStatement();
  void WhileStatement();
  void ReadStatement();
  void WriteStatement();
  int Condition();
  void Expression();
  void Term();
  void Factor();
  void Identifier();
  void Number();
/*
** �����ܼ�
*/
  struct symbolTag *token;
  int errorCount = 0;
  struct idobjTag *idobj;
  int level = -1;
/*
** Error()
*/
  void Error(int n)
  {
    int j;
    printf("****");
    for (j=0; j<token->right; j++) printf(" ");
    printf("^%d  %s\n",n, errmsgs[n]);
    errorCount++;
  }
/*
** checkexist()
*/
  int checkexist()
  {
    idobj=getIdobj(token->value);
    if (idobj==NULL)
      return 0;
    else
      return 1;
  }
/*
** skip()
*/
  void skip(char follows[], int n)
  {
    if (follows[token->sym]==0)
    {
      Error(n);
      while (follows[token->sym]==0)
        token = nextToken();
    }
  }
/*
** �y�k�W�h#1 <Program>
*/
  void Program()
  {
  	if(token->sym == symHEAD)
  	{
  		ProgramHead();	
	}
	MainFunction();
  }
/*
** �y�k�W�h#2 <ProgramHead>
*/
  void ProgramHead()
  {
    if (token->sym == symHEAD)
    {
      token = nextToken();
      if (strcmp(token->value, "include")==0)
      {
        token = nextToken();
      }
      else
        Error(1);
      if (token->sym == symSTRING)
      {
      	token = nextToken();
	  }
    }
    else
      Error(0);
  }
/*
** �y�k�W�h#3 <MainFunction>
*/
  void MainFunction()
  {
    if (strcmp(token->value, "int")==0)
    {
    	token = nextToken();
    }
	else
	{
		Error(9);	
	} 
    if (strcmp(token->value, "main")==0)
    {
    	token = nextToken();
    }
    else
    {
    	Error(8);
	}
    if (token->sym == symLPAREN)
	{
	  	token = nextToken();
	}
	else
	{
	  	Error(2);
	}
    if (token->sym == symRPAREN)
	{
	  	token = nextToken();
	}
	else
	{
	  	Error(3);
	}
    if (token->sym == symLBRACE)
	{
	  	token = nextToken();
	}
	else
	{
	  	Error(4);
	}
	Block();
	if (strcmp(token->value, "return")==0)
	{
	  	token = nextToken();
	}
	else
	{
	  	Error(6);
	}
	if (strcmp(token->value, "0")==0)
	{
	  	token = nextToken();
	}
	if (token->sym == symSEMI)
	{
	  	token = nextToken();
	}
	else
	{
	  	Error(7);
	}
	if (token->sym == symRBRACE)
	{
	  	token = nextToken();
	}
	else
	{
	  	Error(5);
	}    
  }
/*
** �y�k�W�h#4 <Block>
*/
  void Block()
  {
    ++level;
    if (strcmp(token->value, "int")==0)
      IntDeclaration();
    CompoundStatement();
    --level;
  }
/*
** �y�k�W�h#5 <IntDeclaration>
*/
  void IntDeclaration()
  {
    if (strcmp(token->value, "int")==0)
    {
      token = nextToken();
      if (token->sym == symIDENTIFIER)
      {
        if (checkexist()) Error(12);
        idobj = newIdobj(token->value, token->sym,
                         symINT, level);
        idobjpush(idobj);
        token = nextToken();
      }
      while (token->sym == symCOMMA)
      {
        token = nextToken();
        if (token->sym == symIDENTIFIER)
        {
          if (checkexist()) Error(12);
          idobj = newIdobj(token->value, token->sym,
                           symINT, level);
          idobjpush(idobj);
          token = nextToken();
        }
      }
      if (token->sym == symSEMI)
      {
        token = nextToken();
      }
      else
      {
        Error(7);
        skip(statement, 20);
        if (token->sym == symSEMI)
          token = nextToken();
      }
    }
    else
    {
      Error(9);
    }
  }
/*
** �y�k�W�h#6 <Statement>
*/
  void Statement()
  {
	if (strcmp(token->value,"printf")==0)
        WriteStatement();
    else if (token->sym == symIDENTIFIER)
      AssignmentStatement();
    if(token->sym == symSEMI)
    {
    	token = nextToken();
	}
	else
		Error(7); 
  }
/*
** �y�k�W�h#7 <AssignmentStatement>
*/
  void AssignmentStatement()
  {
    Identifier();
    if (token->sym == symASSIGN)
    {
      token = nextToken();
      Expression();
    }
    else
    {
      Error(10);
      skip(statement, 20);
    }
  }
/*
** �y�k�W�h#8 <CompoundStatement>
*/
  void CompoundStatement()
  {
  	while(strcmp(token->value,"printf")==0||strcmp(token->value,"scanf")==0||
	  (token->sym == symIDENTIFIER&&strcmp(token->value,"return")!=0)||
	  strcmp(token->value,"if")==0||strcmp(token->value,"while")==0)
	{
		if(strcmp(token->value,"if")==0)
		{
			IfStatement();
		}
		else if(strcmp(token->value,"while")==0)
		{
			WhileStatement();
		}
		else if(strcmp(token->value,"scanf")==0)
		{
			ReadStatement();
		}
	  	else if(strcmp(token->value,"printf")==0||token->sym == symIDENTIFIER)
	  	{
	  		Statement();
		}
	}
  }
/*
** �y�k�W�h#9 <IfStatement>
*/
  void IfStatement()
  {
  	int erroroccur=0;
    if (strcmp(token->value,"if")==0)
    {
    	token = nextToken();
      	if(token->sym == symLPAREN)
      	{
      		token = nextToken();
      		erroroccur=Condition();
      		if(erroroccur==1||token->sym == symRPAREN)
      		{
      			token = nextToken();		
			}
			else
				Error(3);
	  	}
	  	else
	  		Error(2);
	  	if(token->sym == symLBRACE)
	  	{
	  		token = nextToken();
      		while(strcmp(token->value,"printf")==0||token->sym == symIDENTIFIER)
      		{
      			Statement();
			}
      		if(token->sym == symRBRACE)
      		{
      			token = nextToken();		
			}
			else
				Error(5);
	  		
		}
		else
			Error(4);
    }
    else
    {
      Error(11);
      skip(statement, 20);
    }
  }
/*
** �y�k�W�h#10 <WhileStatement>
*/
  void WhileStatement()
  {
  	
    if (strcmp(token->value,"while")==0)
    {
      int erroroccur=0;
      printf("%");
      token = nextToken();
      if(token->sym == symLPAREN)
      {
      	token = nextToken();
      	erroroccur=Condition();
      	if(erroroccur==1||token->sym == symRPAREN)
      	{
      		token = nextToken();
      		if(token->sym == symLBRACE)
      		{
      			token = nextToken();
      			while(strcmp(token->value,"printf")==0||token->sym == symIDENTIFIER)
      			{
      				Statement();
				}
      			if(token->sym == symRBRACE)
      			{
      				token = nextToken();
				}
				else
				{
					Error(5);
				}  
			}
			else
			{
				Error(4);
			}
		}
		else
		{
			Error(3);
		}
	  }
	}
	else
		Error(14);  
  }
/*
** �y�k�W�h#11 <ReadStatement>
*/
  void ReadStatement()
  {
    if (strcmp(token->value,"scanf")==0)
    {
      token = nextToken();
      if (token->sym == symLPAREN)
      {
        token = nextToken();
        if (token->sym == symSTRING&&strcmp(token->value,"%d")==0)
        {
          token = nextToken();
        }
        if (token->sym == symCOMMA)
        {
          token = nextToken();
          if (token->sym == symIDENTIFIER)
          {
            Identifier();
          }
          else
          	Error(17);
        }
        if (token->sym == symRPAREN)
          token = nextToken();
        else
        {
          Error(3);
          skip(statement, 20);
        }
        if (token->sym == symSEMI)
        {
        	token = nextToken();
		}
		else
			Error(7);
      }
      else
      {
        Error(2);
        skip(statement, 20);
      }
    }
    else
    {
      Error(13);
      skip(statement, 20);
    }
  }
/*
** �y�k�W�h#12 <WriteStatement>
*/
  void WriteStatement()
  {
    if (strcmp(token->value,"printf")==0)
    {
      token = nextToken();
      if (token->sym == symLPAREN)
      {
        token = nextToken();
        if (token->sym == symSTRING&&strcmp(token->value,"%d")==0)
        {
          token = nextToken();
        }
        if (token->sym == symCOMMA)
        {
          token = nextToken();
          if (token->sym == symIDENTIFIER)
          {
            Identifier();
          }
          else
          	Error(17);
        }
        if (token->sym == symRPAREN)
          token = nextToken();
        else
        {
          Error(3);
          skip(statement, 20);
        }
      }
      else
      {
        Error(2);
        skip(statement, 20);
      }
    }
    else
    {
      Error(15);
      skip(statement, 20);
    }
  }
/*
** �y�k�W�h#13 <Condition>
*/
  int Condition()
  {
    Expression();
    if (token->sym == symLESS ||
        token->sym == symLEQ ||
        token->sym == symEQ ||
        token->sym == symNEQ ||
        token->sym == symGREATER ||
        token->sym == symGEQ)
    {
      token = nextToken();
      Expression();
      return 0;
    }
    else
    {
      Error(16);
      skip(condition, 20);
      return 1;
    }
  }
/*
** �y�k�W�h#14 <Expression>
*/
  void Expression()
  {
    if (token->sym == symPLUS ||
        token->sym == symMINUS)
    {
      token = nextToken();
    }
    Term();
    while (token->sym == symPLUS ||
           token->sym == symMINUS)
    {
      token = nextToken();
      Term();
    }
  }
/*
** �y�k�W�h#15 <Term>
*/
  void Term()
  {
    Factor();
    while (token->sym == symMUL || token->sym == symDIV)
    {
      token = nextToken();
      Factor();
    }
  }
/*
** �y�k�W�h#16 <Factor>
*/
  void Factor()
  {
    if (token->sym == symIDENTIFIER)
    {
      Identifier();
    }
    else if (token->sym == symNUMBER)
    {
      Number();
    }
    else if (token->sym == symLPAREN)
    {
      token = nextToken();
      Expression();
      if (token->sym == symRPAREN)
        token = nextToken();
      else
      {
        Error(3);
        skip(factor, 20);
      }
    }
  }
/*
** �ѧO�r�ŰO�B�z
*/
  void Identifier()
  {
    if (token->sym == symIDENTIFIER)
    {
      idobj=getIdobj(token->value);
      if (idobj == NULL)
        Error(18); 
      token = nextToken();
    }
    else
      Error(17);
  }
/*
** �Ʀr�ŰO�B�z
*/
  void Number()
  {
    if (token->sym == symNUMBER)
      token = nextToken();
    else
      Error(19);
  }
/*
************************** �D�{�� **********************
*/
  int main(int argc, char *argv[])
  {
    FILE *f=fopen(argv[1], "r");
    scanner(f);
    followsyminit();
    token = nextToken();
    Program();
    fclose(f);
    printf("\n  C compile completed. "
      "\n  Error count : %d\n", errorCount);
    if (argc==3)
    {
      printf("\n�ѧO�r���c���|���e�p�U�G\n%s\n",
        idobjstackToString());
    }
    return 0;  
  }
