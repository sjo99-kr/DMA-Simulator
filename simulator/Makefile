CC      = g++
CFLAGS  = -std=c++11 -Wall
SRCDIR  = /home/airplon/dma_modeling/dma_modeling_sw
BINDIR  = /home/airplon/dma_modeling/dma_modeling_sw/program

# 소스 파일들
SRCS    = $(wildcard $(SRCDIR)/*.cc)
OBJS    = $(SRCS:.cc=.o)
EXE     = dma_simulator

#all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $< -o $@
%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o

.PHONY: all clean
