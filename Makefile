ossys: ossys.c
	gcc -g3 ossys.c -o ossys

install:
	install {distro_desc.sh,ossys} /usr/local/bin

uninstall:
	rm /usr/local/bin/{distro_desc.sh,ossys}
