#include <sys/utsname.h>
#include <sys/time.h>
#include <stdio.h>
#include <time.h>

static void
print_osinfo()
{
  struct utsname un;
  uname(&un);
  printf("Операционная система: %s %s\n", un.sysname, un.release);
  printf("Имя хоста: %s\n", un.nodename);
  printf("Имя домена: %s\n", un.__domainname);
  printf("Архитектура: %s\n", un.machine);
}

static void
print_currenttime()
{
  struct timeval tval;   /* gettimeofday */
  struct tm *ptm;        /* localtime    */
  char time_string[15];

  /* определяет текущее системной время - секунды */
  gettimeofday(&tval, NULL);

  /* возвращает структуру типа tm, принимает
  в роли аргумента число секунд -> поле tv_sec
  структуры timeval */
  ptm = localtime(&tval.tv_sec);

  /* осуществляет формат данных в time_string */
  strftime(time_string, sizeof(time_string), "%H:%M:%S", ptm);
  printf("Время запроса %s\n\n", time_string);
}

int main(void)
{
  print_currenttime();
  print_osinfo();

  return 0;
}
