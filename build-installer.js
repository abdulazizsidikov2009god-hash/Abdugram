#!/usr/bin/env node

const { execSync } = require('child_process');
const path = require('path');
const fs = require('fs');

console.log('🔨 Сборка установщиков для Windows...');
console.log('=====================================\n');

const electronDir = path.join(__dirname, 'electron');
process.chdir(electronDir);

const installers = [
  { name: 'NSIS (.exe)', cmd: 'electron-builder --win nsis --publish never', icon: '📦' },
  { name: 'MSI Installer', cmd: 'electron-builder --win msi --publish never', icon: '💿' },
  { name: 'Portable (ZIP)', cmd: 'electron-builder --win portable --publish never', icon: '📁' }
];

for (const installer of installers) {
  try {
    console.log(`${installer.icon} Создание ${installer.name}...`);
    execSync(installer.cmd, { stdio: 'inherit' });
    console.log(`✅ ${installer.name} успешно создан!\n`);
  } catch (error) {
    console.error(`❌ Ошибка при создании ${installer.name}:`, error.message);
    process.exit(1);
  }
}

console.log('=====================================');
console.log('✨ Все установщики готовы!');
console.log('📂 Файлы находятся в папке: electron/dist/');
console.log('\nСписок файлов:');

const distDir = path.join(electronDir, 'dist');
if (fs.existsSync(distDir)) {
  const files = fs.readdirSync(distDir);
  files.forEach(file => {
    const filePath = path.join(distDir, file);
    const stats = fs.statSync(filePath);
    const size = (stats.size / 1024 / 1024).toFixed(2);
    console.log(`  • ${file} (${size} MB)`);
  });
}
