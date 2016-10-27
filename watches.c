#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#ifdef _TEST_MAIN
#include<math.h>
#endif


#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))


#include "watches.h"

#define MAX_WATCHES 32
#define MAX_NAME_LEN 32
#define MAX_LEN ((size_t)(MAX_NAME_LEN-1))



#define IDX(i) MAX(0, MIN(MAX_WATCHES-1, i))
#define WTC_FMT " %16.2lf "

static clock_t clk_id_tmp;
static int curr=0;

static double walltime[MAX_WATCHES];
static double cputime[MAX_WATCHES];
static char names[MAX_WATCHES][MAX_NAME_LEN];

double wtc_ctime()
{
  const double factor = 1.0/((double)CLOCKS_PER_SEC);
  return factor*clock();
}

/*
double wtc_ctime()
{
  struct timespec tp;

  clock_gettime(clk_id_tmp, &tp);

  return tp.tv_sec+tp.tv_nsec*1.0e-9;
}
*/



double wtc_wtime()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);

  return tv.tv_sec+tv.tv_usec*1.0e-6;
}


void wtc_init()
{
  for (int i=0; i<MAX_WATCHES; i++)
  {
    walltime[i]=0.0;
    cputime[i]=0.0;
    names[i][0]='\0';
  }
  curr=0;
  cputime[curr]  =-wtc_ctime();
  walltime[curr] =-wtc_wtime();

  

}

void wtc_set_name(const int idx, const char *name)
{
   const int i=IDX(idx);
   snprintf(names[i], MAX_LEN , "%s", name);
   names[i][MAX_LEN]='\0';
}


  
void wtc_change_watch(const int idx)
{
  const double ctime = wtc_ctime();
  const double wtime = wtc_wtime();

  cputime[curr]  += ctime;
  walltime[curr] += wtime;
 
  curr           = IDX(idx); 
  cputime[curr]  -= ctime;
  walltime[curr] -= wtime;
}


  
void wtc_print_watches()
{
  const double ctime = wtc_ctime();
  const double wtime = wtc_wtime();
  double total=0;

  cputime[curr]  += ctime;
  walltime[curr] += wtime;

  printf("Watch name       CPU Time         Wall Time        Percentage\n");
 
  for (int i=0; i<MAX_WATCHES; i++) total+=walltime[i];
  for (int i=0; i<MAX_WATCHES; i++)
    if(names[i][0] != '\0')
  {
    printf("%s " WTC_FMT  WTC_FMT "%16.1lf%%\n",
      names[i], cputime[i], walltime[i], 100.0*walltime[i]/total);
  }
  printf("Total Wall Time %.2lf\n", total);

  cputime[curr]  -= ctime;
  walltime[curr] -= wtime;
}


#ifdef _TEST_MAIN
int main(int argc, char *argv)
{
  wtc_init();
  wtc_set_name(0, "watch 0");
  wtc_set_name(1, "watch 1");
  wtc_set_name(2, "watch 2");
  wtc_set_name(3, "watch 3");

  for (int i=0; i<MAX_WATCHES; i++)
    if(names[i][0] != '\0')
       printf("Name[%2.2d]=%s\n", i, names[i]);

  printf("\n\n");

  wtc_change_watch(1);
  sleep(1);
  wtc_change_watch(2);
  sleep(2);
  wtc_change_watch(3);

  const int n=7777;
  double sum=10.0;
  for (int j=0; j<n; j++)
    for (int i=0; i<n; i++)
      sum+=log(sum);

  printf("Sum %lf\n", sum);
  wtc_change_watch(0);
  wtc_print_watches();
  return 0;
}
#endif 
