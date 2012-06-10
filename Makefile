OBJS = Director.o \
	   IdleState.o \
	   BeforeGameState.o \
	   HumanState.o \
	   FadeoutAnimation.o \
	   FlipAnimation.o \
	   Picture.o \
	   main.o

all: main run
%.o: %.cpp
	g++ -g -D__STDC_LIMIT_MACROS -c $<
main: $(OBJS)
	g++ `pkg-config --cflags --libs opencv` -D__STDC_LIMIT_MACROS -o final $(OBJS)
run:
	./final
clean:
	rm -f *.o
