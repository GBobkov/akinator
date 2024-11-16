#include "tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


// Создать узел с массивом data.
NODE* Create_Node(const char * data, NODE* parent,  NODE* left, NODE* right)
{
    char* question = (char *) calloc(64, sizeof(char));
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
    fprintf (dump_file, "nodesep=0.5;\n"); // расстояние между ячейками
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