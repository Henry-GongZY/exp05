#ifndef TREE_CPP
#define TREE_CPP
#include "tree.h"
int nodeid = 0;

void TreeNode::addChild(TreeNode* child) {
    if (this->child == nullptr)   //没有孩子添加孩子
        this->child = child;
    else{
        this->child->addSibling(child);   //有孩子添加为孩子的兄弟
    }
}

void TreeNode::addSibling(TreeNode* sibling){
    if (this->sibling == nullptr)  //没有兄弟的直接添加为兄弟
        this->sibling = sibling;
    else {                         //有兄弟则添加为最后一个兄弟的兄弟
        TreeNode *curr = this->sibling;   
        while (curr->sibling != nullptr)
            curr = curr->sibling;
        curr->sibling = sibling;
    }
}

TreeNode::TreeNode(int lineno, NodeType type) {
    this->lineno = lineno;
    this->nodeType = type;
    genNodeId();
}

void TreeNode::genNodeId() {
    this->nodeID = nodeid;
    nodeid++;
}

void TreeNode::printNodeInfo(TreeNode* t) {
    //具体逻辑是根据不同的输入选择不同的函数进行字符串化输出
    string type = "";
    string const_type;
    switch (t->nodeType){
        case NODE_STMT:
            type = sType2String(t->stype);
            break;
        case NODE_EXPR:
            type = "OP: " + opType2String(t->optype);
            break;
        case NODE_TYPE:
            type = t->type->getTypeInfo();
            break;
        case NODE_VAR:
            type = "var name: " + t->var_name;
            break;
        case NODE_CONST:
            const_type = t->type->getTypeInfo();
            if(const_type == "int"){
                type = to_string(t->int_val);
            }
            else if(const_type == "string"){
                type = t->str_val;
            }
            else if(const_type == "bool"){
                type = to_string(t->b_val);
            }
            else if(const_type == "double"){
                type = to_string(t->d_val);
            }
            else if(const_type == "char"){
                type = to_string(t->ch_val);
            }
            type = const_type+":"+type;
            break;
        default:
            break;
    }
    cout << "lno@" << t->lineno << "  "<< "@" << t->nodeID << "  " << nodeType2String(t->nodeType) << "  " << type << "  children:[";
    t->printChildrenId();
    cout << "]" << endl;
}

void TreeNode::printChildrenId() {
    if(this->child!=nullptr){
        TreeNode* child = this->child;
        while(child!=nullptr){
            cout<<"@"<<child->nodeID<<" ";
            child = child->sibling;
        }
    }
}

void TreeNode::printAST() {
    printNodeInfo(this);
    for (TreeNode *t2 = this->child; t2; t2 = t2->sibling)
        t2->printAST();
}

void TreeNode::printData(){
    this->printAST();
    this->tableInsert();
    this->tablePrint();
}

void TreeNode::tableInsert(){
    TreeNode *curr = this;
    if (curr->nodeType == NODE_STMT && curr->stype == STMT_DECL)
    {
        if (curr->child != nullptr)
        {
            if (curr->child->nodeType == NODE_VAR)
                this->SymbolTable[curr->child->var_name] = curr->child;
            TreeNode *tmp = curr->child->sibling;
            while (tmp != nullptr)
            {
                if (tmp->nodeType == NODE_VAR)
                    this->SymbolTable[tmp->var_name] = tmp;
                tmp = tmp->sibling;
            }
        }
    }
    for (TreeNode *t = this->child; t; t = t->sibling)
        t->tableInsert();
}

void TreeNode::genSymbolTable(){
    //本节点为作用域
    if(this->stype==STMT_SCOPE){
        this->tableInsert();
    }
    //本节点不是作用域
    else{
        if(this->sibling!=nullptr){
            this->sibling->genSymbolTable();
        } 
        if(this->child!=nullptr){
            this->child->genSymbolTable();
        }
        return;
    }
}

void TreeNode::tablePrint(){
    std::map<string, TreeNode *>::iterator iter;
    for (iter = this->SymbolTable.begin(); iter != this->SymbolTable.end(); iter++)
    {
        cout<< iter->first<< "  "<<iter->second->nodeID<<",";
    }
}

void TreeNode::printSpecialInfo() {
    switch(this->nodeType){
        case NODE_CONST:
            break;
        case NODE_VAR:
            break;
        case NODE_EXPR:
            break;
        case NODE_STMT:
            break;
        case NODE_TYPE:
            break;
        default:
            break;
    }
}

string TreeNode::sType2String(StmtType type) {
    switch(type){
        case STMT_ASSIGN:
            return "STMT_ASSIGN";
            break;
        case STMT_DECL:
            return "STMT_DECL";
            break;
        case STMT_DIVID_ASSIGN:
            return "/=";
            break;
        case STMT_ELSE:
            return "STMT_ELSE";
            break;
        case STMT_FUNC_CALL:
            return "STMT_FUNC_CALL";
            break;
        case STMT_FUNC_DECL:
            return "STMT_FUNC_DECL";
            break;
        case STMT_FUNC_DEF:
            return "STMT_FUNC_DEF";
            break;
        case STMT_IF:
            return "STMT_IF";
            break;
        case STMT_MINUS_ASSIGN:
            return "-=";
            break;
        case STMT_MOD_ASSIGN:
            return "%=";
            break;
        case STMT_MULTI_ASSIGN:
            return "*=";
            break;
        case STMT_PLUS_ASSIGN:
            return "+=";
            break;
        case STMT_PRINTF:
            return "STMT_PRINTF";
            break;
        case STMT_RETURN:
            return "STMT_RETURN";
            break;
        case STMT_SCANF:
            return "STMT_SCANF";
            break;
        case STMT_SCOPE:
            return "STMT_SCOPE";
            break;
        case STMT_SKIP:
            return "STMT_SKIP";
            break;
        case STMT_WHILE:
            return "STMT_WHILE";
            break;
        case STMT_FOR:
            return "STMT_FOR";
            break;
        default:
            return "?";
            break;
    }
}

string TreeNode::nodeType2String (NodeType type){
    switch(type){
        case NODE_CONST:
            return "NODE_CONST";
        case NODE_EXPR:
            return "NODE_EXPR";
        case NODE_MAIN:
            return "NODE_MAIN";
        case NODE_PROG:
            return "NODE_PROG";
        case NODE_STMT:
            return "NODE_STMT";
        case NODE_TYPE:
            return "NODE_TYPE";
        case NODE_VAR:
            return "NODE_VAR";
        default:
            return "<>";
            break;
    }
}

string TreeNode::opType2String (OperatorType type) {
    switch(type){
        case OP_AND:
            return "&";
        case OP_BEQ:
            return ">=";
        case OP_BT:
            return ">";
        case OP_DIVID:
            return "/";
        case OP_EQ:
            return "==";
        case OP_LG_AND:
            return "&&";
        case OP_LG_NOT:
            return "!";
        case OP_LG_OR:
            return "||";
        case OP_MINUS:
            return "-";
        case OP_MULTI:
            return "*";
        case OP_NEQ:
            return "!=";
        case OP_MOD:
            return "%";
        case OP_ST:
            return "<";
        case OP_OR:
            return "|";
        case OP_NOT:
            return "~";
        case OP_PLUS:
            return "+";
        case OP_SEQ:
            return "<=";
        case OP_SELFM:
            return "--";
        case OP_SELFP:
            return "++";
        default:
            return "?";
            break;
    }
}

TreeNode* expr_addChild(TreeNode* node1, TreeNode* node2, TreeNode* node3){
    TreeNode* curr = node2;
    curr->addChild(node1);
    if(node3 != nullptr){
        curr->addChild(node3);
    }
    return curr;
}

TreeNode *for_addChild(int lineno,TreeNode *node1, TreeNode *node2, TreeNode *node3, TreeNode *node4){
    //for根节点
    TreeNode *node = new TreeNode(lineno, NODE_STMT);
    node->stype = STMT_FOR;
    //添加作用域
    TreeNode* node_scope = new TreeNode(lineno, NODE_STMT);
    node_scope->stype = STMT_SCOPE;
    node_scope->addChild(node1);
    node_scope->addChild(node2);
    node_scope->addChild(node3);
    node_scope->addChild(node4);
    node->addChild(node_scope);
    return node;
}
#endif