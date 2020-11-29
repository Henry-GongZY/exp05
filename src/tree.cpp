#include "tree.h"

int nodeid;

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
    this->type = type;
    genNodeId();
}

void TreeNode::genNodeId() {
    this->nodeID = nodeid;
    nodeid++;
}

void TreeNode::printNodeInfo() {
    string TYPE = "";
    cout<<"lno@"<<this->lineno<<"   "<<"@"<<this->nodeID<<"   "<<TYPE<<
}

void TreeNode::printChildrenId() {

}

void TreeNode::printAST() {

}


// You can output more info...
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
    return "?";
}


string TreeNode::nodeType2String (NodeType type){
    return "<>";
}
