LOCAL_BASE	:= unitTest/TEST_JSON/TEST_JSON_OBJECT_ARRAY
LOCAL_MDLS	:=
LOCAL_BLDS	:= $(addprefix $(LOCAL_BASE)/, $(LOCAL_MDLS))

LOCAL_INCS	:= $(wildcard $(LOCAL_BASE)/*.h)
LOCAL_INCS	+= $(wildcard $(INCLUDE_DIRS)/$(LOCAL_MDLS)/*.h)

LOCAL_SRCS	:= $(wildcard $(LOCAL_BASE)/*.c)
LOCAL_SRCS	+= $(foreach sdir, $(LOCAL_BLDS), $(wildcard $(sdir)/*.c))

OA_OBJS	:= $(patsubst %.c, %.o, $(LOCAL_SRCS))
LOCAL_DEPS	:= $(patsubst %.c, %.deps, $(LOCAL_SRCS))

TMPS		+= $(OA_OBJS)
DEPS		+= $(LOCAL_DEPS)

TARGET		:= TEST_JSON_OBJECT_ARRAY_EXE
TEST_OA		:= $(addprefix $(LOCAL_BASE)/, $(TARGET))
TESTS		+= $(TEST_OA)

$(LOCAL_DEPS): $(LOCAL_SRCS) $(LOCAL_INCS)
	@$(CC) -MM $< > $@
	@$(CC) $< -MM -MT $@ >> $@

$(OA_OBJS): $(LOCAL_SRCS) $(LOCAL_DEPS)

$(TEST_OA): $(OA_OBJS) $(OBJS) $(LIBS)
	$(CC) $(OA_OBJS) $(OBJS) $(LIBS) -o $@

