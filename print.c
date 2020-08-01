#include <stdio.h>
#include "ossys.h"

void print_info()
{
  const char *row_names[] = {"Название дистрибутива:", "Релиз:", "Кодовое название:"};
  const char *dbuf = sys.tmpf_buf;

  printf("Время запроса: %s\n", sys.ctime_buf);
  puts("-----------------------");
  printf("Операционная система: %s\n", sys.osname);

  /* distro information */
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
  printf("Время работы: %s\n", sys.uptime_buf);
  printf("Кол-во процессов (вместе с потоками): %s\n", sys.cprocs);
  printf("Общая загруженность: %s", sys.loadavg_buf);
  printf("Графическая среда: %s\n", genv.dskt_session);
  printf("Оболочка: %s\n", genv.shell);
  printf("Файл терминала: %s\n", sys.tty_buf);
}
