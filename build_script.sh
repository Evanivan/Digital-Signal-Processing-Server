#!/bin/bash

# Универсальный скрипт сборки проекта
BDIR="build"
BUILD_TYPE="Release"
ENABLE_TESTS="OFF"

# Парсинг аргументов
while [[ $# -gt 0 ]]; do
    case $1 in
        --debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        --tests)
            ENABLE_TESTS="ON"
            shift
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

# Очистка предыдущей сборки
if [ -d "$BDIR" ]; then
    echo "Removing old build directory..."
    rm -rf "$BDIR"
fi

# Создание и переход в директорию
mkdir -p "$BDIR" && cd "$BDIR" || exit 1

# Генерация проекта
echo "Generating project with options:"
echo "   - Build type: $BUILD_TYPE"
echo "   - Tests: $ENABLE_TESTS"

cmake -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
      -DBUILD_TESTING="$ENABLE_TESTS" \
      ..

# Сборка
echo "Building project..."
cmake --build . --config "$BUILD_TYPE"

# Запуск тестов если включены
if [ "$ENABLE_TESTS" = "ON" ]; then
    echo "Running tests..."
    ctest --output-on-failure -C "$BUILD_TYPE"
fi

echo "Build completed successfully!"
