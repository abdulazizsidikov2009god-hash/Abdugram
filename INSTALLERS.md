# 📦 Инструкция по созданию установщиков Abdugram

## 🎯 Три типа установщиков

### 1️⃣ **NSIS Installer (.exe)** - Классический установщик
- ✅ Поддерживает 32-bit и 64-bit Windows
- ✅ Создаёт ярлыки на рабочий стол и в меню "Пуск"
- ✅ Возможность выбора папки установки
- ✅ Встроенное удаление приложения
- 📦 Размер: ~180 MB
- 🎯 Рекомендуется для большинства пользователей

### 2️⃣ **MSI Installer** - Windows Installer
- ✅ Интеграция с Windows (добавление/удаление программ)
- ✅ Поддержка групповых политик
- ✅ Возможность автоматизации установки
- 📦 Размер: ~170 MB
- 🎯 Для корпоративного использования

### 3️⃣ **Portable Version (.exe)** - Портативная версия
- ✅ Не требует установки
- ✅ Можно запустить с флешки
- ✅ Не требует прав администратора
- 📦 Размер: ~160 MB
- 🎯 Для пользователей, которые хотят запустить сразу

---

## 🚀 Как собрать установщики?

### ✅ Требования:
- Node.js (v14+)
- npm или yarn
- 500 MB свободного места на диске

### 📝 Шаг 1: Установка зависимостей

```bash
npm install
```

### 📝 Шаг 2: Сборка всех установщиков

#### На Windows (PowerShell или CMD):
```batch
scripts\build-windows.bat
```

#### На macOS/Linux:
```bash
chmod +x scripts/build-windows.sh
./scripts/build-windows.sh
```

#### Или вручную через npm:
```bash
npm run build
```

### 📝 Шаг 3: Сборка одного установщика

```bash
# Только .exe (NSIS)
npm run build:exe

# Только .msi (MSI)
npm run build:msi

# Только портативная версия
npm run build:portable
```

---

## 📂 Где находятся готовые файлы?

После сборки файлы будут в папке:
```
electron/dist/
```

Вы найдёте:
- `Abdugram-Setup-1.0.0.exe` - NSIS установщик
- `Abdugram-1.0.0.msi` - MSI установщик  
- `Abdugram-1.0.0-portable.exe` - Портативная версия

---

## 🔧 Конфигурация (electron-builder.json)

### NSIS опции:
- `oneClick: false` - Пользователь может выбрать папку
- `allowToChangeInstallationDirectory: true` - Разрешить выбор папки
- `createDesktopShortcut: true` - Создать ярлык на рабочем столе
- `createStartMenuShortcut: true` - Создать ярлык в меню Пуск

### Иконки (опционально):
- `assets/icon.ico` - Основная иконка приложения
- `assets/installer-sidebar.bmp` - Картинка слева в установщике (164x314)

---

## ✨ Загрузить установщики на GitHub Releases

### Через GitHub CLI:
```bash
gh release create v1.0.0 \
  electron/dist/Abdugram-Setup-1.0.0.exe \
  electron/dist/Abdugram-1.0.0.msi \
  electron/dist/Abdugram-1.0.0-portable.exe \
  --title "Abdugram v1.0.0" \
  --notes "Первый релиз"
```

### Вручную на GitHub:
1. Перейти на: https://github.com/abdulazizsidikov2009god-hash/Abdugram/releases
2. Нажать "Create a new release"
3. Заполнить:
   - Tag: `v1.0.0`
   - Title: `Abdugram v1.0.0`
   - Description: Описание версии
4. Загрузить 3 файла из `electron/dist/`
5. Нажать "Publish release"

---

## 🐛 Возможные проблемы

### ❌ "electron-builder не найден"
```bash
npm install -g electron-builder
```

### ❌ "Ошибка при создании .exe"
- Проверьте, что все зависимости установлены: `npm install`
- Очистите кэш: `npm cache clean --force`
- Переустановите electron: `npm install electron --save-dev`

### ❌ "Недостаточно места на диске"
- Нужно 500 MB свободного места
- Очистите временные файлы: `npm cache clean --force`

---

## 📊 Статистика установщиков

| Установщик | Формат | Размер | Архитектура | Использование |
|------------|--------|--------|-------------|---------------|
| NSIS | .exe | ~180 MB | 32/64-bit | Домашние пользователи |
| MSI | .msi | ~170 MB | 32/64-bit | Корпоративные сети |
| Portable | .exe | ~160 MB | 32/64-bit | Флешка, портативность |

---

## 💾 Автоматическое обновление (опционально)

Для добавления функции автоматического обновления:

```bash
npm install electron-updater
```

Тогда в `electron-builder.json` добавьте:
```json
"publish": {
  "provider": "github",
  "owner": "abdulazizsidikov2009god-hash",
  "repo": "Abdugram"
}
```

---

✅ **Готово!** Теперь у вас есть 3 рабочих установщика для Windows!
