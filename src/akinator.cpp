#include "akinator.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "tree.h"

static const char* database_file_name = "database.txt";
static const char* output_file_name = "speaking.txt";
static FILE* output_file_ptr = NULL;



// Открыть файл для вывода текста для польззователя.
static void Open_Output_File(void)
{
    output_file_ptr = fopen(output_file_name, "w");
    assert(output_file_ptr != NULL);
}


// Закрыть файл для вывода текста для пользователя.
static void Close_Output_File(void)
{
    assert(output_file_ptr != NULL);
    fclose(output_file_ptr);
}


// Вывод в файл и консоль
static void print_consol_and_file(const char* line)
{
    fprintf(output_file_ptr, "%s", line);
    fflush(output_file_ptr);
    printf("%s", line);
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


// Функция ведёт взаимодействие с пользователем, до того момента пока не дойдёт до конца дерева.
static NODE* Search_Answer(NODE* head)
{
    assert(head != NULL);

    NODE* cur_node = head;
    while (cur_node->right && cur_node->left)
    {
        fprintf(output_file_ptr, "%s\n", cur_node->data);
        fflush(output_file_ptr);
        printf("%s?\n(Y/N):", cur_node->data);
        char answer[12] = {};
        scanf("%s", answer);
        
        // printf("your answer=%d=%c\n", answer, answer);
        // printf("shoud answer=%d=%c\n", 'Y', 'Y');
        if (answer[0] == 'Y')
            cur_node = cur_node->right;
        else if (answer[0] == 'N')
            cur_node = cur_node->left;
        else
            print_consol_and_file("Incorrect answer! Try again.\n");
        
    }

    return cur_node;
}


// Функция проверяет совпало ли загадонное имя.
static bool Check_Answer(NODE* node)
{
    assert(node != NULL);
    char answer[64] = {};
    while (true)
    {
        fprintf(output_file_ptr, "His name: %s?\n", node->data);
        fflush(output_file_ptr);
        printf("His name: %s?\n(Y/N):", node->data);
        
        scanf("%s", answer);
        
        if (answer[0] == 'Y')
            return true;
        else if (answer[0] == 'N')
            return false; 
        else
            print_consol_and_file("Incorrect answer! Try again.\n");
        
    }

}


// Функция узнаёт у пользователя какой ответ был правильным.
static void Find_Corr_Ans(char* answer)
{
    assert(answer != NULL);

    print_consol_and_file("Fuck me.. Who knows... What is the answer?\n");
    
    scanf("%s", answer);
    fprintf(output_file_ptr, "It was: %s?\n", answer);
    fflush(output_file_ptr);
    printf("It was: %s?\n(Y/N):", answer);

    char correctness[64] = {};
    scanf("%s", correctness);
    
    while (correctness[0] != 'Y')
    {
        print_consol_and_file("What's the answer?\n");
        scanf("%s", answer);
        fprintf(output_file_ptr, "It was: %s?\n", answer);
        fflush(output_file_ptr);
        printf("It was: %s?\n(Y/N):", answer);
        scanf("%s", correctness);
    }     
}


// Функция узнаёт от пользователя признак, различающий правильный ответ от предложенного.
static void Find_New_Difference(char *difference)
{
    assert(difference != NULL);

    print_consol_and_file("What's the difference?\n");
    scanf("%s", difference);

    fprintf(output_file_ptr, "You guess that the difference is %s?\n", difference);
    fflush(output_file_ptr);
    printf("You guess that the difference is %s?\n(Y/N):", difference);
    char correctness[64] = {};
    scanf("%s", correctness);
    
    while (correctness[0] != 'Y')
    {
        printf("correctness[0] != 'Y'. correctness[0]=%d=%c\n", correctness[0], correctness[0]);
        print_consol_and_file("What's the difference?\n");
        scanf("%s", difference);

        fprintf(output_file_ptr, "You guess that the difference is %s?\n", difference);
        fflush(output_file_ptr);
        printf("You guess that the difference is %s?\n(Y/N):", difference);
        scanf("%s", correctness);
        
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
        //FIXME - Вынести из функции через return сообщение об ошибке
        printf("Somthing wrong. %s:%d(%s)\n", __FILE__, __LINE__, __FUNCTION__);
        printf("Do dump in filename=\"dump.dot\"\n"); 
        const char *dump_fname = "dump.dot";
        Node_Dump(dump_fname, node);
        abort();
        return UPDATE_TREE_ERROR;
    }

    Destroy_Node(node); // старый node больше не нужен.
    
    return NO_ERROR;
}


// Обработка результата (т.к. мы попали в конечный узел). 
static void Processing_Result(bool success, NODE* final_node)
{
    if (success)
        print_consol_and_file("Suck my dick, boy. It was very easy! Bye, bye...\n ");
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
    Open_Output_File();

    NODE* head = Create_Node("", NULL, NULL, NULL);
    Read_Data(head);
    Node_Dump("dump.dot", head);
    NODE* final_node = Search_Answer(head);

    Processing_Result(Check_Answer(final_node), final_node);
    
    Write_Data(head);

    Node_Dump("dump.dot", head);

    Destroy_Tree(head);

    Close_Output_File();

    return 0;
}