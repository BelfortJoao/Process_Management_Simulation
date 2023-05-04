make:
	gcc -o main src/main.c \
                        src/manager/timer.c \
                        src/manager/timer.h \
                        src/manager/cpu.c \
                        src/manager/cpu.h \
                        src/manager/executingstate.c \
                        src/manager/executingstate.h \
                        src/manager/blockedstate.c \
                        src/manager/blockedstate.h \
                        src/manager/TESTEFILA.c \
                        src/manager/readystate.h \
                        src/manager/processtable.c \
                        src/manager/processtable.h \
                        src/process/process.c \
                        src/process/process.h \
                        src/control/controller.c \
                        src/control/controller.h \
                        src/printer/printer.c \
                        src/printer/printer.h \
                        src/manager/manager.c \
                        src/manager/manager.h -lm
run:
	./main