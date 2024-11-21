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
    NO_ERROR_TREE=0
};

// Создаёт узел
NODE* Create_Node(const char * data, NODE* parent,  NODE* left, NODE* right);

// Освобождает память
ERROR_FLAGS Destroy_Node(NODE* node);

//рекурсивно удаляет дерево.
void Destroy_Tree(NODE* head);

// Заполнить dotfile.
void Node_Dump(const char* dump_fname, NODE* node);

// Считать данные с файла и создать дерево.
int Read_Data(NODE* node);


// Сохранить данные
int Write_Data(NODE* node);


// функция добовляет в дерево новый ответ, перестраивая его конечную часть.
ERROR_FLAGS Update_Tree(NODE* node, char* difference, char* answer);

#endif