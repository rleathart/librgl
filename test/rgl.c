#include <stdio.h>

#include <stdbool.h>

#include <rgl/util.h>
#include <rgl/linkedlist.h>
#include <rgl/fileinfo.h>

int main(int argc, char* argv[])
{
  char input[64];
  get_user_input("Enter: ", sizeof(input), input);
  printf("You entered: %s\n", input);

  FileInfo* test = fileinfo_new("test/rgl.c");

  int line_count = 0;
  char** lines = fileinfo_read_lines(test, &line_count);

  for (int i = 0; i < line_count; ++i)
    printf("%s\n", lines[i]);

  LinkedList* list = linkedlist_new();

  char data = 'H';
  linkedlist_append(list, linkedlist_node_new(&data, sizeof(data)));
  data = 'I';
  linkedlist_append(list, linkedlist_node_new(&data, sizeof(data)));

  for (Node* Item = list->head; Item; Item = Item->next)
    printf("%c\n", *(char*)Item->data);

  return 0;
}
