ossys: ossys.o print.o
	gcc -g3 -o ossys obj/ossys.o obj/print.o

ossys.o: ossys.c
	@test -e obj/ || mkdir obj/
	gcc -g3 -c ossys.c -o obj/ossys.o

print.o: print.c
	gcc -g3 -c print.c -o obj/print.o

clean:
	rm -rf obj/

install:
	install {info.sh,ossys} /usr/local/bin

uninstall:
	rm /usr/local/bin/{info.sh,ossys}
