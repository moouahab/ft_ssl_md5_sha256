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

SRCS    =   main.c                                      \
            src/module/args/check_args.c                \
            src/module/args/parse_args.c                \
            src/module/args/free_args.c                 \
            src/module/serialize/serialize_bytes.c      \
            src/module/serialize/serialize.c            \
            src/module/md5/md5_init.c                   \
            src/module/md5/md5_padding.c                \
            src/module/md5/md5_process.c                \
            src/module/md5/md5_transform.c              \
            src/module/md5/md5.c                        \
            src/module/sha256/sha256_init.c             \
            src/module/sha256/sha256_transform.c        \
            src/module/sha256/sha256_process.c          \
            src/module/sha256/sha256_padding.c          \
            src/module/sha256/sha256.c                  \
            src/module/sha512/sha512_init.c             \
            src/module/sha512/sha512_transform.c        \
            src/module/sha512/sha512_process.c          \
            src/module/sha512/sha512_padding.c          \
            src/module/sha512/sha512.c                  \
            src/module/whirlpool/whirlpool_init.c          \
            src/module/whirlpool/whirlpool_transform.c     \
            src/module/whirlpool/whirlpool_process.c       \
            src/module/whirlpool/whirlpool_padding.c       \
            src/module/whirlpool/whirlpool.c               \
            src/module/algohash/algo_whirlpool.c           \
            src/module/algohash/algo_sha512.c           \
            src/module/input/input_from_string.c        \
            src/module/input/input_from_fd.c            \
            src/module/input/input_from_stdin.c         \
            src/module/input/free_input.c               \
            src/module/algohash/algo_md5.c              \
            src/module/algohash/algo_sha256.c           \
            src/module/algohash/dispatch.c              \
            src/module/algohash/run_hash.c              \
            src/module/algohash/display_hash.c          \
            src/module/utils/ft_calloc.c                \
            src/module/utils/ft_realloc.c               \
            src/module/utils/ft_strcmp.c                \
            src/module/utils/ft_strlen.c                \
            src/module/utils/print.c

OBJS_DIR = objs
OBJS     = $(SRCS:%.c=$(OBJS_DIR)/%.o)

all:    $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

test:
	$(MAKE) -C test

clean:
	rm -rf $(OBJS_DIR)
	$(MAKE) -C test clean

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re test