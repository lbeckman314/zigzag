all:
	gcc -o zigzag-client.out zigzag-client.c

clean:
	rm -f zigzag-client.out

zip:
	zip project1.zip  \
	README.txt        \
	available-port.sh \
	makefile          \
	zigzag-client.c   \
	zigzag-client.h   \
	zigzag-server.py

uninstall:
	rm -f project1.zip \
	README.txt         \
	available-port.sh  \
	makefile           \
	zigzag-client.c    \
	zigzag-client.h    \
	zigzag-server.py
