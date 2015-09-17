GCC=g++
INC=-I/usr/include/irods/ -I/usr/include/irods/boost/

all: unmount

unmount:
	${GCC} ${INC} -DRODS_SERVER -fPIC -shared -o libirods_unmount.so unmount.cpp

clean:
	@rm -f ./*.so
