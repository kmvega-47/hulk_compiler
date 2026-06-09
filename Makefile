# Compilador y flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

# Rutas de includes
INCLUDES = -Iinclude/common \
           -Iinclude/diagnostic_manager \
           -Iinclude/enums \
           -Iinclude/semantic/type_system \
           -Iinclude/semantic/scope \
           -Ilib/collections/include \
           -Ilib/collections/include/common \
           -Ilib/collections/include/vector \
           -Ilib/collections/include/list

# Flags para CUnit
CUNIT_FLAGS = -lcunit

# Directorios de fuentes del proyecto
PROJECT_SRC_DIRS = src/common \
                   src/diagnostic_manager \
                   src/enums \
                   src/semantic/type_system \
                   src/semantic/scope

# Directorios de fuentes de collections
COLLECTIONS_SRC_DIRS = lib/collections/src/common \
                       lib/collections/src/vector \
                       lib/collections/src/list

# Buscar todos los .c en los directorios
PROJECT_SRC = $(foreach dir,$(PROJECT_SRC_DIRS),$(wildcard $(dir)/*.c))
COLLECTIONS_SRC = $(foreach dir,$(COLLECTIONS_SRC_DIRS),$(wildcard $(dir)/*.c))

# Directorio de build para objetos
BUILD_DIR = build

# Crear directorios de build reflejando la estructura
PROJECT_BUILD_OBJ = $(patsubst %.c,$(BUILD_DIR)/%.o,$(PROJECT_SRC))
COLLECTIONS_BUILD_OBJ = $(patsubst %.c,$(BUILD_DIR)/%.o,$(COLLECTIONS_SRC))

# Compilar todos los .c (sin linkear)
all: $(PROJECT_BUILD_OBJ) $(COLLECTIONS_BUILD_OBJ)

# Regla build: compila el ejecutable ./hulk
build: $(PROJECT_BUILD_OBJ) $(COLLECTIONS_BUILD_OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) src/main.c $^ -o hulk

# Regla para compilar tests
test: $(PROJECT_BUILD_OBJ) $(COLLECTIONS_BUILD_OBJ)
	@if [ -z "$(TEST)" ]; then \
		echo "Uso: make test TEST=<ruta/al/test.c>"; \
		exit 1; \
	fi
	$(CC) $(CFLAGS) $(INCLUDES) $(TEST) $^ -o test_runner $(CUNIT_FLAGS)
	./test_runner
	rm -f test_runner

test-memcheck: $(PROJECT_BUILD_OBJ) $(COLLECTIONS_BUILD_OBJ)
	@if [ -z "$(TEST)" ]; then \
		echo "Uso: make test-memcheck TEST=<ruta/al/test.c>"; \
		exit 1; \
	fi
	$(CC) $(CFLAGS) $(INCLUDES) $(TEST) $^ -o test_runner $(CUNIT_FLAGS)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./test_runner
	rm -f test_runner

# Compilar objetos del proyecto
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Limpiar
clean:
	rm -rf $(BUILD_DIR)
	rm -f test_runner
	rm -f hulk

.PHONY: all build test test-memcheck clean