# Compilador y flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -D_POSIX_C_SOURCE=200809L

# Rutas de includes
INCLUDES = -Iinclude/common \
           -Iinclude/diagnostic_manager \
           -Iinclude/enums \
           -Iinclude/parser \
           -Iinclude/semantic/type_system \
           -Iinclude/semantic/scope \
           -Iinclude/semantic/symbol_binding \
           -Iinclude/semantic/ast \
           -Iinclude/semantic/visitor \
           -Iinclude/semantic/visitor/print_visitor \
           -Iinclude/semantic/visitor/free_visitor \
           -Iinclude/semantic/visitor/type_inference_visitor \
           -Iinclude/semantic/visitor/constraint_collector_visitor \
           -Iinclude/semantic/visitor/type_check_visitor \
           -Ilib/collections/include \
           -Ilib/collections/include/common \
           -Ilib/collections/include/vector \
           -Ilib/collections/include/list \
           -Ibuild

# Flags para CUnit y flex
LIBS = -lcunit -lfl -lm

# Directorio de build
BUILD_DIR = build

# Fuentes del proyecto
PROJECT_SRC = src/common/hulk_common.c \
              src/diagnostic_manager/diagnostic_manager.c \
              src/enums/enums.c \
              src/semantic/type_system/type_descriptor.c \
              src/semantic/type_system/user_type_descriptor.c \
              src/semantic/type_system/type_table.c \
              src/semantic/type_system/global_tables.c \
              src/semantic/type_system/symbol_binding.c \
              src/semantic/scope/scope.c \
              src/semantic/scope/function_table.c \
              src/semantic/ast/ast.c \
              src/semantic/visitor/visitor.c \
              src/semantic/visitor/print_visitor/print_visitor.c \
              src/semantic/visitor/free_visitor/free_visitor.c \
              src/semantic/visitor/type_inference_visitor/type_inference_visitor.c \
              src/semantic/visitor/constraint_collector_visitor/type_constraint.c \
              src/semantic/visitor/constraint_collector_visitor/constraint_collector_visitor.c \
              src/semantic/visitor/type_check_visitor/type_check_visitor.c

# Fuentes de collections
COLLECTIONS_SRC = lib/collections/src/common/common.c \
                  lib/collections/src/vector/vector.c \
                  lib/collections/src/list/list.c

# Lexer y parser
LEXER_SRC = src/lexer/hulk_lexer.l
PARSER_SRC = src/parser/hulk_parser.y
LEXER_C = $(BUILD_DIR)/lex.yy.c
PARSER_C = $(BUILD_DIR)/parser.tab.c
PARSER_H = $(BUILD_DIR)/parser.tab.h

# Main
MAIN_SRC = src/main.c

# Objetos del proyecto
PROJECT_OBJ = $(patsubst %.c,$(BUILD_DIR)/%.o,$(PROJECT_SRC))
COLLECTIONS_OBJ = $(patsubst %.c,$(BUILD_DIR)/%.o,$(COLLECTIONS_SRC))
LEXER_OBJ = $(BUILD_DIR)/lex.yy.o
PARSER_OBJ = $(BUILD_DIR)/parser.tab.o
MAIN_OBJ = $(BUILD_DIR)/src/main.o

# Todos los objetos para el compilador
ALL_OBJS = $(PROJECT_OBJ) $(COLLECTIONS_OBJ) $(LEXER_OBJ) $(PARSER_OBJ) $(MAIN_OBJ)

# Objetos para tests (sin main)
TEST_OBJS = $(PROJECT_OBJ) $(COLLECTIONS_OBJ)

# --- Reglas ---

all: build

build: hulk

hulk: $(ALL_OBJS)
	$(CC) $(CFLAGS) $(ALL_OBJS) -o hulk $(LIBS)

# Lexer
$(LEXER_C): $(LEXER_SRC)
	@mkdir -p $(BUILD_DIR)
	flex -o $@ $<

# Parser
$(PARSER_C) $(PARSER_H): $(PARSER_SRC)
	@mkdir -p $(BUILD_DIR)
	bison -d -o $(PARSER_C) $<

# Objeto del lexer
$(LEXER_OBJ): $(LEXER_C) $(PARSER_H)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Objeto del parser
$(PARSER_OBJ): $(PARSER_C) $(PARSER_H)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Objeto de main
$(MAIN_OBJ): $(MAIN_SRC) $(PARSER_H)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Objetos del proyecto (dependen del header del parser)
$(BUILD_DIR)/%.o: %.c $(PARSER_H)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Test
test: $(TEST_OBJS)
	@if [ -z "$(TEST)" ]; then \
		echo "Uso: make test TEST=<ruta/al/test.c>"; \
		exit 1; \
	fi
	$(CC) $(CFLAGS) $(INCLUDES) $(TEST) $(TEST_OBJS) -o test_runner $(LIBS)
	./test_runner
	rm -f test_runner

test-memcheck: $(TEST_OBJS)
	@if [ -z "$(TEST)" ]; then \
		echo "Uso: make test-memcheck TEST=<ruta/al/test.c>"; \
		exit 1; \
	fi
	$(CC) $(CFLAGS) $(INCLUDES) $(TEST) $(TEST_OBJS) -o test_runner $(LIBS)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./test_runner
	rm -f test_runner

# Limpiar
clean:
	rm -rf $(BUILD_DIR)
	rm -f test_runner
	rm -f hulk

.PHONY: all build test test-memcheck clean