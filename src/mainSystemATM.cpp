/**
 * @file mainSystemATM.cpp
 * @author Zaka, Dhimas, Jauza, Erlan, dan Roi
 * @brief Sebuah program buatan 6 orang sangar untuk menunjuang Nilai Algoritma Pemrograman
 * @version 0.1
 * @date 2025-05-13
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib> // untuk menggunakan system("cls")
#include <ctime>   // untuk mengatur seed pada fungsi rand()
#include <thread>  // untuk menggunakan fungsi multithreading cnth sleep_for
#include <chrono>  // untuk menangani waktu dengan tepat sangat penting karena untuk penggunaan sleep_for yang membutuhkan parameter bertipe durasi
#include <vector>
#ifdef _WIN32
#include <conio.h>
#endif
#include <map> // untuk menghitung jumlah dan total nilai transaksi berdasarkan tipe/deskripsi
// Library Untuk Multifile
#include <fstream>
#include "include/json.hpp"

using namespace std;
using json = nlohmann::json;

// TODO - {Multifile} Struct
//  >> Struct Init
/**
 * @brief Struct Histori Transaksi Nasabah
 *
 */
struct histori
{
      int idTrans;
      int nominal;
      int noRektuj;
      string deskripsi;
};
/**
 * @brief Struct Untuk Template Data Nasabah
 *
 */
struct nasabah
{
      int noRek;
      string pass;
      string nama;
      int saldo;
      int jumlahTrans;
      histori historiNasabah[100];
};
// NOTE -  >> Global Var Init
bool mainMenuLoop = true, loginStat = false, adminStat = false;
int indexNasabah = -1;
nasabah *currentNasabahPtr = nullptr;
nasabah currentNasabah, anotherNasabah;
string pathJsonCurNasabah, pathJsonAnotherNasabah;
// TODO - {Multifile} Snake Function
// >> Snake Function
const int snakeWidth = 20;
const int snakeHeight = 20;
int snakeX, snakeY, fruitX, fruitY, snakeScore;
int tailX[100], tailY[100];
int nTail = 0;
enum eDirection
{
      STOP = 0,
      LEFT,
      RIGHT,
      UP,
      DOWN
};
eDirection snakeDir;
// >> Inisialisasi Global Array (Wajib Ada)
nasabah dataNasabah[100]; // menyimpan 100 data 100 nasabah
int jumlahNasabah = 0;    // Total Jumlah Data yang dimiliki //REVIEW - Setelah Di multifile coba dihapus
// >> Decklare Funtion
// >> Menu
void loopMenu();
void menuAdmin();
void menuUtama();
// TODO - {Multifile} Inputhandling
// Error handling Func (Overload)
void inputHandling(string question, string &var, short lineOr); // Untuk String
void inputHandling(string question, int &var);                  // untuk int
void inputHandling(string question, float &var);                // untuk float
void inputHandling(string question, short &var);                // untuk short]
// TODO - {Multifile} Init Data nasabah
// Multifile Manajemen System
bool noRekVal(int noRekIn, string &pathJsonCurNasabah);
void importDataJson(string jsonPathIn, nasabah &nasabahExport);
void nasabahExportJson(string jsonPathIn, nasabah nasabahExport);
// void inisialisasiData(); //Resmi Dinonaktifkan
// CRUD Funct Data Nasabah
void tampilDataNasabah();
void inputNasabah();
void delNasabah();
// TODO - {Multifile} Operational Function
// Main Feature
void login();
void loginAttempt(int Attempts); // Login Rekursif
void cekSaldo();
void setorSaldo();
void tarikSaldo();
void transferSaldo();
void riwayatTransaksi();
// Funct Sort
void sortByNoRek();
void sortBySaldo();
void sortHistoriTransaksi(nasabah &n);
// Perhitungan total transaksi
int TotalTransaksi(int MulaiIndex, int AkhirIndex); // Perhitungan Total Rekursif
// TODO - {Multifile} Animasi Pindah Multifile
// Pause Function
void Pause();
// Function untuk Pause
void SlowType(const string &kata, int delay);
void DeleteText(int count, int delay);
// Function untuk SnakeGame
void Snake();
void SetupSnake();
void DrawSnake();
void InputSnake();
void LogicSnake();
void TitleSnake();

int main(int argc, char const *argv[])
{
      short opt;
      // Identitas Pembuat
      cout << "\n<Selamat Datang di ATM Program ATM> \nKarya: "
           << " \t\n1. Zaka Ahmad Ghofari             (123240144)"
           << " \t\n2. Dimas Hafid Fathoni            (123240159)"
           << " \t\n3. Erlan Rifqi Davin Darmawan     (123240173)"
           << " \t\n4. Jauza Ilham Mahardika Putra    (123240174)"
           << " \t\n5. Rio Meidi Ataurrahman          (123240175)\n\n";
      system("pause");
      // REVIEW - Ini kalau udah jadi migrate dipindah
      // Inisialisasi Data awal
      // inisialisasiData();
      // Login dan Masuk Main Menu
      loopMenu();
      return 0;
}

// REVIEW - Main untuk Developing
int main1(int argc, char const *argv[])
{
      nasabah dummy;
      dummy.noRek = 12345678;
      dummy.pass = "dummy123";
      dummy.nama = "Dummy User";
      dummy.saldo = 1000000;
      dummy.jumlahTrans = 2;
      dummy.historiNasabah[0] = {1001, 500000, 87654321, "Setor Saldo"};
      dummy.historiNasabah[1] = {1002, -200000, 87654321, "Tarik Saldo"};
      nasabahExportJson("../db/json/test/23240144.json", dummy);
      return 0;
}

// Menu Admin dan Menu Utama
void loopMenu()
{
      do
      {
            loginStat = false;
            system("cls");
            login();
            do
            {
                  if (adminStat == true)
                  {
                        menuAdmin();
                  }
                  else
                  {
                        menuUtama();
                  }
            } while (mainMenuLoop == true);
      } while (loginStat == false);
}
void menuAdmin()
{
      short opt;
      system("cls");
      cout << "\t\t <<Menu Admin>>\n\n";
      cout << left << setw(25) << "1> Input Nasabah" << right << setw(25) << "\n\n";
      cout << left << setw(25) << "2> Delete Nasabah" << right << setw(25) << "\n\n";
      cout << left << setw(25) << "3> Tampil Nasabah" << right << setw(25) << "Exit Admin <4\n\n";
      inputHandling("\n Admin Mau Yang Mana?: ", opt);
      switch (opt)
      {
      case 1:
            Pause();
            inputNasabah();
            mainMenuLoop = true;
            break;
      case 2:
            Pause();
            delNasabah();
            mainMenuLoop = true;
            break;
      case 3:
            Pause();
            tampilDataNasabah();
            mainMenuLoop = true;
            break;
      case 4:
            cout << "\nTerima kasih telah menggunakan ATM ini Mas Admin\n";
            loopMenu();
            mainMenuLoop = false;
            currentNasabahPtr = nullptr;
            break;
      default:
            Pause();
            cout << "[error System] - Jane Ngopo si?" << endl;
            break;
      }
}
void menuUtama()
{
      short opt;
      // Penampil Menu Utama
      system("cls");
      cout << "\t\t <<Menu ATM>>\n";
      cout << left << setw(25) << "1> Cek Saldo" << right << setw(25) << "Tarik Tunai <4\n\n";
      cout << left << setw(25) << "2> Deposit Saldo" << right << setw(25) << "Riwayat Transaksi <5\n\n";
      cout << left << setw(25) << "3> Transfer Rekening" << right << setw(25) << "Exit <6\n\n";
      // Input User
      inputHandling("\nNasabah Mau Yang Mana?: ", opt);
      switch (opt)
      {
      case 1:
            Pause();
            cekSaldo();
            mainMenuLoop = true;
            break;
      case 2:
            Pause();
            setorSaldo();
            mainMenuLoop = true;
            break;
      case 3:
            Pause();
            transferSaldo();
            mainMenuLoop = true;
            break;
      case 4:
            Pause();
            tarikSaldo();
            mainMenuLoop = true;
            break;
      case 5:
            Pause();
            riwayatTransaksi();
            mainMenuLoop = true;
            break;
      case 6:
            cout << "\nTerimakasih telah menggunakan ATM ini\n";
            Pause();
            mainMenuLoop = false;
            loginStat = false;
            currentNasabahPtr = nullptr;
            return; // for stubborn anythin is that la
            break;
      case 24:
            Pause();
            Snake();
            break;
      default:
            cout << "[Input Error] - Pilihan yang Anda masukkan salah\n";
      }
}
/**
 * @brief Menverifikasi No Rek Apakah ada didalam DB atau tidak
 * @return mereturn Boolean True jika ada False jika tidak
 *
 * @param noRekIn nomor Rekening Masukan untuk dicek
 * @param pathJsonCurNasabah global variabel yang mengatur path json yang diperlukan
 */
bool noRekVal(int noRekIn, string &pathJsonCurNasabah)
{
      string pathJsonCurNasabahIn;
      // Make Path for JSON
      pathJsonCurNasabahIn = "../db/json/" + to_string(noRekIn) + ".json";
      ifstream validate(pathJsonCurNasabahIn);
      bool statFile = validate.is_open();
      if (statFile == 1)
      {
            pathJsonCurNasabah = pathJsonCurNasabahIn;
      }
      validate.close();
      return statFile;
}
/**
 * @brief Mengimport data nasabah dari json yang sudah di verifikasi noRekVal
 *
 * @param jsonPathIn path dari json file
 * @param nasabahExport struct yang ada di global variabel menyimpan data nasabah
 */
void importDataJson(string jsonPathIn, nasabah &nasabahExport)
{
      ifstream importStream(jsonPathIn, ios::in);
      json dataNasabahJson;
      if (importStream.is_open())
      {

            importStream >> dataNasabahJson;
            // cout << "test value : " << dataNasabahJson["saldo"].get<int>() << endl;
            if (dataNasabahJson.contains("noRek"))
            {
                  nasabahExport.noRek = dataNasabahJson["noRek"].get<int>();
            }
            else
            {
                  cout << "[Err] - No Rek error Not Found" << endl;
                  return;
            }
            if (dataNasabahJson.contains("pass"))
            {
                  nasabahExport.pass = dataNasabahJson["pass"].get<string>();
            }
            else
            {
                  cout << "[Err] - Password error Not Found" << endl;
                  return;
            }
            if (dataNasabahJson.contains("nama"))
            {
                  nasabahExport.nama = dataNasabahJson["nama"].get<string>();
            }
            else
            {
                  cout << "[Err] - Nama error Not Found" << endl;
                  return;
            }
            if (dataNasabahJson.contains("saldo"))
            {
                  nasabahExport.saldo = dataNasabahJson["saldo"].get<int>();
            }
            else
            {
                  cout << "[Err] - Saldo error Not Found" << endl;
                  return;
            }
            if (dataNasabahJson.contains("jumlahTrans"))
            {
                  nasabahExport.jumlahTrans = dataNasabahJson["jumlahTrans"].get<int>();
            }
            else
            {
                  cout << "[Err] - Jumlah Transaksi error Not Found" << endl;
                  return;
            }
            if (dataNasabahJson.contains("historiTransaksi"))
            {
                  for (int i = 0; i < dataNasabahJson["jumlahTrans"].get<int>(); ++i)
                  {
                        nasabahExport.historiNasabah[i].idTrans = dataNasabahJson["historiTransaksi"][i]["idTrans"].get<int>();
                        nasabahExport.historiNasabah[i].nominal = dataNasabahJson["historiTransaksi"][i]["nominal"].get<int>();
                        nasabahExport.historiNasabah[i].noRektuj = dataNasabahJson["historiTransaksi"][i]["noRekTuj"].get<int>();
                        nasabahExport.historiNasabah[i].deskripsi = dataNasabahJson["historiTransaksi"][i]["deskripsi"].get<string>();
                  }
            }
            else
            {
                  cout << "[Err] - Histori Nasabah error Not Found" << endl;
                  return;
            }
      }
      else
      {
            cout << "[Err Opening File] - Something Seems Off in This file" << endl;
      }
      importStream.close();
}
/**
 * @brief Fungsi untuk mengexport nasabah kedalam sebuah file JSON
 *
 * @param jsonPathOut path json untuk keluar kemana
 * @param nasabahExport struct nasabah yang mau di export
 */
void nasabahExportJson(string jsonPathOut, nasabah nasabahExport)
{
      ofstream exportStream(jsonPathOut, ios::trunc);
      json dataNasabahJson;
      if (exportStream.is_open())
      {
            dataNasabahJson["noRek"] = nasabahExport.noRek;
            dataNasabahJson["pass"] = nasabahExport.pass;
            dataNasabahJson["nama"] = nasabahExport.nama;
            dataNasabahJson["saldo"] = nasabahExport.saldo;
            dataNasabahJson["jumlahTrans"] = nasabahExport.jumlahTrans;

            dataNasabahJson["historiTransaksi"] = json::array();
            for (int i = 0; i < nasabahExport.jumlahTrans; ++i)
            {
                  json transaksi;
                  transaksi["idTrans"] = nasabahExport.historiNasabah[i].idTrans;
                  transaksi["nominal"] = nasabahExport.historiNasabah[i].nominal;
                  transaksi["noRekTuj"] = nasabahExport.historiNasabah[i].noRektuj;
                  transaksi["deskripsi"] = nasabahExport.historiNasabah[i].deskripsi;
                  dataNasabahJson["historiTransaksi"].push_back(transaksi);
            }

            exportStream << dataNasabahJson.dump(4);
            exportStream.close();
      }
      else
      {
            cout << "[Err Exporting File] - Nah im not going to export ts" << endl;
      }
}
// Fungsi Login Rekursif
void loginAttempt(int Attempts)
{
      int noRekIn;
      string passIn;
      if (Attempts <= 0)
      {
            cout << "<Maaf Kesempatan Anda Habis. Silahkan Hubungi Costumer Service jika ada masalah.>" << endl;
            exit(0);
      }
      inputHandling("\n\n  Masukkan Nomor Rekening (8 Digit) \texit(404)\n>> ", noRekIn);
      // Untuk keluar
      if (noRekIn == 404)
      {
            cout << "\nTerima Kasih Telah Menggunakan ATM Kami:D" << endl;
            exit(0);
      }
      // Untuk Lanjut ke Password
      else if (noRekVal(noRekIn, pathJsonCurNasabah))
      {
            inputHandling("\n  Masukkan Password Rekening\n>> ", passIn, 1);
      }
      else
      {
            cout << "Err - Whoops Rekening Salah" << endl;
            cout << "\n[Login Gagal] - Kesempatan Anda Tersisa " << (Attempts - 1) << endl;
            loginAttempt(Attempts - 1);
      }
      importDataJson(pathJsonCurNasabah, currentNasabah);
      if (currentNasabah.pass == passIn)
      {
            if (noRekIn == 459777345)
            {
                  adminStat = true;
                  loginStat = true;
            }
            else // kalau bukan admin
            {
                  adminStat = false;
                  loginStat = true;
            }
            currentNasabahPtr = &currentNasabah;
            // Ucapan Selamat Login
            cout << "\n<Login berhasil>\n";
            Pause();
            system("cls");
            cout << "\n <= Selamat Datang " << currentNasabahPtr->nama << " =>\n\n";
            cout << "Data Rekening :" << endl;
            cout << setw(10) << "A.N." << setw(5) << ": " << currentNasabahPtr->nama << endl;
            cout << setw(10) << "No.Rek" << setw(5) << " : " << currentNasabahPtr->noRek << endl
                 << endl;
            system("pause");
            return; // Penting ini cah
      }
      // REVIEW - ini udah dimigrate
      // for (int i = 0; i < jumlahNasabah; i++)
      // {
      //       if (noRekIn == dataNasabah[i].noRek && passIn == dataNasabah[i].pass)
      //       {
      //             if (dataNasabah[i].noRek == 459777345)
      //             {
      //                   adminStat = true;
      //                   loginStat = true;
      //                   indexNasabah = i;
      //                   currentNasabahPtr = &dataNasabah[i];
      //             }
      //             else
      //             {
      //                   adminStat = false;
      //                   loginStat = true;
      //                   indexNasabah = i;
      //                   currentNasabahPtr = &dataNasabah[i];
      //             }
      //             cout << "\n<Login berhasil>\n";
      //             Pause();
      //             system("cls");
      //             cout << "\n <= Selamat Datang " << currentNasabahPtr->nama << " =>\n\n";
      //             cout << "Data Rekening :" << endl;
      //             cout << setw(10) << "A.N." << setw(5) << ": " << currentNasabahPtr->nama << endl;
      //             cout << setw(10) << "No.Rek" << setw(5) << " : " << currentNasabahPtr->noRek << endl
      //                  << endl;
      //             system("pause");
      //             return;
      //       }
      // }
      cout << "\n[Login Gagal] - Kesempatan Anda Tersisa " << (Attempts - 1) << endl;
      loginAttempt(Attempts - 1);
}
// Fungsi Login
void login()
{
      loginAttempt(3);
}
// Overloading Funct untuk Error Handling Pengguna
void inputHandling(string question, string &var, short lineOr)
{
      bool statLoop = false;
      do
      {
            cout << question;
            // Jika 1 maka Spasi dianggap kesalahan
            if (lineOr == 1)
            {
                  cin >> var;
            }
            // Jika 2 maka spasi akan dibaca
            else if (lineOr == 2)
            {
                  cin.ignore(30, '\n'); // Mengabaikan Input
                  getline(cin, var);
            }
            if (cin.fail())
            {
                  cin.clear();          // Menghapus Semua Fail Flag
                  cin.ignore(30, '\n'); // Mengabaikan Input
                  cout << "\n[ERROR CIN,not str] -" << "Input Tidak Sesuai\n";
                  statLoop = true;
            }
            else
            {
                  statLoop = false;
            }
      } while (statLoop == true);
};
// Untuk Variabel Integer
void inputHandling(string question, int &var)
{
      bool statLoop = false;
      do
      {
            cout << question;
            cin >> var;
            if (cin.fail())
            {
                  cin.clear();          // Menghapus Semua Fail Flag
                  cin.ignore(30, '\n'); // Mengabaikan Input
                  cout << "\n[ERROR CIN,not int] -" << "Input Tidak Sesuai\n";
                  statLoop = true;
            }
            else
            {
                  statLoop = false;
            }
      } while (statLoop == true);
};
// Untuk Variabel FLoat
void inputHandling(string question, float &var)
{
      cout << question;
      bool statLoop = false;
      do
      {
            cin >> var;
            if (cin.fail())
            {
                  cin.clear();          // Menghapus Semua Fail Flag
                  cin.ignore(30, '\n'); // Mengabaikan Input
                  cout << "\n[ERROR CIN,not float] -" << "Input Tidak Sesuai\n";
                  statLoop = true;
            }
            else
            {
                  statLoop = false;
            }
      } while (statLoop == true);
};
// Untuk Short Short
void inputHandling(string question, short &var)
{
      cout << question;
      bool statLoop = false;
      do
      {
            cin >> var;
            if (cin.fail())
            {
                  cin.clear();          // Menghapus Semua Fail Flag
                  cin.ignore(30, '\n'); // Mengabaikan Input
                  cout << "\n[ERROR CIN,not shortInt] -" << "Input Tidak Sesuai\n";
                  statLoop = true;
            }
            else
            {
                  statLoop = false;
            }
      } while (statLoop == true);
};

// REVIEW - Resmi ditutup data hardcode ini
// void inisialisasiData()
// {
//       dataNasabah[jumlahNasabah++] =
//           {459777345, "admin", "Sosok Asli Admin", 9999999, 0, {}}; // Sosok Asli Admin
//       dataNasabah[jumlahNasabah++] =
//           {23240144, "admin123", "Zaka Ahmad Ghofari", 2500000, 12,
//            {// biar spasi
//             {51, 50000, 87654326, "Qris"},
//             {61, 900000, 87654327, "Top Up Dana"},
//             {71, 200000, 87654318, "Virtual Acc"},
//             {52, 400000, 87654328, "Qris"},
//             {62, 75000, 87654353, "Top UP Dana"},
//             {72, 800000, 87654328, "Virtual Acc"},
//             {73, 960000, 87654330, "Virtual Acc"},
//             {53, 115000, 87654402, "Qris"},
//             {63, 120000, 87654560, "Top Up Dana"},
//             {64, 980000, 87654975, "Top Up Gopay"},
//             {90, 650000, 87650642, "Transfer"},
//             {74, 850000, 87654340, "Transfer Saldo"}}};
//       dataNasabah[jumlahNasabah++] =
//           {23240159, "admin321", "Dimas Hafid Fathoni", 6500000, 12,
//            {// biar spasi
//             {51, 50000, 87654326, "Qris"},
//             {61, 900000, 87654327, "Top Up Dana"},
//             {71, 200000, 87654318, "Virtual Acc"},
//             {52, 400000, 87654328, "Qris"},
//             {62, 75000, 87654353, "Top UP Dana"},
//             {72, 800000, 87654328, "Virtual Acc"},
//             {73, 960000, 87654330, "Virtual Acc"},
//             {53, 115000, 87654402, "Qris"},
//             {63, 120000, 87654560, "Top Up Dana"},
//             {64, 980000, 87654975, "Top Up Gopay"},
//             {90, 650000, 87650642, "Transfer"},
//             {74, 850000, 87654340, "Virtual Acc"}}};
//       dataNasabah[jumlahNasabah++] =
//           {23240173, "admin3221", "Erlan Rifqi Davin D", 5000000, 12,
//            {// biar spasi
//             {51, 50000, 87654326, "Qris"},
//             {61, 900000, 87654327, "Top Up Dana"},
//             {71, 200000, 87654318, "Virtual Acc"},
//             {52, 400000, 87654328, "Qris"},
//             {62, 75000, 87654353, "Top UP Dana"},
//             {72, 800000, 87654328, "Virtual Acc"},
//             {73, 960000, 87654330, "Virtual Acc"},
//             {53, 115000, 87654402, "Qris"},
//             {63, 120000, 87654560, "Top Up Dana"},
//             {64, 980000, 87654975, "Top Up Gopay"},
//             {90, 650000, 87650642, "Transfer"},
//             {74, 850000, 87654340, "Virtual Acc"}}};
//       dataNasabah[jumlahNasabah++] =
//           {23240174, "admin3214", "Jauza Ilham Mahardika P", 10000000, 12,
//            {// biar spasi
//             {51, 50000, 87654326, "Qris"},
//             {61, 900000, 87654327, "Top Up Dana"},
//             {71, 200000, 87654318, "Virtual Acc"},
//             {52, 400000, 87654328, "Qris"},
//             {62, 75000, 87654353, "Top UP Dana"},
//             {72, 800000, 87654328, "Virtual Acc"},
//             {73, 960000, 87654330, "Virtual Acc"},
//             {53, 115000, 87654402, "Qris"},
//             {63, 120000, 87654560, "Top Up Dana"},
//             {64, 980000, 87654975, "Top Up Gopay"},
//             {90, 650000, 87650642, "Transfer"},
//             {74, 850000, 87654340, "Virtual Acc"}}};
//       dataNasabah[jumlahNasabah++] =
//           {23240175, "admin3215", "Rio Meidi A", 9000000, 12,
//            {// biar spasi
//             {51, 50000, 87654326, "Qris"},
//             {61, 900000, 87654327, "Top Up Dana"},
//             {71, 200000, 87654318, "Virtual Acc"},
//             {52, 400000, 87654328, "Qris"},
//             {62, 75000, 87654353, "Top UP Dana"},
//             {72, 800000, 87654328, "Virtual Acc"},
//             {73, 960000, 87654330, "Virtual Acc"},
//             {53, 115000, 87654402, "Qris"},
//             {63, 120000, 87654560, "Top Up Dana"},
//             {64, 980000, 87654975, "Top Up Gopay"},
//             {90, 650000, 87650642, "Transfer"},
//             {74, 850000, 87654340, "Virtual Acc"}}};
//       dataNasabah[jumlahNasabah++] =
//           {1, "admin", "Test Empty Set", 0, 0, {
//                                                    // biar spasi
//                                                }};
//       // Kalau Kurang silahkan di tambah sendiri
// }
// CRUD Funct
// Fungsi Untuk Menampilkan Data Nasabah dengan parameter nomor rekening
void sortByNoRek() // sort norek manual (bubble sort)
{
      for (int i = 0; i < jumlahNasabah - 1; i++)
      {
            for (int j = 0; j < jumlahNasabah - 1; j++)
            {
                  string rekA = to_string(dataNasabah[j].noRek);
                  string rekB = to_string(dataNasabah[j + 1].noRek);

                  if (rekA.length() > rekB.length() || (rekA.length() == rekB.length() && dataNasabah[j].noRek > dataNasabah[j + 1].noRek))
                  {
                        swap(dataNasabah[j], dataNasabah[j + 1]);
                  }
            }
      }
}

void sortBySaldo() // Selection Sort
{
      for (int i = 0; i < jumlahNasabah - 1; i++)
      {
            int maxIdx = i;
            for (int j = i + 1; j < jumlahNasabah; j++) // Perbaikan kondisi loop
            {
                  if (dataNasabah[j].saldo > dataNasabah[maxIdx].saldo ||
                      (dataNasabah[j].saldo == dataNasabah[maxIdx].saldo && dataNasabah[j].nama < dataNasabah[maxIdx].nama))
                  {
                        maxIdx = j;
                  }
            }
            swap(dataNasabah[i], dataNasabah[maxIdx]);
      }
}
void sortHistoriTransaksi(nasabah &n) // sort manual histori transaksi di satu nasabah (bubble sort)
{
      for (int i = 0; i < n.jumlahTrans - 1; i++)
      {
            for (int j = 0; j < n.jumlahTrans - i - 1; j++)
            {
                  if (n.historiNasabah[j].nominal < n.historiNasabah[j + 1].nominal)
                  {
                        swap(n.historiNasabah[j], n.historiNasabah[j + 1]);
                  }
                  else if (n.historiNasabah[j].nominal == n.historiNasabah[j + 1].nominal && n.historiNasabah[j].idTrans < n.historiNasabah[j + 1].idTrans)
                  {
                        swap(n.historiNasabah[j], n.historiNasabah[j + 1]);
                  }
            }
      }
}
// TODO - Load Data JSON nasabah
void tampilDataNasabah()
{
      system("cls");
      bool find = false;
      int noRekIn = 0;
      short pilihanSort;
      cout << "\n\t>- Menu Tampil Data Nasabah -<\n\n";
      cout << "Pilih metode pengurutan\n";
      cout << "1. Berdasarkan Nomor Rekening\n";
      cout << "2. Berdasarkan Saldo\n";
      inputHandling("Pilihan : ", pilihanSort);
      if (pilihanSort == 1)
      {
            sortByNoRek();
      }
      else if (pilihanSort == 2)
      {
            sortBySaldo();
      }
      else
            cout << "[ERROR INPUT] - Pilihan tidak vald, menampilkan tanpa pengurutan\n";

      cout << "\n\t>- Menu Tampil Data Nasabah -<\n\n";
      cout << "\nDaftar Nasabah:\n";
      cout << left << "| " << setw(10) << "No Rek" << " | " << setw(25) << "Nama" << " |\n";
      cout << setfill('-') << setw(41) << "-" << setfill(' ') << endl;
      for (int i = 0; i < jumlahNasabah; i++)
      {
            cout << "| " << setw(10) << dataNasabah[i].noRek << " | " << setw(25) << dataNasabah[i].nama << " |\n";
      }
      cout << setfill('-') << setw(41) << "-" << setfill(' ') << endl;
      // Pause();
      inputHandling("Berapa Nomor Rekeningnya?: ", noRekIn);
      for (int i = 0; i < jumlahNasabah; i++)
      {
            if (noRekIn == dataNasabah[i].noRek)
            {
                  find = true;
                  cout << "\n\t<Lihat Data Dan Histori>\n"
                       << endl;
                  cout << "Nasabah:" << endl;
                  cout << "\n Data Nasabah:" << endl;
                  cout << "No Rek  : " << dataNasabah[i].noRek << endl;
                  cout << "Nama    : " << dataNasabah[i].nama << endl;
                  cout << "Saldo   : " << dataNasabah[i].saldo << endl;
                  cout << "\nHistori Transaksi:" << endl;
                  if (dataNasabah[i].historiNasabah[0].idTrans > 0)
                  {
                        for (int j = 0; j < dataNasabah[i].jumlahTrans; ++j)
                        {
                              cout << "Nomor     : " << j + 1 << endl;
                              cout << "ID Trans  : " << dataNasabah[i].historiNasabah[j].idTrans << endl;
                              cout << "Nominal   : " << dataNasabah[i].historiNasabah[j].nominal << endl;
                              cout << "No Rek T  : " << dataNasabah[i].historiNasabah[j].noRektuj << endl;
                              cout << "Deskripsi : " << dataNasabah[i].historiNasabah[j].deskripsi << endl;
                              cout << endl;
                        }
                  }
                  else
                  {
                        cout << "\n [Empty Set] - Belum Memiliki Riwayat Transaksi\n";
                  }
                  break;
            }
            else
            {
                  find = false;
            }
      }
      find == false ? cout << "\n [Error Tag]- Rekening Tak ditemukan\n" : cout << "\n <Data Tertampil Semua>";
      cout << "\nTekan karakter apapun untuk melanjutkan...";
      cin.ignore(1000, '\n');
      cin.get();
}
// Funct Untuk Input Nasabah
void inputNasabah()
{
      system("cls");
      Pause();
      cout << "\n\n\t>-Menu Input Nasabah Baru-<\n";
      cout << "\n\nHallo Nasabah Baru!!";
      cout << "\n Data Nasabah:" << endl;
      inputHandling("\nNo Rek  : ", dataNasabah[jumlahNasabah].noRek);
      inputHandling("\nPass    : ", dataNasabah[jumlahNasabah].pass, 1);
      inputHandling("\nNama    : ", dataNasabah[jumlahNasabah].nama, 2);
      inputHandling("\nSaldo   : ", dataNasabah[jumlahNasabah].saldo);
      dataNasabah[jumlahNasabah].jumlahTrans = 0; // Nasabah Baru Tidak Memiliki Histori Transaksi
      jumlahNasabah++;
}
// Fungsi untuk delete data nasabah
void delNasabah()
{
      cout << "\n\n\t>-Menu Delete Data Nasabah-<\n";
      system("cls");
      Pause();
      int noRekIn;
      short opt;
      bool find = true;
      do
      {
            inputHandling("Berapa Nomor Rekeningnya?: ", noRekIn);
            for (int i = 0; i <= jumlahNasabah; i++)
            {
                  if (noRekIn == dataNasabah[i].noRek)
                  {
                        find = true;
                        cout << "\nSuccess Delete Data user {" << dataNasabah[i].nama << "} ";
                        dataNasabah[i] = {
                            0,
                            "",
                            "",
                            0,
                            0,
                            {}}; // Code Pengosongan Data Nasabah
                        break;
                  }
                  else
                  {
                        find = false;
                  }
            }
            if (find == false)
            {
                  cout << "\n[Error Tag] - Input Not Found - Wah ini gaada nih gimana dong?\n";
                  inputHandling("\n(1)Ulang or (2)Kembali?: ", opt);
                  if (opt == 1)
                  {
                        find = false;
                  }
                  else
                  {
                        find = true;
                        cout << "\n Hapus No Rek <" << noRekIn << "> Dibatalkan";
                  }
            }
      } while (find == false);
      cin.ignore(1000, '\n');
      cin.get();
}
// >> Fungsi Menu Utama (nasabah)
// Fungsi Cek Saldo
void cekSaldo()
{
      short pilihan;
      do
      {
            cout << "\nSaldo Anda: \n";
            cout << "\tRp" << fixed << currentNasabahPtr->saldo << ",00" << endl;
            cout << "\n\t\t Kembali <1";
            inputHandling("\n>> ", pilihan);
            if (pilihan == 1)
            {
                  Pause();
                  menuUtama();
                  return;
            }
            else
            {
                  cout << "\n[ERROR] - Input tidak valid\n";
            }
      } while (pilihan != 1);
}
// Function Setor Saldo
void setorSaldo()
{
      int jumlah;
      short pilihan;
      srand(time(0)); // mengatur seed pada rand() untuk menggunakan waktu saat ini memastikan tidak membawa seed bawaan ctime
      do
      {
            cout << "Cek Data nasabah" << currentNasabah.saldo << endl; // REVIEW - Buat ngecek dev
            cout << "\n <= Setor Saldo =>\n\n";
            cout << "Data Rekening :" << endl;
            cout << setw(10) << "A.N." << setw(5) << ": " << currentNasabahPtr->nama << endl;
            cout << setw(10) << "No.Rek" << setw(5) << " : " << currentNasabahPtr->noRek << endl;
            cout << "Deposit" << setw(5) << ": Rp";
            inputHandling("", jumlah);
            cout << endl;
            cout << setw(15) << "Konfirmasi <1" << endl;
            cout << setw(15) << "Kembali <2" << endl;
            inputHandling("\n>> ", pilihan);
            if (pilihan == 1)
            {
                  currentNasabahPtr->saldo += jumlah;
                  currentNasabahPtr->historiNasabah[currentNasabahPtr->jumlahTrans].nominal = jumlah;
                  currentNasabahPtr->historiNasabah[currentNasabahPtr->jumlahTrans].idTrans = rand() % 90000000 + 10000000; // Nomor Id acak mengambil dari rentang 0 hingga 89999999 kemudian ditambahkan 10000000
                  currentNasabahPtr->historiNasabah[currentNasabahPtr->jumlahTrans].noRektuj = currentNasabahPtr->noRek;
                  currentNasabahPtr->historiNasabah[currentNasabahPtr->jumlahTrans].deskripsi = "Setor Saldo";
                  currentNasabahPtr->jumlahTrans++;
                  // Diexport untuk mengupdate data
                  nasabahExportJson(pathJsonCurNasabah, currentNasabah);
                  // REVIEW - lagi Dev
                  // cout << "\nSystem is ";
                  // SlowType("processing", 100);
                  // this_thread::sleep_for(chrono::seconds(1));
                  // DeleteText(20, 100);
                  // SlowType("Setor Tunai berhasil", 100);
                  // cout << endl;
                  // this_thread::sleep_for(chrono::seconds(1));
                  system("cls");
                  return;
            }
            else if (pilihan == 2)
            {
                  cout << "\nSetoran dibatalkan\n";
                  Pause();
                  return;
            }
            else
            {
                  cout << "\n[ERROR] - Input tidak valid\n";
            }
      } while (pilihan != 1 && pilihan != 2);
}
// Function Tarik Saldo
void tarikSaldo()
{
      int jumlah;
      short pilihan;
      srand(time(0)); // mengatur seed pada rand() untuk menggunakan waktu saat ini memastikan tidak membawa seed bawaan ctime
      do
      {
            cout << "\n <= Tarik Tunai =>\n\n";
            cout << "Data Rekening :" << endl;
            cout << setw(10) << "A.N." << setw(5) << ": " << currentNasabahPtr->nama << endl;
            cout << setw(10) << "No.Rek" << setw(5) << " : " << currentNasabahPtr->noRek << endl;
            cout << "Nominal" << setw(5) << ": Rp";
            inputHandling("", jumlah);
            cout << endl;
            cout << setw(15) << "Konfirmasi <1" << endl;
            cout << setw(15) << "Kembali <2" << endl;
            inputHandling("\n>> ", pilihan);

            if (pilihan == 1)
            {
                  if (currentNasabahPtr->saldo < jumlah)
                  {
                        cout << "\nSystem is ";
                        SlowType("processing", 100);
                        this_thread::sleep_for(chrono::seconds(1));
                        DeleteText(20, 100);
                        SlowType("Tarik Tunai Gagal Saldo tidak Mencukupi", 300);
                        cout << endl;
                        this_thread::sleep_for(chrono::seconds(1));
                        system("cls");
                        return;
                  }
                  else
                  {
                        currentNasabahPtr->saldo -= jumlah;
                        currentNasabahPtr->historiNasabah[currentNasabahPtr->jumlahTrans].nominal = -jumlah;
                        currentNasabahPtr->historiNasabah[currentNasabahPtr->jumlahTrans].idTrans = rand() % 90000000 + 10000000; // Nomor Id acak mengambil dari rentang 0 hingga 89999999 kemudian ditambahkan 10000000
                        currentNasabahPtr->historiNasabah[currentNasabahPtr->jumlahTrans].noRektuj = currentNasabahPtr->noRek;
                        currentNasabahPtr->historiNasabah[currentNasabahPtr->jumlahTrans].deskripsi = "Tarik Saldo";
                        currentNasabahPtr->jumlahTrans++;
                        nasabahExportJson(pathJsonCurNasabah, currentNasabah);
                        cout << "\nSystem is ";
                        SlowType("processing", 100);
                        this_thread::sleep_for(chrono::seconds(1));
                        DeleteText(20, 100);
                        SlowType("Tarik Tunai berhasil", 100);
                        cout << endl;
                        this_thread::sleep_for(chrono::seconds(1));
                        system("cls");
                        return;
                  }
            }
            else if (pilihan == 2)
            {
                  cout << "\nTarik Tunai dibatalkan\n";
                  Pause();
                  menuUtama();
                  return;
            }
            else
            {
                  cout << "\n[ERROR] - Input tidak valid\n";
            }
      } while (pilihan != 1 && pilihan != 2);
}
// Function Transfer Saldo
void transferSaldo()
{
      int rekening_tujuan, jumlah;
      short pilihan;
      bool find = false;
      nasabah *targetNasabah = &anotherNasabah;
      srand(time(0)); // mengatur seed pada rand() untuk menggunakan waktu saat ini memastikan tidak membawa seed bawaan ctime
      do
      {
            do
            {
                  inputHandling("\nMasukkan Nomor Rekening tujuan (8 digit) : ", rekening_tujuan);
                  if (noRekVal(rekening_tujuan, pathJsonAnotherNasabah))
                  {
                        find = true;
                        importDataJson(pathJsonAnotherNasabah, *targetNasabah);
                        break;
                  }
                  else
                        find = false;

                  if (find == true)
                  {
                        cout << "\n <= Transfer Deposito =>\n\n";
                        cout << "Data Rekening :" << endl;
                        cout << setw(40) << "A.N." << setw(5) << " : " << targetNasabah->nama << endl;
                        cout << setw(40) << "No.Rek" << setw(5) << " : " << targetNasabah->noRek << endl;
                  }
                  else
                  {
                        cout << "\nNomor Rekening yang Anda Masukkan tidak valid\n";
                  }
            } while (find == false);
            cout << "Nominal" << setw(5) << ": Rp";
            inputHandling("", jumlah);
            cout << endl;
            cout << setw(15) << "Konfirmasi <1" << endl;
            cout << setw(15) << "Kembali <2" << endl;
            inputHandling("\n>> ", pilihan);
            if (pilihan == 1)
            {
                  if (currentNasabahPtr->saldo < jumlah)
                  {
                        cout << "\nSystem is ";
                        SlowType("processing", 100);
                        this_thread::sleep_for(chrono::seconds(1));
                        DeleteText(20, 100);
                        SlowType("Transfer Gagal Saldo tidak Mencukupi", 300);
                        cout << endl;
                        this_thread::sleep_for(chrono::seconds(1));
                        system("cls");
                        return;
                  }
                  else
                  {
                        // Update Data Nasabah - Pengirim
                        currentNasabahPtr->saldo -= jumlah;
                        currentNasabahPtr->historiNasabah[currentNasabahPtr->jumlahTrans].nominal = -jumlah;
                        currentNasabahPtr->historiNasabah[currentNasabahPtr->jumlahTrans].idTrans = rand() % 90000000 + 10000000; // Nomor Id acak mengambil dari rentang 0 hingga 89999999 kemudian ditambahkan 10000000
                        currentNasabahPtr->historiNasabah[currentNasabahPtr->jumlahTrans].noRektuj = targetNasabah->noRek;
                        currentNasabahPtr->historiNasabah[currentNasabahPtr->jumlahTrans].deskripsi = "Transfer Saldo";
                        currentNasabahPtr->jumlahTrans++;
                        nasabahExportJson(pathJsonCurNasabah, currentNasabah);
                        // Update Data Nasabah - Penerima
                        targetNasabah->saldo += jumlah;
                        targetNasabah->historiNasabah[targetNasabah->jumlahTrans].nominal = jumlah;
                        targetNasabah->historiNasabah[targetNasabah->jumlahTrans].idTrans = rand() % 90000000 + 10000000; // Nomor Id acak mengambil dari rentang 0 hingga 89999999 kemudian ditambahkan 10000000
                        targetNasabah->historiNasabah[targetNasabah->jumlahTrans].noRektuj = currentNasabahPtr->noRek;
                        targetNasabah->historiNasabah[targetNasabah->jumlahTrans].deskripsi = "Menerima Saldo";
                        targetNasabah->jumlahTrans++;
                        nasabahExportJson(pathJsonAnotherNasabah, *targetNasabah);
                        // REVIEW - Dev Mode
                        // cout << "\nSystem is ";
                        // SlowType("processing", 100);
                        // this_thread::sleep_for(chrono::seconds(1));
                        // DeleteText(20, 100);
                        // SlowType("Trasnfer berhasil", 100);
                        // cout << endl;
                        // this_thread::sleep_for(chrono::seconds(1));
                        system("cls");
                        return;
                  }
            }
            else if (pilihan == 2)
            {
                  cout << "\n[Transfer dibatalkan]\n";
                  targetNasabah = nullptr;
                  Pause();
                  return;
            }
            else
            {
                  cout << "\n[ERROR] - Input tidak valid\n";
            }
      } while (pilihan != 1 && pilihan != 2);
      targetNasabah = nullptr;
}
// Function Riwayat Transaksi
void riwayatTransaksi()
{
      const int transHal = 5;
      int halaman = 0, totalHal = (currentNasabahPtr->jumlahTrans) / transHal; // Menghitung Jumlah Halaman
      currentNasabahPtr->jumlahTrans % transHal != 0 ? totalHal++ : totalHal;
      short pilihan;
      bool exit = false;
      if (currentNasabahPtr->jumlahTrans == 0)
      {
            cout << "[Empty Set] - Riwayat transaksi anda belum ada\n";
            Pause();
            return;
      }
      do
      {
            system("cls");
            cout << "<= Riwayat Transaksi =>\n";
            cout << "Page (" << halaman + 1 << "/" << totalHal << ")\n";
            // menghitung total menggunakan rekursif
            int TotalAll = TotalTransaksi(0, currentNasabahPtr->jumlahTrans - 1);
            // menghitung total transaksi pada halaman ini
            int MulaiIndex = halaman * transHal;
            int AkhirIndex = min((halaman + 1) * transHal - 1, currentNasabahPtr->jumlahTrans - 1);
            int TotalPage = TotalTransaksi(MulaiIndex, AkhirIndex);
            cout << left << "| " << setw(3) << "No." << " | " << setw(15) << "Id Transaksi" << " | " << setw(20) << "Nominal" << " | " << setw(15) << "Rekening Tujuan" << " | " << setw(15) << "Deskripsi" << " |" << endl;
            cout << setfill('+') << setw(84) << "+" << endl
                 << setfill(' ');
            for (int i = halaman * transHal; i < min((halaman * transHal) + transHal, currentNasabahPtr->jumlahTrans); i++)
            {
                  cout << right << "| " << setw(3) << i + 1 << " | " << setw(15) << currentNasabahPtr->historiNasabah[i].idTrans << " | Rp" << setw(15) << currentNasabahPtr->historiNasabah[i].nominal << ",00" << " | " << setw(15) << currentNasabahPtr->historiNasabah[i].noRektuj << " | " << setw(15) << currentNasabahPtr->historiNasabah[i].deskripsi << " |" << endl;
            }
            cout << setfill('+') << setw(84) << "+" << endl
                 << setfill(' ') << endl;
            cout << "Total semua transaksi: Rp" << TotalAll << ",00" << endl;
            cout << "Total transaksi halaman ini: Rp" << TotalPage << ",00" << endl
                 << endl;
            cout << right << setw(75) << "Next Page " << setw(5) << "1<\n";
            cout << right << setw(75) << "Previous Page " << setw(5) << "2<\n";
            cout << right << setw(75) << "Analisis Transaksi " << setw(5) << "3<\n";
            cout << right << setw(75) << "Kembali " << setw(5) << "4<\n";
            inputHandling("\n>> ", pilihan);
            if (pilihan == 1)
            {
                  if (halaman + 1 < totalHal)
                  {
                        halaman++;
                  }
            }
            else if (pilihan == 2)
            {
                  if (halaman > 0)
                  {
                        halaman--;
                  }
            }
            else if (pilihan == 3)
            {
                  // menampilkan analisis transaksi menggunakan metode rekursif
                  system("cls");
                  cout << "\n\t<= Analisis Transaksi =>\n\n";

                  int TotalPositif = 0, TotalNegatif = 0;
                  for (int i = 0; i < currentNasabahPtr->jumlahTrans; i++)
                  {
                        if (currentNasabahPtr->historiNasabah[i].nominal > 0)
                        {
                              TotalPositif += currentNasabahPtr->historiNasabah[i].nominal;
                        }
                        else
                        {
                              TotalNegatif += abs(currentNasabahPtr->historiNasabah[i].nominal);
                        }
                  }
                  cout << "Total pemasukan: Rp" << TotalPositif << ",00" << endl;
                  cout << "Total pengeluaran: Rp" << TotalNegatif << ",00" << endl;
                  // Menghitung jumlah transaksi berdasarkan tipe
                  map<string, int> TransByType;
                  map<string, int> AmountByType;

                  for (int i = 0; i < currentNasabahPtr->jumlahTrans; i++)
                  {
                        string tipe = currentNasabahPtr->historiNasabah[i].deskripsi;
                        TransByType[tipe]++;
                        AmountByType[tipe] += abs(currentNasabahPtr->historiNasabah[i].nominal);
                  }
                  cout << "\nJumlah transaksi berdasarkan tipe:\n";
                  for (const auto &pair : TransByType)
                  {
                        cout << pair.first << ": " << pair.second << " transaksi (Rp"
                             << AmountByType[pair.first] << ",00)" << endl;
                  }
                  cout << "\nTekan Enter untuk kembali ke menu Riwayat Transaksi";
                  cin.ignore();
                  cin.get();
                  Pause();
            }
            else if (pilihan == 4)
            {
                  exit = true;
            }
            else
            {
                  cout << "[ERROR INPUT] - pilihan yang Anda masukkan salah\n";
                  Pause();
            }
      } while (exit == false);
      Pause();
}
int TotalTransaksi(int MulaiIndex, int AkhirIndex)
{
      if (MulaiIndex > AkhirIndex) // enggak ada transaksi lagi untuk di sum
      {
            return 0;
      }
      if (MulaiIndex == AkhirIndex) // Transkasi hanya satu
      {
            return currentNasabahPtr->historiNasabah[MulaiIndex].nominal;
      }

      // Recursive menggunakan metode divide and conquer
      int IndexTengah = MulaiIndex + (AkhirIndex - MulaiIndex) / 2;
      return TotalTransaksi(MulaiIndex, IndexTengah) +
             TotalTransaksi(IndexTengah + 1, AkhirIndex);
}

// Function pause dilengkapi cls
void Pause()
{
      // REVIEW - Developing Mode ini di Deactivate dulu yaaa
      // cout << "\nSystem is ";
      // SlowType("processing", 100);                // menggunakan fungsi slowtype() mengetik dengan delay 100 milidetik setiap karakter
      // this_thread::sleep_for(chrono::seconds(1)); // memberikan jeda selama 1 detik menggunakan sleep_for dari library thread
      // DeleteText(10, 100);                        // menghapus 10 karakter dengan delay 100 milidetik
      // SlowType("completed", 100);                 // mengetik dengan delay 100 milidetik setiap karakter
      // cout << endl;
      // this_thread::sleep_for(chrono::seconds(1)); // memberikan jeda selama 1 detik menggunakan sleep_for dari library thread
      // system("cls");
      cout << "{Ini tadi ke Pause}" << endl;
}
// Animasi untuk pause
void SlowType(const string &kata, int delay) // menampilkan kata satu per satu dengan jeda waktu
{
      for (char c : kata) // membaca setiap karakter yang diinginkan
      {
            cout << c << flush;
            this_thread::sleep_for(chrono::milliseconds(delay));
      }
}
void DeleteText(int count, int delay) // menghapus karakter satu per satu dengan jeda waktu
{
      for (int i = 0; i < count; ++i)
      {
            cout << "\b \b" << flush;
            this_thread::sleep_for(chrono::milliseconds(delay)); // memberikan jeda pada penghapusan kata
      }
}
// Function Game Ular
void SetupSnake()
{
      snakeDir = STOP;
      snakeX = snakeWidth / 2;
      snakeY = snakeHeight / 2;
      fruitX = rand() % snakeWidth;
      fruitY = rand() % snakeHeight;
      snakeScore = 0;
      nTail = 0;
}
void DrawSnake()
{
      system("cls");
      for (int i = 0; i < snakeWidth + 2; i++)
            cout << "#";
      cout << endl;
      for (int i = 0; i < snakeHeight; i++)
      {
            for (int j = 0; j < snakeWidth; j++)
            {
                  if (j == 0)
                        cout << "#";
                  if (i == snakeY && j == snakeX)
                        cout << "O";
                  else if (i == fruitY && j == fruitX)
                        cout << "F";
                  else
                  {
                        bool printed = false;
                        for (int k = 0; k < nTail; k++)
                        {
                              if (tailX[k] == j && tailY[k] == i)
                              {
                                    cout << "o";
                                    printed = true;
                                    break;
                              }
                        }
                        if (!printed)
                              cout << " ";
                  }
                  if (j == snakeWidth - 1)
                        cout << "#";
            }
            cout << endl;
      }
      for (int i = 0; i < snakeWidth + 2; i++)
            cout << "#";
      cout << "\nScore: " << snakeScore << "\n";
      cout << "Use WASD to move. Press 'x' to exit game.\n";
}

void InputSnake()
{
      static char lastkey = '\0';
      if (_kbhit())
      {
            char key = _getch();
            if (key == lastkey)
            {
                  while (_kbhit())
                  {
                        _getch();
                  }
                  return;
            }
            lastkey = key;
            // mencegah bertabrakan dengan arah yang berlawanan
            switch (key)
            {
            case 'a':
            case 'A':
                  if (snakeDir != RIGHT)
                  {
                        snakeDir = LEFT;
                  }
                  break;
            case 'd':
            case 'D':
                  if (snakeDir != LEFT)
                  {
                        snakeDir = RIGHT;
                  }
                  break;
            case 'w':
            case 'W':
                  if (snakeDir != DOWN)
                  {
                        snakeDir = UP;
                  }
                  break;
            case 's':
            case 'S':
                  if (snakeDir != UP)
                  {
                        snakeDir = DOWN;
                  }
                  break;
            case 'x':
            case 'X':
                  snakeScore = -1;
                  break;
            default:
                  break;
            }
            // Flush input buffer
            while (_kbhit())
            {
                  _getch();
            }
      }
}

void LogicSnake()
{
      int prevX = tailX[0];
      int prevY = tailY[0];
      int prev2X, prev2Y;
      tailX[0] = snakeX;
      tailY[0] = snakeY;
      for (int i = 1; i < nTail; i++)
      {
            prev2X = tailX[i];
            prev2Y = tailY[i];
            tailX[i] = prevX;
            tailY[i] = prevY;
            prevX = prev2X;
            prevY = prev2Y;
      }
      switch (snakeDir)
      {
      case LEFT:
            snakeX--;
            break;
      case RIGHT:
            snakeX++;
            break;
      case UP:
            snakeY--;
            break;
      case DOWN:
            snakeY++;
            break;
      default:
            break;
      }
      // Collision border
      if (snakeX < 0 || snakeX >= snakeWidth || snakeY < 0 || snakeY >= snakeHeight)
      {
            snakeScore = -1; // mati jika kena border
            return;
      }
      // Collision ekor
      for (int i = 0; i < nTail; i++)
      {
            if (tailX[i] == snakeX && tailY[i] == snakeY)
            {
                  snakeScore = -1; // mati jika kena ekor
            }
      }
      // check jika snake makan fruit
      if (snakeX == fruitX && snakeY == fruitY)
      {
            snakeScore += 10;
            fruitX = rand() % snakeWidth;
            fruitY = rand() % snakeHeight;
            nTail++;
      }
}
void Snake()
{
      int pilihan;
      static bool firstRun = true;
      do
      {
            if (firstRun)
            {
                  TitleSnake();
                  firstRun = false;
            }
            SetupSnake();
            while (snakeScore != -1)
            {
                  DrawSnake();
                  InputSnake();
                  LogicSnake();
                  this_thread::sleep_for(chrono::milliseconds(100));
            }
            system("cls");
            cout << "\n\t<< Game over  >>\n\n";
            cout << left << setw(30) << "1> Play Again" << "\n";
            cout << left << setw(30) << "2> Exit to Main Menu" << "\n";
            cout << "\n>> ";
            cin >> pilihan;
      } while (pilihan == 1);
}
void TitleSnake()
{
      system("cls");
      cout << "\nSystem is ";
      SlowType("Rebooting", 100);
      this_thread::sleep_for(chrono::seconds(1));
      DeleteText(20, 100);
      SlowType("Ular_Senyum OS V 0.24 started", 150);
      cout << endl;
      this_thread::sleep_for(chrono::seconds(1));
      system("cls");
      cout << "\n\n";
      cout << "###############################################\n";
      cout << "#                                             #\n";
      cout << "#                 Ular Senyum OS              #\n";
      cout << "#                                             #\n";
      cout << "#        Press any key to start the game      #\n";
      cout << "#                                             #\n";
      cout << "###############################################\n";
#ifdef _WIN32
      _getch();
#else
      cin.ignore();
      cin.get();
#endif
}