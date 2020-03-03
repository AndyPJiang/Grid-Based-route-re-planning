/* Solution to comp10002 Assignment 1, 2019 semester 2.

   Authorship Declaration:

   (1) I certify that the program contained in this submission is completely
   my own individual work, except where explicitly noted by comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students,
   or by non-students as a result of request, solicitation, or payment,
   may not be submitted for assessment in this subject.  I understand that
   submitting for assessment work developed by or in collaboration with
   other students or non-students constitutes Academic Misconduct, and
   may be penalized by mark deductions, or by other penalties determined
   via the University of Melbourne Academic Honesty Policy, as described
   at https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will
   I do so until after the marks are released. I understand that providing
   my work to other students, regardless of my intention or any undertakings
   made to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring
   service, or drawing the attention of others to such services and code
   that may have been made available via such a service, may be regarded
   as Student General Misconduct (interfering with the teaching activities
   of the University and/or inciting others to commit Academic Misconduct).
   I understand that an allegation of Student General Misconduct may arise
   regardless of whether or not I personally make use of such solutions
   or sought benefit from such actions.

   Signed by: Andy 1083536
   Dated:    20.10.2019

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

/* all formatting line breaks */
#define STAGE0 "==STAGE 0======================================="
#define STAGE1 "==STAGE 1======================================="
#define STAGE2 "==STAGE 2======================================="
#define BREAK "================================================"
#define SEPARATOR "------------------------------------------------"
#define INITIAL 2  /* size of a cell (contains x,y coordinates) */
#define TRUE 1  /* dummy variable, used in while loops*/

typedef int cell_t[INITIAL];  /* an array of 2 integers - datatype for cells */

/* taken from listops.c from lecture slides. Changed the type of data
stored in each node to type cell_t */
typedef struct node node_t;
struct node{
  cell_t cell;
  node_t *next;
  int counter;
};

typedef struct {
	node_t *head;
	node_t *foot;
} list_t;


/* Read all blocks into a 2d array */
int **read_blocks(int *num_blocks, char **grid);

list_t *read_route(char **grid);  /* Read route into linked list */

char **make_grid(int height, int width);  /* make empty grid with axes */
void grid_route(list_t *list,char**grid); /* visualise route onto grid */
void print_grid(char **grid, int height, int width);  /* print grid */
list_t *make_empty_list();   /* make an empty linked list structure */

/* insert node at tail of linked list */
list_t *insert_at_foot(list_t *list, cell_t value, int counter);
/* insert node at head of linked list */
list_t *insert_at_head(list_t *list, cell_t value, int counter);

void print_status(int *status); /* print the status of the route */

/* check if route starts from inital cell and ends at goal cell */
void check_ig(list_t *list,int fin_row,int fin_col,int *status,
  int init_row,int init_col);

/* check if any illegal moves are made in the route */
void legal_move(list_t *list,int *status,int width,int height,
  int *num_blocks,int **all_blocks);
  /* check if route passes through a block */
int check_block(int cur_x,int cur_y,int **all_blocks,int *num_blocks);

/* repair 1 segment of the route */
list_t *repair_route(int x,int y,int *block_cell,list_t *list,
  int **all_blocks, int *num_blocks, int width, int height,int *repble);

int check_cell(list_t *list, int ori_x, int ori_y, int x, int y,
  int *block_cell);    /* check if segment of route is repaired */

/* check if a cell is in the linked list */
int check_list(list_t *list_r, int x, int y);

/* add valid cells to list */
int add_cell(list_t *list_temp,int **all_blocks,cell_t cell,
  int width,int height,int *num_blocks, int counter,int **count_grid);

void free_list(list_t *list);     /* deallocate previously allocated memory */

/* splits route up into thirds to repair broken segment */
void split_route(list_t *list,list_t *list_new,list_t *list_l,
  list_t *list_r,int *block_cell);

/* update grid after broken segment is repaired */
void update_grid(list_t *list, char **grid, int width, int height);
void print_route(list_t *list);  /* print route */
void find_broken(list_t *list,int **all_blocks,int *num_blocks,
  int *broke_cell,int *block_cell); /* find first broken segment encountered */

/* make an empty grid for values of counters for repairting route */
int **mcount_grid(int height,int width);

list_t *find_adjacent(int **count_grid,int x,int y,int fin_x, int fin_y,
  int height,int width); /* find a valid path to repair route */

/* update route after repaired segment found */
list_t *update_route(list_t *list, list_t *list_new,int *block_cell);

/* clear all blocks on grid in order to add new config of blocks later */
void clear_gridblocks(char **grid, int height, int width);

/* makes a copy of a linked list */
list_t *copy_list(list_t *list);
/* free memory space for 2d array of block cells */
void free_blocks(int **all_blocks,int *num_blocks);



int main(int argc, char *argv[]){
  /* I chose to use int instead of short to generalise for possible larger
  grid sizes */
  int height, width, init_row, init_col,fin_row,fin_col;

  scanf("%dx%d", &height, &width);
  scanf(" [%d,%d]", &init_row, &init_col);  /* read grid specifications */
  scanf(" [%d,%d]", &fin_row, &fin_col);

  char **grid;    /* grid that contains the route and block cells */
  grid = make_grid(height, width);

  int **all_blocks;   /*2d array containing coordinates of all blocks */
  int zero = 0;
  int *num_blocks = &zero;  /*count of number of blocks */
  all_blocks = read_blocks(num_blocks, grid);   /* read blocks */

  char o;
  if (scanf("%c",&o)==1){}    /* to take for "$" */

  printf("%s\n",STAGE0);
  printf("andy\n");
  printf("The grid has %d rows and %d columns.\n", height, width);
  printf("The grid has %d block(s).\n", *num_blocks);
  printf("The initial cell in the grid is [%d,%d].\n", init_row, init_col);
  printf("The goal cell in the grid is [%d,%d].\n", fin_row, fin_col);
  printf("The proposed route in the grid is:\n");

  list_t *list;
  list = read_route(grid); /*read and store proposed route in a linked list */
  print_route(list);

  int *status,zero1=0,zero2=0;
  status = &zero1;

  /* check and print current status of route */
  legal_move(list, status, width, height, num_blocks, all_blocks);
  check_ig(list, fin_row, fin_col, status,init_row,init_col);
  print_status(status);

  printf("%s\n",STAGE1);
  /* add initial and goal cells to grid, and print grid*/
  grid[fin_row+1][fin_col+1] = 'G';
  grid[init_row+1][init_col+1] = 'I';
  print_grid(grid, height, width);

  /*broke_cell contains starting cell of broken segment, block_cell contains
  the first block cell encountered on the route */
  int *broke_cell, *block_cell;
  broke_cell = (int*)malloc(sizeof(int)*2);
  broke_cell[0] = -1; broke_cell[1] = -1;
  block_cell = (int*)malloc(sizeof(int)*2);

  list_t *list2;
  list2 = make_empty_list();
  list2 = copy_list(list);    /* make a copy of the route */
  /* dummy vairable to check if route is repairable */
  int *repble; repble = &zero2;

  if (*status == 4 || *status == 0){
    find_broken(list,all_blocks,num_blocks, broke_cell,block_cell);

    /* check if route needs repairing. If so, repair the first broken segment,
    update the grid, and print new route and status */
    if (broke_cell[0]!= -1){
      free_list(list2);
      list2 = repair_route(broke_cell[0],broke_cell[1],block_cell,list,
        all_blocks,num_blocks,width,height,repble);
      printf("%s\n",SEPARATOR);
      if (*repble == 0){                /* if route is unrepairable */
        print_grid(grid, height, width);
        printf("%s\n",SEPARATOR);
        printf("The route cannot be repaired!\n");
        /* discard list2 by replacing content with list */
        free_list(list2);
        list2 = copy_list(list);
      }
      else{
        *repble = 0;
        update_grid(list2,grid,width,height);  /* update grid */
        print_grid(grid, height, width);
        printf("%s\n",SEPARATOR);
        print_route(list2);
        *status = 0;                          /* reset status */
        legal_move(list2, status, width, height, num_blocks, all_blocks);
        check_ig(list2, fin_row, fin_col, status,init_row,init_col);
        print_status(status);
        list = copy_list(list2);
      }
    }

    int first = 1,stage2=0;   /* for purposes of formatting */

    /* start reading in new configurations of blocks */
    while (scanf("%c",&o)==1){
      if (first==1){
        first = 0;
      } else{
        printf("%s\n",BREAK);
      }
      if (stage2 == 0){
        stage2 = 1;
        printf("%s\n",STAGE2);
      }

      /* clear previous blocks in grid and read in new blocks */
      free_blocks(all_blocks, num_blocks);
      *num_blocks = 0;
      clear_gridblocks(grid,height,width);
      all_blocks = read_blocks(num_blocks, grid);
      grid[fin_row+1][fin_col+1] = 'G';
      grid[init_row+1][init_col+1] = 'I';
      update_grid(list2,grid,width,height);
      print_grid(grid,height,width);

      /* find first broken segment. If none, skip */
      broke_cell[0] = -1; broke_cell[1] = -1;
      find_broken(list,all_blocks,num_blocks, broke_cell,block_cell);

      if (broke_cell[0]!=-1){
        /* iteratively repair each broken segment until route fully repaired */
        while (broke_cell[0]!=-1){
          *repble = 0;   /* reset dummy variable */
          list2 = repair_route(broke_cell[0],broke_cell[1],block_cell,list2,
            all_blocks,num_blocks,width,height,repble);
          if (*repble == 0){break;}   /* break if segment unrepairable */
          broke_cell[0] = -1; broke_cell[1] = -1;
          find_broken(list2,all_blocks,num_blocks,broke_cell,block_cell);
        }

        printf("%s\n",SEPARATOR);
        /* if route unrepairable */
        if (*repble == 0){
          print_grid(grid, height, width);
          printf("%s\n",SEPARATOR);
          printf("The route cannot be repaired!\n");
          /* discard list2 by replacing content with list */
          list2 = copy_list(list);
        }
        else{
          /* if route repairable, update route and grid and print */
          *repble = 0;
          update_grid(list2,grid,width,height);
          print_grid(grid, height, width);
          printf("%s\n",SEPARATOR);
          print_route(list2);
          *status = 0;
          printf("The route is valid!\n");
          /*discard list by replacing content with list2 */
          list = copy_list(list2);
        }
      }
    }
  }
  printf("%s\n",BREAK);
  free_list(list2);
  free_list(list);
  free(broke_cell);
  free(block_cell);
  int i;                         /* free memory */
  for (i=0;i<height+1;i++){
    free(grid[i]);
    grid[i] = NULL;
  }
  free(grid);
  free_blocks(all_blocks,num_blocks); free(all_blocks);

  return 0;
}



void free_blocks(int **all_blocks,int *num_blocks){
  int i;
  for (i=0;i< *num_blocks;i++){
    free(all_blocks[i]);   /* free every cell in */
    all_blocks[i] = NULL;
  }
}

list_t *copy_list(list_t *list) {
  list_t *list3;
  list3 = make_empty_list();
  node_t *cur;
  cell_t cell;
  cur = list->head;            /* make a copy of list and store in list3 */
  while (cur){
    cell[0] = cur->cell[0]; cell[1] = cur->cell[1];
    list3 = insert_at_foot(list3,cell,0);
    cur = cur->next;
  }
  return list3;
}

void clear_gridblocks(char **grid, int height, int width){
  /* loop through cells in grid and clear all blocks */
  int i,j;
  for (i=0;i<height+1;i++){
    for (j=0;j<width+1;j++){
      if (grid[i][j] == '#'){
        grid[i][j] = ' ';
      }
    }
  }
}

int **mcount_grid(int height,int width){
  int num,ind;
  int **count_grid;  /* initialise a count grid, set each cell to -2 */
  count_grid = (int**)malloc((height)*sizeof(*count_grid));
  for (num=0;num<height;num++){
    count_grid[num]=(int*)malloc(sizeof(int)*width);
    for (ind=0;ind<width;ind++){
      count_grid[num][ind] = -2;
    }
  }
  return count_grid;
}

void find_broken(list_t *list,int **all_blocks,int *num_blocks,
  int *broke_cell, int *block_cell){
  node_t *cur;
  int prev_x,prev_y,cur_x,cur_y;
  cur = list->head;
  prev_x = cur->cell[0]; prev_y = cur->cell[1];
  /* loop through list. Stop if a block is found. Store coordinates of
  blocked cell and the previous cell */
  while (cur){
    cur = cur->next;
    if (cur){
      cur_x = cur->cell[0]; cur_y = cur->cell[1];
      if (check_block(cur_x, cur_y, all_blocks, num_blocks)==1){
        broke_cell[0] = prev_x; broke_cell[1] = prev_y;
        block_cell[0] = cur_x; block_cell[1] = cur_y;
        break;
      }
      prev_x = cur_x; prev_y = cur_y;
    }
  }
}

void print_route(list_t *list){
  int first = 1, count = 0;
  node_t *cur;
  int cur_x, cur_y;
  cur = list->head;
  cur_x = cur->cell[0]; cur_y = cur->cell[1];
  while (cur){
    if (first){
      first = 0;   /* dont print -> at start of route */
    } else {
      printf("->");
    }
    if (count==5){    /* start new line after printing 5 cells */
      printf("\n");
      count = 0;
    }
    printf("[%d,%d]",cur_x,cur_y);
    count++;
    cur = cur->next;
    if (cur){cur_x = cur->cell[0]; cur_y = cur->cell[1];}
  }
  printf(".\n");
}

void update_grid(list_t *list, char **grid, int width, int height){
  int i,j;
  for (i=0;i<height+1;i++){
    for (j=0;j<width+1;j++){
      if (grid[i][j] == '*'){
        grid[i][j] = ' ';  /* clear previous route in grid */
      }
    }
  }
  grid_route(list,grid); /* add new route to grid */
}

void grid_route(list_t *list,char**grid){
  node_t *cur;
  int cur_x, cur_y;
  cur = list->head;
  cur_x = cur->cell[0]; cur_y = cur->cell[1];
  while (cur) {
    if (grid[cur_x+1][cur_y+1]==' '){
      grid[cur_x+1][cur_y+1] = '*';    /* iteratively add route to grid */
    }
    cur = cur->next;
    if (cur){
      cur_x = cur->cell[0]; cur_y = cur->cell[1];
    }
  }
}

list_t *update_route(list_t *list, list_t *list_new,int *block_cell){
  list_t *list_l, *list_r;
  list_l = make_empty_list();
  list_r = make_empty_list();
  split_route(list, list_new, list_l, list_r,block_cell);

  if (list_l->head == NULL && list_r->head == NULL){
    free_list(list); free_list(list_l); free_list(list_r);
    free(list);free(list_l);free(list_r);
    return list_new;
  }
  else if(list_l->head == NULL){
    list_new->foot->next = list_r->head;
    list_new->foot = list_r->foot;
    free_list(list); free_list(list_l);
    free(list); free(list_l);
    return list_new;
  }
  else{
  list_l->foot->next = list_new->head;
  list_l->foot = list_new->foot;
  list_l->foot->next = list_r->head;
  if (list_r->foot != NULL){
    list_l->foot = list_r->foot;
  }
  else{
    free_list(list_r);
  }
}
  free_list(list);free(list_new); free(list);
  return list_l;

}

void split_route(list_t *list,list_t *list_new,list_t *list_l,
  list_t *list_r,int *block_cell){
  node_t *cur,*next;
  int cur_x, cur_y, split = 0;
  cur = list->head;
  cur_x = cur->cell[0]; cur_y = cur->cell[1];

  while (cur) {
    if (cur_x==list_new->head->cell[0]&&cur_y==list_new->head->cell[1]){
      next = cur->next;
      if(next->cell[0]==block_cell[0]&&next->cell[1]==block_cell[1]){
        split = 1;
        cur = cur->next;
        if (cur){cur_x = cur->cell[0]; cur_y = cur->cell[1];}
        continue;
      }
    }
    if (cur_x==list_new->foot->cell[0]&&cur_y==list_new->foot->cell[1]){
      if (split == 2){
        list_r = insert_at_foot(list_r,cur->cell,0);
      }
      split = 2;
      cur = cur->next;
      if (cur){cur_x = cur->cell[0]; cur_y = cur->cell[1];}
      continue;
    }
    if (split == 0){
      list_l = insert_at_foot(list_l,cur->cell,0);
    }
    else if (split == 2){
      list_r = insert_at_foot(list_r,cur->cell,0);
    }
    cur = cur->next;
    if (cur){cur_x = cur->cell[0]; cur_y = cur->cell[1];}
  }
}

list_t *repair_route(int x,int y,int *block_cell,list_t *list,
  int **all_blocks, int *num_blocks,int width,int height,int *repble){
  cell_t cur_cell;
  cur_cell[0]=x; cur_cell[1]=y;

  int counter = 0, cur_x, cur_y;
  node_t *cur;
  list_t *list_temp;  /* list to store all counters of adjacent cells */
  list_temp = make_empty_list();
  list_temp = insert_at_foot(list_temp, cur_cell, counter);

  cur = list_temp->head;
  cur_x = cur->cell[0]; cur_y = cur->cell[1];

  int **count_grid;  /* grid to store counter values of each cell */
  count_grid = mcount_grid(height,width);
  count_grid[x][y] = 0;

  while(cur){
    /* check up,down,left,right of current cell, and add cell to grid and
    list_temp if valid. Stop when you reach end of list or have reached a cell
    that is in the route that follows start of broken segment */
    counter = cur->counter + 1;
    cur_cell[0] = cur_x - 1; cur_cell[1] = cur_y;   /* check cell above */
    if (add_cell(list_temp,all_blocks,cur_cell,width,height,num_blocks,
      counter,count_grid)==1){
      if (check_cell(list,x,y,cur_cell[0],cur_cell[1],block_cell) == 1){
        *repble = 1; break;
      }
    }
    cur_cell[0] = cur_x + 1; cur_cell[1] = cur_y;  /* check cell below */
    if (add_cell(list_temp,all_blocks,cur_cell,width,height,num_blocks,
      counter,count_grid)==1){
      if (check_cell(list,x,y,cur_cell[0],cur_cell[1],block_cell) == 1){
        *repble = 1; break;
      }
    }
    cur_cell[0] = cur_x; cur_cell[1] = cur_y - 1;   /* check cell left */
    if (add_cell(list_temp,all_blocks,cur_cell,width,height,num_blocks,
      counter,count_grid)==1){
      if (check_cell(list,x,y,cur_cell[0],cur_cell[1],block_cell) == 1){
        *repble = 1; break;
      }
    }
    cur_cell[0] = cur_x; cur_cell[1] = cur_y + 1;  /* check cell right */
    if (add_cell(list_temp,all_blocks,cur_cell,width,height,num_blocks,
      counter,count_grid)==1){
      if (check_cell(list,x,y,cur_cell[0],cur_cell[1],block_cell) == 1){
        *repble = 1; break;
      }
    }
    cur = cur->next;
    if (cur){
      cur_x = cur->cell[0]; cur_y = cur->cell[1];
    }
  }

  /* last element of is the goal cell of repaired sub-route */
  int fin_x=list_temp->foot->cell[0], fin_y =list_temp->foot->cell[1];
  /* construct route from grid */
  list_temp = find_adjacent(count_grid,x,y,fin_x,fin_y,height,width);
  int i;
  for (i=0;i<height;i++){
    free(count_grid[i]);
    count_grid[i] = NULL;   /* free memory */
  }
  free(count_grid);

  return update_route(list,list_temp,block_cell);
}

list_t *find_adjacent(int **count_grid,int x,int y,int fin_x, int fin_y,
  int height,int width){
  cell_t cell;
  cell[0] = fin_x; cell[1] = fin_y;
  int cur_x = fin_x, cur_y = fin_y;
  list_t *list_repr;
  list_repr = make_empty_list();
  list_repr = insert_at_head(list_repr,cell,0);

  /* traverse queue, each time checking validity of adjacent cells. If valid,
insert cell into list_repr. Stop when you reach the start of broken segment.
  An adjacent cell is valid if the counter decresases. */
  while (x!=cur_x || y!=cur_y){
    if (cur_x-1<height && cur_x-1>=0       /* check cell above */
      && count_grid[cur_x][cur_y] - count_grid[cur_x-1][cur_y] == 1 ){
      cell[0] = cur_x-1;
      list_repr = insert_at_head(list_repr,cell,0);
      cur_x = cur_x -1;
    }
    else if(cur_x+1<height                /* check cell below */
      &&count_grid[cur_x][cur_y] - count_grid[cur_x+1][cur_y] == 1 ){
      cell[0] = cur_x+1;
      list_repr = insert_at_head(list_repr,cell,0);
      cur_x = cur_x + 1;
    }
    else if(cur_y-1<width&&cur_y-1>=0      /* check cell left */
      &&count_grid[cur_x][cur_y] - count_grid[cur_x][cur_y-1] == 1 ){
      cell[1] = cur_y -1;
      list_repr = insert_at_head(list_repr,cell,0);
      cur_y = cur_y -1;
    }
    else{
      cell[1] = cur_y + 1;     /* if none are valid, must be right cell */
      list_repr = insert_at_head(list_repr,cell,0);
      cur_y = cur_y + 1;
    }
  }
  return list_repr;

}

int check_list(list_t *list_r, int x, int y){
  int cur_x, cur_y;
  node_t *cur;
  cur = list_r->head;
  cur_x = cur->cell[0]; cur_y = cur->cell[1];
  while(cur){
    if (cur_x == x && cur_y == y){ /* stop once cell matches that in thelist */
      return 1;
    }
    cur = cur->next;
    if (cur){
      cur_x = cur->cell[0]; cur_y = cur->cell[1];
    }
  }
  return 0; /* return 0 if no match is found */
}

int check_cell(list_t *list, int ori_x, int ori_y, int x, int y,int
  *block_cell){
  int cur_x, cur_y, after=0;
  node_t *cur;
  cur = list->head;
  cur_x = cur->cell[0]; cur_y = cur->cell[1];

  /* check if the cell reached repairs the route. It repairs route if the cells
  comes after the block and start of broken segment */
  while(cur){
    if (cur_x==ori_x && cur_y==ori_y && after != 2){
      after = 1;    /* signifies start of broken segment is reached */
    }
    else if (cur_x==block_cell[0]&&cur_y==block_cell[1]&&after ==1){
      after = 2;                      /* signifies block is reached */
    }
    else if (cur_x == x && cur_y == y && after == 2){
      return 1;     /* return if current cell reached repairs broken segment */
    }
    cur = cur->next;
    if (cur){
      cur_x = cur->cell[0]; cur_y = cur->cell[1];
    }
  }
  return 0;  /* return 0 if current cell does not repair broken segment */
}

int add_cell(list_t *list_temp,int **all_blocks,cell_t cell,
  int width,int height,int *num_blocks, int counter,int **count_grid){
    /* add cell to grid and list if it is valid. It is valid if it is in
    bounds, not a block, and hasn't previously been added */
    if (check_block(cell[0], cell[1], all_blocks, num_blocks) != 1
    && cell[0]<height && cell[1]<width && cell[0]>=0 && cell[1]>=0){
      if (check_list(list_temp, cell[0], cell[1])==0){

        list_temp = insert_at_foot(list_temp, cell, counter);
        count_grid[cell[0]][cell[1]] = counter;
        return 1;
      }
    }
    return 0;
}

void print_grid(char **grid, int height, int width){
  int i,j;
  for (i=0;i<height+1;i++){
    for (j=0;j<width+1;j++){
      printf("%c",grid[i][j]);  /* prints each cell in grid */
    }
    printf("\n");
  }

}

char **make_grid(int height, int width){
  int count_h = 0, i,j, count_v = 0, num,ind;
  char **grid;
  grid = (char**)malloc((height+1)*sizeof(*grid));
  for (num=0;num<height+1;num++){
    grid[num]=(char*)malloc(width+1);
    for (ind=0;ind<width+1;ind++){
      grid[num][ind] = ' ';    /* initialise all cells to blanks(ASCII 32) */
    }
  }
  for (i=0;i<height+1;i++){
    if (i==0){       /* create a horizontal axes for the grid */
      for (j=1; j<width+1;j++){
        if (count_h > 9){count_h = 0;} /* reset to 0 if length reaches 10 */
        grid[0][j] = count_h + '0';  /* convert count to character */
        count_h++;
      }
    }
    else{      /* create a vertical for the grid */
      if (count_v > 9){count_v = 0;}  /* reset to 0 if length reaches 10 */
      grid[i][0] = count_v + '0';   /* convert count to character */
      count_v++;
    }
  }
  return grid;
}

list_t *make_empty_list() {
  /* taken from listops.c from lecture */
	list_t *list;
	list = (list_t*)malloc(sizeof(*list)); /* allocate memory space */
	list->head = list->foot = NULL; /* initialise head and foot cells to NULL */
	return list;
}

list_t *insert_at_head(list_t *list, cell_t value, int counter) {
  /* taken from listops.c from lecutre, except type of
  data stored in nodes is different */
	node_t *new;
	new = (node_t*)malloc(sizeof(*new));
	new->cell[0] = value[0];            /* add data into node */
  new->cell[1] = value[1];
  new->counter = counter;
	if (list->head==NULL) {    /* if list is previously empty */
		list->head = list->foot = new; /* head and foot point to same cell */
	} else {
    new->next = list->head;
		list->head = new;   /* alter head pointer to new node */
	}
	return list;
}

list_t *insert_at_foot(list_t *list, cell_t value, int counter) {
  /* taken from listops.c from lecutre, except type of
  data stored in nodes is different */
	node_t *new;
	new = (node_t*)malloc(sizeof(*new));
	new->cell[0] = value[0];             /* add data into node */
  new->cell[1] = value[1];
  new->counter = counter;
	new->next = NULL;
	if (list->foot==NULL) {    /* if list is previously empty */
		list->head = list->foot = new;   /* head and foot point to same cell */
	} else {
		list->foot->next = new;
		list->foot = new;         /* alter foot pointer to new node */
	}
	return list;
}

int **read_blocks(int *num_blocks, char **grid){
  cell_t block;
  int **all_blocks;
  int cur_mem = INITIAL;
  all_blocks = (int**)malloc(INITIAL*sizeof(*all_blocks));/* allocate memory */
  while (scanf(" [%d,%d]", block, block+1) == 2){
    if (*num_blocks == cur_mem){  /* if memory space is full, allocate more */
      cur_mem *=2;
      all_blocks = realloc(all_blocks, cur_mem*sizeof(*all_blocks));
    }
    /* allocate space for 2 integers, and store x,y coorindates of block */
    all_blocks[*num_blocks]=(int*)malloc(sizeof(int)*2);
    all_blocks[*num_blocks][0] = block[0];
    all_blocks[*num_blocks][1] = block[1];
    grid[block[0]+1][block[1]+1] = '#';     /* add block cell to grid */

    *num_blocks = *num_blocks + 1;      /* increment number of blocks */
  }
  return all_blocks;
}

list_t *read_route(char **grid){
  cell_t cell;
  list_t *list;
  int x,y;
  list = make_empty_list();   /* initialise linked list */
  while (scanf(" [%d,%d]->",&x,&y)==2){
    cell[0] = x;
    cell[1] = y;
    /* insert cell into list to construct route */
    list = insert_at_foot(list, cell, 0);
    if (grid[x+1][y+1] != '#'){  /* update grid */
      grid[x+1][y+1] = '*';
    }
  }
  return list;
}

void check_ig(list_t *list,int fin_row,int fin_col,int *status,
  int init_row, int init_col) {
  if (list->head->cell[0] != init_row || list->head->cell[1] != init_col){
    *status = 1;     /* check if start cell is illegal */
  }
	else if (list->foot->cell[0] != fin_row || list->foot->cell[1] != fin_col){
    *status = 2;     /* check if goal cell is illegal */
  }
}

void legal_move(list_t *list, int *status, int width, int height,
  int *num_blocks, int **all_blocks) {
  int cur_x, cur_y, prev_x, prev_y;
  node_t *cur;
  cur = list->head;
  cur_x = cur->cell[0]; cur_y = cur->cell[1];
  while(cur){
    prev_x = cur_x;
    prev_y = cur_y;
    cur = cur->next;
    if (cur!=NULL){
      cur_x = cur->cell[0];
      cur_y = cur->cell[1];
      /* check if move if legal. It is illegal if it does not move to the cell
      above, below, left, or right, or if it goes out of bounds */
      if (abs(cur_x-prev_x)+abs(cur_y-prev_y) != 1 || cur_x>height-1
      || cur_y>width-1){
        *status = 3;
      }
      /* check if cell goes through a block cell */
      if (check_block(cur_x, cur_y, all_blocks, num_blocks) == 1){
        if (*status!=3){*status = 4;}
      }
    }
  }
}

void print_status(int *status){
  /* print message according to status */
  if (*status == 0){
    printf("The route is valid!\n");
  } else if(*status == 1){
    printf("Initial cell in the route is wrong!\n");
  } else if (*status == 2){
    printf("Goal cell in the route is wrong!\n");
  } else if (*status == 3){
    printf("There is an illegal move in this route!\n");
  } else{
    printf("There is a block on this route!\n");
  }
}

int check_block(int cur_x,int cur_y,int **all_blocks,int *num_blocks){
  int i;
  for(i=0;i< *num_blocks;i++){               /* loop through all blocks */
      if (all_blocks[i][0]==cur_x && all_blocks[i][1]==cur_y){
        return 1;                /* return 1 if cell hits a block cell */
      }
  }
  return 0;  /* return 0 if cell does not hit a block cell */
}

void free_list(list_t *list){
  /* taken from listops.c from lecture. Free memory space of linked list */
	node_t *curr, *prev;
  if (list!=NULL){
  	curr = list->head;
  	while (curr) {
  		prev = curr;
  		curr = curr->next;     /* free each node */
  		free(prev);
  	}
    list->head = list->foot = NULL;
  }
}

/* algorithms are fun */
