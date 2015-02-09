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

/* Get identifiers and reserved words */
TOKEN identifier (TOKEN tok)
  {
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
    }

/* Get and convert unsigned numbers of all types. */
TOKEN number (TOKEN tok)
  { long num;
    int  c, charval;
    num = 0;
    while ( (c = peekchar()) != EOF
            && CHARCLASS[c] == NUMERIC)
      {   c = getchar();
          charval = (c - '0');
          num = num * 10 + charval;
        }
    tok->tokentype = NUMBERTOK;
    tok->datatype = INTEGER;
    tok->intval = num;
    return (tok);
  }
