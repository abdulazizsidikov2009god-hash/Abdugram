@echo off
echo.
echo 🔨 Начало сборки Windows установщиков...
echo.

REM Проверка Node.js
node -v >nul 2>&1
if %errorlevel% neq 0 (
    echo ❌ Node.js не установлен!
    pause
    exit /b 1
)

echo ✅ Node.js найден: 
node -v
echo.

REM Переход в директорию electron
cd electron || exit /b 1

REM Установка зависимостей
echo 📦 Установка зависимостей...
call npm install

if %errorlevel% neq 0 (
    echo ❌ Ошибка при установке зависимостей
    pause
    exit /b 1
)

echo.
echo 🏗️ Сборка установщиков...
echo.

REM NSIS установщик (.exe)
echo 1️⃣ Создание NSIS установщика (.exe)...
call npx electron-builder --win nsis --publish never

if %errorlevel% equ 0 (
    echo ✅ NSIS установщик создан успешно!
) else (
    echo ❌ Ошибка при создании NSIS установщика
    pause
    exit /b 1
)

echo.

REM MSI установщик
echo 2️⃣ Создание MSI установщика...
call npx electron-builder --win msi --publish never

if %errorlevel% equ 0 (
    echo ✅ MSI установщик создан успешно!
) else (
    echo ❌ Ошибка при создании MSI установщика
    pause
    exit /b 1
)

echo.

REM Portable версия
echo 3️⃣ Создание Portable версии...
call npx electron-builder --win portable --publish never

if %errorlevel% equ 0 (
    echo ✅ Portable версия создана успешно!
) else (
    echo ❌ Ошибка при создании Portable версии
    pause
    exit /b 1
)

echo.
echo =====================================
echo ✨ Все установщики готовы!
echo 📂 Файлы находятся в: electron\dist\
echo =====================================
echo.
dir dist\*.exe dist\*.msi dist\*.zip 2>nul || echo Файлы не найдены
echo.
pause
