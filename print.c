#include <stdio.h>
#include "ossys.h"

#define CYN "\x1B[1m"
#define RES "\x1B[0m"

void print_info()
{
  const char *row_names[] = {"Название дистрибутива:", "Релиз:", "Кодовое название:"};
  const char *dbuf = sys.tmpf_buf;

  printf("Время запроса: %s%s%s\n", CYN, sys.ctime_buf, RES);
  puts("-----------------------");
  printf("Операционная система: %s%s%s\n", CYN, sys.osname, RES);

  for (int b, i = 0; *dbuf; dbuf++) {
    if (b) {printf("%s ", row_names[i++]); b = 0; }
    if (*dbuf == '\n') { b = 1; }
    printf("%s%c%s", CYN, *dbuf, RES);
  }

  printf("Имя хоста: %s%s%s\n", CYN, sys.hostname, RES);
  printf("Имя домена: %s%s%s\n", CYN, sys.domain, RES);
  printf("Архитектура: %s%s%s\n", CYN, sys.arch, RES);
  printf("Доступно ОЗУ: %s%s%s\n", CYN, sys.ramtot, RES);
  printf("Свободно ОЗУ: %s%s%s\n", CYN, sys.ramfree, RES);
  printf("Время работы: %s%s%s\n", CYN, sys.uptime_buf, RES);
  printf("Кол-во процессов (вместе с потоками): %s%s%s\n", CYN, sys.cprocs, RES);
  printf("Общая загруженность: %s%s%s", CYN, sys.loadavg_buf, RES);
  printf("Графическая среда: %s%s%s\n", CYN, genv.dskt_session, RES);
  printf("Оболочка: %s%s%s\n", CYN, genv.shell, RES);
  printf("Файл терминала: %s%s%s\n", CYN, sys.tty_buf, RES);
}
