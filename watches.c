#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>

#ifdef _TEST_MAIN
#include<math.h>
#endif

#ifdef __APPLE__
typedef int clockid_t;
#endif

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))


#include "watches.h"

#define MAX_WATCHES 32
#define MAX_NAME_LEN 32
#define MAX_LEN ((size_t)(MAX_NAME_LEN-1))


static char name[MAX_NAME_LEN];

#define IDX(i) MAX(0, MIN(MAX_WATCHES-1, i))
#define WTC_FMT " %10.2lf "

static clockid_t clk_id_tmp;
static int curr=0;
static int thisrank=0;

static double walltime[MAX_WATCHES];
static double cputime[MAX_WATCHES];
static char names[MAX_WATCHES][MAX_NAME_LEN];

static FILE *flog=NULL;

int wtc_printf(const char *format, ...)
{
  const size_t maxlen=255;
  char line[maxlen+1];

  va_list argList;
  va_start(argList, format);
     vsnprintf(line, maxlen, format, argList);
  va_end(argList);
  line[maxlen]='\0';

  int ret;
  if (flog) ret=fprintf(flog, "%s", line);
  else      ret= printf(      "%s", line);
  return ret;
}


int wtc_dateprintf(const char *format, ...)
{
  const size_t maxlen=128;
  char dtime[2*maxlen];
  char line[2*maxlen];
  time_t now=time(0);
  strftime(dtime, maxlen, "%Y-%m-%d %H:%M:%S: ", localtime(&now));

  va_list argList;
  va_start(argList, format);
     vsnprintf(line, maxlen, format, argList);
  va_end(argList);
  line[maxlen]='\0';

  strncat(dtime, line, maxlen);

  int ret;
  if (flog) ret=fprintf(flog, "%s", dtime);
  else      ret= printf(      "%s", dtime);
  return ret;
}


void wtc_open(const int rank, const char const *hname)
{
  const size_t maxlen=31;
  char fnam[maxlen+1];
  thisrank=rank;
  snprintf(fnam, maxlen, "log_rank_%4.4d.txt", rank);
  fnam[maxlen]='\0';

  flog=fopen(fnam, "a");
  wtc_dateprintf("Rank %d started on node %s\n", rank, hname);
  
   
}
void wtc_close()
{
  fclose(flog);
}

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

void wtc_set_name(const int idx, const char const *name)
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



  wtc_printf("Watch name                         "
             "CPU Time   Wall Time        Percentage\n");
 
  for (int i=0; i<MAX_WATCHES; i++) total+=walltime[i];
  for (int i=0; i<MAX_WATCHES; i++)
    if(names[i][0] != '\0')
  {
    const int len=strlen(names[i]);
    if (len>0) for(int j=0;j<MAX_LEN ; j++) 
      if (j>=len) name[j]=' ';
      else        name[j]=names[i][j];
    name[MAX_LEN]='\0';

    wtc_printf("%s " WTC_FMT  WTC_FMT "%16.1lf%%\n",
      name, cputime[i], walltime[i], 100.0*walltime[i]/total);
  }
  wtc_printf("Total Wall Time %.2lf\n", total);

  // Print a CSV report
  if(thisrank==0)
  {
    wtc_printf("CSV");
    for (int i=0; i<MAX_WATCHES; i++) if(names[i][0] != '\0')
       wtc_printf(",%s", names[i]);
  }
  wtc_printf("\nCSV");
  for (int i=0; i<MAX_WATCHES; i++) if(names[i][0] != '\0')
    wtc_printf(",%.2lf", walltime[i]);
  wtc_printf("\n");
  // End of Print a CSV report


  cputime[curr]  -= ctime;
  walltime[curr] -= wtime;
}


#ifdef _TEST_MAIN
int main(int argc, char *argv)
{
  wtc_printf("stdout test %d\n", (int)1001);
  wtc_dateprintf("stdout test with date %d\n", (int)1002);
  wtc_open(0, "localnode.br");
  wtc_printf("log test %d\n", (int)1003);
  wtc_dateprintf("log test with date %d\n", (int)1004);

  wtc_init();
  wtc_set_name(0, "watch 0");
  wtc_set_name(1, "watch 1");
  wtc_set_name(2, "watch 2");
  wtc_set_name(3, "watch 3");

  for (int i=0; i<MAX_WATCHES; i++)
    if(names[i][0] != '\0')
       wtc_printf("Name[%2.2d]=%s\n", i, names[i]);

  wtc_printf("\n\n");

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

  wtc_printf("Sum %lf\n", sum);
  wtc_change_watch(0);
  wtc_print_watches();

  wtc_close();
  return 0;
}
#endif 
