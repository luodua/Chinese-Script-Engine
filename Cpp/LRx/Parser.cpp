#include "Parser.h"

#include <stack>
#include <map>
#include <string>
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
#define $GOTO 33
#define $JL 34
static int lengthOfRule[] = {//记录弹出的数量 
    1,  // <program> ::= <block>
    4,  // <block> ::= "{" <decls> <stmts> "}"
    2,  // <decls> ::= <decls> <decl>
    0,  // <decls> ::= ""
    3,  // <decl> ::= <type> id ";"
    4,  // <type> ::= <type> "[" num "]"
	1,  // <type> ::= basic
    3,  // <stmts> ::= <stmts> <stmt> <M> 				7
    0,  // <stmts> ::= ""
    4,  // <stmt> ::= <loc> "=" <bool> ";"
    6,  // <stmt> ::=  "if" "(" <bool> ")" <M> <stmt>    10
    10,  // <stmt> ::= "if" "(" <bool> ")" <M> <stmt> <N> "else" <M> <stmt>   11
    7,  // <stmt> ::="while" <M> "(" <bool> ")" <M> <stmt>  12
    9,  // <stmt> ::= "do" <M> <stmt> "while" <M> "(" <bool> ")" ";"
    2,  // <stmt> ::= "break" ";"
    1,  // <stmt> ::= <block>
    4,  // <loc> ::= <loc> "[" num "]"
    1,  // <loc> ::= id
    4,  // <bool> ::= <bool> "||" <M> <join>               18
    1,  // <bool> ::= <join> 19
    4,  // <join> ::=  <join> "&&" <M> <equality>            20
    1,  // <join> ::= <equality>                       21
    3,  // <equality> ::= <equality> "==" <rel>        22
    3,  // <equality> ::= <equality> "!=" <rel>			23
    1,  // <equality> ::= <rel>                        24
    3,  // <rel> ::= <expr> "<" <expr>                   25
    3,  // <rel> ::= <expr> "<=" <expr>					26
    3,  // <rel> ::= <expr> ">=" <expr>				27
    3,  // <rel> ::= <expr> ">" <expr>			28
    1,  // <rel> ::= <expr>						29
    3,  // <expr> ::= <expr> "+" <term>			30
    3,  // <expr> ::= <expr> "-" <term>
    1,  // <expr> ::= <term>
    3,  // <term> ::= <term> "*" <unary>
    3,  // <term> ::= <term> "/" <unary>
    1,  // <term> ::= <unary>
    2,  // <unary> ::= "!" <unary>
    2,  // <unary> ::= "-" <unary>
    1,  // <unary> ::= <factor>
    3,  // <factor> ::= "(" <bool> ")"
    1,  // <factor> ::= <loc>
    1,  // <factor> ::= num
    1,  // <factor> ::= real
    1,  // <factor> ::= "true"
    1,   // <factor> ::= "false"
    0,
    0
};
struct mymap 
{  int code;
   std::string value;
};
struct list
{ int value;
  struct list *next;
};

int place=1;
int nextpos=1;
std::stack<list *> stknext;
std::stack<list *> stktrue;
std::stack<list *> stkfalse;
std::stack<int> stkpos;
std::stack<mymap> stktemp;//常量，变量，临时变量


mymap gen[100][4];//生成的四元式

void backpatch(list *list1,int t)
{ int i;
  while(list1)
  {i=list1->value;
   gen[i][3].code=$GOTO;
   gen[i][3].value=std::to_string(t);
   list1=list1->next;
  }
}
list* mymerge(list *list1,list *list2)
{ list *list3=list1;
  if(list1==NULL)
  {return list2;
  }
  while(list3->next)
   list3=list3->next;

  list3->next=list2;
  return list1;
}
void tiaojian_3()  //S->if ( B ) M S1  10
{  

list *mytrue=stktrue.top();

    stktrue.pop();
	list *myfalse=stkfalse.top();
     stkfalse.pop();

	int mypos=stkpos.top();
	stkpos.pop();

	backpatch(mytrue,mypos);
//	std::cout<<"333";
    list *mynext=stknext.top();
	stknext.pop();

	mynext=mymerge(myfalse,mynext);
	stknext.push(mynext);
}
void tiaojian_while()  //S->while
{  
list *mytrue=stktrue.top();
  stktrue.pop();
  list *myfalse=stkfalse.top();
  stkfalse.pop();
 
  int myposm2=stkpos.top();
  stkpos.pop();
   int myposm1=stkpos.top();
  stkpos.pop();


  list *mynexts1=stknext.top();
  stknext.pop();
   
  backpatch(mynexts1,myposm1);
  backpatch(mytrue,myposm2); 
  
//    list *mynext=stknext.top();

//	mynext=mymerge(myfalse,mynexts1);
stknext.push(myfalse);
	
	gen[nextpos][0].code=$JMP;
  gen[nextpos][1].code=0;
  gen[nextpos][2].code=0;
  gen[nextpos][3].value=std::to_string(myposm1);
	gen[nextpos][3].code=$GOTO;
  nextpos++;
}

void tiaojian_2()  //S->if ( B ) M S1  10  else
{  
list *mytrue=stktrue.top();
  stktrue.pop();
  list *myfalse=stkfalse.top();
  stkfalse.pop();

  int myposm2=stkpos.top();
  stkpos.pop();
   int myposm1=stkpos.top();
  stkpos.pop();

  list *mynexts2=stknext.top();
  stknext.pop(); 
  list *mynextn=stknext.top(); 
  stknext.pop();
  list *mynexts1=stknext.top();
  stknext.pop();
   
  backpatch(mytrue,myposm1);
  backpatch(myfalse,myposm2); 
  
  mynexts1=mymerge(mynexts1,mynexts2);
  mynexts1=mymerge(mynexts1,mynextn);
  stknext.push(mynexts1);
}
void tiaojian_14()   //M->0
{ 
  stkpos.push(nextpos);

}

void tiaojian_7()   //B->!B1
{ list *mytrue=stktrue.top();
  stktrue.pop();
  list *myfalse=stkfalse.top();
  stkfalse.pop();
  stktrue.push(myfalse);
  stkfalse.push(mytrue);
  

}
void tiaojian_15()   //N->0
{ list *mylist=new list;
  mylist->next=NULL;
  mylist->value=nextpos;
  stknext.push(mylist);
  gen[nextpos][0].code=$JMP;
  gen[nextpos][1].code=0;
  gen[nextpos][2].code=0;
  gen[nextpos][3].code=$GOTO;
  nextpos++;

}
void tiaojian_stmts()   // <stmts> ::= <stmts> <stmt> <M>
{
list *mynext=stknext.top();
//list *myfalse=stkfalse.top();
  if(mynext!=NULL)
  {  int mypos=stkpos.top();
        stkpos.pop();
        backpatch(mynext,mypos); 
	    //stknext.pop();
	    

  }


}

void tiaojian_5()   // <join> ::= <join> "&&" <equality>           20
{
   list *mytrue2=stktrue.top();
   stktrue.pop();
   list *mytrue1=stktrue.top();
   stktrue.pop();

   stktrue.push(mytrue2);
   int mypos=stkpos.top();
   stkpos.pop();

   backpatch(mytrue1,mypos);
   list *myfalse2=stkfalse.top();
   stkfalse.pop();
   list *myfalse1=stkfalse.top();
   stkfalse.pop();

   myfalse1=mymerge(myfalse1,myfalse2);
   stkfalse.push(myfalse1);
}
std::shared_ptr<Node> tiaojian_8(std::stack<std::shared_ptr<Node>> & nodes) // <rel> ::= <expr> "<" <expr>		25
{ list *mylist=new list;
  mylist->next=NULL;
  mylist->value=nextpos;
  stktrue.push(mylist);
  mylist=new list;
  mylist->next=NULL;
  mylist->value=nextpos+1;
  stkfalse.push(mylist);
  

std::shared_ptr<Node> right =nodes.top();
  gen[nextpos][2].value=nodes.top()->ToString();
  gen[nextpos][2].code=$ID;
  nodes.pop();
  
Token t = nodes.top()->GetToken();
    gen[nextpos][0].code=t.code;
 // gen[nextpos][0].value="j<";
  nodes.pop();
  gen[nextpos][1].value=nodes.top()->ToString();
  gen[nextpos][1].code=$ID;

  std::shared_ptr<Node> left =nodes.top();

  nodes.pop();
  gen[nextpos][3].code=$GOTO;
  nextpos++;
//	std::cout<<"四元式：("<<"j<"<<"  "<<gen[nextpos][1].value<<"  "<<"-"<<"  "<<gen[nextpos][2].value<<")\n";
   

   // return std::make_shared<Node>(t, left, left);



  gen[nextpos][0].code=$JMP;
  gen[nextpos][1].code=0;
  gen[nextpos][2].code=0;
  gen[nextpos][3].code=$GOTO;
  nextpos++;
 //       std::cout<<"四元式：("<<"j"<<"  "<<gen[nextpos][1].value<<"  "<<"-"<<"  "<<gen[nextpos][2].value<<")\n";
       return std::make_shared<Node>(t, left, right);
}



void tiaojian_6() // <bool> ::= <bool> "||" <join>               18 
{  list *myfalse2=stkfalse.top();
   stkfalse.pop();
   list *myfalse1=stkfalse.top();
   stkfalse.pop();

   stkfalse.push(myfalse2);
   int mypos=stkpos.top();
   stkpos.pop();
   backpatch(myfalse1,mypos);

   list *mytrue2=stktrue.top();
   stktrue.pop();
   list *mytrue1=stktrue.top();
   stktrue.pop();

   mytrue1=mymerge(mytrue1,mytrue2);
   stktrue.push(mytrue1);

}


std::shared_ptr<Node> MakeNode2(std::stack<std::shared_ptr<Node>> & nodes)
{

    
    std::shared_ptr<Node> right = nodes.top();
	nodes.pop();
	Token t = nodes.top()->GetToken();
    //printToken(t);
    nodes.pop();
    std::shared_ptr<Node> left = nodes.top();
    std::cout<<"四元式：("<<returntoken(t)<<"  "<<left->ToString()<<"  "<<"-"<<"  "<<right->ToString()<<")\n";
    
    nodes.pop();
    

	gen[nextpos][2].value=right->ToString();
	gen[nextpos][2].code=  $ID;
	gen[nextpos][0].value=t.type;
	gen[nextpos][0].code= t.code;
	
	gen[nextpos][1].value=left->ToString();
	gen[nextpos][1].code=  $ID;
	
	gen[nextpos][3].value=std::to_string(place) ;
	gen[nextpos][3].code=$TEMP;


t.code=$TEMP;
t.codevalue=place;

  nextpos++;
  place++;
    return std::make_shared<Node>(t, left, right);
}

std::shared_ptr<Node> Parser::Parse()
{
    Token token = NextToken();
   // std::stack<Token> op = {};
    std::stack<std::shared_ptr<Node>> nodes = {};
    std::stack<int> states = {};
    std::string signstates[50]={};
	int signtop=-1;
    states.push(0); 
    int top = states.top();
list *mylist=new list;
  mylist->next=NULL;
  //mylist->value=nextpos;
  stkfalse.push(mylist);
    while (true)
    {
        top = states.top();
        int indexOfToken = Convert(token);
        int action = actionTable[top][indexOfToken];
        printf("下一个");
        printToken(token);

        //std::cout<<indexOfToken<<"\n";
        //printf("indexoftoken = %d, action = %d, top = %d\n", indexOfToken, action, top);
        if (action > 0 && action < 500) // shift to some state
        {
            states.push(action);
            signtop++;
            //printToken(token);
            //std::cout<<(returntoken(token));
            signstates[signtop]=returntoken(token);
            
            std::cout<<signtop;
            //printf("移入：actiontable 列 = %d, action = %d, 行 = %d\n", indexOfToken, action, top);
            printf("移入，actiontable位置（列 = %d,行 = %d）,action = %d 。\n", indexOfToken,  top,action);
             printf("\n");
            if (token.type == NUM){
            	token.code=$ID;
            	//printf("integer %d   \n",token.value.integer);
                nodes.push(std::make_shared<Node>(token));
                //stktemp.push()
			}
            
            else if (token.type ==   PLUS) {
            	//op.push(token);
            	token.code=$PLUS;
            		nodes.push(std::make_shared<Node>(token));
                //printf("op %d   \n",token.value.integer);
			} else if (token.type ==   MINUS) {
            	//op.push(token);
            	token.code=$SUBSIGN;
            		nodes.push(std::make_shared<Node>(token));
                //printf("op %d   \n",token.value.integer);
			}else if (token.type ==   ID) {
				token.code=$ID;
            	nodes.push(std::make_shared<Node>(token));
                //printf("id %s   \n",token.value.string);
			}else if (token.type ==   LESS) {
            	//op.push(token);
            	token.code=$BELOW;
            		nodes.push(std::make_shared<Node>(token));
                //printf("op %d   \n",token.value.integer);
			}else if (token.type ==   GREATER) {
            	//op.push(token);
            	token.code=$ABOVE;
            		nodes.push(std::make_shared<Node>(token));
                //printf("op %d   \n",token.value.integer);
			}else if (token.type ==   LESSEQUAL) {
            	//op.push(token);
            	token.code=$BELOWE;
            		nodes.push(std::make_shared<Node>(token));
                //printf("op %d   \n",token.value.integer);
			}
			else if (token.type ==   GREATEREQUAL) {
            	//op.push(token);
            	token.code=$ABOVEE;
            		nodes.push(std::make_shared<Node>(token));
                //printf("op %d   \n",token.value.integer);
			}	else if (token.type ==   EQUAL) {
            	//op.push(token);
            	token.code=$EQUATION;
            		nodes.push(std::make_shared<Node>(token));
                //printf("op %d   \n",token.value.integer);
			}else if (token.type ==   NOTEQUAL) {
            	//op.push(token);
            	token.code=$NOTE;
            		nodes.push(std::make_shared<Node>(token));
                //printf("op %d   \n",token.value.integer);
			}
			else if (token.type ==   ASSIGN) {
            //op.push(token);
            token.code=$ASSIGN;
            		nodes.push(std::make_shared<Node>(token));
                //printf("= %s   \n",token.value.string);
			}else if (token.type ==   AND) {
            //op.push(token);
            //token.code=$ASSIGN;
            		nodes.push(std::make_shared<Node>(token));
                //printf("= %s   \n",token.value.string);
			}
			token = NextToken();
        
        }
        else if (action >= 500) // reduce a handle
        {
        	
        	
            int indexOfRule = action-500;
            //std::cout<<indexOfRule<<".";
            //int indexOfRule = action & ~(1 << 8); 这行代码将 action 的最高位清零，并将结果赋值给 indexOfRule 变量。
            int i=0;
            int j=lengthOfRule[indexOfRule];
            
            for (i =0; i < j; i++)
            {
            	//printf("pop = %d \n", i);
            	states.pop();
            	if (signtop>-1)signtop--;
			}
            
            printf("归约，对应文法序号 = %d，gototable 位置（列(未转换前) = %d, 行 = %d）。\n",indexOfRule, indexOfRule, states.top());
            printf("\n");
			//top = states.top();
			//printf("lengthOfRule[indexOfRule] = %d, action = %d, top = %d\n", lengthOfRule[indexOfRule], action, states.top());
			
switch (indexOfRule)
			{
    case 0: 
        states.push(gotoTable[states.top()][program]);
        //nodes.push(MakeNode(op, nodes));
        signtop++;

        signstates[signtop]="<program>";
        break;
    case 1:
        states.push(gotoTable[states.top()][block]);
        signtop++;

        signstates[signtop]="<block>";
        break;
    case 2:case 3: 
    //printf("top = %d \n", gotoTable[states.top()][decls]);
        states.push(gotoTable[states.top()][decls]);
        //nodes.push(MakeNode(op, nodes));
        signtop++;

        signstates[signtop]="<decls>";
        break;
    case 4:
        states.push(gotoTable[states.top()][decl]);
        signtop++;
        signstates[signtop]="<decl>";
        //nodes.push(MakeNode(op, nodes));
        break;
    case 5: case 6:
        states.push(gotoTable[states.top()][type]);
        //nodes.push(MakeNode(op, nodes));
        signtop++;

        signstates[signtop]="<type>";
        break;
        
     case 7:
	 tiaojian_stmts();
	    states.push(gotoTable[states.top()][stmts]);
        signtop++;

        signstates[signtop]="<stmts>";
        //nodes.push(MakeNode(op, nodes));
        break;
	 case 8:
        states.push(gotoTable[states.top()][stmts]);
                signtop++;

        signstates[signtop]="<stmts>";
        //nodes.push(MakeNode(op, nodes));
        break;
    case 9:// <stmt> ::= <loc> "=" <bool> ";"
    	MakeNode1(nodes);
		states.push(gotoTable[states.top()][stmt]);
		signtop++;

        signstates[signtop]="<stmt>";
		break;
	case 10:
	 stknext.push(NULL);
	    states.push(gotoTable[states.top()][stmt]);
        signtop++;

        signstates[signtop]="<stmt>";
        	tiaojian_3();
                break;
	case 11:
	        states.push(gotoTable[states.top()][stmt]);
        signtop++;

        signstates[signtop]="<stmt>";
        tiaojian_2();
        //nodes.push(MakeNode(op, nodes));
        break;
	case 12:
	
		        states.push(gotoTable[states.top()][stmt]);
        signtop++;

        signstates[signtop]="<stmt>";
        tiaojian_while();
	case 13:case 14:case 15:
        states.push(gotoTable[states.top()][stmt]);
        signtop++;

        signstates[signtop]="<stmt>";
        //nodes.push(MakeNode(op, nodes));
        break;
    case 16:case 17:
        states.push(gotoTable[states.top()][loc]);
        		signtop++;

        signstates[signtop]="<loc>";
        //nodes.push(MakeNode(op, nodes));
        break;
    case 18:
    	tiaojian_6();
	        states.push(gotoTable[states.top()][bool_term]);
        signtop++;

        signstates[signtop]="<bool_term>";
        //nodes.push(MakeNode(op, nodes));
        break;
	case 19:
        states.push(gotoTable[states.top()][bool_term]);
        signtop++;

        signstates[signtop]="<bool_term>";
        //nodes.push(MakeNode(op, nodes));
        break;
    case 20:
	tiaojian_5();
	states.push(gotoTable[states.top()][join]);
    signtop++;

    signstates[signtop]="<join>";
        //nodes.push(MakeNode(op, nodes));
        break;
	case 21:
        states.push(gotoTable[states.top()][join]);
        signtop++;

        signstates[signtop]="<join>";
        //nodes.push(MakeNode(op, nodes));
        break;
    case 22:case 23:   //eq
	    	nodes.push(tiaojian_8(nodes));
	
	    states.push(gotoTable[states.top()][rel]);
		signtop++;

        signstates[signtop]="<rel>";
        //nodes.push(MakeNode(op, nodes));
        break;
	
	case 24:
        states.push(gotoTable[states.top()][equality]);
                signtop++;

        signstates[signtop]="<equality>";
        //nodes.push(MakeNode(op, nodes));
        break;
    case 25:case 26: case 27:case 28:
    
    	nodes.push(tiaojian_8(nodes));
	
	    states.push(gotoTable[states.top()][rel]);
		signtop++;

        signstates[signtop]="<rel>";
        //nodes.push(MakeNode(op, nodes));
        break;
	 case 29:
        states.push(gotoTable[states.top()][rel]);
		signtop++;

        signstates[signtop]="<rel>";
        //nodes.push(MakeNode(op, nodes));
        break;
    case 30:           //plus
    	signtop++;

        signstates[signtop]="<expr>";
		states.push(gotoTable[states.top()][expr]);
    	nodes.push(MakeNode2(nodes));
		break;
	case 31:
    	signtop++;

        signstates[signtop]="<expr>";
		states.push(gotoTable[states.top()][expr]);
		nodes.push(MakeNode2(nodes));
		break;
	case 32:
    	signtop++;

        signstates[signtop]="<expr>";
    	states.push(gotoTable[states.top()][expr]);
        //nodes.push(MakeNode(op, nodes));
        break;
    case 33:case 34:case 35:
    	    	signtop++;

        signstates[signtop]="<term>";
        states.push(gotoTable[states.top()][term]);
        //nodes.push(MakeNode(op, nodes));
        break;
    case 36:  //非运算 
	tiaojian_7();
	signtop++;

    signstates[signtop]="<unary>";
    states.push(gotoTable[states.top()][unary]);
	case 37:case 38:
    	signtop++;

        signstates[signtop]="<unary>";
        states.push(gotoTable[states.top()][unary]);
        //nodes.push(MakeNode(op, nodes));
        break;
    case 39:case 40:case 41:case 42:case 43:case 44:
    	    	signtop++;

        signstates[signtop]="<factor>";
        states.push(gotoTable[states.top()][factor]);
        //nodes.push(MakeNode(op, nodes));
        break;
    case 45:
    	    	    	signtop++;

        signstates[signtop]="<M>";
    	tiaojian_14();
    	states.push(gotoTable[states.top()][m_zero]);
    	break;
	case 46:
		signtop++;

        signstates[signtop]="<N>";
		tiaojian_15();
        states.push(gotoTable[states.top()][n_zero]);
        //nodes.push(MakeNode(op, nodes));
        break;
    default:
    	printf("\n err %d",action);
        break;
        }
		// token = NextToken();
		}
        
		
		else if (-action){
			printf("\n 格式错误！！ %d",action);
        	 return nodes.top();
        	 
		}else{
			
			printf("\n accept！！ %d \n",action);
			
        	return nodes.top();
		}
        printf("  栈内容：");
        for(int k=0;k<=signtop;k++){
        	std::cout<<(signstates[k]);
		}
		printf("  \n");
    
	}
}
void tiaojian_4()   // S->i=E
{  gen[nextpos][1]=stktemp.top();
   stktemp.pop();
   gen[nextpos][0]=stktemp.top();
   stktemp.pop();
   gen[nextpos][3]=stktemp.top();
   stktemp.pop();
   gen[nextpos][2].code=0;
   nextpos++;
   stknext.push(NULL);
}
void Parser::SetLine(std::string& l)
{
    //lexer.SetLine(l);
}
// how do terminals map to indices: "!" -> 0; "!=" -> 1; "&&" -> 2; "(" -> 3; ")" -> 4; "*" -> 5; "+" -> 6; "-" -> 7; "/" -> 8; ";" -> 9; "<" -> 10; "<=" -> 11; "=" -> 12; "==" -> 13; ">" -> 14; ">=" -> 15; "[" -> 16; "]" -> 17; "break" -> 18; "do" -> 19; "else" -> 20; "false" -> 21; "if" -> 22; "true" -> 23; "while" -> 24; "{" -> 25; "||" -> 26; "}" -> 27; basic -> 28; eof -> 29; id -> 30; num -> 31; real -> 32; 

int Parser::Convert(const Token& t)
{
switch (t.type)
    {
        case NOT:
            return 0;
        case NOTEQUAL:
            return 1;
        case LOGICAL_AND:
            return 2;
        case LPAREN:
            return 3;
        case RPAREN:
            return 4;
        case MULTIPLY:
            return 5;
        case PLUS:
            return 6;
        case MINUS:
            return 7;
        case DIVIDE:
            return 8;
        case SEMICOLON:
            return 9;
        case LESS:
            return 10;
        case LESSEQUAL:
            return 11;
        case ASSIGN:
            return 12;
        case EQUAL:
            return 13;
        case GREATER:
            return 14;
        case GREATEREQUAL:
            return 15;
        case LBRACKET:
            return 16;
        case RBRACKET:
            return 17;
        case BREAK:
            return 18;
        case DO:
            return 19;
        case ELSE:
            return 20;
        case FALSE:
            return 21;
        case IF:
            return 22;
        case TRUE:
            return 23;
        case WHILE:
            return 24;
        case LBRACE:
            return 25;
        case OR:
            return 26;
        case RBRACE:
            return 27;
        case BASIC:
        	return 28;
        case ID:
            return 30;
        case END://important 
            return 29;
        case NUM:
            return 31;
        case REAL:
            return 32;
        default:
            return -1;
    }
}

std::shared_ptr<Node> Parser::MakeNode(std::stack<Token>& op, std::stack<std::shared_ptr<Node>>& nodes)
{
    Token t = op.top();
    op.pop();
    std::shared_ptr<Node> right = nodes.top();
    nodes.pop();
    std::shared_ptr<Node> left = nodes.top();
    nodes.pop();
    return std::make_shared<Node>(t, left, right);
}


std::shared_ptr<Node> Parser::MakeNode1(std::stack<std::shared_ptr<Node>> & nodes)
{

    
    std::shared_ptr<Node> right = nodes.top();
	nodes.pop();
	Token t = nodes.top()->GetToken();
    //printToken(t);
    nodes.pop();
    std::shared_ptr<Node> left = nodes.top();
    std::cout<<"四元式：("<<returntoken(t)<<"  "<<left->ToString()<<"  "<<"-"<<"  "<<right->ToString()<<")\n";
    
    nodes.pop();
    

	gen[nextpos][1].value=right->ToString();
	gen[nextpos][1].code= right->GetToken().code;
	
	
	gen[nextpos][0].value=t.type;
	
	gen[nextpos][3].value=left->ToString();
	
	gen[nextpos][3].code=  $ID;
	gen[nextpos][0].code=$ASSIGN;
   nextpos++;
 stknext.push(NULL);
    return std::make_shared<Node>(t, left, right);
}


