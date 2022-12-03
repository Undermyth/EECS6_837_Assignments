# for g++ on linux platforms

# ===============================================================

SRCS 	 	= main.cpp \
			lib/matrix.cpp \
			lib/image.cpp \
			ifs.cpp

# ===============================================================

LIBS            = -lm 
CC		= g++
INCLUDE 	= -I/usr/include -Iinclude
EXE 	  	= ifs
OBJS =  $(SRCS:.C=.o)

# ===============================================================
# targets

.PHONY: all depend clean

all: depend $(EXE) 

depend:
	$(CC) -E -M $(INCLUDE) $(SRCS) > Makefile.depend

clean: 
	rm -f *~ *bak *.o  $(EXE) Makefile.depend

# ===============================================================
# compilation rules

$(EXE): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS) $(INCLUDE)

.cpp.o:
	$(CC) $< -c -o $@

# ===============================================================

-include Makefile.depend

# DO NOT DELETE
