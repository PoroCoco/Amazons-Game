GSL_PATH ?= /net/ens/renault/save/gsl-2.6/install
CFLAGS = -std=c99 -Wall -Wextra -fPIC -I$(GSL_PATH)/include

ifeq ($(TURBO),true)
    CFLAGS += -O3
else
    CFLAGS += -O3
endif

LDFLAGS = -lm -L$(GSL_PATH)/lib -L$(GSL_PATH)/lib64 \
	-Wl,--rpath=${GSL_PATH}/lib,--no-as-needed -ldl -lgsl -lgslcblas -lm --coverage

# -Wstrict-prototypes 

COMPILER = gcc
BUILDDIR = build
INSTALLDIR = install
SOURCEDIR = src
TESTDIR = tst
TESTFILES = test.o test_graph.o test_client_random.o test_queens.o test_board.o test_move_logic.o test_territory.o test_tree.o
SOURCEFILES = graph.o client_random.o queens.o board.o move_logic.o heuristic.o queue.o territories.o tree.o

LDFLAGS += -I$(SOURCEDIR)/


all: build 

powerPlay : build
	./install/server -m 14 ./install/client_random1.so ./install/client_power_heuristic.so

play : build
		./install/server -m 5 ./install/client_random1.so ./install/client_random2.so

	montecarlo : build
		./install/server -m 8 ./install/client_monte_carlo.so ./install/client_random1.so

	valgrind: build
		valgrind ./install/server -m 5 ./install/client_random1.so ./install/client_monte_carlo.so


%.o: $(SOURCEDIR)/%.c
	$(COMPILER) -c $(CFLAGS) $(LDFLAGS) -o $(BUILDDIR)/$@ $<

%.o: $(TESTDIR)/%.c
	$(COMPILER) -c $(CFLAGS) $(LDFLAGS) -o $(BUILDDIR)/$@ $<

build: server client_random1.so client_new.so explosive_client.so
	rm -f build/*.gcda
	rm -f build/*.gcno

server: server.o graph.o queens.o game.o board.o move_logic.o
	$(COMPILER) $(CFLAGS) $(LDFLAGS) $(addprefix $(BUILDDIR)/, $^) -o $(BUILDDIR)/server 

arena: arena.o graph.o queens.o game.o board.o move_logic.o
	$(COMPILER) $(CFLAGS) $(LDFLAGS) $(addprefix $(BUILDDIR)/, $^) -o $(INSTALLDIR)/arena

client_random1.so: client_random.o board.o graph.o queens.o move_logic.o territories.o heuristic.o queue.o
	$(COMPILER) $(CFLAGS) $(LDFLAGS) --shared $(addprefix $(BUILDDIR)/, $^) -o $(BUILDDIR)/$@

client_power_heuristic.so: client_power_heuristic.o board.o graph.o queens.o move_logic.o heuristic.o
	$(COMPILER) $(CFLAGS) $(LDFLAGS) --shared $(addprefix $(BUILDDIR)/, $^) -o $(BUILDDIR)/$@

client_new.so: client_new_heuristic.o board.o graph.o queens.o move_logic.o heuristic.o  territories.o heuristic.o queue.o alphabeta.o
	$(COMPILER) $(CFLAGS) $(LDFLAGS) --shared $(addprefix $(BUILDDIR)/, $^) -o $(BUILDDIR)/$@

client_monte_carlo.so: montecarlo.o board.o graph.o queens.o move_logic.o heuristic.o tree.o territories.o queue.o heuristic.o
	$(COMPILER) $(CFLAGS) $(LDFLAGS) --shared $(addprefix $(BUILDDIR)/, $^) -o $(BUILDDIR)/$@

explosive_client.so: explosive_client.o board.o graph.o queens.o move_logic.o heuristic.o  territories.o heuristic.o queue.o 
	$(COMPILER) $(CFLAGS) $(LDFLAGS) --shared $(addprefix $(BUILDDIR)/, $^) -o $(BUILDDIR)/$@






alltests: $(TESTFILES) $(SOURCEFILES)
	$(COMPILER) $(CFLAGS) $(LDFLAGS) $(addprefix $(BUILDDIR)/, $^) -o $(INSTALLDIR)/alltests -lgsl

test: alltests

install: 
	mv $(BUILDDIR)/server $(INSTALLDIR)/server
	mv $(BUILDDIR)/client_new.so $(INSTALLDIR)/client_new.so
	mv $(BUILDDIR)/explosive_client.so $(INSTALLDIR)/explosive_client.so
	mv $(BUILDDIR)/client_random1.so $(INSTALLDIR)/client_random1.so

clean:
	@rm -f *~ src/*~ build/* install/*

.PHONY: client install test clean
