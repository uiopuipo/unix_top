# Makefile

<<<<<<< HEAD
# ���ϸ��� �Է� (Ȯ���� ����)
FILE1=main

OBJS=$(FILE1)

CC=gcc
CFLAGS=
LIBS=

all: $(FILE1)

$(FILE1):	$(FILE1).c
	$(CC) $(FILE1).c -o top

c:
	rm -f $(OBJS) output core
