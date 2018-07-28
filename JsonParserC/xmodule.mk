LOCAL_BASE	:= JsonParserC
LOCAL_MDLS	:= 
LOCAL_BLDS	:= $(addprefix $(LOCAL_BASE)/, $(LOCAL_MDLS))

LOCAL_INCS	:= $(wildcard $(LOCAL_BASE)/*.h)
LOCAL_INCS	+= $(wildcard $(INCLUDE_DIRS)/$(LOCAL_MDLS)/*.h)

LOCAL_SRCS	:= $(wildcard $(LOCAL_BASE)/*.c)
LOCAL_SRCS	+= $(foreach sdir, $(LOCAL_BLDS), $(wildcard $(sdir)/*.c))

LOCAL_OBJS	:= $(patsubst %.c, %.o, $(LOCAL_SRCS))
LOCAL_DEPS	:= $(patsubst %.c, %.deps, $(LOCAL_SRCS))

SRCS		+= $(LOCAL_SRCS)
TMPS		+= $(LOCAL_OBJS)
DEPS		+= $(LOCAL_DEPS)

TARGET		:= JsonParserC
PROGRAM		:= $(addprefix $(LOCAL_BASE)/, $(TARGET))
PROGRAMS		+= $(PROGRAM)

$(LOCAL_DEPS): $(LOCAL_SRCS) $(LOCAL_INCS)
	@$(CC) -MM $< > $@
	@$(CC) $< -MM -MT $@ >> $@

$(LOCAL_OBJS): $(LOCAL_SRCS) $(LOCAL_DEPS)

$(PROGRAM): $(LOCAL_OBJS) $(OBJS) $(LIBS)
	$(CC) $(LOCAL_OBJS) $(OBJS) $(LIBS) -o $@


