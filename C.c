/************************* plone.c *********************/
#include <stdio.h>
#include <stdlib.h>
/*
** 自訂表頭檔
*/
  #include "scanner.h"
  #include "resword.h"
  #include "err.h"
  #include "followsym.h"
  #include "idobj.h"
  #include "idobjstack.h"
/*
** 自訂常數
*/
  #define IDLEN      50
  #define BUFSIZE   512
  #define PROCDEPTH  50
/*
** 自訂函式原型
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
** 整體變數
*/
  struct symbolTag *token;
  int errorCount = 0;
  struct idobjTag *idobj;
  int level = -1;
  int labelCount = 0;
  char progname[IDLEN];
  char outname[IDLEN];
  FILE *outfile;
  char buf[BUFSIZE];
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
/*+
** 語法規則#1 <Program>
*/
  void Program()
  {
  	if(token->sym == symHEAD)
  	{
  		ProgramHead();	
	}	
	MainFunction();
  }
/*+
** 語法規則#2 <ProgramHead>
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
** 語法規則#3 <MainFunction>
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
/*+
** 語法規則#4 <Block>
*/
  void Block()
  {
    ++level;
    if (strcmp(token->value, "int")==0)
      IntDeclaration();
    sprintf(buf, "_start%d:\n", labelCount);
    fprintf(outfile, buf);
    CompoundStatement();
    --level;
  }
/*+
** 語法規則#5 <IntDeclaration>
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
        sprintf(buf, "%s\tDW\t0\n", token->value);
        fprintf(outfile, buf);
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
          sprintf(buf, "%s\tDW\t0\n", token->value);
          fprintf(outfile, buf);
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
/*+
** 語法規則#6 <Statement>
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
/*+
** 語法規則#7 <AssignmentStatement>
*/
  void AssignmentStatement()
  {
    char result[BUFSIZE];
    idobj=getIdobj(token->value);
    if (idobj != NULL)
    {
      sprintf(result, "%s", idobj->name);
    }
    Identifier();
    if (token->sym == symASSIGN)
    {
      token = nextToken();
      Expression();
      sprintf(buf, "\tPOP\tAX\n"
                   "\tMOV\t[%s], AX\n", result);
      fprintf(outfile, buf);
    }
    else
    {
      Error(10);
      skip(statement, 20);
    }
  }
/*+
** 語法規則#8 <CompoundStatement>
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
/*+
** 語法規則#9 <IfStatement>
*/
  void IfStatement()
  {
  	int erroroccur=0;
  	int head, tail;
    if (strcmp(token->value,"if")==0)
    {
    	token = nextToken();
      	if(token->sym == symLPAREN)
      	{
      		token = nextToken();
      		erroroccur=Condition();
      		if(erroroccur==1||token->sym == symRPAREN)
      		{
      			head = labelCount;
                tail = ++labelCount;
                sprintf(buf, "\tJMP\t_go%d\n"
                             "_go%d:\n", tail,head);
                fprintf(outfile, buf);
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
		    sprintf(buf, "_go%d:\n", tail);
            fprintf(outfile, buf);
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
/*+
** 語法規則#10 <WhileStatement>
*/
  void WhileStatement()
  {
    int home, head, tail;
    if (strcmp(token->value,"while")==0)
    {
      int erroroccur=0;
      token = nextToken();
      if(token->sym == symLPAREN)
      {
      	token = nextToken();
      	home = ++labelCount;
        sprintf(buf, "_go%d:\n", home);
        fprintf(outfile, buf);
      	erroroccur=Condition();
      	if(erroroccur==1||token->sym == symRPAREN)
      	{
      		head = labelCount;
      		tail = ++labelCount;
      		sprintf(buf, "\tJMP\t_go%d\n"
            		       "_go%d:\n", tail, head);
      		fprintf(outfile, buf);
      		token = nextToken();
      		if(token->sym == symLBRACE)
      		{
      			token = nextToken();
      			while(strcmp(token->value,"printf")==0||token->sym == symIDENTIFIER)
      			{
      				Statement();
				}
				sprintf(buf, "\tJMP\t_go%d\n", home);
                fprintf(outfile, buf);
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
	  sprintf(buf, "_go%d:\n", tail);
      fprintf(outfile, buf);
	}
	else
		Error(14); 
  }
/*+
** 語法規則#11 <ReadStatement>
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
          	idobj=getIdobj(token->value);
            if (idobj != NULL)
            {
                sprintf(buf, "\treadstr\t_buf\n"
                             "\tstrtoi\t_buf, '$', %s\n"
                             "\tnewline\n", idobj->name);
                fprintf(outfile, buf);
            }
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
/*+
** 語法規則#12 <WriteStatement>
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
          	idobj=getIdobj(token->value);
            if (idobj != NULL)
            {
            	sprintf(buf, "\titostr\t%s, _intstr, '$'\n"
                  			 "\tMOV\tDX, _intstr\n"
                  			 "\tMOV\tAH, 09H\n"
                  			 "\tINT\t21H\n"
                  			 "\tnewline\n", idobj->name);
                fprintf(outfile, buf);
            }
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
/*+
** 語法規則#13 <Condition>
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
      int operator = token->sym;
      token = nextToken();
      Expression();
      sprintf(buf, "\tPOP\tBX\n"
                   "\tPOP\tAX\n"
                   "\tCMP\tAX, BX\n");
      fprintf(outfile, buf);
      switch (operator)
      {
        case symEQ:
          sprintf(buf,"\tJE\t_go%d\n",(++labelCount));
          fprintf(outfile, buf);
          break;
        case symNEQ:
          sprintf(buf,"\tJNE\t_go%d\n",(++labelCount));
          fprintf(outfile, buf);
          break;
        case symLESS:
          sprintf(buf,"\tJL\t_go%d\n",(++labelCount));
          fprintf(outfile, buf);
          break;
        case symLEQ:
          sprintf(buf,"\tJLE\t_go%d\n",(++labelCount));
          fprintf(outfile, buf);
          break;
        case symGREATER:
          sprintf(buf,"\tJG\t_go%d\n",(++labelCount));
          fprintf(outfile, buf);
          break;
        case symGEQ:
          sprintf(buf,"\tJGE\t_go%d\n",(++labelCount));
          fprintf(outfile, buf);
          break;
      }
      return 0;
    }
    else
    {
      Error(16);
      skip(condition, 20);
      return 1;
    }
  }
/*+
** 語法規則#14 <Expression>
*/
  void Expression()
  {
    if (token->sym == symPLUS ||
        token->sym == symMINUS)
    {
      int operator = token->sym;
      token = nextToken();
      Term();
      if (operator == symMINUS)
      {
        fprintf(outfile,"\tPOP\tAX\n"
                        "\tNEG\tAX\n"
                        "\tPUSH\tAX\n");
      }
    }
    else
      Term();
    while (token->sym == symPLUS ||
           token->sym == symMINUS)
    {
      int operator = token->sym;
      token = nextToken();
      Term();
      if (operator == symPLUS)
      {
        fprintf(outfile,"\tPOP\tBX\n"
                        "\tPOP\tAX\n"
                        "\tADD\tAX, BX\n"
                        "\tPUSH\tAX\n");
      }
      else
      {
        fprintf(outfile, "\tPOP\tBX\n"
                         "\tPOP\tAX\n"
                         "\tSUB\tAX, BX\n"
                         "\tPUSH\tAX\n");
      }
    }
  }
/*+
** 語法規則#15 <Term>
*/
  void Term()
  {
    Factor();
    while (token->sym == symMUL ||
           token->sym == symDIV)
    {
      int operator = token->sym;
      token = nextToken();
      Factor();
      if (operator == symMUL)
      {
        fprintf(outfile, "\tPOP\tBX\n"
                         "\tPOP\tAX\n"
                         "\tMUL\tBX\n"
                         "\tPUSH\tAX\n");
      }
      else
      {
        fprintf(outfile, "\tPOP\tBX\n"
                         "\tMOV\tDX, 0\n"
                         "\tPOP\tAX\n"
                         "\tDIV\tBX\n"
                         "\tPUSH\tAX\n");
      }
    }
  }
/*+
** 語法規則#16 <Factor>
*/
  void Factor()
  {
    if (token->sym == symIDENTIFIER)
    {
      sprintf(buf,"\tPUSH\tWORD [%s]\n",token->value);
      fprintf(outfile, buf);
      Identifier();
    }
    else if (token->sym == symNUMBER)
    {
      sprintf(buf, "\tPUSH\t%s\n", token->value);
      fprintf(outfile, buf);
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
/*+
** 識別字符記處理
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
/*+
** 數字符記處理
*/
  void Number()
  {
    if (token->sym == symNUMBER)
      token = nextToken();
    else
      Error(19);
  }
/*
************************** 主程式 **********************
*/
  int main(int argc, char *argv[])
  {
    FILE *f=fopen(argv[1], "r");
    scanner(f);
    followsyminit();
    token = nextToken();
    strcpy(progname, argv[1]);
    strcpy(outname, argv[1]);
    strcat(outname, ".asm");
    outfile = fopen(outname, "w");
    ++labelCount;
    sprintf(buf,
        ";************** %s ****************\n"
        ";\n"
        "\tORG\t100H\n"
        "\tJMP\t_start%d\n"
        "_intstr\tDB\t'     ','$'\n"
        "_buf\tTIMES 256 DB ' '\n"
        "\tDB 13,10,'$'\n",
        outname, labelCount);
    fprintf(outfile, buf);
    strcpy(buf, "%include\t\"dispstr.mac\"\n");
    strcat(buf, "%include\t\"itostr.mac\"\n");
    strcat(buf, "%include\t\"readstr.mac\"\n");
    strcat(buf, "%include\t\"strtoi.mac\"\n");
    strcat(buf, "%include\t\"newline.mac\"\n");
    fputs(buf, outfile);
    Program();
    fprintf(outfile, "\tMOV\tAX, 4C00H\n"
                 "\tINT\t21H\n");
    printf("\n  C compile completed. "
      "\n  Error count : %d\n", errorCount);
    if (argc==3)
    {
      printf("\n識別字結構堆疊內容如下：\n%s\n",
        idobjstackToString());
    }      
    fclose(outfile);
    fclose(f);
    if (errorCount==0)
    {
      FILE *batchfile;
      sprintf(buf, "%s.bat", progname);
      batchfile=fopen(buf,"w");
      fprintf(batchfile, "nasmw %s.asm -o %s.com\n",
              progname, progname);
      fprintf(batchfile, "%s.com\n", progname);
      fclose(batchfile);
     }
    return 0;  
  }
