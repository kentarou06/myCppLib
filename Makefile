
# define macro
MYLIB=${HOME}/lib/myCppLib

SRC        = $(shell cat include_source.txt)
OBJ        = $(SRC:%.cpp=%.o)
HEADER     = $(SRC:%.cpp=%.h)
ALL_HEADER = headerfile.h

libmylib.so: $(OBJ)
	g++ -shared $(OBJ) -o $@
	make $(ALL_HEADER)

.cpp.o:
	g++ -fPIC -c $< -o $@ -I${MYLIB}

$(ALL_HEADER): $(HEADER)
	$(shell cat $(HEADER) > $@)

clean:
	rm $(OBJ)
	rm *~ speech/*~ test/*~'
