OBJECTS=obj/ossys.o obj/print.o
SRCDIR=src
OBJDIR=obj
CC=gcc -g3

ossys: $(OBJECTS)
	$(CC) -o ossys $(OBJECTS)

$(OBJDIR)/ossys.o: $(SRCDIR)/ossys.c
	@test -e obj/ || mkdir obj/
	$(CC) -c $(SRCDIR)/ossys.c -o $(OBJDIR)/ossys.o

$(OBJDIR)/print.o: $(SRCDIR)/print.c
	$(CC) -c $(SRCDIR)/print.c -o $(OBJDIR)/print.o

clean:
	rm -rf $(OBJDIR)

install:
	install $(SRCDIR)/info.sh /usr/local/bin
	install ossys /usr/local/bin

uninstall:
	rm /usr/local/bin/info.sh
	rm /usr/local/bin/ossys

