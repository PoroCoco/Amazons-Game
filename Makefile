GSL_PATH ?= /net/ens/renault/save/gsl-2.6/install
CFLAGS = -std=c99 -Wall -Wextra -fPIC -g3 -I$(GSL_PATH)/include
LDFLAGS = -lm -lgsl -lgslcblas \
	-L$(GSL_PATH)/lib -L$(GSL_PATH)/lib64 \
	-Wl,--rpath=${GSL_PATH}/lib,--no-as-needed -ldl


COMPILER = gcc
BUILDDIR = build
INSTALLDIR = install
SOURCEDIR = src

LDFLAGS += -I$(SOURCEDIR)/


all: build

%.o: $(SOURCEDIR)/%.c
	$(COMPILER) -c $(CFLAGS) $(LDFLAGS) -o $(BUILDDIR)/$@ $<

build: server client_1.so

server: server.o
	$(COMPILER) $(CFLAGS) $(LDFLAGS) $(addprefix $(BUILDDIR)/, $^) -o $(INSTALLDIR)/server

client_1.so: client_1.o
	$(COMPILER) $(CFLAGS) $(LDFLAGS) --shared $(addprefix $(BUILDDIR)/, $^) -o $(BUILDDIR)/client_1.so

alltests:

test: alltests

install: server client test

clean:
	@rm -f *~ src/*~ build/* install/*

.PHONY: client install test clean
