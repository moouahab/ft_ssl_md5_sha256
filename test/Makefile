CC      = gcc
CFLAGS  = -Wall -Wextra -Werror
INCLUDES = -I ../src/include -I utils

SRCS_COMMON =   ../src/module/args/check_args.c             \
                ../src/module/args/parse_args.c             \
                ../src/module/args/free_args.c              \
                ../src/module/serialize/serialize_bytes.c   \
                ../src/module/serialize/serialize.c         \
                ../src/module/md5/md5_init.c                \
                ../src/module/md5/md5_padding.c             \
                ../src/module/md5/md5_process.c             \
                ../src/module/md5/md5_transform.c           \
                ../src/module/md5/md5.c                     \
                ../src/module/input/input_from_string.c     \
                ../src/module/input/input_from_fd.c         \
                ../src/module/input/input_from_stdin.c      \
                ../src/module/input/free_input.c            \
                ../src/module/utils/ft_calloc.c             \
                ../src/module/utils/ft_realloc.c            \
                ../src/module/utils/ft_strcmp.c             \
                ../src/module/utils/ft_strlen.c             \
                ../src/module/utils/print.c                 \
                utils/test_utils.c

all: test_args test_serialize test_md5_init     \
     test_md5_transform test_md5_process        \
     test_md5_padding test_md5_final            \
     test_input test_display_hash

test_args:
	@echo "\n=== test_args ==="
	$(CC) $(CFLAGS) $(INCLUDES) \
		$(SRCS_COMMON) \
		args/test_parse_args.c \
		-o args/test_parse_args
	./args/test_parse_args

test_serialize:
	@echo "\n=== test_serialize ==="
	$(CC) $(CFLAGS) $(INCLUDES) \
		$(SRCS_COMMON) \
		serialize/test_serialize.c \
		-o serialize/test_serialize
	./serialize/test_serialize

test_md5_init:
	@echo "\n=== test_md5_init ==="
	$(CC) $(CFLAGS) $(INCLUDES) \
		$(SRCS_COMMON) \
		md5/test_md5_init.c \
		-o md5/test_md5_init
	./md5/test_md5_init

test_md5_transform:
	@echo "\n=== test_md5_transform ==="
	$(CC) $(CFLAGS) $(INCLUDES) \
		$(SRCS_COMMON) \
		md5/test_md5_transform.c \
		-o md5/test_md5_transform
	./md5/test_md5_transform

test_md5_process:
	@echo "\n=== test_md5_process ==="
	$(CC) $(CFLAGS) $(INCLUDES) \
		$(SRCS_COMMON) \
		md5/test_md5_process.c \
		-o md5/test_md5_process
	./md5/test_md5_process

test_md5_padding:
	@echo "\n=== test_md5_padding ==="
	$(CC) $(CFLAGS) $(INCLUDES) \
		$(SRCS_COMMON) \
		md5/test_md5_padding.c \
		-o md5/test_md5_padding
	./md5/test_md5_padding

test_md5_final:
	@echo "\n=== test_md5_final ==="
	$(CC) $(CFLAGS) $(INCLUDES) \
		$(SRCS_COMMON) \
		md5/test_md5_final.c \
		-o md5/test_md5_final
	./md5/test_md5_final

test_input:
	@echo "\n=== test_input ==="
	$(CC) $(CFLAGS) $(INCLUDES) \
		$(SRCS_COMMON) \
		input/test_input.c \
		-o input/test_input
	./input/test_input

test_display_hash:
	@echo "\n=== test_display_hash ==="
	$(CC) $(CFLAGS) $(INCLUDES) \
		$(SRCS_COMMON) \
		../src/module/algohash/display_hash.c \
		algohash/test_display_hash.c \
		-o algohash/test_display_hash
	./algohash/test_display_hash

clean:
	rm -f args/test_parse_args
	rm -f serialize/test_serialize
	rm -f md5/test_md5_init
	rm -f md5/test_md5_process
	rm -f md5/test_md5_transform
	rm -f md5/test_md5_padding
	rm -f md5/test_md5_final
	rm -f input/test_input
	rm -f algohash/test_display_hash

.PHONY: all clean                   \
    test_args test_serialize        \
    test_md5_init test_md5_transform \
    test_md5_process test_md5_padding \
    test_md5_final test_input       \
    test_display_hash