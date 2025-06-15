# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/01 16:19:15 by san               #+#    #+#              #
#    Updated: 2025/06/11 10:16:07 by ryoussfi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NC		=	\033[0m
BOLD	=	\033[1m
RESET	=	\033[0m
BLACK	=	\033[0;30m
RED		=	\033[1;31m
GREEN	=	\033[1;32m
YELLOW	=	\033[0;33m
BLUE	=	\033[1;34m
PURPLE	=	\033[1;35m
CYAN	=	\033[1;36m
WHITE	=	\033[0;37m
MARK		=	✗
TICK		=	✓
INFO		=	ⓘ

NAME		=	libft.a

VERBOSE		?=	1
ECHO = echo
PRINTF = printf
ECHO_MAIN = echo
PRINTF_MAIN = printf

ifeq ($(VERBOSE),2)
	ECHO = echo > /dev/null 2>&1
	PRINTF = printf > /dev/null 2>&1
else ifeq ($(VERBOSE),0)
	ECHO = echo > /dev/null 2>&1
	PRINTF = printf > /dev/null 2>&1
	ECHO_MAIN = echo > /dev/null 2>&1
	PRINTF_MAIN = printf > /dev/null 2>&1
endif

CC			=	gcc
CFLAGS		=	-Wall -Wextra -Werror
DFLAGS		=	-fPIE -MMD -MP
# CC		=	cc

HEAD_DIR	=	./includes/
SRCS_DIR	=	./sources/
BUILD_DIR	=	./build/

LIBC		=	ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c \
				ft_memmove.c ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
				ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c \
				ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_strncmp.c ft_isnumber.c ft_free_arr.c get_next_line.c ft_strcpy.c \
				ft_strjoin_arr.c ft_strcmp.c ft_isspace.c ft_strndup.c ft_strisspace.c ft_strncpy.c ft_strequ.c
				
LIBA		=		ft_bonus.lstadd_back.c ft_bonus.lstadd_front.c ft_bonus.lstclear.c \
					ft_bonus.lstdelone.c ft_bonus.lstiter.c ft_bonus.lstlast.c \
					ft_bonus.lstmap.c ft_bonus.lstnew.c ft_bonus.lstsize.c ry_safe_write.c


SOURCES		=	${LIBC} ${LIBA}
SRCS		=	$(addprefix ${SRCS_DIR}, ${SOURCES})
OBJS		=	$(patsubst ${SRCS_DIR}%.c, ${BUILD_DIR}%.o, ${SRCS})

SRC_NBR		=	$(shell expr $(words ${SRCS}))


all: clear_failures ${BUILD_DIR}
	$(call entry_message)
	@make -s ${OBJS}
	@if [ -s .failures ]; then \
		echo "\n${RED}Compilation failed for files:${NC}"; \
		cat .failures; \
		echo; \
		exit 1; \
	fi
	$(ECHO) "${BLUE}${INFO} Compilation successful, object files generated in ${BUILD_DIR} ${TICK}${NC}"
	@make -s ${NAME}

${NAME}:	${OBJS}
	$(ECHO) "${BOLD} ${INFO} ar -rsc ${NAME} object_file${NC}"
	ar -rsc ${NAME} ${OBJS};
	$(ECHO) "${BLUE}${INFO} Compilation completed, executable created ${TICK}${NC}"


${BUILD_DIR}:
	@if [ ! -d "${BUILD_DIR}" ]; \
	then mkdir -p $(sort $(dir ${OBJS})); \
	fi

${BUILD_DIR}%.o:	$(SRCS_DIR)%.c | ${BUILD_DIR}
	@$(PRINTF) "${RED}Compiling $(notdir $<) : ${MARK}${NC}\n"; \
	if ${CC} ${CFLAGS} ${DFLAGS} -I${HEAD_DIR} -c $< -o $@; \
	then \
		$(PRINTF) "\033[A\r${GREEN}Compiling $(notdir $<) : ${TICK}${NC}\n"; \
	else \
		$(ECHO) "-------------------------------------------------------------"; \
		echo "$(notdir $<)" >> .failures; \
	fi

clean:
	@if [ ! -d "${BUILD_DIR}" ]; \
	then \
		$(ECHO_MAIN) "${YELLOW}${INFO} ${NAME} : Repo .o already clean${NC}"; \
	else \
		$(ECHO_MAIN) "${RED}${INFO} ${NAME} : Removing all .o files${NC}"; \
		rm -r ${BUILD_DIR}; \
	fi

fclean:			clean clear_failures 
	@if [ ! -f "${NAME}" ]; \
	then \
		$(ECHO_MAIN) "${YELLOW}${INFO} ${NAME} : Excutables already clean${NC}"; \
	else \
		$(ECHO_MAIN) "${RED}${INFO} ${NAME} : Removing excutables files from root ${NC}"; \
		rm -f ${NAME}; \
	fi

re:				fclean all

define entry_message
	@$(ECHO_MAIN) "${CYAN}\nARCHIVE $$($(ECHO_MAIN) ${NAME} | tr '[:lower:]' '[:upper:]')\n${NC}${BOLD}Archive necessary .o files out of ${SRC_NBR} .c files in total${NC}"
endef

clear_failures:
	@rm -f .failures

-include $(OBJS:.o=.d)

.PHONY:			all clean fclean re entry_message clear_failures




# # **************************************************************************** #
# #                                                                              #
# #                                                         :::      ::::::::    #
# #    Makefile                                           :+:      :+:    :+:    #
# #                                                     +:+ +:+         +:+      #
# #    By: ddecilap <marvin@42.fr>                    +#+  +:+       +#+         #
# #                                                 +#+#+#+#+#+   +#+            #
# #    Created: 2024/11/25 20:47:34 by ddecilap          #+#    #+#              #
# #    Updated: 2024/11/25 20:52:24 by ddecilap         ###   ########.fr        #
# #                                                                              #
# # **************************************************************************** #

# .PHONY: all clean fclean re bonus

# NAME = libft.a
# CC = cc -D BUFFER_SIZE=42
# CFLAGS = -Wall -Wextra -Werror -I $(HEADER) -fPIE
# HEADER = .

# BONUS = ft_bonus.lstadd_back.c ft_bonus.lstadd_front.c ft_bonus.lstclear.c \
#         ft_bonus.lstdelone.c ft_bonus.lstiter.c ft_bonus.lstlast.c \
#         ft_bonus.lstmap.c ft_bonus.lstnew.c ft_bonus.lstsize.c

# SRCS = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c \
# ft_memmove.c ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
# ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c \
# ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_strncmp.c ft_isnumber.c ft_free_arr.c get_next_line.c ft_strcpy.c \
# ft_strjoin_arr.c ft_strcmp.c ft_isspace.c ft_strndup.c ft_strisspace.c \

# OBJ = $(SRCS:.c=.o)
# OBJB = $(BONUS:.c=.o)

# $(NAME): $(OBJ)
# 	ar rcs $(NAME) $(OBJ)

# %.o: %.c
# 	$(CC) $(CFLAGS) -o $@ -c $<

# all: $(NAME)

# bonus: $(OBJB) $(NAME)
# 	ar rcs -o $(NAME) $(OBJB)

# clean:
# 	rm -f $(OBJ) $(OBJB)

# fclean: clean
# 	rm -f $(NAME)

# re: fclean all
