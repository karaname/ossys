ossys: ossys.c
	gcc -g3 ossys.c -o ossys

install:
	install {info.sh,ossys} /usr/local/bin

uninstall:
	rm /usr/local/bin/{info.sh,ossys}
