#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <error.h>

struct system_info {
  char cur_time[12];
  char loadavg[64];
  char tty[12];
} sys;

struct env_info {
  char *dskt_session;
  char *shell;
} genv;

FILE *fopen_wrap(char *pathname)
{
  FILE *stream;
  stream = fopen(pathname, "r");
  if (stream == NULL) {
    error(0, 0, "fopen() return NULL");
    exit(EXIT_SUCCESS);
  }
  return stream;
}

int main(void)
{
  const long minute = 60;
  const long hour = minute * 60;
  const long day = hour * 24;
  const double megabyte = 1024 * 1024;
  struct timeval tval;
  struct tm *ptm;
  FILE *s1;
  FILE *s2;
  double uptime;

  struct sysinfo info;
  sysinfo(&info);

  struct utsname un;
  uname(&un);

  gettimeofday(&tval, NULL);
  ptm = localtime(&tval.tv_sec);
  strftime(sys.cur_time, sizeof(sys.cur_time), "%H:%M:%S", ptm);

  s1 = fopen_wrap("/proc/loadavg");
  fgets(sys.loadavg, sizeof(sys.loadavg), s1);
  fclose(s1);

  s2 = fopen_wrap("/proc/uptime");
  fscanf(s2, "%lf", &uptime);
  fclose(s2);

  genv.dskt_session = getenv("DESKTOP_SESSION");
  genv.shell = getenv("SHELL");

  int rval = readlink("/proc/self/fd/0", sys.tty, sizeof(sys.tty));
  if (rval == -1) {
    error(0, 0, "readlink() return -1");
    exit(EXIT_SUCCESS);
  }

  printf("Current time: %s\n", sys.cur_time);
  puts("-----------------------");
  printf("OS: %s\n", un.sysname);
  printf("Hostname: %s\n", un.nodename);
  printf("Domain name: %s\n", un.__domainname);
  printf("Machine hardware: %s\n", un.machine);
  printf("RAM Total: %.0f\n", info.totalram / megabyte);
  printf("RAM Free: %.0f\n", info.freeram / megabyte);
  printf("Uptime: %ld days, %ld:%02ld:%02ld\n", (long)uptime / day,
   ((long)uptime % day) / hour, ((long)uptime % hour) / minute, (long)uptime % minute);
  printf("Number of current processes: %d\n", info.procs);
  printf("Load average: %s", sys.loadavg);
  printf("Desktop environment: %s\n", genv.dskt_session);
  printf("Shell: %s\n", genv.shell);
  printf("tty: %s\n", sys.tty);

  return EXIT_SUCCESS;
}
