#ifndef TREE_H
#define TREE_H

#include "pch.h"
#include "type.h"

enum NodeType
{
    NODE_CONST,  //常量
    NODE_VAR,    //变量
    NODE_EXPR,   //表达式
    NODE_TYPE,   //类型
    NODE_STMT,   //语句
    NODE_PROG,   
    NODE_MAIN    //main函数入口
};

enum OperatorType
{
    OP_PLUS,
    OP_MINUS,
    OP_MULTI,
    OP_DIVID,
    OP_MOD,
    OP_AND,
    OP_OR,
    OP_NOT,
    OP_SELFP,
    OP_SELFM,
    OP_EQ,
    OP_BT,
    OP_ST,
    OP_BEQ,
    OP_SEQ,
    OP_NEQ,
    OP_LG_AND,
    OP_LG_OR,
    OP_LG_NOT
};

enum StmtType {
    STMT_SKIP,  //跳过
    STMT_DECL,  //声明
    STMT_WHILE,
    STMT_FUNC_DECL,
    STMT_FOR,
    STMT_FUNC_DEF,
    STMT_FUNC_CALL,
    STMT_IF,
    STMT_SCOPE, //作用域
    STMT_ELSE,
    STMT_ASSIGN,
    STMT_PLUS_ASSIGN,
    STMT_MINUS_ASSIGN,
    STMT_MULTI_ASSIGN,
    STMT_DIVID_ASSIGN,
    STMT_MOD_ASSIGN,
    STMT_PRINTF,
    STMT_SCANF,
    STMT_RETURN
}
;

struct TreeNode {
public:
    int nodeID;  //用于作业的序号输出
    int lineno;  //记录行号
    NodeType nodeType; //节点类型

    TreeNode* child = nullptr;  //一个孩子，其他孩子表现为此孩子的兄弟
    TreeNode* sibling = nullptr;  //兄弟

    void addChild(TreeNode*);   //添加孩子
    void addSibling(TreeNode*);   //添加兄弟
    
    void printNodeInfo(TreeNode*);
    void printChildrenId();

    void printAST(); // 先输出自己 + 孩子们的id；再依次让每个孩子输出AST。
    void printSpecialInfo();

    void genNodeId();

public:

    OperatorType optype;  // 运算符操作类型
    Type* type;     //常量类型     
    StmtType stype; //表达式类型

    int int_val;  //整型值数值
    char ch_val;  //char类型数值
    bool b_val;   //bool类型数值
    double d_val; //双精度浮点类型数值
    string str_val;  //string类型数值

    string var_name;  //变量名
public:
    static string nodeType2String (NodeType type);
    static string opType2String (OperatorType type);
    static string sType2String (StmtType type);

public:
    TreeNode(int lineno, NodeType type);
};

TreeNode *expr_addChild(TreeNode*, TreeNode*, TreeNode*);  //简化表达式的插入
TreeNode *for_addChild(int, TreeNode*, TreeNode*, TreeNode*, TreeNode*);  //简化for循环的插入，加入作用域支持

#endif