EXECUTABLE_NAME := he2ExecFile
CFLAGS := -std=c++14 -g -pthread 
CC := g++
DEPS := worker.hpp observer.hpp writeOutput.h
INPUT_FİLE := inp
OUTPUT_FILE := out
INP_CREATOR := inpcreate/creator.py
ERR_LOG := error.log
ZIP_FILE := oshw2.tar.gz
HOST := e2171874@external.ceng.metu.edu.tr
DEPLOYMENT_PWD := /home/bs05/e2171874/Desktop/Ceng334/hw2

writeOutput.o: writeOutput.h writeOutput.c
	gcc -c -g writeOutput.c -o writeOutput.o

%.o: %.c %.h $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: clean writeOutput.o worker.o producer.o transporter.o miner.o smelter.o foundry.o main.cpp worker.hpp producer.hpp smelter.hpp miner.hpp foundry.hpp transporter.hpp writeOutput.h
	g++ -Wall $(CFLAGS) writeOutput.o main.cpp worker.o producer.o miner.o smelter.o foundry.o transporter.o -o $(EXECUTABLE_NAME)

clean:
	rm -rf *.o

ioop:
	echo > $(ERR_LOG)
	echo > $(INPUT_FİLE)
	echo > $(OUTPUT_FILE)
	python3 $(INP_CREATOR) $(INPUT_FİLE)

run: ioop
	./$(EXECUTABLE_NAME) < $(INPUT_FİLE) > $(OUTPUT_FILE)

zip:
	tar -zcvf $(ZIP_FILE) *.cpp *.hpp *.c *.h inpcreate Makefile

send: zip
	scp -P 8085 $(ZIP_FILE) $(HOST):$(DEPLOYMENT_PWD)

connect: send
	ssh -p 8085 $(HOST)

debug:
	gdb ./$(EXECUTABLE_NAME)







