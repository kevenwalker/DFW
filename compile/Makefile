TARGET = DFW
INC = -I ../base \
      -I ../map \
	  -I ../main \
	  -I ../cli \
	  -I ../element
	  
FUN = ../main/main.o \
      ../map/map_base.o \
	  ../map/map_data.o \
	  ../cli/cli_show.o \
	  ../cli/cli_set.o \
	  ../base/misc.o \
	  ../base/log.o \
	  ../element/elm_player.o

FUNC = ../main/*.c \
       ../map/*.c \
	   ../cli/*.c \
	   ../base/*.c \
	   ../element/*.c
	  
CFLAGS = $(INC) -Wall -Werror -Wno-unused 

$(TARGET):$(FUN)
	gcc -o $(TARGET) ./*.o
$(FUN):$(FUNC)
	gcc $(CFLAGS) -c $(FUNC)
install:
	.\DFW.exe
clean:
	-del *.o
	