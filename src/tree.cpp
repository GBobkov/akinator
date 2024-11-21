#include "tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define BUFSIZE 64

static const char* database_file_name = "database.txt";


// Создать узел с массивом data.
NODE* Create_Node(const char * data, NODE* parent,  NODE* left, NODE* right)
{
    char* question = (char *) calloc(BUFSIZE, sizeof(char));
    question = strcpy(question, data);

    NODE* node = (NODE *) calloc(1, sizeof(NODE));
    node->data = question;
    node->parent = parent;
    node->left = left;
    node->right = right;

    return node;
}


// удалить конкретный узел
ERROR_FLAGS Destroy_Node(NODE* node)
{
    assert(node != NULL);

    free(node->data); // предполагается, что память для data динамическая.
    free(node); 
    return NO_ERROR;
}

// рекурсивно удаляет дерево
void Destroy_Tree(NODE* head)
{
    if (head->right) Destroy_Tree(head->right);
    if (head->left) Destroy_Tree(head->left);
    Destroy_Node(head);
}


// инициализация узлов в dot-file
static void Nodes_Init_2Dump(FILE* dump_file, NODE* node)
{
    assert(dump_file != NULL);
    assert(node != NULL);

    #define TITLE_COLOR "\"lightblue\""
    if (node->left)
        fprintf (dump_file, "NODE_0x%p[label = \"%s?\", fillcolor =" TITLE_COLOR "];\n", node, node->data);
    else 
        fprintf (dump_file, "NODE_0x%p[label = \"%s\", fillcolor =" TITLE_COLOR "];\n", node, node->data);
    if (node->left) Nodes_Init_2Dump(dump_file, node->left);
    if (node->right) Nodes_Init_2Dump(dump_file, node->right);
    #undef TITLE_COLOR
}


// соеденить стрелками элементы дерева
static void Write_Connections_2Dump(FILE* dump_file, NODE* node)
{
    assert(dump_file != NULL);
    assert(node != NULL);

    if (node->left)
    {
        fprintf(dump_file, "NODE_0x%p->NODE_0x%p [label = \"Нет\", weight = 0, color = deeppink]\n", node, node->left);
        Write_Connections_2Dump(dump_file, node->left);
    }
    if (node->right)
    {
        fprintf(dump_file, "NODE_0x%p->NODE_0x%p [label = \"Да\", weight = 0, color = deeppink]\n", node, node->right);
        Write_Connections_2Dump(dump_file, node->right);
    }
}



// Сформировать dot-file и png.
void Node_Dump(const char* dump_fname, NODE* node)
{

    FILE* dump_file = fopen(dump_fname, "w");

    assert(dump_file != NULL);
    assert(node != NULL);

    #define FREE_COLOR  "\"lightgreen\""
    #define BUSY_COLOR  "\"coral\""


    fprintf (dump_file, "digraph G\n");
    fprintf (dump_file, "{\n");
    fprintf (dump_file, "splines=line;\n");
    fprintf (dump_file, "nodesep=2  ;\n"); // расстояние между ячейками
    fprintf (dump_file, "node[shape=\"oval\", style=\"rounded, filled\"];\n\n");

    Nodes_Init_2Dump(dump_file, node);
    fprintf (dump_file, "\n");

    Write_Connections_2Dump(dump_file, node);
    fprintf (dump_file, "\n");

    fprintf (dump_file, "}\n");

    fclose(dump_file);
    #undef FREE_COLOR
    #undef BUSY_COLOR

    system("dot dump.dot -Tpng -o dump.png");
}



// функция добовляет в дерево новый ответ, перестраивая его конечную часть.
ERROR_FLAGS Update_Tree(NODE* node, char* difference, char* answer)
{
    assert(node         != NULL);
    assert(difference   != NULL);
    assert(answer       != NULL);

    NODE* parent = node->parent;
    
    NODE* left_ans  = Create_Node(node->data, node, NULL, NULL);
    NODE* right_ans = Create_Node(answer, node, NULL, NULL);
    node->data = difference;
    node->left  = left_ans;
    node->right = right_ans;

    
    return NO_ERROR;
}



// Считать очередной узел из файла
static void Read_New_Node(FILE* file, NODE* node)
{
    fscanf(file, "%[^\n]", node->data);
    fgetc(file); // Достаём \n.

    char bracket = '\0';
    bracket = fgetc(file); // Достаём скобку.
    fgetc(file);    // Достаём \n.

    //printf("firstcall...  data=%s\t\tbracket=%c\n", node->data, bracket);
    if (bracket == '{')
    {
        NODE* right_son = Create_Node("", node, NULL, NULL);
        node->right = right_son;
        Read_New_Node(file, right_son);
    }

    
    //printf("after_first_if=%c\n", bracket);
    if (bracket == '{')
    {
        bracket = fgetc(file);  // Достаём скобку.
        fgetc(file); // Достаём \n.
        //printf("rebracket...  data=%s\t\tnew_bracket=%c\n", node->data, bracket);
    }

    //printf("secondcall...  data=%s\t\tbracket=%c\n", node->data, bracket);
    if (bracket == '{')
    {
        NODE* left_son = Create_Node("", node, NULL, NULL);
        node->left = left_son;
        Read_New_Node(file, left_son);
        fgetc(file);    // Достаём скобку.
        fgetc(file);    // Достаём \n.
    }
    //printf("exit_from_func=%c\n", bracket);
    
}


// Считать данные с файла и создать дерево.
int Read_Data(NODE* node)
{
    FILE* file = fopen(database_file_name, "r");

    assert(node != NULL);
    assert(file != NULL);
    
    fgetc(file);    // Убираем первую '{'
    fgetc(file);

    Read_New_Node(file, node);
    fclose(file);

    return 0;
}


// Рекурсивно записываем дерево
static void Write_New_Node(FILE* file, NODE* node)
{
    fprintf(file, "{\n");
    fprintf(file,"%s\n", node->data);
    if (node->right) Write_New_Node(file, node->right);
    if (node->left) Write_New_Node(file, node->left);
    fprintf(file, "}\n");
}   


// Сохранить данные
int Write_Data(NODE* node)
{
    FILE* file = fopen(database_file_name, "w");
    Write_New_Node(file, node);
    fclose(file);
    return 0;
}