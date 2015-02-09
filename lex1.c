/* lex1.c         14 Feb 01; 31 May 12       */

/* This file contains code stubs for the lexical analyzer.
   Rename this file to be lexanc.c and fill in the stubs.    */

/* Copyright (c) 2001 Gordon S. Novak Jr. and
   The University of Texas at Austin. */

/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "token.h"
#include "lexan.h"

/* This file will work as given with an input file consisting only
   of integers separated by blanks:
   make lex1
   lex1
   12345 123    345  357
   */

/* Skip blanks and whitespace.  Expand this function to skip comments too. */

void skipParenComments() 
{
  int c;
  int nextc;
  while ((c = peekchar()) != EOF
            && (nextc = peek2char()) != EOF
            && !((c == '*') && (nextc == ')')))
  {  
  
    getchar();
  }
  getchar(); //get the next character which is *
  getchar(); //get the next character which is )
}

void skipBracketComments()
{
  int c;

  while ((c = peekchar()) != EOF
             && (c != '}'))
  {
    
    getchar();
  }
  getchar(); //get the last character which is }

}
void skipblanks ()
{
      int c;
      int nextc;

      

      while ((c = peekchar()) != EOF
             && (nextc = peek2char())
             && (c == ' ' || c == '\n' || c == '\t' || c == '{' || (c=='('&& nextc=='*'))) {
          if (c=='{') 
          {
            skipBracketComments();
          }
          if(c=='(' && nextc == '*')
          {
            skipParenComments();
          }
          else 
          {
            getchar();
          }
      }
}

int isOperatorString(char* str)
{
    static char* opprnt[]  = {" ", "+", "-", "*", "/", ":=", "=", "<>", "<", "<=",
                          ">=", ">",  "^", ".", "and", "or", "not", "div",
                          "mod", "in"};
    int i = 0; 
    while(i<(sizeof(opprnt)/8))
    {
        if(strcmp(opprnt[i], str)==0)
        {
            return i;
        }
        i = i +1;
    }
    return 0;
}

/* Get identifiers and reserved words */
TOKEN identifier (TOKEN tok)
{
    char *resprnt[] = { " ", "array", "begin", "case", "const", "do",
                           "downto", "else", "end", "file", "for",
                   "function", "goto", "if", "label", "nil",
                           "of", "packed", "procedure", "program", "record",
                           "repeat", "set", "then", "to", "type",
                   "until", "var", "while", "with" };

    char str[16];
    int i = 0;
    
   
    int  c, nextc, charval;
    while ( (c = peekchar()) != EOF
        && (CHARCLASS[c]==ALPHA || CHARCLASS[c]==NUMERIC)
        && i<15)
        {  
          str[i]=c; 
          c = getchar();
          i = i + 1;
        }

    str[i]='\0';
    
    i = 0;
    
    while(i<(sizeof(resprnt)/8)) 
    {
        if (strcmp(resprnt[i],str)==0) 
        {
           break;
        }
        i = i + 1;

    }
    
    if (i==sizeof(resprnt)/8)
    {

       int val = isOperatorString(str);
       if(val)
       {
            tok->tokentype = OPERATOR;
            tok->whichval=i;
            return (tok);
       }

        tok->tokentype = IDENTIFIERTOK;
        strcpy(tok->stringval, str);
        return (tok);
    }
    else
    {
        tok->tokentype = RESERVED;
        tok->whichval = i;
        return (tok);
    }

}

TOKEN getstring (TOKEN tok)
{
    getchar(); //move past initial '
    char str[16];
    int i = 0;
    int twoQuotes = 0;
    //long num;
    int  c, nextc, charval;
    while ( (c = peekchar()) != EOF
        && (nextc = peek2char())
        && ((c != '\'') || (c=='\'' && nextc =='\'') || (twoQuotes)))
        { 

            if(i>=15)
            {
                 while ( (c = peekchar()) != EOF
                    && (nextc = peek2char())
                    && ((c != '\'') || (c=='\'' && nextc =='\'') || (twoQuotes))) 
                 {
                    getchar();
                 }
            } 
          if (twoQuotes)
          {
            twoQuotes = 0;
          }  
          if ((c=='\'' && nextc =='\'')) 
          {
            twoQuotes = 1;
          }  
          str[i]=c; 
          c = getchar();
          i = i + 1;
        }
    str[i]='\0';
    getchar(); //get the ending ' 
    tok->tokentype = STRINGTOK;
    strcpy(tok->stringval, str);
    return (tok);
}

TOKEN special (TOKEN tok)
{

    char* opprnt[]  =  {" ", "+", "-", "*", "/", ":=", "=", "<>", "<", "<=",
                          ">=", ">",  "^", "."};

    char *delprnt[] = { "  ", ",", ";", ":", "(", ")", "[", "]",
                           ".."};

    int i = 0;
    int c, nextc;
    if((c = peekchar())==EOF)
        return (tok);
    c = peekchar();
    nextc = peek2char();

    char cstr[2];
    cstr[0]=c;
    cstr[1]='\0';
    char c2str[3];
    c2str[0]=c;
    c2str[1]=nextc;
    c2str[2]='\0';


    while(i<(sizeof(delprnt)/8))
    {
        if (c==':' && nextc=='=')
        {
            tok->tokentype = OPERATOR;
            //stupid but correct
            tok->whichval = 5;
            getchar();
            getchar();
            return(tok);
        }

        if (c=='.' && nextc=='.')
        {
            tok->tokentype = DELIMITER;
            //stupid but correct
            tok->whichval = (sizeof(delprnt)/8);
            getchar();
            getchar();
            return(tok);
        }
        if(strcmp(cstr,delprnt[i])==0)
        {
            tok->tokentype = DELIMITER;
            tok->whichval = i;
            getchar();
            return(tok);
        }
        i = i + 1;
    }

    i = 0;

    while(i<(sizeof(opprnt)/8))
    {
        if(strcmp(cstr,opprnt[i])==0)
        {
            tok->tokentype = OPERATOR;
            tok->whichval = i;
            getchar();
            return(tok);
        }
        if(strcmp(c2str,opprnt[i])==0)
        {
            tok->tokentype = OPERATOR;
            tok->whichval = i;
            getchar();
            return(tok);
        }
        i = i + 1;    
    }      

}

double getDecimalPortion() 
{
    getchar(); //move past decimal
    long num;
    int  c, charval;
    num = 0;
    long divideBy = 1;
    while ( (c = peekchar()) != EOF
            && (CHARCLASS[c] == NUMERIC))
    {  
        
        c = getchar();
        charval = (c - '0');
        num = num * 10 + charval;
        divideBy = divideBy*10;
    }
    return (double)num/(double)divideBy;
}

/* Get and convert unsigned numbers of all types. */
TOKEN number (TOKEN tok)
{ 
    double realNum;
    int isReal = 0;
    long num;
    int  c, charval;
    num = 0;
    while ( (c = peekchar()) != EOF
            && ((CHARCLASS[c] == NUMERIC)||(c=='.')))
    {  
        if (c=='.') 
        {
            //TODO get precision right
            double decimalPortion = getDecimalPortion();
           
            realNum = (double)num + decimalPortion;
         
         

            isReal = 1;
            break;
        } 
        c = getchar();
        charval = (c - '0');
        num = num * 10 + charval;
    }
    if(isReal) 
    {
        tok->tokentype=NUMBERTOK;
        tok->datatype=REAL;
       
        tok->realval = realNum;
    }
    else
    {
        tok->tokentype = NUMBERTOK;
        tok->datatype = INTEGER;
        tok->intval = num;
    }
    return (tok);
}
