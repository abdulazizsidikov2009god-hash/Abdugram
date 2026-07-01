#!/bin/bash

echo "🔨 Начало сборки Windows установщиков..."
echo ""

# Проверка Node.js
if ! command -v node &> /dev/null; then
    echo "❌ Node.js не установлен!"
    exit 1
fi

echo "✅ Node.js найден: $(node -v)"
echo ""

# Переход в директорию electron
cd electron || exit 1

# Установка зависимостей
echo "📦 Установка зависимостей..."
npm install

if [ $? -ne 0 ]; then
    echo "❌ Ошибка при установке зависимостей"
    exit 1
fi

echo ""
echo "🏗️ Сборка установщиков..."
echo ""

# NSIS установщик (.exe)
echo "1️⃣ Создание NSIS установщика (.exe)..."
electron-builder --win nsis --publish never

if [ $? -eq 0 ]; then
    echo "✅ NSIS установщик создан успешно!"
else
    echo "❌ Ошибка при создании NSIS установщика"
    exit 1
fi

echo ""

# MSI установщик
echo "2️⃣ Создание MSI установщика..."
electron-builder --win msi --publish never

if [ $? -eq 0 ]; then
    echo "✅ MSI установщик создан успешно!"
else
    echo "❌ Ошибка при создании MSI установщика"
    exit 1
fi

echo ""

# Portable версия
echo "3️⃣ Создание Portable версии..."
electron-builder --win portable --publish never

if [ $? -eq 0 ]; then
    echo "✅ Portable версия создана успешно!"
else
    echo "❌ Ошибка при создании Portable версии"
    exit 1
fi

echo ""
echo "=====================================" 
echo "✨ Все установщики готовы!"
echo "📂 Файлы находятся в: electron/dist/"
echo "====================================="
echo ""
ls -lh dist/*.exe dist/*.msi dist/*.zip 2>/dev/null || echo "Файлы не найдены"
