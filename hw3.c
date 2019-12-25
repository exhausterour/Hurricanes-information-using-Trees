/*
  Author: Anthony Garcia
  Email: anthony2018@my.fit.edu
  Course: CSE 2010
  Section: 2
  Description: A program that saves the information of hurricanes a tree format and prints the desire infromation by specific queries.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//The structure of the nodes of the tree
typedef struct hurricanes{
  char name[40];
  struct hurricanes *parent;
  struct hurricanes *child;
  struct hurricanes *siblings;
}HURRICANES;

//the structure of the nodes of the print linked list
typedef struct linked_list{
  char name[40];
  struct linked_list *next;
}LINKED_LIST;

//the structure use for saving all the names of the hurricanes with their state(first_parent) and category(second_parent)
typedef struct multiple_linked_list{
  char name[40];
  struct multiple_linked_list *next;
  HURRICANES *first_parent;
  HURRICANES *second_parent;
}MULTIPLE_LINKED_LIST;

/*Create_tree receives the address of the file and the root of the tree. The function returns no value since its null.
The function reads the complete file and while its reading it's creating the tree.*/
void create_tree(FILE *file_p, HURRICANES **head);
//the add child function receives the address of the parent node and the child node. It save the child node as a child of the parent node
void addchild(HURRICANES **parent_node, HURRICANES **child_node);
//The free tree function receives the addres of the head tree and recursively free's every node in that tree
void free_tree(HURRICANES **head);
/*The chase position function receives the address of the tree and the name of the node that is wanted. It returns the address of a node with data
type HURRICANE. What it does is looks for the address recursively by comparing the name of the node with the given name in the paremeter*/
HURRICANES* chase_position(HURRICANES **head, char name[]);
/*The read queries function receives the address of the file and the head of the tree. The read queries reads the first part of the query which
is the desired action and the sends the information to check_queries*/
void read_queries(FILE *file_p, HURRICANES **head);
/*The compare without recives two words and compares them without the last value which is the number. It returns 1 if they are the same and 0
if they are different.*/
int compare_without_number(char comaper1[], char compare2[]);
/*get name category gets the name of the hurricanes in a certain category by receiving the head of the tree, the head of the print linked list, 
and the name of the category*/
void get_names_category(HURRICANES **head, LINKED_LIST **print_head, char name[]);
/*get states receives the head of the tree, the print list, and the state wanted. The function finds the state in all the categories and calls get names states
to save the name of the hurricanes in the print list*/
void get_states(HURRICANES **head, LINKED_LIST **print_head, char name[]);
/*the add_linked_list receives the head of the print list and the word that wants to be added. The function adds the name in alphabetical order and 
doesn't let a value be repeated*/
void add_linked_list(LINKED_LIST **print_head, char name[]);
/*get names state receive the child of the state and the print linked list. It saves all the children in the print linked list if they exist*/
void get_names_state(HURRICANES **head, LINKED_LIST **print_head);
/*The free list function only receives the head of the print list and free's everything recursively*/
void free_list(LINKED_LIST **head);
/*The print list function receives the head of the print linked list and prints all the value that are in that linked list. If there's no value then it
print 'none'.*/
void print_list(LINKED_LIST **head);
/*The check query function receives the address of the file, the head of the tree, the head of the print list, and the query. It compares the queries to the 
available ones and depending the query it makes a certain action.*/
void check_query(FILE *file_p, HURRICANES **head, LINKED_LIST **print_head, MULTIPLE_LINKED_LIST **multiple_head, char query[]);
/*The funcion receives the head of the tree, the print list, the name of the hurricane, and the query. The function finds the name of the hurricane and saves 
the state or the category in the print list, the choice of saving depends on the query*/
void get_category_state(HURRICANES **head, LINKED_LIST **print_head, char name[], char query[]);
/*The save all names function receives the head of the tree twice and the head of the multiple list. The function saves all the hurricanes into the multiple head list
by sending the values to add_multiple_linked_list*/
void save_all_names(HURRICANES **always_head, HURRICANES **head, MULTIPLE_LINKED_LIST **multiple_head);
/*This function receives the multiple linked list, the state, the category and the name of the hurricane. It saves all the values in the multiple head list */
void add_multiple_linked_list(MULTIPLE_LINKED_LIST **multiple_head, HURRICANES **first_parent, HURRICANES **second_parent, char name[]);
/*The compare linked list receives the multiple list, the print list, and the query. The function compares all its values and the repeated ones in terms of the query
 will be saved into the print list*/
void compare_linked_list_value(MULTIPLE_LINKED_LIST **multiple_head, LINKED_LIST **print_head, char query[]);
/*The free_multiple_list receives the multiple list and free's the list recursively*/
void free_multiple_list(MULTIPLE_LINKED_LIST **head);

//void print_tree(HURRICANES **head);

int main(int argc, char* argv[])
{
  //the file pointer receives the address of the file
  FILE *file_pointer = NULL;
  //the head tree is the pointer that will be pointing the root of the tree
  HURRICANES *head_tree = NULL;
  file_pointer = fopen(argv[argc - 2], "r");
  //calls the function to create the tree with the information given in the file
  create_tree(file_pointer, &head_tree);
  fclose(file_pointer);
  file_pointer = fopen(argv[argc - 1], "r");
  //calls the function to do the queries given in the other file
  read_queries(file_pointer, &head_tree);
  fclose(file_pointer);
  //print_tree(&head_tree);
  //at the end it free's the tree
  free_tree(&head_tree);
  
  return 1;
}


void addchild(HURRICANES **parent_node, HURRICANES **child_node){
  //sets the corresponging values to the node
  (*child_node)->parent = *parent_node;
  (*child_node)->child = NULL;
  (*child_node)->siblings = NULL;
  //if there's no child then add the child node as the child of the parent node
  if ((*parent_node)->child == NULL)
    (*parent_node)->child = *child_node;
  //if the child of the parent is alphebitacly after than the child node than make the child node be the first child
  else if (strcmp((*parent_node)->child->name, (*child_node)->name) > 0){
    (*child_node)->siblings = (*parent_node)->child;
    (*parent_node)->child = *child_node;
  }
  //if there's no siblings than make the child node be after the first child
  else if((*parent_node)->child->siblings == NULL)
    (*parent_node)->child->siblings = *child_node;
  else{
    //create two tempnode so it can be added easier
    HURRICANES *temp_node = (*parent_node)->child->siblings;
    HURRICANES *temp_node2 = (*parent_node)->child;
    //if the child is smaller in alphabetically order and there's more siblings keep looping
    while((strcmp(temp_node->name, (*child_node)->name) < 0) && (temp_node->siblings != NULL)){
      //change the position every time
      temp_node = temp_node->siblings;
      temp_node2 = temp_node2->siblings;
    }
    //if child node goes before the temp_node than make the child node point to temp node and the temp_node 2 point to child node
    if (strcmp(temp_node->name, (*child_node)->name) > 0){
      (*child_node)->siblings = temp_node;
      temp_node2->siblings = *child_node;
    }
    //if we are at the end than make the child node be the last
    else if(temp_node->siblings == NULL)
      temp_node->siblings = *child_node;
  }
}

HURRICANES* chase_position(HURRICANES **head, char name[]){
  //if they have the same name than return the position of the node
  if (strcmp((*head)->name, name) == 0)
    return *head;
  else{
    HURRICANES *temp_node = NULL;
    //if there's more sibling than change to the sibling
    if ((*head)->siblings != NULL){
      temp_node = chase_position(&((*head)->siblings), name);
      //if the position was already found than keep returning the position
      if (temp_node != NULL)
        return temp_node;
    }
    //if there's more childs than change to the child
    if ((*head)->child != NULL){
      temp_node = chase_position(&((*head)->child), name);
      //if the position was found than keep returning the position
      if (temp_node != NULL)
        return temp_node;
    }
  }
  //if it wasn't found than return null
  return NULL;
}

void free_tree(HURRICANES **head){
  if (*head != NULL){
    //if there are children than change to the children
    if ((*head)->child != NULL)
     free_tree(&((*head)->child));
   //if there are siblings than change to the siblings
    if ((*head)->siblings != NULL)
      free_tree(&((*head)->siblings));
    //than free the node and set it to NULL
    free(*head);
    *head = NULL;
  }
}

void create_tree(FILE *file_p, HURRICANES **head){
  char word_by_character;
  char chase_word[30];
  char sub_category[30];
  int chase_pos_word = 0;
  int sub_pos_word = 0;

  HURRICANES *chase_node = NULL;
  HURRICANES *temp_child = NULL;
  //loop while the file is not at the end
  do{
    word_by_character = fgetc(file_p);
    //if the character is valid than add the character into the chase array or the sub_category array
    if (word_by_character > '0'-1){
      //if sub_pos_word than the character will be added to the chase array
      if (sub_pos_word == 0){
        chase_word[chase_pos_word] = word_by_character;
        chase_pos_word++;
      }
      //if sub_pos_word is bigger than 0 than the character will be added to the sub_category array
      else{
        sub_category[sub_pos_word-1] = word_by_character;
        sub_pos_word++;
      }
    }
    else if(word_by_character != EOF){
      //if the character gets to the new line than the next word will be the chase word
      if (word_by_character == '\n'){
        //if sub_pos word bigger than 0 finish the sub_category word and added to the tree. Than change the value to 0 because the next word is a chase word
        if (sub_pos_word > 0){
          sub_category[sub_pos_word-1] = '\0';
          sub_pos_word = 0;
          //if the length is not 0 than add it to the tree
          if (strlen(sub_category) != 0){
            //create child and then add it to the chase
            temp_child = (HURRICANES*)malloc(sizeof(HURRICANES));
            strcpy(temp_child->name,sub_category);
            addchild(&chase_node, &temp_child);
          }
        }
        //else change the sub_pos word to 0 and finish the chase word even though is going to be empty.
        else{
          chase_word[chase_pos_word] = '\0';
          chase_pos_word = 0;
          sub_pos_word = 0;
        }
      }
      //if its not a new line character than the sub_category will be the next word
      else{
        //if sub_pos_word is bigger than 0 than change the sub_pos_word to 1 so it can be added to the sub_category array and add the word to the tree
        if (sub_pos_word > 0){
          sub_category[sub_pos_word-1] = '\0';
          sub_pos_word = 1;
          if (strlen(sub_category) != 0){
            //create child and then add it to the chase
            temp_child = (HURRICANES*)malloc(sizeof(HURRICANES));
            strcpy(temp_child->name,sub_category);
            addchild(&chase_node, &temp_child);
          }
        }
        //if sub_pos_word is 0 than finish the chase word and look for the location of that word. Then change sub_pos_word to 1 becuase the next word is a sub category
        else{
          chase_word[chase_pos_word] = '\0';
          chase_pos_word = 0;
          sub_pos_word = 1;
          if (strlen(chase_word) != 0){
            //look for the name and reurn the position of the chase if the head is not empty. If the head is empty than add the chase as the head of the tree.
            if (*head == NULL){

              chase_node = (HURRICANES*)malloc(sizeof(HURRICANES));
              chase_node->parent = NULL;
              chase_node->child = NULL;
              chase_node->siblings = NULL;
              strcpy(chase_node->name, chase_word);
              *head = chase_node;
            }
            else
              chase_node = chase_position(&(*head), chase_word);
          }
        }
      }
    }
  }while(word_by_character != EOF);
}

void read_queries(FILE *file_p, HURRICANES **head){
  char word_by_character;
  char word_query[30];
  int query_pos_word = 0;

  LINKED_LIST *print_list = NULL;
  MULTIPLE_LINKED_LIST *multiple_head = NULL;

  //read the first word of each line until an "unvalid character" then send the word to check_query.
  do{
    word_by_character = fgetc(file_p);
    //keep adding the character to the word_query array until an unvalid character is entered
    if (word_by_character > '0'-1){
      word_query[query_pos_word] = word_by_character;
      query_pos_word++;
    }
    //if we are not a the end of the file then finish the word_query and send it to check_query.
    else if(word_by_character != EOF){
      word_query[query_pos_word] = '\0';
      query_pos_word = 0;
      check_query(file_p, &(*head), &print_list, &multiple_head, word_query);
    }
  }while(word_by_character != EOF);

}

void check_query(FILE *file_p, HURRICANES **head, LINKED_LIST **print_head, MULTIPLE_LINKED_LIST **multiple_head, char query[]){
  HURRICANES *temp_node = NULL;

  if (strcmp(query, "GetNamesByCategory") == 0){
    char name[30];
    //reads the next string and prints the query(action) and from where
    fscanf(file_p, "%s", name);
    printf("%s %s ", query, name);
    //loos for the position of the category
    temp_node = chase_position(&(*head), name);
    //if the category has a child then send the address the first child to the get_name_category function with the print linked list and the name of the category
    if (temp_node->child != NULL)
      get_names_category(&(temp_node->child), &(*print_head), name);
    //prints the linked list and free's the linked list
    print_list(&(*print_head));
    free_list(&(*print_head));
  }
  else if(strcmp(query,"GetNamesByState") == 0){
    char name[30];
    //reads the next string and prints the query(action) and from where
    fscanf(file_p, "%s", name);
    printf("%s %s ", query, name);
    //send the address of the tree and the print linked list with the name of the state
    get_states(&(*head), &(*print_head), name);
    //prints the linked list and free's the linked list
    print_list(&(*print_head));
    free_list(&(*print_head));
  }
  else if(strcmp(query,"GetNamesByCategoryAndState") == 0){
    char name1[30];
    char name2[30];
    //reads the next two string and prints the query(action) and from where
    fscanf(file_p, "%s %s", name1, name2);
    printf("%s %s %s ", query, name1, name2);
    //first find the category position and then uses the get_states function to find the state and save the values to the print list linked list
    temp_node = chase_position(&(*head), name1);
    get_states(&temp_node, &(*print_head), name2);
    //prints the linked list and free's the linked list
    print_list(&(*print_head));
    free_list(&(*print_head));
  }
  else if(strcmp(query,"GetNamesWithMultipleStates") == 0){
    printf("%s ", query);
    save_all_names(&(*head), &((*head)->child), &(*multiple_head));
    compare_linked_list_value(&(*multiple_head), &(*print_head), query);
    print_list(&(*print_head));
    free_list(&(*print_head));
    free_multiple_list(&(*multiple_head));
  }
  else if(strcmp(query,"GetNamesWithMultipleCategories") == 0){
    printf("%s ", query);
    save_all_names(&(*head), &((*head)->child), &(*multiple_head));
    compare_linked_list_value(&(*multiple_head), &(*print_head), query);
    print_list(&(*print_head));
    free_list(&(*print_head));
    free_multiple_list(&(*multiple_head));
  }
  else if(strcmp(query,"GetCategory") == 0 || strcmp(query,"GetState") == 0){
    char name[30];
    //reads the next string and prints the query(action) and from where
    fscanf(file_p, "%s", name);
    printf("%s %s ", query, name);
    get_category_state(&(*head), &(*print_head), name, query);
    //prints the linked list and free's the linked list
    print_list(&(*print_head));
    free_list(&(*print_head));
  }
}

void save_all_names(HURRICANES **always_head, HURRICANES **head, MULTIPLE_LINKED_LIST **multiple_head){
    //if there are children move to the children
  if ((*head)->child != NULL){
    save_all_names(&(*always_head), &((*head)->child), &(*multiple_head));
  }
  //if there are siblings move to the siblings
  if ((*head)->siblings != NULL){
    save_all_names(&(*always_head), &((*head)->siblings), &(*multiple_head));
  }
  //if the parent parent is equal to null or equal to the head of the tree then it's not a hurrican and will not be added to the list
  if ((*head)->parent->parent != NULL && (*head)->parent->parent != *always_head){
    //add to the linked list of adresses
    add_multiple_linked_list(&(*multiple_head), &((*head)->parent), &((*head)->parent->parent), (*head)->name);
  }
}

void add_multiple_linked_list(MULTIPLE_LINKED_LIST **multiple_head, HURRICANES **first_parent, HURRICANES **second_parent, char name[]){
  //create a new node with all the necessary information
  MULTIPLE_LINKED_LIST *new_node = (MULTIPLE_LINKED_LIST*)malloc(sizeof(MULTIPLE_LINKED_LIST));
  strcpy(new_node->name, name);
  new_node->next = NULL;
  new_node->first_parent = *first_parent;
  new_node->second_parent = *second_parent;
  //if multiple head is null then the new node will be the head
  if (*multiple_head == NULL){
    *multiple_head = new_node;
  }
  //otherwise add the new node at the front of the list
  else{
    new_node->next = *multiple_head;
    *multiple_head = new_node;
  }
}

void compare_linked_list_value(MULTIPLE_LINKED_LIST **multiple_head, LINKED_LIST **print_head, char query[]){
  if (*multiple_head == NULL);
  //if the next position is not null then start comparing
  else if ((*multiple_head)->next != NULL){
    //create a temp node that points the next position fo the multiple head
    MULTIPLE_LINKED_LIST *temp_node = (*multiple_head)->next;
    //while the temp_node is not null compare
    while(temp_node != NULL){
      //if the names are the same and the desired parent are different then save the name into the print linked list
      if (strcmp((*multiple_head)->name, temp_node->name) == 0){
        if (strcmp(query,"GetNamesWithMultipleStates") == 0){
         if (compare_without_number((*multiple_head)->first_parent->name, temp_node->first_parent->name) == 0){
           add_linked_list(&(*print_head), temp_node->name);
         }
        }
        else if(strcmp(query,"GetNamesWithMultipleCategories") == 0){
          if (strcmp((*multiple_head)->second_parent->name, temp_node->second_parent->name) != 0){
           add_linked_list(&(*print_head), temp_node->name);
          }
        }
      }
      temp_node = temp_node->next;
    }
    //then change to the next position for comparing
    compare_linked_list_value(&((*multiple_head)->next), &(*print_head), query);
  }
}

void get_names_category(HURRICANES **head, LINKED_LIST **print_head, char name[]){
  //if there are sinlings than change to the siblings
  if ((*head)->siblings != NULL)
    get_names_category(&((*head)->siblings), &(*print_head), name);
  //if there are children than change to the children
  if ((*head)->child != NULL)
    get_names_category(&((*head)->child), &(*print_head), name);
  //if the parent of the actual position is not the category than add the name to the linked list
  if (strcmp((*head)->parent->name, name) != 0)
    add_linked_list(&(*print_head), (*head)->name);
}

void get_category_state(HURRICANES **head, LINKED_LIST **print_head, char name[], char query[]){
  //if there are sinlings than change to the siblings
  if ((*head)->siblings != NULL)
    get_category_state(&((*head)->siblings), &(*print_head), name, query);
  //if there are children than change to the children
  if ((*head)->child != NULL)
    get_category_state(&((*head)->child), &(*print_head), name, query);
  //if we are at the desired position then
  if (strcmp((*head)->name, name) == 0){
    //if the query is get category then add the category to the print linked list
    if (strcmp(query,"GetCategory") == 0)
      add_linked_list(&(*print_head), (*head)->parent->parent->name);
    //if the query is get state then add the state to the print linked list
    else if(strcmp(query,"GetState") == 0)
      add_linked_list(&(*print_head), (*head)->parent->name);
  }
}

void get_states(HURRICANES **head, LINKED_LIST **print_head, char name[]){
  //if the head is not null then
  if (*head != NULL){
    //if there are children then change to the children
    if ((*head)->child != NULL)
     get_states(&((*head)->child), &(*print_head), name);
   //if there are siblings then change to the siblings
    if ((*head)->siblings != NULL)
      get_states(&((*head)->siblings), &(*print_head), name);
    //if the state was found and the state have children then get all the names of the childrens
    if (compare_without_number((*head)->name, name))
    {
      if ((*head)->child != NULL)
        get_names_state(&((*head)->child), &(*print_head));
    }
  }
}

void add_linked_list(LINKED_LIST **print_head, char name[]){
  LINKED_LIST *new_node = (LINKED_LIST*)malloc(sizeof(LINKED_LIST));
  strcpy(new_node->name, name);
  new_node->next = NULL;
  //if the print head is empty then the new node will become the head
  if (*print_head == NULL)
    *print_head = new_node;
  //if there's only one value check which one will go first
  else if ((*print_head)->next == NULL){
    //if new node goes first then change the positions
    if (strcmp((*print_head)->name, new_node->name) > 0)
    {
      new_node->next = *print_head;
      *print_head = new_node;
    }
    //if the head goes first then new node will be after the head
    else if(strcmp((*print_head)->name, new_node->name) < 0)
      (*print_head)->next = new_node;
    //if they are equal then dont save it and free the new node
    else
      free(new_node);
  }
  else{
    //if the first node is after the new node then cahnge the positions
    if (strcmp((*print_head)->name, new_node->name) > 0){
      new_node->next = *print_head;
      *print_head = new_node;
    }
    else{
      //create a temp node that points to the head and iterates through the whole linked list unttil the correct position or there's no more linked liist
      LINKED_LIST *temp_node = *print_head;
      while(strcmp(temp_node->next->name, new_node->name) < 0 && temp_node->next->next != NULL){
        //if they are equal break the loop
        if(strcmp(temp_node->name, new_node->name) == 0)
          break;
        temp_node = temp_node->next;
      }
      //if they are equal then dont save it and free the new node
      if(strcmp(temp_node->name, new_node->name) == 0)
        free(new_node);
      //if the loop broke becuase the temp_node goes after the new node then add the new node behind the temp_node->next
      else if (strcmp(temp_node->next->name, new_node->name) > 0){
        new_node->next = temp_node->next;
        temp_node->next = new_node;
      }
      //if the last node is equal to the new node then do not add it to the list and free the new node
      else if(strcmp(temp_node->next->name, new_node->name) == 0)
        free(new_node);
      //if the loop broke because there no more linked list set the new node be the last node
      else if(temp_node->next->next == NULL)
        temp_node->next->next = new_node;
    }
  }
}

void free_list(LINKED_LIST **head){
  //if the head next is not null then it decomposes until it gets to the base case case which is when you get to next null.
  //Then it composes be freeing everything and setting it to NULL
  if(*head != NULL){
    if ((*head)->next != NULL)
      free_list(&((*head)->next));
    free(*head);
    *head = NULL;
  }
}

void free_multiple_list(MULTIPLE_LINKED_LIST **head){
  //if the head next is not null then it decomposes until it gets to the base case case which is when you get to next null.
  //Then it composes be freeing everything and setting it to NULL
  if(*head != NULL){
    if ((*head)->next != NULL)
      free_multiple_list(&((*head)->next));
    free(*head);
    *head = NULL;
  }
}

void print_list(LINKED_LIST **head){
  //if head is null then do not print
  if (*head != NULL)
  {
    //it traverse through the whole linked list until it gets to null but it prints while it traverse
    LINKED_LIST *temp_node = *head;
    do{
      printf("%s ", temp_node->name);
      temp_node = temp_node->next;
    }while(temp_node != NULL);
    printf("\n");
  }
  //if threre's nothing in the print linked list than print "none"
  else
    printf("none\n");
}

void get_names_state(HURRICANES **head, LINKED_LIST **print_head){
  //add the name into the linked list and then change to the sibling if there is a sibling
  add_linked_list(&(*print_head), (*head)->name);
  if ((*head)->siblings != NULL)
    get_names_state(&((*head)->siblings), &(*print_head));
}


int compare_without_number(char comaper1[], char compare2[]){
  int length_word = strlen(comaper1);
  //compares all the values until before the number.
  for (int i = 0; i < length_word-1; ++i)
  {
    //if they are different at a certain point then return 0(false)
    if (comaper1[i] != compare2[i])
      return 0;
  }
  //if the loop was complete then they are equal so return 1(true)
  return 1;
}

