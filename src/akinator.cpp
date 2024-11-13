#include "akinator.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "tree.h"

static const char* file_name = "database.txt";

static int Read_Data(const char* file_name, NODE* node);
static int Write_Data(const char* file_name, NODE* node);


// Функция ведёт взаимодействие с пользователем, до того момента пока не дойдёт до конца дерева.
static NODE* Search_Answer(NODE* head)
{
    assert(head != NULL);

    NODE* cur_node = head;
    while (cur_node->right && cur_node->left)
    {
        printf("%s\n(Y/N):", cur_node->data);
        char answer = '\0';
        scanf("%c", &answer);
        switch (answer)
        {
        case 'Y':
            cur_node = cur_node->right;
            break;
        case 'N':
            cur_node = cur_node->left;
            break;
        default:
            printf("Incorrect answer! Try again.\n");
        }
    }

    return cur_node;
}


// Функция проверяет совпало ли загадонное имя.
static bool Check_Answer(NODE* node)
{
    assert(node != NULL);

    printf("Его имя: %s?\n(Y/N):", node->data);
    char answer = '\0';
    scanf("%c", &answer);
    switch (answer)
    {
    case 'Y':
        return true;
    case 'N':
        return false;
    default:
        printf("Incorrect answer! Try again.\n");
    }
}


// Функция узнаёт у пользователя какой ответ был правильным.
static void Find_Corr_Ans(char* answer)
{
    assert(answer != NULL);

    printf("Твою мать! Кто ж знал что ей нет восемнадцати... Ладно. Колись, какой ответ?\n");
    scanf("%s", answer);
    printf("Ты загадал такое имя: %s?\n(Y/N):", answer);
    char correctness = 'Y';
    scanf("%c", &correctness);
    while (correctness != 'Y')
    {
        printf("Какой ответ?\n");
        scanf("%s", answer);
        printf("Ты загадал такое имя: %s?\n(Y/N):", answer);
        scanf("%c", &correctness);
    }     
}


// Функция узнаёт от пользователя признак, различающий правильный ответ от предложенного.
static void Find_New_Difference(char *difference)
{
    assert(difference != NULL);

    printf("А чем мой персонаж отличается от твоего?\n");
    scanf("%s", difference);
    printf("Ты считаешь, что мой персонаж отличается от твоего тем, что %s?\n(Y/N):", difference);
    char correctness = 'Y';
    scanf("%c", &correctness);
    while (correctness != 'Y')
    {
        printf("А чем он отличается?\n");
        scanf("%s", difference);
        printf("Ты считаешь, что мой персонаж отличается от твоего тем, что %s?\n(Y/N):", difference);
        scanf("%c", &correctness);
    }  
}

// функция добовляет в дерево новый ответ, перестраивая его конечную часть.
static ERROR_FLAGS Update_Tree(NODE* node, char* difference, char* answer)
{
    assert(node         != NULL);
    assert(difference   != NULL);
    assert(answer       != NULL);

    NODE* parent = node->parent;
    char *left_data = node->data;
    NODE* new_node  = Create_Node(difference, parent, NULL, NULL); 

    NODE* left_ans  = Create_Node(left_data, new_node, NULL, NULL);
    NODE* right_ans = Create_Node(answer, new_node, NULL, NULL);
    new_node->left  = left_ans;
    new_node->right = right_ans;

    if (parent->left == node)
        parent->left = new_node;
    else if (parent->right == node)
        parent->right = new_node;
    else
    {
        //FIXME - Вынести из функции через return
        printf("Somthing wrong. %s:%d(%s)\n", __FILE__, __LINE__, __FUNCTION__);
        printf("Do dump in filename=\"errdump.dot\"\n"); 
        const char *dump_fname = "errdump.dot";
        Node_Dump(dump_fname, node);
        return UPDATE_TREE_ERROR;
    }

    Destroy_Node(node); // старый node больше не нужен.
    
    return NO_ERROR;
}

// Обработка результата (т.к. мы попали в конечный узел). 
static void Processing_Result(bool success, NODE* final_node)
{
    if (success)
        printf("Легчайшая! Слит, что тут скажешь... Бай бай.\n ");
    else 
    {
        char *answer = (char*) calloc(64, sizeof(char));
        Find_Corr_Ans(answer);
        char *difference = (char*) calloc(64, sizeof(char));
        Find_New_Difference(difference);

        Update_Tree(final_node, difference, answer);
        free(answer);
        free(difference);
    }
}


// функиця запускающая игру.
int Play_Akinator(void)
{
    NODE head = {};
    Read_Data(file_name, &head);

    NODE* final_node = Search_Answer(&head);

    Processing_Result(Check_Answer(final_node), final_node);
    
    Write_Data(file_name, &head);
    return 0;
}