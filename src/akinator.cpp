#include "akinator.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "my_stack.h"
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


// Печать панели интерфейса.
static void Print_Panel(void)
{
    printf("Welcome to Akinator! Choose the mode(write digit below).\n");
    printf("[0] Exit.\n[1] Play Akinator.\n[2] Find description.\n[3] Find difference.\n");
}


// функция узнаёт у пользователя режим.
static MODE_VARIATIONS Choice_Mode(void)
{
    char answer[BUFSIZE] = {};
    Get_New_Line(answer);
    switch (answer[0])
    {
    case '0':
        return MODE_EXIT;
        break;

    case '1':
        return MODE_GAME;
        break;
    
    case '2':
        return MODE_SEARCH;
        break;

    case '3':
        return MODE_DIFFERENCE;
        break;
    default:
        return MODE_EXIT;
        break;
    }

}


// Функция игры в акинатора.
static void Start_Game(NODE* head)
{
    NODE* final_node = Search_Answer(head);
    Processing_Result(Check_Answer(final_node), final_node);
    Write_Data(head);
    Node_Dump("dump.dot", head);

    Destroy_Tree(head);
}



static int Start_Search(NODE* head, const char* name, STACK* stk_line)
{   
    
    if (head->left || head->right)
    {
        Stack_Push(*stk_line, head->data);
        Stack_Push(*stk_line, "No");
        if (Start_Search(head->left, name, stk_line))
            return 1;
        Stack_Pop(*stk_line);
        Stack_Push(*stk_line, "Yes");
        if (Start_Search(head->right, name, stk_line))
            return 1;
        Stack_Pop(*stk_line);
        Stack_Pop(*stk_line);
    }
    else
    {
        if (strcmp(name, head->data) == 0)
            return 1;
    }
    return 0;

}



static void Searching_Person(NODE* head)
{
    char name[BUFSIZE] = {};
    printf("Whom information do you want to get?\nhero:");
    scanf("%s", name);
    STACK line_qualites = {};
    Stack_Init(line_qualites, BUFSIZE);
    Start_Search(head, name, &line_qualites);
    if (line_qualites.size == 0) 
    {
        printf("I can't find %s\n", name);
        return;
    }

    printf("Info about %s:\n", name);
    for (int i = 0; i < line_qualites.size; i++)
    {
        if (i % 2 == 0)
            printf("\t\t%s?", line_qualites.data[i]);
        else
            printf(" %s!\n", line_qualites.data[i]);
    }
    Stack_Destroy(line_qualites);
}

static void Find_Difference(NODE* head)
{
    char name1[BUFSIZE] = {};
    char name2[BUFSIZE] = {};
    printf("Which two person do you want to compare?\nhero1:");
    scanf("%s", name1);
    printf("\nhero2:");
    scanf("%s", name2);

    STACK line_qualites1 = {};
    Stack_Init(line_qualites1, BUFSIZE);
    Start_Search(head, name1, &line_qualites1);

    STACK line_qualites2 = {};
    Stack_Init(line_qualites2, BUFSIZE);
    Start_Search(head, name2, &line_qualites2);

    if (line_qualites1.size == 0) 
    {
        printf("I can't find %s\n", name1);
        return;
    }

    if (line_qualites2.size == 0) 
    {
        printf("I can't find %s\n", name2);
        return;
    }

    printf("Info about %s:\n", name1);
    for (int i = 0; i < line_qualites1.size; i++)
    {
        if (i % 2 == 0)
            printf("\t\t%s?", line_qualites1.data[i]);
        else
            printf(" %s!\n", line_qualites1.data[i]);
    }

    printf("Info about %s:\n", name2);
    for (int i = 0; i < line_qualites2.size; i++)
    {
        if (i % 2 == 0)
            printf("\t\t%s?", line_qualites2.data[i]);
        else
            printf(" %s!\n", line_qualites2.data[i]);
    }


    int i = 1;
    while (line_qualites1.data[i] == line_qualites2.data[i])
    {
        i += 2;
        if (line_qualites1.size <= i)
        {
            printf("It's the same!\n");
            return;
        }
    }
    
    printf("I found the difference.\n");
    printf("The crucial question was: %s?", line_qualites1.data[i - 1]);
    printf("answers:\n\t%s:%s\n\t%s:%s", name1, line_qualites1.data[i], name2, line_qualites2.data[i]);

    Stack_Destroy(line_qualites1);
    Stack_Destroy(line_qualites2);
}


// функиця запускающая игру.
int Play_Akinator(void)
{

    NODE* head = Create_Node("", NULL, NULL, NULL);
    Read_Data(head);
    Node_Dump("dump.dot", head);
    Print_Panel();
    MODE_VARIATIONS mode = Choice_Mode();


    switch (mode)
    {
    case MODE_GAME:
        Start_Game(head);
        break;
    case MODE_SEARCH:
        Searching_Person(head);
        break;
    case MODE_DIFFERENCE:
        Find_Difference(head);
        break;
    case MODE_EXIT:
        printf("Goodbye!\n");
        break;
    default:
        printf("Wrong mode.\n");
        break;
    }

    return 0;
}


// TODO: Определение уже заданного
// TODO: Сравнение