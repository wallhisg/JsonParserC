LOCAL_BASE	:= unitTest/TEST_JSON/TEST_JSON_CODER_DECODER
LOCAL_MDLS	:=
LOCAL_BLDS	:= $(addprefix $(LOCAL_BASE)/, $(LOCAL_MDLS))

LOCAL_INCS	:= $(wildcard $(LOCAL_BASE)/*.h)
LOCAL_INCS	+= $(wildcard $(INCLUDE_DIRS)/$(LOCAL_MDLS)/*.h)

LOCAL_SRCS	:= $(wildcard $(LOCAL_BASE)/*.c)
LOCAL_SRCS	+= $(foreach sdir, $(LOCAL_BLDS), $(wildcard $(sdir)/*.c))

CODECODER_OBJS	:= $(patsubst %.c, %.o, $(LOCAL_SRCS))
LOCAL_DEPS	:= $(patsubst %.c, %.deps, $(LOCAL_SRCS))

TMPS		+= $(CODECODER_OBJS)
DEPS		+= $(LOCAL_DEPS)

TARGET		:= TEST_JSON_CODER_DECODER_EXE
TEST_CD		:= $(addprefix $(LOCAL_BASE)/, $(TARGET))
TESTS		+= $(TEST_CD)

$(LOCAL_DEPS): $(LOCAL_SRCS) $(LOCAL_INCS)
	@$(CC) -MM $< > $@
	@$(CC) $< -MM -MT $@ >> $@

$(CODECODER_OBJS): $(LOCAL_SRCS) $(LOCAL_DEPS)

$(TEST_CD): $(CODECODER_OBJS) $(OBJS) $(LIBS)
	$(CC) $(CODECODER_OBJS) $(OBJS) $(LIBS) -o $@

