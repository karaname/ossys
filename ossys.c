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

static struct system_info {
  char time_buf[16];
  char tmpf_buf[128];
  char loadavg_buf[64];
  char osname[16];
  char hostname[32];
  char domain[32];
  char arch[16];
  char ramtot[8];
  char ramfree[8];
  char cprocs[8];
  char tty_buf[12];
} sys;

static struct env_info {
  char *dskt_session;
  char *shell;
} genv;

static void
print_info()
{
  printf("Время запроса: %s\n", sys.time_buf);
  puts("-----------------------");
  printf("Операционная система: %s\n", sys.osname);

  char *row_names[] = {"Название дистрибутива:", "Релиз:", "Кодовое название:"};
  char *dbuf = sys.tmpf_buf;

  for (int b, i = 0; *dbuf; dbuf++) {
    if (b) { printf("%s ", row_names[i++]); b = 0; }
    if (*dbuf == '\n') { b = 1; }
    putchar(*dbuf);
  }

  printf("Имя хоста: %s\n", sys.hostname);
  printf("Имя домена: %s\n", sys.domain);
  printf("Архитектура: %s\n", sys.arch);
  printf("Доступно ОЗУ: %s\n", sys.ramtot);
  printf("Свободно ОЗУ: %s\n", sys.ramfree);
  printf("Кол-во процессов (вместе с потоками): %s\n", sys.cprocs);
  printf("Общая загруженность: %s", sys.loadavg_buf);
  printf("Графическая среда: %s\n", genv.dskt_session);
  printf("Оболочка: %s\n", genv.shell);
  printf("Файл терминала: %s\n", sys.tty_buf);
}

static void
get_tty()
{
  char *self = "/proc/self/fd/0";
  readlink(self, sys.tty_buf, sizeof(sys.tty_buf));
}

static void
get_envinfo()
{
  genv.dskt_session = getenv("DESKTOP_SESSION");
  genv.shell = getenv("SHELL");
}

static void
get_sysinfo()
{
  FILE *loadavg_stream;
  const double megabyte = 1024 * 1024;
  struct sysinfo info;
  struct utsname un;
  sysinfo(&info);
  uname(&un);

  loadavg_stream = fopen("/proc/loadavg", "r");
  fgets(sys.loadavg_buf, sizeof(sys.loadavg_buf), loadavg_stream);

  sprintf(sys.ramtot, "%.1f", info.totalram / megabyte);
  sprintf(sys.ramfree, "%.1f", info.freeram / megabyte);
  sprintf(sys.cprocs, "%d", info.procs);
  sprintf(sys.osname, "%s", un.sysname);
  sprintf(sys.hostname, "%s", un.nodename);
  sprintf(sys.domain, "%s", un.__domainname);
  sprintf(sys.arch, "%s", un.machine);
}

static void
get_distroinfo()
{
  int fd, nb;
  char tmpf_template[] = "/tmp/dinfo-XXXXXX";
  char script_name[] = "info.sh";
  char tmpf_fd_path[20];
  char tmpf_name[24];
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

  if (nb = read(fd, sys.tmpf_buf, sizeof(sys.tmpf_buf)) == -1) {
    fprintf(stderr, "read() can't read temporary file\n");
    exit(1);
  }

  if (unlink(tmpf_name) == -1) {
    fprintf(stderr, "unlink() can't remove temporary file\n");
    exit(1);
  }
}

static void
get_currenttime()
{
  struct timeval tval;   /* gettimeofday */
  struct tm *ptm;        /* localtime    */

  /* определяет текущее системной время - секунды */
  gettimeofday(&tval, NULL);

  /* возвращает структуру типа tm, принимает
  в роли аргумента число секунд -> поле tv_sec
  структуры timeval */
  ptm = localtime(&tval.tv_sec);

  /* осуществляет форматирование данных в time_buf */
  strftime(sys.time_buf, sizeof(sys.time_buf), "%H:%M:%S", ptm);
}

int main(void)
{
  get_currenttime();
  get_distroinfo();
  get_sysinfo();
  get_envinfo();
  get_tty();
  print_info();

  return 0;
}
