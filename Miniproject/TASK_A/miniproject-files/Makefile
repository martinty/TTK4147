CC = /home/student/Desktop/buildroot/output/staging/usr/bin/avr32-linux-gcc
EXECNAME = miniproject.out
NFSTARGET = /export/nfs/miniproject

all: main.o udp.o com.o controller.o time.o
	$(CC) main.o udp.o com.o controller.o time.o -o $(EXECNAME)
	cp $(EXECNAME) $(NFSTARGET)

main.o: main.c
	$(CC) -c main.c -o main.o

udp.o: network/udp.c
	$(CC) -c network/udp.c -o udp.o

com.o: com/com.c
	$(CC) -c com/com.c -o com.o

controller.o: controller/controller.c
	$(CC) -c controller/controller.c -o controller.o

time.o: time/time.c
	$(CC) -c time/time.c -o time.o

clean:
	rm com.o udp.o main.o controller.o time.o $(EXECNAME)
