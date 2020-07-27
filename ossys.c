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

static void
print_osinfo()
{
  struct utsname un;
  FILE *tmpf_stream;
  int fd, nb, index = 0;
  char tmpf_buffer[124];
  char tmpf_template[] = "/tmp/dinfo-XXXXXX";
  char *script_name = "distro_desc.sh";
  char tmpf_name[32];
  char fd_path[20];
  pid_t child;

  uname(&un);
  printf("Операционная система: %s\n", un.sysname);
  printf("Версия ядра: %s\n", un.release);

  if ((fd = mkstemp(tmpf_template)) == -1) {
    fprintf(stderr, "mkstemp() can't create temporary file\n");
    exit(1);
  }

  memset(tmpf_name, 0, sizeof(tmpf_name));
  sprintf(fd_path, "/proc/%d/fd/%d", getpid(), fd);
  readlink(fd_path, tmpf_name, sizeof(tmpf_name));

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

  /* конверт файлогово дескриптора в поток */
  tmpf_stream = fdopen(fd, "r");

  char *col_names[] = {"Название дистрибутива:", "Релиз:", "Кодовое название:"};
  int len = (sizeof(col_names) / sizeof(col_names[0]));

  while (fgets(tmpf_buffer, sizeof(tmpf_buffer), tmpf_stream)) {
    printf("%s %s", col_names[index++], tmpf_buffer);
  }

  if (unlink(tmpf_name) == -1) {
    fprintf(stderr, "unlink() can't remove temporary file\n");
    exit(1);
  }

  printf("\nИмя хоста: %s\n", un.nodename);
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
  print_sysinfo();

  return 0;
}
