AUTHOR := hmartzol
NAME := lem-in
SRCDIR := src
INCDIRS := inc
OBJDIR := .obj
DEPDIR := .dep
MAIN := main.c
CFLAGS := -Wall -Wextra -Werror -g
PINC := libft/inc
CLIB := libft
LIB := libft/libft.a
include Makefiles/Makefile.gen
