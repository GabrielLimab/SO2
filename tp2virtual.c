#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Page{
  char id[8];
  int lastAccess;
  int reference;
  int altered;
} Page;

int main(int argc, char* argv[]){
  // Variables
  char* algorithm = argv[1];
  char* inputFile = argv[2];
  int pageSize = atoi(argv[3]); 
  int memorySize = atoi(argv[4]);

  // Print variables
  printf("Algorithm: %s\n", algorithm);
  printf("Input file: %s\n", inputFile);
  printf("Memory size: %d\n", memorySize);
  printf("Page size: %d\n", pageSize);

  Page* memory = malloc(sizeof(Page) * memorySize/pageSize);

  int tmp = pageSize * 1024;
  int s = 0;

  while(tmp > 1){
    tmp = tmp >> 1;
    s++;
  }

  s = s/4;

  printf("S: %d\n", s);

  char addr[8];
  char rw;
  FILE* file = fopen(inputFile, "r");
  int i = 0;
  int time = 0;

  while(fscanf(file, "%s %c", addr, &rw) != EOF){
    // check if page is in memory
    int k = 0;
    int found = 0;
    char id[8];
    strncpy(id, addr, 8-s);

    while(k < memorySize/pageSize){
      if (memory[k].id[0] == '\0'){
        break;
      }
      if(strcmp(memory[k].id, id) == 0){
        found = 1;
        break;
      }
      k++;
    }

    if (found == 1){
      memory[k].lastAccess = time;
      memory[k].reference++;
      if(rw == 'W'){
        memory[k].altered++;
      }
    }
    else{
      Page* page = malloc(sizeof(Page));
      page->lastAccess = 0;
      page->reference = 1;
      if (rw == 'W'){
        page->altered = 1;
      }
      else{
        page->altered = 0;
      }

      // get page id
      strcpy(page->id, id);

      if(i < memorySize/pageSize){
        memory[i] = *page;
        i++;
      }
      else{
        printf("Memory full\n");
        break;
      }
      free(page);
    }

    time++;
  }

  //print memory
  int j = 0;

  while(j < memorySize/pageSize){
    printf("Page id: %s\n", memory[j].id);
    printf("Last access: %d\n", memory[j].lastAccess);
    printf("Reference: %d\n", memory[j].reference);
    printf("Altered: %d\n", memory[j].altered);
    printf("\n");
    j++;
  }

  free(memory);

  fclose(file);

  return 0;
}