MKDIR_P = mkdir -p
# project name (generate executable with this name)
TARGET = main

CC=gcc
# compiling flags here
CFLAGS= -g -Wall
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings

# define any directories containing header file other then /usr/include
INCLUDES = -I../../lib/include

# define any libraries to link into executable:
# if I want to link in libraries
# (libx.so or libx.a) I use the -llibname (-lx -lx) = (-lx)
# libfdr => -lfdr -lm
LFLAGS = -lfdr -lbt 
# define any directories containing implementation of the file in INCLUDES
# ../..lib means up 2 level then search for /lib
LIBS = -L../../lib/implementation

# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
# For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix

SRCS = $(wildcard *.c)
ODIR = obj
_OBJS = $(SRCS:.c=.o)
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

.PHONY: clean
#action
all: $(TARGET)

# This uses Suffix Replacement within a macro:
#   $@ = left-part of ":" ( == $(TARGET))
# 

$(ODIR)/%.o: %.c $(DEPS)
	@mkdir -p obj
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@echo "Compiled "$<" successfully!"

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LIBS) $(LFLAGS)
	@echo "Linking complete!"	
clean:
	rm -f core $(ODIR)/*.o *~ $(TARGET)
	@echo "Cleanup complete!"
