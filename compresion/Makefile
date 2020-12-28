
CC = c++

COMP = compresion
MAIN = compMain
EXENAME = comp

todo: $(COMP).o
	$(CC) -o $(EXENAME) $(COMP).o $(MAIN).cpp

%.o: %.c
	$(CC) -c $@ $<

clean:
	rm *.o $(EXENAME)
