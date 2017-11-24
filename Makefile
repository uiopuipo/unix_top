# Makefile

# 파일명을 입력 (확장자 빼고)
FILE1=main

OBJS=$(FILE1)

CC=gcc
CFLAGS=
LIBS=

all: $(FILE1)

$(FILE1):	$(FILE1).c
	$(CC) $(FILE1).c -o mytop

c:
	rm -f $(OBJS) output core
