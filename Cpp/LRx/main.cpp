#define $INT 1                     //int
#define $IF 2                     //if
#define $ELSE 3                 //else
#define $FLOAT 4                 //float
#define $PRINT 5                 //print
#define $ID 6                    //identify
#define $CONSTANT 7              //constant
#define $ASSIGN 8                //=
#define $PLUS 9                    //+
#define $STAR 10                   //*
#define $POWER 11                  //**
#define $SEMICOLON 12             //;
#define $LPAR 13                   //(
#define $RPAR 14                 //)
#define $LBRACE 15               //{
#define $RBRACE 16                  //}
#define $LZKH 17                 //[
#define $RZKH 18                  //]
#define $SUBSIGN 19              //-
#define $DIVISION 20             // /
#define $COMMA 21               //,
#define $AND 22                 //&&
#define $OR  23                 //||
#define $NOT 24                 //!
#define $EQUATION  25         //==
#define $ABOVE 26              //>
#define $BELOW 27              //<
#define $ABOVEE 28             //>=
#define $BELOWE 29             //<=
#define $NOTE   30             //!=
#define $TEMP 31
#define $NULL 0
#define $JMP 32


#include <cstdio>
#include <iostream>
#include "LR1Table.cpp"
#include "Parser.h"
#include "Parser.cpp"

#include <string>
#include <fstream>

#include "utilities/Node.h"
#include "utilities/Node.cpp"




#define $GOTO 33

#define $JL 34
using namespace std;

int main()
{
	//initTokenTable();
	initializeSymbolTable();
	inputFile = fopen("test-2.txt", "r");
    if (inputFile == NULL) {
        error("Cannot open input file.");
    }
   //printf("1");
    Parser p;
//printf("2");
    std::shared_ptr<Node> root = p.Parse();
//printf("3");
            //printf(">> %lf\n", root->Yield());
//    printf(">> %s\n", root->ToString());
//        }
    ofstream coutf;
coutf.open("genquad.txt");
   if(!coutf)
   { cout<<"Can not open genquad file !"<<endl;
    return 0;
   }
   coutf<<'\t'<<"Op"<<'\t'<<"arg1"<<'\t'<<"arg2"<<'\t'<<"result"<<endl;
   int i ;
   for(i=1;i<nextpos;i++)
   {  coutf<<'('<<i<<')'<<'\t';
	   for(int j=0;j<4;j++){
	   
	  switch (gen[i][j].code)
	  
	 { case $JMP:coutf<<'j'<<'\t';break;
       case $NULL:coutf<<'\t';break;
	   case $ID:coutf<<gen[i][j].value<<'\t';break;
	  // case $CONSTANT:coutf<<myconst[gen[i][j].value]<<'\t';break;
	   case $TEMP:coutf<<'t'<<gen[i][j].value<<'\t';break;
	   case $PLUS:coutf<<'+'<<'\t';break;
	   case $SUBSIGN:coutf<<"-"<<'\t';break;
	   case $ASSIGN:coutf<<'='<<'\t';break;
	   case $STAR:coutf<<'*'<<'\t';break;
	   case $EQUATION:coutf<<'j'<<'='<<'='<<'\t';break;
	   case $ABOVE:coutf<<'j'<<'>'<<'\t';break;
       case $BELOW:coutf<<'j'<<'<'<<'\t';break;
       case $ABOVEE:coutf<<'j'<<'>'<<'='<<'\t';break;
	   case $BELOWE:coutf<<'j'<<'<'<<'='<<'\t';break;
	   case $NOTE:coutf<<'j'<<'!'<<'='<<'\t';break;
	   case $GOTO:coutf<<'('<<gen[i][j].value<<')'<<'\t';break;

	 }
	 coutf<<'|'<<'\t';
     }coutf<<endl;

   }
   coutf<<'('<<i<<')'<<'\t';
   coutf.close();

    return 0;
}
