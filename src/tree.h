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
    OP_EQ,  // ==
};

enum StmtType {
    STMT_SKIP,
    STMT_DECL,
}
;

struct TreeNode {
public:
    int nodeID;  //用于作业的序号输出
    int lineno;  //记录行号
    NodeType nodeType; //类型

    TreeNode* child = nullptr;  //一个孩子，其他孩子表现为此孩子的兄弟
    TreeNode* sibling = nullptr;  //兄弟

    void addChild(TreeNode*);   //添加孩子
    void addSibling(TreeNode*);   //添加兄弟
    
    void printNodeInfo();
    void printChildrenId();

    void printAST(); // 先输出自己 + 孩子们的id；再依次让每个孩子输出AST。
    void printSpecialInfo();

    void genNodeId();

public:
    OperatorType optype;  // 运算符操作类型
    Type* type;
    StmtType stype; //表达式类型
    int int_val;  //整型值数值
    char ch_val;  //char类型数值
    bool b_val;   //bool类型数值
    string str_val;  //string类型数值
    string var_name;  //变量名
public:
    static string nodeType2String (NodeType type);
    static string opType2String (OperatorType type);
    static string sType2String (StmtType type);

public:
    TreeNode(int lineno, NodeType type);
};

#endif