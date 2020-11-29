%{
    #include "common.h"
    #define YYSTYPE TreeNode *  
    TreeNode* root;
    extern int lineno;
    int yylex();
    int yyerror( char const * );
%}
%token T_CHAR T_INT T_STRING T_BOOL T_DOUBLE 

%token LOP_ASSIGN PLUS_ASSIGN MINUS_ASSIGN MULTI_ASSIGN DIVID_ASSIGN MOD_ASSIGN

%token SEMICOLON LBRACE RBRACE LPAREN RPAREN

%token IDENTIFIER INTEGER CHAR BOOL STRING DOUBLE

%token PLUS MINUS MULTI DIVIDE MOD SELFP SELFM AND OR NOT EQ

%token BT ST BEQ SEQ NEQ LG_AND LG_OR LG_NOT UMINUS

%token FOR MAIN IF ELSE WHILE RETURN

%token PRINTF SCANF

%token EOL

%left LOP_EQ
%left LG_OR
%left LG_AND
%left OR
%left XOR
%left AND
%left EQ NEQ
%left BT ST BEQ SEQ
%left SHIFT_LEFT SHIFT_RIGHT
%left PLUS MINUS
%left MULTI DIVIDE MOD
%left NOT LG_NOT
%left UMINUS
%left SELFP SELFM

%%

program
: statements {root = new TreeNode(0, NODE_PROG); root->addChild($1);};

statements
:  statement {$$=$1;}
|  statements statement {$$=$1; $$->addSibling($2);}
|  LBRACE statement RBRACE { $$ = $2; }
;

statement
: T MAIN LPAREN RPAREN statements {
    TreeNode* node = new TreeNode($1->lineno,NODE_STMT);
    node->stype = 
}
| SEMICOLON  {$$ = new TreeNode(lineno, NODE_STMT); $$->stype = STMT_SKIP;}
| declaration SEMICOLON {$$ = $1;}
;

declaration
: T IDENTIFIER LOP_ASSIGN expr{  //声明并初始化
    TreeNode* node = new TreeNode($1->lineno, NODE_STMT);
    node->stype = STMT_DECL;
    node->addChild($1);
    node->addChild($2);
    node->addChild($4);
    $$ = node;   
} 
| T IDENTIFIER {
    TreeNode* node = new TreeNode($1->lineno, NODE_STMT);
    node->stype = STMT_DECL;
    node->addChild($1);
    node->addChild($2);
    $$ = node;   
}
;

expr
: IDENTIFIER {
    $$ = $1;
}
| INTEGER {
    $$ = $1;
}
| CHAR {
    $$ = $1;
}
| STRING {
    $$ = $1;
}
;

T: T_INT {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_INT;} 
| T_CHAR {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_CHAR;}
| T_BOOL {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_BOOL;}
;

%%

int yyerror(char const* message)
{
  cout << message << " at line " << lineno << endl;
  return -1;
}