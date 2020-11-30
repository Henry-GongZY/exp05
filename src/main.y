%{
    #include "common.h"
    #define YYSTYPE TreeNode *  
    TreeNode* root;
    extern int lineno;
    int yylex();
    int yyerror( char const * );
%}
%token T_CHAR T_INT T_STRING T_BOOL T_DOUBLE T_VOID

%token LOP_ASSIGN PLUS_ASSIGN MINUS_ASSIGN MULTI_ASSIGN DIVID_ASSIGN MOD_ASSIGN

%token SEMICOLON COMMA LBRACE RBRACE LPAREN RPAREN

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
    node->stype = STMT_SCOPE;
    node->addChild($5);
    $2->addChild($1);
    $2->addChild(node);
    $$ = $2;
}
| if_stmt {$$ = $1;} 
| if_else_stmt {$$ = $1;}
| for_stmt {$$ = $1;}
| while_stmt {$$ = $1;}
| function_declaration {$$=$1;}
| function_definition {$$=$1;}
| function_call {$$=$1;}
| function_return {$$=$1;}
| scanf_stmt {$$ = $1;}
| printf_stmt {$$ = $1;}
| assign_stmt {$$ = $1;}
| SEMICOLON  {
    $$ = new TreeNode(lineno, NODE_STMT); 
    $$->stype = STMT_SKIP;
}
| declaration SEMICOLON {$$ = $1;}
;

if_else_stmt
: if_stmt else_stmt {$1->addSibling($2); $$ = $1;}
;

if_stmt
: IF LPAREN expr RPAREN statements {
    //开辟 if 空间
    TreeNode* node = new TreeNode($1->lineno,NODE_STMT);
    node->stype = STMT_IF;
    node->addChild($3);
    //开辟作用域空间，并将之链接到if下
    Treenode* node_scope = new TreeNode($1->lineno,NODE_STMT);
    node_scope->stype = STMT_scope;
    node_scope->addchild($5);
    node->addChild(node_scope);
    $$ = node;
}

else_stmt
: ELSE statements {
    TreeNode* node = new TreeNode($1->listno,NODE_STMT);
    node->stype = STMT_ELSE;
    Treenode* node_scope = new TreeNode($1->lineno,NODE_STMT);
    node_scope->stype = STMT_scope;
    node_scope->addchild($2);
    node->addChild(node_scope);
    $$ = node;
}

declaration
: T IDENTIFIER LOP_ASSIGN expr{ 
    TreeNode* node = new TreeNode($1->lineno, NODE_STMT);
    node->stype = STMT_DECL;
    node->addChild($1);
    node->addChild($2);
    node->addChild($4);
    $$ = node;   
} 
| T IDLIST {
    TreeNode* node = new TreeNode($1->lineno, NODE_STMT);
    node->stype = STMT_DECL;
    node->addChild($1);
    node->addChild($2);
    $$ = node;   
}
;

IDLIST
: IDENTIFIER COMMA IDLIST {
    $1->addSibling($3);
    $$ = $1;
}
| IDENTIFIER { $$ = $1; }
;

assign_stmt
: IDENTIFIER LOP_ASSIGN expr SEMICOLON{
    TreeNode* node = new TreeNode($1->lineno, NODE_STMT);
    node->stype = STMT_ASSIGN;
    node->addChild($1);
    node->addChild($3);
    $$ = node;}
| IDENTIFIER PLUS_ASSIGN expr SEMICOLON{
    TreeNode* node = new TreeNode($1->lineno, NODE_STMT);
    node->stype = STMT_PLUS_ASSIGN;
    node->addChild($1);
    node->addChild($3);
    $$ = node;}
| IDENTIFIER MINUS_ASSIGN expr SEMICOLON{
    TreeNode* node = new TreeNode($1->lineno, NODE_STMT);
    node->stype = STMT_MINUS_ASSIGN;
    node->addChild($1);
    node->addChild($3);
    $$ = node;}
| IDENTIFIER MULTI_ASSIGN expr SEMICOLON{
    TreeNode* node = new TreeNode($1->lineno, NODE_STMT);
    node->stype = STMT_MULTI_ASSIGN;
    node->addChild($1);
    node->addChild($3);
    $$ = node;}
| IDENTIFIER DIVID_ASSIGN expr SEMICOLON{
    TreeNode* node = new TreeNode($1->lineno, NODE_STMT);
    node->stype = STMT_DIVID_ASSIGN;
    node->addChild($1);
    node->addChild($3);
    $$ = node;}
| IDENTIFIER MOD_ASSIGN expr SEMICOLON{
    TreeNode* node = new TreeNode($1->lineno, NODE_STMT);
    node->stype = STMT_MOD_ASSIGN;
    node->addChild($1);
    node->addChild($3);
    $$ = node;}
;

for_stmt
: FOR LPAREN expr SEMICOLON expr SEMICOLON expr RPAREN statements {
    TreeNode* node = for_addChild($1->lineno, $3, $5, $7, $9);
    $$ = node;}
| FOR LPAREN declaration SEMICOLON expr SEMICOLON expr RPAREN statements {
    TreeNode* node = for_addChild($1->lineno, $3, $5, $7, $9);
    $$ = node;}
| FOR LPAREN SEMICOLON expr SEMICOLON expr RPAREN statements {
    TreeNode* node = for_addChild($1->lineno, nullptr, $4, $6, $8);
    $$ = node;}
| FOR LPAREN expr SEMICOLON SEMICOLON expr RPAREN statements {
    TreeNode* node = for_addChild($1->lineno, $3, nullptr, $6, $8);
    $$ = node;}
| FOR LPAREN expr SEMICOLON expr SEMICOLON RPAREN statements {
    TreeNode* node = for_addChild($1->lineno, $3, $5, nullptr, $8);
    $$ = node;}
| FOR LPAREN SEMICOLON SEMICOLON expr RPAREN statements {
    TreeNode* node = for_addChild($1->lineno, nullptr, nullptr, $5, $7);
    $$ = node;}
| FOR LPAREN SEMICOLON expr SEMICOLON RPAREN statements {
    TreeNode* node = for_addChild($1->lineno, nullptr, $4, nullptr, $7);
    $$ = node;}
| FOR LPAREN expr SEMICOLON SEMICOLON RPAREN statements {
    TreeNode* node = for_addChild($1->lineno, $3, nullptr, nullptr, $7);
    $$ = node;}
| FOR LPAREN SEMICOLON SEMICOLON RPAREN statements {
    TreeNode* node = for_addChild($1->lineno, nullptr, nullptr, nullptr, $6);
    $$ = node;}
;

expr
: LPAREN expr RPAREN  { $$ = $2; }
| expr PLUS expr	{ $$ = expr_addChild($1, $2, $3); }
| expr MINUS expr	{ $$ = expr_addChild($1, $2, $3); }
| expr MULTI expr	{ $$ = expr_addChild($1, $2, $3); }
| expr DIVID expr	{ $$ = expr_addChild($1, $2, $3); }
| expr MOD expr	    { $$ = expr_addChild($1, $2, $3); }
| expr SELFM        { $$ = expr_addChild($1, $2, NULL); }
| expr SELFP        { $$ = expr_addChild($1, $2, NULL); }
| expr AND expr     { $$ = expr_addChild($1, $2, $3); } 
| expr OR expr      { $$ = expr_addChild($1, $2, $3); }
| NOT expr          { $$ = expr_addChild($2, $1, NULL); }
| expr XOR expr     { $$ = expr_addChild($1, $2, $3); }
| expr EQ expr      { $$ = expr_addChild($1, $2, $3); }
| expr BT expr      { $$ = expr_addChild($1, $2, $3); }
| expr ST expr      { $$ = expr_addChild($1, $2, $3); }
| expr BEQ expr     { $$ = expr_addChild($1, $2, $3); }
| expr SEQ expr     { $$ = expr_addChild($1, $2, $3); }
| expr NEQ expr     { $$ = expr_addChild($1, $2, $3); }
| expr LG_AND expr  { $$ = expr_addChild($1, $2, $3); }
| expr LG_OR expr   { $$ = expr_addChild($1, $2, $3); }
| LG_NOT expr       { $$ = expNode($2, $1, NULL); }
| MINUS expr %prec UMINUS   { $$ = expNode($1, $2, NULL); }
| IDENTIFIER     { $$ = $1;}
| INTEGER        { $$ = $1;}
| DOUBLE         { $$ = $1;}
| CHAR           { $$ = $1;}
| STRING         { $$ = $1;}
;

T: T_INT {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_INT;} 
| T_CHAR {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_CHAR;}
| T_BOOL {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_BOOL;}
| T_DOUBLE {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_DOUBLE;}
| T_VOID {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_VOID;}
;

%%

int yyerror(char const* message)
{
  cout << message << " at line " << lineno << endl;
  return -1;
}