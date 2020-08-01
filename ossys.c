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
#include "ossys.h"

struct system_info sys;
struct env_info genv;
void print_info();

static void
format_uptime(long time)
{
  const long minute = 60;
  const long hour = minute * 60;
  const long day = hour * 24;
  sprintf(sys.uptime_buf, "%ld days, %ld:%02ld:%02ld", time / day, (time % day) / hour,
    (time % hour) / minute, time % minute);
}

static void
get_uptime()
{
  FILE *fp;
  double uptime;

  fp = fopen("/proc/uptime", "r");
  /* читает информацию из потока fp и записывает в uptime */
  fscanf(fp, "%lf", &uptime);
  fclose(fp);
  format_uptime(uptime);
}

static void
get_tty()
{
  const char *self = "/proc/self/fd/0";
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
  FILE *fp;
  const double megabyte = 1024 * 1024;
  struct sysinfo info;
  struct utsname un;
  sysinfo(&info);
  uname(&un);

  fp = fopen("/proc/loadavg", "r");
  fgets(sys.loadavg_buf, sizeof(sys.loadavg_buf), fp);
  fclose(fp);

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
  const char script_name[] = "info.sh";
  char tmpf_template[] = "/tmp/dinf-XXXXXX";
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

  /* осуществляет форматирование данных в ctime_buf */
  strftime(sys.ctime_buf, sizeof(sys.ctime_buf), "%H:%M:%S", ptm);
}

int main(void)
{
  get_currenttime();
  get_distroinfo();
  get_sysinfo();
  get_envinfo();
  get_tty();
  get_uptime();
  print_info();

  return 0;
}
