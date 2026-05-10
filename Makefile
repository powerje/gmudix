UNAME_S := $(shell uname -s)

# MSYS2 / MinGW shells report MINGW64_NT-* / MSYS_NT-*. Treat all as Windows.
WIN_HOST :=
ifneq (,$(findstring MINGW,$(UNAME_S)))
  WIN_HOST := 1
endif
ifneq (,$(findstring MSYS,$(UNAME_S)))
  WIN_HOST := 1
endif

# Default compiler: gcc on MinGW (no clang/cc available), clang elsewhere.
# `?=` is intentionally avoided because make's built-in CC=cc would win.
# Override with `make CC=gcc` (or any other value) on the command line.
ifdef WIN_HOST
  CC = gcc
else
  CC = clang
endif

PKG_DEPS = glib-2.0 gthread-2.0 gtk+-2.0 zlib
INCS     = -I./include $(shell pkg-config --cflags $(PKG_DEPS))
LDLIBS   = $(shell pkg-config --libs $(PKG_DEPS))

ifdef WIN_HOST
  EXE_SUFFIX := .exe
  LDLIBS     += -lws2_32
else
  EXE_SUFFIX :=
endif

# TODO(javier): Renable -Werror
CFLAGS = -std=c99 -Wall -pedantic -g -D_XOPEN_SOURCE -D_POSIX_C_SOURCE=200809L $(INCS)

SRCS = $(wildcard src/*.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))
BIN  = gmudix$(EXE_SUFFIX)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o $@

clean:
	rm -f $(OBJS) gmudix gmudix.exe

.PHONY: clean
