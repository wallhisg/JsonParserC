LOCAL_BASE	:= unitTest/TEST_JSON/TEST_JSON_FUNCTIONS
LOCAL_MDLS	:=
LOCAL_BLDS	:= $(addprefix $(LOCAL_BASE)/, $(LOCAL_MDLS))

LOCAL_INCS	:= $(wildcard $(LOCAL_BASE)/*.h)
LOCAL_INCS	+= $(wildcard $(INCLUDE_DIRS)/$(LOCAL_MDLS)/*.h)

LOCAL_SRCS	:= $(wildcard $(LOCAL_BASE)/*.c)
LOCAL_SRCS	+= $(foreach sdir, $(LOCAL_BLDS), $(wildcard $(sdir)/*.c))

OBJ_FUNC	:= $(patsubst %.c, %.o, $(LOCAL_SRCS))
LOCAL_DEPS	:= $(patsubst %.c, %.deps, $(LOCAL_SRCS))

TMPS		+= $(OBJ_FUNC)
DEPS		+= $(LOCAL_DEPS)

TARGET		:= TEST_JSON_FUNCTIONS_EXE
TEST_FUNC	:= $(addprefix $(LOCAL_BASE)/, $(TARGET))
TESTS		+= $(TEST_FUNC)

$(LOCAL_DEPS): $(LOCAL_SRCS) $(LOCAL_INCS)
	@$(CC) -MM $< > $@
	@$(CC) $< -MM -MT $@ >> $@

$(OBJ_FUNC): $(LOCAL_SRCS) $(LOCAL_DEPS)

$(TEST_FUNC): $(OBJ_FUNC) $(OBJS) $(LIBS)
	$(CC) $(OBJ_FUNC) $(OBJS) $(LIBS) -o $@

