CC = gcc
OBJS = tp2virtual.o
EXE = tp2virtual

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(OBJS) -o $(EXE)

clean:
	rm $(EXE) $(OBJS)
