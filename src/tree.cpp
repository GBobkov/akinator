#include "tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

NODE* Create_Node(char * data, NODE* parent,  NODE* left, NODE* right)
{

    NODE* node = (NODE *) calloc(1, sizeof(NODE));
    node->data = data;
    node->parent = parent;
    node->left = left;
    node->right = right;

    return node;
}

ERROR_FLAGS Destroy_Node(NODE* node)
{
    assert(node != NULL);

    free(node);
    return NO_ERROR;
}



static void Nodes_Init_2Dump(FILE* dump_file, NODE* node)
{
    assert(dump_file != NULL);
    assert(node != NULL);

    #define TITLE_COLOR "\"lightblue\""
    fprintf (dump_file, "NODE_0x%p[label = \"{ <name>name = N_%p|<d>data = %s|<p>parent = %p|<l>left = %p|<r>right = %p }\", fillcolor =" TITLE_COLOR "];\n", node, node, node->data, node->parent, node->left, node->right);
    if (node->left) Nodes_Init_2Dump(dump_file, node->left);
    if (node->right) Nodes_Init_2Dump(dump_file, node->right);
    #undef TITLE_COLOR
}


static void Write_Connections_2Dump(FILE* dump_file, NODE* node)
{
    assert(dump_file != NULL);
    assert(node != NULL);

    if (node->left)
    {
        fprintf(dump_file, "NODE_0x%p->NODE_0x%p [weight = 0, color = deeppink]\n", node, node->left);
        Write_Connections_2Dump(dump_file, node->left);
    }
    if (node->right)
    {
        fprintf(dump_file, "NODE_0x%p->NODE_0x%p [weight = 0, color = deeppink]\n", node, node->right);
        Write_Connections_2Dump(dump_file, node->right);
    }
}


void Node_Dump(const char* dump_fname, NODE* node)
{

    FILE* dump_file = fopen(dump_fname, "w");

    assert(dump_file != NULL);
    assert(node != NULL);

    #define FREE_COLOR  "\"lightgreen\""
    #define BUSY_COLOR  "\"coral\""


    fprintf (dump_file, "digraph G\n");
    fprintf (dump_file, "{\n");
    fprintf (dump_file, "splines=curved;\n");
    fprintf (dump_file, "nodesep=0.5;\n"); // расстояние между ячейками
    fprintf (dump_file, "node[shape=\"record\", style=\"rounded, filled\"];\n\n");

    Nodes_Init_2Dump(dump_file, node);
    fprintf (dump_file, "\n");

    Write_Connections_2Dump(dump_file, node);
    fprintf (dump_file, "\n");

    fprintf (dump_file, "}\n");

    fclose(dump_file);
    #undef FREE_COLOR
    #undef BUSY_COLOR
}