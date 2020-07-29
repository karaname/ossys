#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>

struct system_info {
  char time_string[15];
  char tmpf_buffer[256];
  char *os;
  char *hostname;
  char *domain;
  char *arch;
} sys;

static void
print_info()
{
  //printf("Операционная система: %s\n", sys.os);
  //printf("Имя хоста: %s\n", sys.hostname);
  //printf("Имя домена: %s\n", sys.domain);
  //printf("Архитектура: %s\n", sys.arch);
}

/*
static void
print_sysinfo()
{
  FILE *loadavg_stream;
  char loadavg_buffer[124];
  const double megabyte = 1024 * 1024;
  struct sysinfo info;

  if (sysinfo(&info) == -1) {
    fprintf(stderr, "sysinfo() can't get system information\n");
    exit(1);
  }

  printf("\nДоступно ОЗУ: %.1f\n", info.totalram / megabyte);
  printf("Свободно ОЗУ: %.1f\n", info.freeram / megabyte);
  printf("Кол-во процессов (вместе с потоками): %d\n", info.procs);

  loadavg_stream = fopen("/proc/loadavg", "r");
  fgets(loadavg_buffer, sizeof(loadavg_buffer), loadavg_stream);
  printf("Общая загруженность: %s", loadavg_buffer);
}
*/

static void
osinfo()
{
  int fd, nb;
  char tmpf_template[] = "/tmp/dinfo-XXXXXX";
  char script_name[] = "info.sh";
  char tmpf_fd_path[20];
  char tmpf_name[24];
  struct utsname un;
  uname(&un);
  pid_t child;

  if ((fd = mkstemp(tmpf_template)) == -1) {
    fprintf(stderr, "mkstemp() can't create temporary file\n");
    exit(1);
  }

  memset(tmpf_name, 0, sizeof(tmpf_name));
  sprintf(tmpf_fd_path, "/proc/%d/fd/%d", getpid(), fd);
  readlink(tmpf_fd_path, tmpf_name, sizeof(tmpf_name));

  switch (child = fork()) {
    case -1:
      fprintf(stderr, "fork() can't create child\n");
      exit(1);
    case 0:
      execlp(script_name, script_name, tmpf_name, NULL);
      fprintf(stderr, "execlp() can't execute - %s\n", script_name);
      exit(1);
    default:
      wait(NULL);
  }

  if (nb = read(fd, sys.tmpf_buffer, sizeof(sys.tmpf_buffer)) == -1) {
    fprintf(stderr, "read() can't read temporary file\n");
    exit(1);
  }

  if (unlink(tmpf_name) == -1) {
    fprintf(stderr, "unlink() can't remove temporary file\n");
    exit(1);
  }

  /*
  sys.os = un.sysname;
  sys.hostname = un.nodename;
  sys.domain = un.__domainname;
  sys.arch = un.machine;
  */
  //printf("Операционная система: %s\n", un.sysname);
  //printf("\nИмя хоста: %s\n", un.nodename);
  //printf("---Имя домена: %s\n", sys.domain);
  //printf("Архитектура: %s\n", un.machine);
}


static void
currenttime()
{
  struct timeval tval;   /* gettimeofday */
  struct tm *ptm;        /* localtime    */

  /* определяет текущее системной время - секунды */
  gettimeofday(&tval, NULL);

  /* возвращает структуру типа tm, принимает
  в роли аргумента число секунд -> поле tv_sec
  структуры timeval */
  ptm = localtime(&tval.tv_sec);

  /* осуществляет форматирование данных в time_string */
  strftime(sys.time_string, sizeof(sys.time_string), "%H:%M:%S", ptm);
}

int main(void)
{
  currenttime();
  osinfo();
  //print_info();

  return 0;
}
