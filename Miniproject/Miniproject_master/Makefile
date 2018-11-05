CC = /home/student/Desktop/buildroot/output/staging/usr/bin/avr32-linux-gcc
LDFLAGS = -pthread
EXECNAME = miniproject.out
NFSTARGET = /export/nfs/miniproject

all: main.o udp.o communication.o controller.o timer.o
	$(CC) $(LDFLAGS) main.o udp.o communication.o controller.o timer.o -o $(EXECNAME)
	cp $(EXECNAME) $(NFSTARGET)

main.o: main.c
	$(CC) -c main.c -o main.o

udp.o: network/udp.c
	$(CC) -c network/udp.c -o udp.o

communication.o: communication/communication.c
	$(CC) -c communication/communication.c -o communication.o

controller.o: controller/controller.c
	$(CC) -c controller/controller.c -o controller.o

timer.o: timer/timer.c
	$(CC) -c timer/timer.c -o timer.o

clean:
	rm communication.o udp.o main.o controller.o timer.o $(EXECNAME)
