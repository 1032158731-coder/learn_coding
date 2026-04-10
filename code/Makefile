TAG=./bin/main
SRC=$(wildcard ./src/*.cpp )
OBJ=$(SRC:%.cpp=%.o  );
#CC=g++
#CC=/usr/bin/arm-linux-gnueabihf-g++ 
#CC=arm-linux-g++
override CCONFIG+=-I./inc  -L./lib   -lpthread   -lrt  -ljpeg -std=c++17 -lstdc++ -lzlog#-g #-ljsoncpp    -lmodbus 
#override CCONFIG+=-L./lib  -ljpeg -lpthread  -I./inc
HOST=root@192.168.0.168# scp ???的
FILE_PATH=/parvin/pratice/

#HOST=payne@192.168.0.105# scp ???的
#FILE_PATH=/usr/local/payne/



$(TAG):$(OBJ)
	$(CC) $(^) -o $(@)  $(CCONFIG)

%.o:%.cpp
	$(CC) $(^) -o $(@)   -c  $(CCONFIG)


scp:$(TAG)
	$(@) $(^) $(HOST):$(FILE_PATH)


clean:
	rm $(TAG) $(OBJ)