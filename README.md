# Abdugram - Secure Messenger

Современный мессенджер для Windows с функциями Telegram и военным шифрованием (End-to-End Encryption).

## 🎯 Функции

- ✅ Текстовые сообщения в реальном времени
- ✅ Групповые чаты
- ✅ Отправка файлов и изображений
- ✅ End-to-End шифрование (E2E) - AES-256
- ✅ RSA 4096-bit для обмена ключами
- ✅ Аутентификация (JWT)
- ✅ История сообщений
- ✅ Уведомления о печатании
- ✅ Прочитанные/непрочитанные статусы
- ✅ Удаление сообщений
- ✅ Поиск сообщений

## 🛠 Технологический стек

**Frontend:**
- Electron (для Windows)
- React
- Socket.io (реал-тайм)
- TweetNaCl.js / libsodium (шифрование)

**Backend:**
- Node.js
- Express
- Socket.io
- SQLite
- jsonwebtoken (JWT)

**Криптография:**
- AES-256 для сообщений
- RSA 4096 для обмена ключами
- PBKDF2 для хеширования паролей

## 📦 Установка

```bash
# Backend
cd server
npm install

# Frontend
cd electron
npm install
```

## 🚀 Запуск

```bash
# Development mode
npm run dev

# Только backend
cd server && npm start

# Только frontend
cd electron && npm start
```

## 📁 Структура проекта

```
Abdugram/
├── server/           # Node.js backend
├── electron/         # Electron app (frontend)
├── client/           # React components
└── docs/             # Документация
```

## 🔐 Безопасность

- Все сообщения зашифрованы end-to-end
- Ключи хранятся локально (не передаются на сервер)
- HTTPS/TLS для всех соединений
- JWT для аутентификации
- Защита от CSRF и XSS атак

## 📝 Лицензия

MIT
