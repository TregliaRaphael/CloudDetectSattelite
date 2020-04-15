
#---------------------------------------------------------
#makefile permettant la compilation du programme principal
#---------------------------------------------------------
.KEEP_STATE:

#definition des variables
CC=gcc
OBJ=../OBJ
EXE=../EXE
CCFLAGS=-Wall -g `pkg-config --cflags gtk+-2.0`
LDFLAGS=`pkg-config --libs gtk+-2.0`

#compilation
OBJ_DEP=compute_misc.o compute.o main.o
OBJ_FILES=${OBJ}/compute_misc.o ${OBJ}/compute.o ${OBJ}/main.o


all: ImaProjet.exe ImaProjet_auto.exe

.c.o:
	${CC} ${CCFLAGS} -o $@ -c $<
	mv *.o ${OBJ}

ImaProjet.exe: ${OBJ_DEP}
	${CC} -o ${EXE}/$@ ${OBJ_FILES} ${LDFLAGS}

ImaProjet_auto.exe:${OBJ_DEP}
	${CC} -D=AUTO -o ${EXE}/$@ ${OBJ_FILES} ${LDFLAGS}

clean:
	\rm -f ${OBJ}/*.o ../*/core ${EXE}/*.exe

install:
	cp *.h ${INC}

.SUFFIXES: .exe .o .c .h