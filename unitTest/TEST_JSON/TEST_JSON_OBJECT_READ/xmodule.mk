LOCAL_BASE	:= unitTest/TEST_JSON/TEST_JSON_OBJECT_READ
LOCAL_MDLS	:=
LOCAL_BLDS	:= $(addprefix $(LOCAL_BASE)/, $(LOCAL_MDLS))

LOCAL_INCS	:= $(wildcard $(LOCAL_BASE)/*.h)
LOCAL_INCS	+= $(wildcard $(INCLUDE_DIRS)/$(LOCAL_MDLS)/*.h)

LOCAL_SRCS	:= $(wildcard $(LOCAL_BASE)/*.c)
LOCAL_SRCS	+= $(foreach sdir, $(LOCAL_BLDS), $(wildcard $(sdir)/*.c))

OBJ_READ	:= $(patsubst %.c, %.o, $(LOCAL_SRCS))
LOCAL_DEPS	:= $(patsubst %.c, %.deps, $(LOCAL_SRCS))

TMPS		+= $(OBJ_READ)
DEPS		+= $(LOCAL_DEPS)

TARGET		:= TEST_JSON_OBJECT_READ_EXE
TEST_OBJECT	:= $(addprefix $(LOCAL_BASE)/, $(TARGET))
TESTS		+= $(TEST_OBJECT)

$(LOCAL_DEPS): $(LOCAL_SRCS) $(LOCAL_INCS)
	@$(CC) -MM $< > $@
	@$(CC) $< -MM -MT $@ >> $@

$(OBJ_READ): $(LOCAL_SRCS) $(LOCAL_DEPS)

$(TEST_OBJECT): $(OBJ_READ) $(OBJS) $(LIBS)
	$(CC) $(OBJ_READ) $(OBJS) $(LIBS) -o $@

