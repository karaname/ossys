ossys: main.c
	gcc -g3 main.c -o ossys

install:
	install {distro_desc.sh,ossys} /usr/local/bin

uninstall:
	rm /usr/local/bin/{distro_desc.sh,ossys}
