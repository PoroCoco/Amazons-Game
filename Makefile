GSL_PATH ?= /net/ens/renault/save/gsl-2.6/install
CFLAGS = -std=c99 -Wall -Wextra -Wstrict-prototypes -fPIC -g3 -I$(GSL_PATH)/include
LDFLAGS = -lm -L$(GSL_PATH)/lib -L$(GSL_PATH)/lib64 \
	-Wl,--rpath=${GSL_PATH}/lib,--no-as-needed -ldl -lgsl -lgslcblas -lm


COMPILER = gcc
BUILDDIR = build
INSTALLDIR = install
SOURCEDIR = src
TESTDIR = tst
TESTFILES = test.o test_graph.o test_client_random.o test_queens.o
SOURCEFILES = graph.o client_random.o queens.o board.o

LDFLAGS += -I$(SOURCEDIR)/


all: build

play : build
	./install/server ./install/client_random.so ./install/client_random.so

%.o: $(SOURCEDIR)/%.c
	$(COMPILER) -c $(CFLAGS) $(LDFLAGS) -o $(BUILDDIR)/$@ $<

%.o: $(TESTDIR)/%.c
	$(COMPILER) -c $(CFLAGS) $(LDFLAGS) -o $(BUILDDIR)/$@ $<

build: server client_random.so

server: server.o graph.o queens.o game.o
	$(COMPILER) $(CFLAGS) $(LDFLAGS) $(addprefix $(BUILDDIR)/, $^) -o $(INSTALLDIR)/server 

client_random.so: client_random.o board.o
	$(COMPILER) $(CFLAGS) $(LDFLAGS) --shared $(addprefix $(BUILDDIR)/, $^) -o $(INSTALLDIR)/client_random.so

alltests: $(TESTFILES) $(SOURCEFILES)
	$(COMPILER) $(CFLAGS) $(LDFLAGS) $(addprefix $(BUILDDIR)/, $^) -o $(INSTALLDIR)/alltests -lgsl

test: alltests

install: server client test

clean:
	@rm -f *~ src/*~ build/* install/*

.PHONY: client install test clean
