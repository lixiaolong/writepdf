# may need change
TARGET = my_app
CC = gcc
# CFLAGS += -g -D__HAVE_SOME_DEFINE
CFLAGS += -g -I2.2.0/include/
LDFLAGS += -lhpdf
# LDFLAGS += -I2.3.0RC2/include/ -L2.3.0RC2/lib/ -lhpdf

# for c
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c = .o)

###################
$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) $(OBJS)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o
.PHONY:clean
