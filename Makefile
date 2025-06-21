# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ryoussfi <ryoussfi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/27 20:46:26 by ryoussfi          #+#    #+#              #
#    Updated: 2025/06/21 18:49:29 by ryoussfi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

### MAKEFILE RIDOUANE	###
GREEN		=	\033[1;32m
CYAN		=	\033[1;36m
YELLOW_B	=	\033[1;33m
YELLOW		=	\033[0;33m
BROWN		=	\033[1;38;5;94m
RED			=	\033[1;31m
BLUE		=	\033[1;34m
NC			=	\033[0m
BOLD		=	\033[1m
PINK		=	\033[1;38;5;205m
TICK		=	✓
INFO		=	ⓘ
MARK		=	✗

NAME		=	minishell

LIBFT		=	libft.a

UNAME_S := $(shell uname -s)

# CC			=	cc
CC			=	gcc -g

DFLAGS		=	-MMD -MP
UNAME_S		=	${shell uname -s}

ifeq ($(UNAME_S),Linux)
	CFLAGS  = -Wall -Wextra -Werror
	LDFLAGS = -lreadline
else ifeq ($(UNAME_S),Darwin)
	ifeq ($(shell test -d /opt/homebrew && echo yes),yes)
		CFLAGS  = -I/opt/homebrew/opt/readline/include -Wextra -Wall -Werror
		LDFLAGS = -L/opt/homebrew/opt/readline/lib -lreadline
	else
		# Sinon on suppose une installation classique (Intel mac)
		CFLAGS  = -I/usr/local/opt/readline/include -Wextra -Wall -Werror
		LDFLAGS = -L/usr/local/opt/readline/lib -lreadline
	endif
endif

LIBFT_DIR	=	./libft/
HEADERS		=	./includes/
SRCS_DIR	=	./srcs/
BUILD_DIR	=	./build/

SRCS		=	${SRCS_DIR}minishell.c \
				${SRCS_DIR}brain/brain.c \
				${SRCS_DIR}brain/tool_loop_heredoc.c \
				${SRCS_DIR}here_doc/here_doc.c \
				${SRCS_DIR}here_doc/go_heredoc.c \
				${SRCS_DIR}here_doc/here_doc_init.c \
				${SRCS_DIR}here_doc/here_doc_utils.c \
				$(SRCS_DIR)lexer/lexer_extract_word.c \
				$(SRCS_DIR)lexer/lexer_redir.c \
				$(SRCS_DIR)lexer/lexer_utils.c \
				$(SRCS_DIR)lexer/lexer_word.c \
				$(SRCS_DIR)lexer/lexer.c \
				${SRCS_DIR}quote/quote_extension.c \
				${SRCS_DIR}quote/quote_handlers.c \
				${SRCS_DIR}quote/quote_utils.c \
				${SRCS_DIR}parser/group_commands.c \
				${SRCS_DIR}parser/parse_error.c \
				${SRCS_DIR}parser/parse_input.c \
				${SRCS_DIR}parser/parse_syntax.c \
				${SRCS_DIR}parser/parse_tokens.c \
				${SRCS_DIR}parser/parse_tokens2.c \
				${SRCS_DIR}parser/parse_utils.c \
				${SRCS_DIR}parser/parser_assignment.c \
				${SRCS_DIR}parser/parser.c \
				${SRCS_DIR}wildcard/wildcard_expand.c \
				${SRCS_DIR}wildcard/wildcard_utils.c \
				${SRCS_DIR}wildcard/wildcard.c \
				$(SRCS_DIR)expansion/expand_dollar.c \
				$(SRCS_DIR)expansion/expander_quotes.c \
				$(SRCS_DIR)expansion/expander_utils.c \
				$(SRCS_DIR)expansion/expander_var.c \
				$(SRCS_DIR)expansion/expander.c \
				$(SRCS_DIR)execution/executor_child.c \
				$(SRCS_DIR)execution/expand_arg_cleanup.c \
				$(SRCS_DIR)execution/expand_single_arg.c \
				${SRCS_DIR}execution/handle_redirection.c \
				$(SRCS_DIR)execution/logic_executor.c \
				$(SRCS_DIR)execution/pipeline_child.c \
				$(SRCS_DIR)execution/pipeline_executor.c \
				$(SRCS_DIR)execution/pipeline_helpers.c \
				$(SRCS_DIR)execution/pipeline_redir.c \
				$(SRCS_DIR)execution/pipeline_step.c \
				$(SRCS_DIR)subshell/subshell.c \
				$(SRCS_DIR)subshell/subshell_check.c \
				$(SRCS_DIR)subshell/subshell_exec.c \
				$(SRCS_DIR)subshell/subshell_error.c \
				$(SRCS_DIR)subshell/subshell_utils.c \
				$(SRCS_DIR)subshell/subshell_utils2.c \
				$(SRCS_DIR)history/history_init.c \
				$(SRCS_DIR)history/history_save.c \
				$(SRCS_DIR)history/history_tool.c \
				$(SRCS_DIR)history/history_tool2.c \
				$(SRCS_DIR)history/history_utils.c \
				${SRCS_DIR}utils/error_utils.c \
				${SRCS_DIR}utils/string_utils.c \
				${SRCS_DIR}utils/init_minishell.c \
				${SRCS_DIR}utils/free_utils.c \
				${SRCS_DIR}utils/ry_ft_split_new_line.c \
				${SRCS_DIR}utils/ry_utils.c \
				${SRCS_DIR}token_utils/token_utils.c \
				${SRCS_DIR}token_utils/string_utils.c \
				${SRCS_DIR}token_utils/token_utils_errors.c \
				$(SRCS_DIR)builtins/builtin.c \
				$(SRCS_DIR)builtins/builtin_utils.c \
				$(SRCS_DIR)builtins/builtin_echo_cd.c \
				$(SRCS_DIR)builtins/builtin_exit.c \
				$(SRCS_DIR)builtins/builtin_unset.c \
				$(SRCS_DIR)builtins/builtin_export.c \
				$(SRCS_DIR)builtins/builtin_env.c \
				$(SRCS_DIR)builtins/builtin_pwd.c \
				${SRCS_DIR}env/cmd_path.c \
				${SRCS_DIR}env/env_setters.c \
				${SRCS_DIR}env/env_utils.c \
				${SRCS_DIR}env/env_local.c \
				${SRCS_DIR}env/env_local_utils.c \
				${SRCS_DIR}signals/signals.c


OBJS	=	$(patsubst ${SRCS_DIR}%.c, ${BUILD_DIR}%.o, ${SRCS})

SRC_NBR	=	$(shell expr $(words ${SRCS}))

SRCS_C = $(shell find $(SRCS_DIR) -type f -name "*.c")
SRC_NBR_C = $(shell expr $(words $(SRCS_C)))

HEADERS_FILE = $(shell find ${HEADERS} -type f)
INC_NBR	=	$(shell expr $(words ${HEADERS_FILE}))

HEADERS_FILE_H = $(shell find ${HEADERS} -type f -name "*.h")
INC_NBR_H	=	$(shell expr $(words ${HEADERS_FILE_H}))



all:	clear_failures ${BUILD_DIR}  ${HEADERS}
	@make -s ${LIBFT} VERBOSE=2
	$(call entry_message)
	@make -s ${OBJS}
	@if [ -s .failures ]; then \
		echo "\n${RED}Compilation failed for files:${NC}"; \
		cat .failures; \
		echo; \
		exit 1; \
	fi
	@echo "${BLUE}${INFO} Compilation successful, object files generated in ${BUILD_DIR} ${TICK}${NC}"
	@make -s ${NAME}


${NAME}:	${OBJS}
	@echo "${BOLD} ${INFO}	${CC} ${CFLAGS} ${DFLAGS} object_file -I${HEADERS} ${LIBFT} -o ${NAME} $(LDFLAGS)${NC}"
	${CC} ${CFLAGS} ${DFLAGS} ${OBJS} -I${HEADERS} ${LIBFT} -o ${NAME} $(LDFLAGS)
	@echo "${BLUE}${INFO} Compilation completed, executable created ${TICK}${NC}"


${LIBFT}:
	make -C ${LIBFT_DIR}
	@echo "${BROWN}Moving ${LIBFT} at the root of the repository${NC}"
	@mv ${LIBFT_DIR}${LIBFT} .


${BUILD_DIR}:
	@if [ ! -d "${BUILD_DIR}" ]; \
	then mkdir -p $(sort $(dir ${OBJS})); \
	fi

$(BUILD_DIR)%.o:	$(SRCS_DIR)%.c  | ${BUILD_DIR}
	@printf "${RED}Compiling $(notdir $<) : ${MARK}${NC}\n"; \
	if ${CC} ${CFLAGS} ${DFLAGS} -I${HEADERS} -c $< -o $@; \
	then \
		printf "\033[A\r${GREEN}Compiling $(notdir $<) : ${TICK}${NC}\n"; \
	else \
		echo "-------------------------------------------------------------"; \
		echo "$(notdir $<)" >> .failures; \
	fi


clean:
	@if [ ! -d "${BUILD_DIR}" ]; \
	then \
		echo "${YELLOW_B}${INFO} ${NAME} : Repo .o already clean${NC}"; \
	else \
		echo "${RED}${INFO} ${NAME} : Removing all .o files${NC}"; \
		rm -r ${BUILD_DIR}; \
	fi

fclean:			clean clear_failures clear_norm_failures
	@if [ ! -f "${NAME}" ] && [ ! -f "${LIBFT}" ] ; \
	then \
		echo "${YELLOW_B}${INFO} ${NAME} : Excutables already clean${NC}"; \
	else \
		echo "${RED}${INFO} ${NAME} : Removing excutables files from root ${NC}"; \
		rm -f ${NAME} ${LIBFT}; \
	fi
	@make -sC ${LIBFT_DIR} clean >/dev/null 2>&1 || true

re:	fclean all

norminette:
	@rm -f .norm_failures .norm .norm_failures_h .norm_h
	@echo "🧹 ${BLUE}Checking norminette file...${NC}"
	@for file in $(SRCS); do \
		ERRORS=$$(norminette -R CheckForbiddenSourceHeader $$file | cat -n); \
		if echo "$$ERRORS" | grep -q "Error"; then \
			echo "$$file" >> .norm_failures; \
			echo "$$ERRORS\n" >> .norm; \
		fi; \
	done
	@if [ -s .norm_failures ]; then \
		ERROR_COUNT=$$(wc -l < .norm_failures); \
		echo "\n${RED}TEST .C : Norminette failed for files .c ($$ERROR_COUNT errors / ${SRC_NBR}):${PINK}"; \
		cat .norm_failures; \
		echo "${NC}"; \
	else \
		echo "\n${GREEN}TEST .C : Norminette passed successfully for all files! (${SRC_NBR} files .c checked)${NC}"; \
	fi
	@for file in $(HEADERS_file); do \
		ERRORS=$$(norminette -R CheckForbiddenSourceHeader $$file | cat -n); \
		if echo "$$ERRORS" | grep -q "Error"; then \
			echo "$$file" >> .norm_failures_h; \
			echo "$$ERRORS\n" >> .norm_h; \
		fi; \
	done
	@if [ -s .norm_failures_h ]; then \
		ERROR_COUNT_INC=$$(wc -l < .norm_failures_h); \
		echo "\n${RED}TEST .H : Norminette failed for files .h ($$ERROR_COUNT_INC errors / ${INC_NBR}):${PINK}"; \
		cat .norm_failures_h; \
		echo "${NC}"; \
	else \
		echo "\n${GREEN}TEST .H : Norminette passed successfully for all files! (${INC_NBR} files .h checked)${NC}"; \
	fi
	@if [ "$(SRC_NBR_C)" -ne "$(SRC_NBR)" ]; then \
		echo "\n${RED}Error: FILE.C ($(SRC_NBR_C)) is not equal to $$(echo ${SRCS_DIR} | tr '[:lower:]' '[:upper:]')FILE ($(SRC_NBR))${NC}"; \
	else \
		echo "\n${GREEN}FILE.C = ($(SRC_NBR_C)) is equal to $$(echo ${SRCS_DIR} | tr '[:lower:]' '[:upper:]')FILE = ($(SRC_NBR))${NC}"; \
	fi
	@if [ "$(INC_NBR_H)" -ne "$(INC_NBR)" ]; then \
		echo "\n${RED}Error: FILE.H ($(INC_NBR_H)) is not equal to $$(echo ${HEADERS} | tr '[:lower:]' '[:upper:]')FILE ($(INC_NBR))${NC}"; \
	else \
		echo "\n${GREEN}FILE.H = ($(INC_NBR_H)) is equal to $$(echo ${HEADERS} | tr '[:lower:]' '[:upper:]')FILE = ($(INC_NBR))${NC}"; \
	fi

define entry_message
	@echo "${CYAN}\nCOMPILING $$(echo ${NAME} | tr '[:lower:]' '[:upper:]')\n${NC}${BOLD}Compiling necessary .o files out of ${SRC_NBR} .c files in total${NC}"
endef

clear_failures:
	@rm -f .failures

clear_norm_failures:
	@rm -f .norm_failures .norm .norm_failures_h .norm_h

-include $(OBJS:.o=.d)

.PHONY:	all clean fclean re entry_message clear_failures clear_norm_failures norminette
