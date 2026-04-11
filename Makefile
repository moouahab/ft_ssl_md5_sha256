NAME    = ft_ssl

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror

# Memory alignment
CFLAGS  += -malign-double
CFLAGS  += -mpreferred-stack-boundary=4

# Attack protection
CFLAGS  += -fstack-protector-strong
CFLAGS  += -D_FORTIFY_SOURCE=2
CFLAGS  += -fPIE
CFLAGS  += -Wl,-z,relro
CFLAGS  += -Wl,-z,now
CFLAGS  += -Wl,-z,noexecstack

INCLUDES = -I src/include

SRCS    =   main.c                              \
            src/module/args/check_args.c        \
            src/module/args/parse_args.c        \
            src/module/args/free_args.c         \
            src/module/serialize/serialize_bytes.c \
            src/module/serialize/serialize.c    \
            src/module/md5/md5_init.c           \
            src/module/md5/md5_padding.c        \
            src/module/md5/md5_process.c        \
            src/module/md5/md5_final.c          \
            src/module/utils/ft_calloc.c        \
            src/module/utils/ft_realloc.c       \
            src/module/utils/ft_strcmp.c        \
            src/module/utils/ft_strlen.c        \
            src/module/utils/print.c

OBJS_DIR = objs
OBJS     = $(SRCS:%.c=$(OBJS_DIR)/%.o)

all:    $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re