# 📄 ATM System Program Documentation 🎉

**🛠️ Version:** 0.2

**📁 File:** `mainSystemATM.cpp`

**📅 Date:** 21 Oktober 2024

**👥 Authors:**

Kelompok 3 IF-D (Informatika 2024) :

1. **Zaka** Ahmad Ghofari (123240144)
2. **Dimas** Hafid Fathoni (123240159)
3. **Erlan** Rifqi Davin Darmawan (123240173)
4. **Jauza** Ilham Mahardika Putra (123240174)
5. **Rio** Meidi Ataurrahman (123240175)

---

## 📖 Table of Contents

1. [📝 Overview](#-overview)
2. [⚙️ Prerequisites](#️-prerequisites)
3. [▶️ Usage](#️-usage)
4. [🧩 Program Structure](#-program-structure)

   - [🔢 Data Structures](#-data-structures)
   - [📦 Modules & Functions](#-modules--functions)

5. [✨ Features](#-features)
6. [💾 Data Persistence](#-data-persistence)
7. [🐍 Snake Game](#-snake-game)
8. [🚧 Future Improvements](#-future-improvements)

---

## 📝 Overview

Selamat datang di **ATM System**! 🎉 Program C++ ini mensimulasikan sistem ATM dengan dua mode:

- **🔐 Admin**
- **👤 Customer**

### Fitur utama

- 🔒 Autentikasi (Account Number & PIN)
- 💰 Cek Saldo, Setor, Tarik, Transfer
- 📊 Riwayat Transaksi dengan analisis
- 🛡️ Operasi Admin: Tambah/Hapus Nasabah, Backup/Restore
- 🕹️ Bonus Snake Game

Dibangun oleh Kelompok 3 untuk memenuhi Proyek Akhir Matakuliah Algoritma Pemrograman Lanjut.

## ⚙️ Prerequisites

- 🖥️ C++17 (atau lebih tinggi) compiler (g++, MSVC)
- 📦 [nlohmann/json](https://github.com/nlohmann/json) untuk JSON
- Standard Library headers: `<iostream>`, `<fstream>`, `<thread>`, `<chrono>`, `<filesystem>`
- 🖱️ Windows: `<conio.h>` (Linux/macOS: sesuaikan input non-blocking)

## ▶️ Usage

1. **Jalankan program:** `./atm_system`
2. **Login:**

   - Masukkan 8-digit account number 🆔
   - Masukkan password 🔑
   - Admin default: `459777345`

3. **Menu Utama:**

   - 👤 Customer: Cek Saldo, Setor, Tarik, Transfer, Riwayat
   - 👥 Admin: Tambah, Hapus, Tampil Semua, Backup, Restore

4. Ikuti petunjuk layar 💡
5. Keluar dengan memilih opsi Exit 🚪 yang ada di login

## 🧩 Program Structure

### 🔢 Data Structures

- `struct histori`:

  - `int idTrans`
  - `int nominal`
  - `int noRektuj`
  - `string deskripsi`

- `struct nasabah`:

  - `int noRek`
  - `string pass`
  - `string nama`
  - `int saldo`
  - `int jumlahTrans`
  - `histori historiNasabah[100]`

Global:

- `nasabah dataNasabah[100]`, `int jumlahNasabah`
- `nasabah currentNasabah, anotherNasabah`
- `nasabah *currentNasabahPtr`
- Path file JSON & backup 📂

### 📦 Modules & Functions

1. **Error Handling** (`inputHandling` overload)
2. **File JSON**:

   - `noRekVal()`, `importDataJson()`, `exportNasabahJson()`
   - `importDb()`, `restoreBackupDb()` 💾

3. **CRUD Nasabah**:

   - `inputNasabah()`, `delNasabah()`, `tampilDataNasabah()` 📋

4. **Authentication & Menu**:

   - `loginAttempt()`, `login()`, `menuAdmin()`, `menuUtama()`

5. **Transaksi**:

   - `cekSaldo()`, `setorSaldo()`, `tarikSaldo()`, `transferSaldo()`

6. **Riwayat & Analisis**:

   - `riwayatTransaksi()`, `TotalTransaksi()` 📈

7. **Auxiliary**:

   - Sort, Pause, SlowType, DeleteText 🌀

8. **Snake Game**:

   - `SetupSnake()`, `DrawSnake()`, `InputSnake()`, `LogicSnake()`, `Snake()`, `TitleSnake()` 🐍

## ✨ Features

- **Admin Mode**:

  - ➕ Tambah Nasabah
  - ➖ Hapus Nasabah
  - 📑 Tampil Semua Data
  - 🔄 Backup & Restore

- **Customer Mode**:

  - ✅ Cek Saldo
  - 💸 Setor & Tarik
  - 🔁 Transfer
  - 📜 Riwayat Transaksi

- **Persistence**: JSON files per nasabah 💾

- **CLI Animations** 🎨

- **Snake Game** 🎮

## 💾 Data Persistence

- Lokasi: `../db/json/<noRek>.json`
- Contoh schema JSON:

```json
{
	"noRek": 12345678,
	"pass": "password",
	"nama": "Full Name",
	"saldo": 100000,
	"jumlahTrans": 3,
	"historiTransaksi": [
		{
			"idTrans": 51,
			"nominal": 50000,
			"noRekTuj": 87654326,
			"deskripsi": "Qris"
		}
	]
}
```

- Backup: `../db/backup/json` 📂

## 🐍 Snake Game

Mainkan Snake dalam ATM:

1. Pilih opsi rahasia (`24`) atau dari menu
2. Gunakan **W/A/S/D** untuk bergerak
3. **X** untuk kembali ke menu ATM ↩️

## 🚧 Future Improvements

- Pindah ke DB (SQLite) 🗄️
- Hashing password 🔐
- Modularisasi kode 📦
- Unit tests & CI/CD ✅

## 📜 License

MIT License. © 2025 ATM System Contributors ❤️

---

_End of Documentation_
