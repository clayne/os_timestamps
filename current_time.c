#ifndef CURRENT_TIME_C
#define CURRENT_TIME_C

#include "current_time.h"


int main (){
  print_current_time_seconds();
  print_current_time_ns();
  check_clock_res();
}

void print_current_time_seconds(){
  time_t* rawtime = current_time_seconds();
  struct tm * timeinfo;
  timeinfo = localtime(rawtime);
  printf("%s", asctime(timeinfo) );
}

time_t* current_time_seconds(){
  time_t* rawtime = (time_t*) calloc(sizeof(time_t), 1);
  time(rawtime);
  return rawtime;
}

void print_current_time_ns(){
  struct timespec* ts = current_time_ns();
  struct tm * timeinfo;
  time_t rawtime = ts->tv_sec;
  timeinfo = localtime(&rawtime);
  char* buf = asctime(timeinfo);
  int n = strlen(buf);
  buf[n-1] = 0;
  printf("%s - ns: %9ld\n", buf, ts->tv_nsec);
}

struct timespec* current_time_ns(){
  struct timespec* ts = (struct timespec*) calloc(sizeof(struct timespec), 1);
  clock_gettime(CLOCK_REALTIME, ts);
  time_t s = ts->tv_sec;
  long ns = ts->tv_nsec;
  
  printf ("%ld-%9ld\n", s, ns);
  return ts;
}

void check_clock_res(){
  struct timespec ts;
  int res = clock_getres(CLOCK_REALTIME, &ts);
  if (res != 0){
    printf("ERROR: check_clock_res - clock_getres failed\n");
    return;
  }
  
  time_t s = ts.tv_sec;
  long ns = ts.tv_nsec;
  
  if (s != 0){
    printf("WARNING: Clock resolution is greater than 1s.");
  }
  else if (ns > 20000000){
    printf("WARNING: Clock resolution is greater than 20 000 000ns/");
  }
  else if (s != 0 && ns != 0){
    printf("WARNING: Clock resolution has both s and ns values.");
  }
  
  printf ("INFO: Clock resolution is: %ld s - %ld ns\n", s, ns);
}

#endif
