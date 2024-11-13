#ifndef TREE_H
#define TREE_H



struct NODE
{
    char* data;
    NODE* parent;
    NODE* left;
    NODE* right;
};

enum ERROR_FLAGS
{
    UPDATE_TREE_ERROR=1,
    NO_ERROR=0
};

// Создаёт узел
NODE* Create_Node(char * data, NODE* parent,  NODE* left, NODE* right);

// Освобождает память
ERROR_FLAGS Destroy_Node(NODE* node);


// Заполнить dotfile.
void Node_Dump(const char* dump_fname, NODE* node);


#endif