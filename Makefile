SRCS		:=
OBJS		:=
DEPS		:=
TMPS		:=
LIBS		:=
PROGRAMS	:=
TESTS		:=
#======================================================================
# MODULES
MODULES		:= src/system src/driver src/json 
MODULES		+= 
# LIBS
MODULES		+=
# APP
MODULES 	+=
# TESTS
MODULES		+=

INCLUDE_DIRS	:= include
vpath %.h $(INCLUDE_DIRS)
#======================================================================
# select which tools to use as compiler, 
# librarian and linker
#======================================================================
CFLAGS	+= -std=c99 -Wall -O1
CFLAGS	+= -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS	+= $(addprefix -I , $(INCLUDE_DIRS))
#======================================================================
# select which tools to use as compiler, librarian and linker
#======================================================================
AR = ar
ARFLAGS = rv
#======================================================================
# Enviroment
#======================================================================
MKDIR		:= mkdir -p
RM		:= rm -f
MV		:= mf -f
#======================================================================
# Common modules
include $(patsubst %, ./%/xmodule.mk, $(MODULES))
# Current program
include ./JsonParserC/xmodule.mk

# Unit test

# JSON
include ./unitTest/TEST_JSON/TEST_JSON_STRING/xmodule.mk
include ./unitTest/TEST_JSON/TEST_JSON_OBJECT/xmodule.mk
include ./unitTest/TEST_JSON/TEST_JSON_ARRAY/xmodule.mk
include ./unitTest/TEST_JSON/TEST_JSON_OBJECT_ARRAY/xmodule.mk
include ./unitTest/TEST_JSON/TEST_JSON_OBJECT_READ/xmodule.mk

#======================================================================
.PHONY: default build run all clean

all: $(OBJS) $(PROGRAMS) $(TESTS)

run: $(PROGRAMS)
	@$<
#======================================================================
# unitTest
test: $(OBJS) $(TMPS) $(TESTS)

# TEST JSON
TEST_JSON_STRING_EXE:
	@./unitTest/TEST_JSON/TEST_JSON_STRING/TEST_JSON_STRING_EXE
	
TEST_JSON_OBJECT_EXE:
	@./unitTest/TEST_JSON/TEST_JSON_OBJECT/TEST_JSON_OBJECT_EXE
	
TEST_JSON_ARRAY_EXE:
	@./unitTest/TEST_JSON/TEST_JSON_ARRAY/TEST_JSON_ARRAY_EXE

TEST_JSON_OBJECT_ARRAY_EXE:
	@./unitTest/TEST_JSON/TEST_JSON_OBJECT_ARRAY/TEST_JSON_OBJECT_ARRAY_EXE

TEST_JSON_OBJECT_READ_EXE:
	@./unitTest/TEST_JSON/TEST_JSON_OBJECT_READ/TEST_JSON_OBJECT_READ_EXE
# TEST MEM	
mem_program: $(PROGRAMS)
	valgrind --tool=memcheck --leak-check=full --undef-value-errors=no ./unitTest/TEST_JSON/TEST_JSON_OBJECT_READ/TEST_JSON_OBJECT_READ_EXE
	
mem_test: $(TEST_HANDLE_EVENT)
	valgrind --tool=memcheck --leak-check=full --undef-value-errors=no $(TEST_HANDLE_EVENT)	
clean:
	$(RM) $(OBJS) $(DEPS) $(PROGRAMS) $(TMPS) $(TESTS)

	
