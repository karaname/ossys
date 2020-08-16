#!/bin/bash

if test -e ossys; then
  file=test_ossys.txt
  ./ossys > $file
else
  echo "ossys - No such file"; exit 1
fi

list=(\
"Время запроса" "Операционная система" "Название дистрибутива"
"Релиз" "Кодовое название" "Имя хоста" "Имя домена" "Архитектура"
"Доступно ОЗУ" "Свободно ОЗУ" "Время работы" "Кол-во процессов (вместе с потоками)"
"Общая загруженность" "Графическая среда" "Оболочка" "Файл терминала"
)

for ((i = 0; i < ${#list[*]}; i++)); do
  if test "$(grep "${list[$i]}" $file)"; then
    echo -e "${list[$i]} - \e[92mpassed\e[0m"
  else
    echo -e "${list[$i]} - \e[91mfailed\e[0m"
  fi
done

rm $file
