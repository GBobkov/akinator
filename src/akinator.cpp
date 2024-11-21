#include "akinator.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

#define BUFSIZE 64


// Убирает \n в конце строки
static void remove_newline(char *str) {
    char *pos;
    // Находим символ новой строки в строке
    if ((pos = strchr(str, '\n')) != NULL) {
        *pos = '\0'; // Заменяем символ новой строки на окончание строки
    }
}


// Считать новую строку с консоли
static void Get_New_Line(char *answer)
{
    fgets(answer, BUFSIZE, stdin);
    remove_newline(answer);

}


// Функция ведёт взаимодействие с пользователем, до того момента пока не дойдёт до конца дерева.
static NODE* Search_Answer(NODE* head)
{
    assert(head != NULL);

    NODE* cur_node = head;
    while (cur_node->right && cur_node->left)
    {
        printf("%s?\n(Y/N):", cur_node->data);
        char answer[BUFSIZE] = {};
        Get_New_Line(answer);
        
        // printf("your answer=%d=%c\n", answer, answer);
        // printf("shoud answer=%d=%c\n", 'Y', 'Y');
        if (answer[0] == 'Y' || answer[0] == 'y')
            cur_node = cur_node->right;
        else if (answer[0] == 'N' || answer[0] == 'n')
            cur_node = cur_node->left;
        else
            printf("Incorrect answer! Try again.\n");
        
    }

    return cur_node;
}


// Функция проверяет совпало ли загадaнное имя.
static bool Check_Answer(NODE* node)
{
    assert(node != NULL);
    char answer[BUFSIZE] = {};
    while (true)
    {
        printf("His name: %s?\n(Y/N):", node->data);
        
        Get_New_Line(answer);
        
        if (answer[0] == 'Y' || answer[0] == 'y')
            return true;
        else if (answer[0] == 'N' || answer[0] == 'n')
            return false; 
        else
            printf("Incorrect answer! Try again.\n");
        
    }

}



// Функция узнаёт у пользователя какой ответ был правильным.
static void Find_Corr_Ans(char* answer)
{
    assert(answer != NULL);

    printf("Fuck me.. Who knows... What is the answer?\n");
    
    Get_New_Line(answer);
    printf("It was: %s?\n(Y/N):", answer);

    char correctness[BUFSIZE] = {};
    Get_New_Line(correctness);
    
    while (correctness[0] != 'Y')
    {
        printf("What's the answer?\n");
        Get_New_Line(answer);
        printf("It was: %s?\n(Y/N):", answer);
        Get_New_Line(correctness);
    }     
}


// Функция узнаёт от пользователя признак, различающий правильный ответ от предложенного.
static void Find_New_Difference(char *difference)
{
    assert(difference != NULL);

    printf("What's the difference?\n");
    Get_New_Line(difference);

    printf("You guess that the difference is %s?\n(Y/N):", difference);
    char correctness[BUFSIZE] = {};
    Get_New_Line(correctness);
    
    while (correctness[0] != 'Y' && correctness[0] != 'y')
    {
        printf("So... What's the difference?\n");
        Get_New_Line(difference);

        printf("You guess that the difference is %s?\n(Y/N):", difference);
        Get_New_Line(correctness); 
    }  
}




// Обработка результата (т.к. мы попали в конечный узел). 
static void Processing_Result(bool success, NODE* final_node)
{
    if (success)
        printf("Suck my dick, boy. It was very easy! Bye, bye...\n ");
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

    NODE* head = Create_Node("", NULL, NULL, NULL);
    Read_Data(head);
    Node_Dump("dump.dot", head);
    NODE* final_node = Search_Answer(head);

    Processing_Result(Check_Answer(final_node), final_node);
    
    Write_Data(head);

    Node_Dump("dump.dot", head);

    Destroy_Tree(head);

    return 0;
}


// TODO: Определение уже заданного
// TODO: Сравнение