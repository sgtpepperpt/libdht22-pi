CFLAGS := -fPIC -O3 -Wall -Werror
LFLAGS := -lwiringPi
IFLAGS := -I .

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

libdht_raspberry.so:
	gcc -c $(CFLAGS) -o dht.o dht.c $(LFLAGS) $(IFLAGS)
	gcc -shared -fPIC -Wl,-soname,libdht_raspberry.so -o libdht_raspberry.so dht.o -lc

install: libdht_raspberry.so
	install -d $(DESTDIR)$(PREFIX)/lib/
	install -m 644 libdht_raspberry.so $(DESTDIR)$(PREFIX)/lib/
	install -d $(DESTDIR)$(PREFIX)/include/
	install -m 644 dht.h $(DESTDIR)$(PREFIX)/include/

clean:
	$(RM) *.o *.so*
