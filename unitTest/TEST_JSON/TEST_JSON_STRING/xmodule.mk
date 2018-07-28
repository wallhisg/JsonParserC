LOCAL_BASE	:= unitTest/TEST_JSON/TEST_JSON_STRING
LOCAL_MDLS	:=
LOCAL_BLDS	:= $(addprefix $(LOCAL_BASE)/, $(LOCAL_MDLS))

LOCAL_INCS	:= $(wildcard $(LOCAL_BASE)/*.h)
LOCAL_INCS	+= $(wildcard $(INCLUDE_DIRS)/$(LOCAL_MDLS)/*.h)

LOCAL_SRCS	:= $(wildcard $(LOCAL_BASE)/*.c)
LOCAL_SRCS	+= $(foreach sdir, $(LOCAL_BLDS), $(wildcard $(sdir)/*.c))

STRING_OBJS	:= $(patsubst %.c, %.o, $(LOCAL_SRCS))
LOCAL_DEPS	:= $(patsubst %.c, %.deps, $(LOCAL_SRCS))

TMPS		+= $(STRING_OBJS)
DEPS		+= $(LOCAL_DEPS)

TARGET		:= TEST_JSON_STRING_EXE
TEST_STRING	:= $(addprefix $(LOCAL_BASE)/, $(TARGET))
TESTS		+= $(TEST_STRING)

$(LOCAL_DEPS): $(LOCAL_SRCS) $(LOCAL_INCS)
	@$(CC) -MM $< > $@
	@$(CC) $< -MM -MT $@ >> $@

$(STRING_OBJS): $(LOCAL_SRCS) $(LOCAL_DEPS)

$(TEST_STRING): $(STRING_OBJS) $(OBJS) $(LIBS)
	$(CC) $(STRING_OBJS) $(OBJS) $(LIBS) -o $@

