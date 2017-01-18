###################################################################
#
# freeman.justin@gmail.com 
#
##################################################################

CC=	gcc

CSRC=	./src/

CFLAGS=	-O3 -g -Wall 

INC=	-I./include 

LFLAGS= 

COBJ=	$(CSRC)main.o \
	$(CSRC)md5.o \
	$(CSRC)fail.o \
	$(CSRC)jutil.o \
	$(CSRC)channelshift.o

OBJ=	$(COBJ) 

EXEC=	./bin/avicon

$(EXEC):$(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ) $(LFLAGS)

$(COBJ) : %.o : %.c
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

clean:
	rm $(COBJ)
	rm $(EXEC)
