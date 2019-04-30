#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LENGTH 4096
struct CourseEntry {
  char name[MAX_LENGTH]; // 과목의 이름
  struct CourseEntry* prerequisites; // 선수 과목들
  int n_prerequisites; // 선수 과목의 수
  float difficulty; // 과목의 난이도
};

void show_struct(struct CourseEntry *ce);
void set_ce_by_everytime(FILE** fp, struct CourseEntry* ce);
void set_ce_by_database(FILE** fp, struct CourseEntry *ce);
void set_topological_sort(struct CourseEntry *ce, struct CourseEntry *sorted_ce, int size);
void print_sorted_ce(struct CourseEntry *sce, int size);

int main(int argc, char** argv)
{
  struct CourseEntry ce[] = {{.difficulty = 5},{.difficulty = 5},{.difficulty = 5},{.difficulty = 5},{.difficulty = 5},{.difficulty = 5}};
  struct CourseEntry sorted_ce[6];

  // database.csv 가져오기
  FILE* fp = fopen("./in/database.csv", "r");
  if (fp == NULL)
  {
    fprintf(stderr, "database open error!");
    return -1;
  }
  set_ce_by_database(&fp, ce);
  fclose(fp);
  
  // everytime0 가져오기
  fp = fopen(*(argv+1), "r");
  //fp = fopen("./in/everytime0.csv", "r");
  if (fp == NULL)
  {
    fprintf(stderr, "everytime open error!");
    return -1;
  }
  set_ce_by_everytime(&fp, ce);
  fclose(fp);
  

  //show_struct(ce);
  // everytime1 가져오기
  //fp = fopen("./in/everytime1.csv", "r");
  //if (fp == NULL)
  //{
    //fprintf(stderr, "everytime1 open error!");
    //return -1;
  //}
  //set_ce_by_everytime(&fp, ce);
  //fclose(fp);


  // everytime2 가져오기
  //fp = fopen("./in/everytime2.csv", "r");
  //if (fp == NULL)
  //{
    //fprintf(stderr, "everytime2 open error!");
    //return -1;
  //}
  //set_ce_by_everytime(&fp, ce);
  //fclose(fp);
  

  set_topological_sort(ce, sorted_ce, sizeof(ce)/sizeof(ce[0]));
  
  print_sorted_ce(sorted_ce, sizeof(sorted_ce)/sizeof(sorted_ce[0]));
//  show_struct(sorted_ce);
  return 0;
}


void set_topological_sort(struct CourseEntry *ce, struct CourseEntry *sorted_ce, int size)
{
  int count = size;
  int offset = 0;
  int delete_index = -1;

  // ce의 길이가 0이 될때까지 실행!
  while(count > 0)
  {
    for(int i = 0; i < count; i++) {
      struct CourseEntry temp = {
        .difficulty = 0
      };
      for (int j = 0; j < count; j++)
      {
        if(ce[j].n_prerequisites != 0) continue;
        // 난이도가 가장 높은 ce를 뽑아냄
        if (ce[j].difficulty > temp.difficulty)
        {
          temp = ce[j];
          delete_index = j;
        } else if(ce[j].difficulty == temp.difficulty)
        {
          // 같으면 이름순서!
          if (strcmp(ce[j].name, temp.name) < 0) {
            temp = ce[j];
            delete_index = j;
          }
        }
      }

      // sorted_ce에 넣어주기
      sorted_ce[offset] = temp;
      offset++;

      // 삭제
      // pre에 있는애들 지우기
      for(int i = 0; i < count; i++)
      {
        if(ce[i].n_prerequisites == 1) {
          if(!strcmp(ce[i].prerequisites[0].name, temp.name)) ce[i].n_prerequisites = 0;
        } else if (ce[i].n_prerequisites == 2) {
          if(!strcmp(ce[i].prerequisites[1].name, temp.name)) ce[i].n_prerequisites = 1;
          else if(!strcmp(ce[i].prerequisites[0].name, temp.name)) {
            ce[i].n_prerequisites = 1;
            ce[i].prerequisites[0] = ce[i].prerequisites[1];
          }
        }
      }

      // ce에서 지우기
      for(int n = delete_index; n < count - 1; n++)
      {
        ce[n] = ce[n+1];
      }
      count--;
    }
  }
}

void set_ce_by_everytime(FILE** fp, struct CourseEntry* ce)
{
  char strBuf[MAX_LENGTH];
  char *string;
  int i = 0;
  
  while (!feof(*fp))
  {
    string = fgets(strBuf, sizeof(strBuf), *fp);
    if (string == NULL) break;
    char* str = strtok(string, ",");
    float num = atof(strtok(NULL, ","));

    for (int i = 0; i < 6; i++)
    {
        if(!strcmp(ce[i].name, str))
        {
          ce[i].difficulty = num;
        }
    }
    
    i++; 
  }
}


void set_ce_by_database(FILE** fp, struct CourseEntry *ce)
{
  char strBuf[MAX_LENGTH];
  char *string;
  int i = 0;

  while (!feof(*fp))
  {
    string = fgets(strBuf, sizeof(strBuf), *fp);
    if (string == NULL) break;
  
   // struct name
    char* str = strtok(string, ",");
    strcpy(ce[i].name, str);

    // struct count
    str = strtok(NULL, ",");
    int count = atoi(str);
    ce[i].n_prerequisites = count;

    for(int j = 0; j < count; j++){
      // 선수과목 이름의 공백, \n 제거
      str = strtok(NULL, ",");

      if(str[0] == ' ')
      {
        for(int l = 1; l < strlen(str); l++)
        {
          str[l-1] = str[l];
        }
        str[strlen(str)-1] = '\0';
      }
      if (str[strlen(str)-1] == '\n') str[strlen(str)-1] = '\0';


      // 선수과목들을 임시로 모아둠
      struct CourseEntry tce[6][2];
      for(int k = 0; k < i; k++)
      {
        if(!strcmp(ce[k].name, str)) tce[i][j] = ce[k];
      }
     
      // 선수과목에 넣어준다
      ce[i].prerequisites = tce[i];
    }

    i++;
  }
};

void print_sorted_ce(struct CourseEntry* sce, int size)
{
  for(int i = 0; i < size; i++)
  {
    printf("%s\n", sce[i].name);
  }
}


void show_struct(struct CourseEntry *ce)
{
  for(int i = 0; i < 6; i++) {
    printf("%d:\n", i);
    printf("\tname:%s\n", ce[i].name);
    printf("\tdifficulty:%lf\n", ce[i].difficulty);
    printf("\tn_prerequisites:%d\n", ce[i].n_prerequisites);
    for(int j = 0; j<ce[i].n_prerequisites; j++) {
      printf("\tprerequisites:%s\n", ce[i].prerequisites[j].name);
    }
  }
}


