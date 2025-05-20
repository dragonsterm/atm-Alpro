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
#include <filesystem>
// Namespace
using namespace std;
using json = nlohmann::json;
namespace fs = std::filesystem;
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
// SECTION - Global Var Init
bool mainMenuLoop = true, loginStat = false, adminStat = false;
int indexNasabah = -1;
nasabah *currentNasabahPtr = nullptr;
nasabah currentNasabah, anotherNasabah;
string pathJsonCurNasabah, pathJsonAnotherNasabah, dbPath = "../db/json/", dbBackupPath = "../db/backup/json";
// >> Inisialisasi Global Array (Wajib Ada)
nasabah dataNasabah[100];       // menyimpan 100 data 100 nasabah
int jumlahNasabah = 0;          // Total Jumlah Data yang dimiliki
vector<string> pathNasabahJson; // untuk dumping semua file json nasabah
// !SECTION
// SECTION - Function Declaration
// >> Declare Funtion
// Error handling Func (Overload)
void inputHandling(string question, string &var, short lineOr); // Untuk String
void inputHandling(string question, int &var);                  // untuk int
void inputHandling(string question, float &var);                // untuk float
void inputHandling(string question, short &var);                // untuk short]
// Multifile Manajemen System
bool noRekVal(int noRekIn, string &pathJsonCurNasabah, bool mode);           // pengecekan apakah nomor rekening terdapat dalam database (validasi)
void importDataJson(string jsonPathIn, nasabah &nasabahExport);              // membaca data nasabah dari file JSON
void exportNasabahJson(string jsonPathIn, nasabah nasabahExport, bool mode); // menyimpan data nasabah ke file JSON
void importDb(nasabah *dataNasabahOut, int &jumlahNasabah);                  // mengimpor seluruh file JSON dari folder ke array dataNasabah
void restoreBackupDb(string dbBackupPath);                                   // mengembalikan data dari folder backup ke folder aktif
// CRUD Funct Data Nasabah
void tampilDataNasabah(); // menampilkan seluruh data nasabah dan histori transaksi
void inputNasabah();      // menambahkan nasabah baru
void delNasabah();        // menghapus file JSON nasabah berdasarkan nomor rekening
// Main Feature
void login();                    // memulai proses login
void loginAttempt(int Attempts); // fungsi rekursif untuk percobaan login (max 3 kali)
void cekSaldo();                 // menampilkan saldo nasabah
void setorSaldo();               // menambah saldo nasabah
void tarikSaldo();               // mengurangi saldo nasabah (tarik tunai)
void transferSaldo();            // transfer uang antar nasabah (menggunakan nomor rekening)
void riwayatTransaksi();         // menampilkan histori transaksi dengan fitur halaman & analisis
// >> Menu
void loopMenu();  // menjalankan menu secara terus menerus
void menuAdmin(); // menu khusus admin (input, hapus, tampilkan nasabah, restore backup)
void menuUtama(); // menu utama nasabah (cek saldo, setor, tarik, transfer, riwayat)
// Function Sort
void sortByNoRek();                    // mengurutkan nasabah berdasarkan rekening (bubble sort)
void sortBySaldo();                    // mengurutkan berdasarkan saldo (selection sort)
void sortHistoriTransaksi(nasabah &n); // mengurutkan histori transaksi nasabah (bubble sort)
// Perhitungan total transaksi
int TotalTransaksi(int MulaiIndex, int AkhirIndex); // menghitung total nominal transaksi secara rekursif
// Pause Function
void Pause(); // jeda & visual antar menu
// Function untuk Pause
void SlowType(const string &kata, int delay); // efek animasi mengetik
void DeleteText(int count, int delay);        // penghapusan text
// Function untuk SnakeGame
void Snake();      // game loop
void SetupSnake(); // inisialisasi posisi awal ular & fruit
void DrawSnake();  // menggambar ular ke tampilan
void InputSnake(); // menerima input user
void LogicSnake(); // logika gerakan ular & tabrakan
void TitleSnake(); // tampilan awal game
// !SECTION
// SECTION - Snake Function
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
// !SECTION
// SECTION - Main Program
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
      // Login dan Masuk Main Menu
      loopMenu();
      return 0;
}

// NOTE - Main untuk Developing
// int main0(int argc, char const *argv[])
// {
//       menuAdmin();
//       return 0;
// }

// Menu Admin dan Menu Utama
/**
 * @brief Melooping Menu dengan terus menerus sampai didapatkan loginStat == true
 *
 */
void loopMenu()
{
      do
      {
            loginStat = false;
            system("cls");
            login();
            while (mainMenuLoop == true)
            {
                  if (adminStat == true)
                  {
                        // Load Data all nasabah
                        importDb(dataNasabah, jumlahNasabah);
                        cout << "[Completed Load] - Jumlah Nasabah : " << jumlahNasabah << endl;
                        menuAdmin();
                  }
                  else
                  {
                        menuUtama();
                  }
            };
      } while (loginStat == false);
}
/**
 * @brief Menu Untuk Admin
 *
 */
void menuAdmin()
{
      short opt;
      system("cls");
      cout << "\t\t <<Menu Admin>>\n\n";
      cout << left << setw(25) << "1> Input Nasabah" << right << setw(25) << "Backup Data <4\n\n";
      cout << left << setw(25) << "2> Delete Nasabah" << right << setw(25) << "\n\n";
      cout << left << setw(25) << "3> Tampil Nasabah" << right << setw(25) << "Exit Admin <6\n\n";
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
            Pause();
            restoreBackupDb(dbBackupPath);
            break;
      case 6:
            cout << "\nTerima kasih telah menggunakan ATM ini Mas Admin\n";
            adminStat = false;
            mainMenuLoop = false;
            currentNasabahPtr = nullptr;
            // Ngosongin Struct Jumlah Nasabah biar ga makan memori dia ini sudah kehapus tapi gamau kehapus
            for (int i = 0; i < jumlahNasabah; ++i)
            {
                  dataNasabah[i] = nasabah{};
            }
            break;
      default:
            Pause();
            cout << "[error System] - Jane Ngopo si jenengan niku?" << endl;
            break;
      }
}

//  Menu Utama
/**
 * @brief Menampilkan menu utama
 *
 */
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
            currentNasabahPtr = nullptr;
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
 * @brief Menduplikat database yang ada di folder backup kedalam folder json yang ada di workspace cwd
 *
 * @param dbBackupPath
 */
void restoreBackupDb(string dbBackupPath)
{
      try
      {
            // Hapus semua file/folder di dbPath
            if (fs::exists(dbPath))
            {
                  for (const auto &entry : fs::directory_iterator(dbPath))
                  {
                        fs::remove_all(entry.path());
                  }
            }
            else
            {
                  fs::create_directories(dbPath);
            }
            // Duplikat seluruh isi folder backup ke dbPath (termasuk subfolder dan file .json)
            for (const auto &entry : fs::recursive_directory_iterator(dbBackupPath))
            {
                  fs::path relativePath = fs::relative(entry.path(), dbBackupPath);
                  fs::path targetPath = fs::path(dbPath) / relativePath;

                  if (fs::is_directory(entry))
                  {
                        fs::create_directories(targetPath);
                  }
                  else if (fs::is_regular_file(entry) && entry.path().extension() == ".json")
                  {
                        fs::copy_file(entry.path(), targetPath, fs::copy_options::overwrite_existing);
                  }
            }
            cout << "[Completed] - Restore database dari backup selesai.\n";
            cout << "\nTekan karakter apapun untuk melanjutkan...";
            cin.get();
            cin.clear();
            cin.ignore(1000, '\n');
      }
      catch (const std::exception &e)
      {
            cout << "[Error] - Gagal restore backup: " << e.what() << endl;
      }
}
/**
 * @brief Menverifikasi No Rek Apakah ada didalam DB atau tidak
 * @return mereturn Boolean True jika ada False jika tidak
 *
 * @param noRekIn nomor Rekening Masukan untuk dicek
 * @param pathJsonCurNasabah global variabel yang mengatur path json yang diperlukan
 */
bool noRekVal(int noRekIn, string &pathJsonCurNasabah, bool mode)
{

      string pathJsonCurNasabahInLoc;
      // Make Path for JSON
      pathJsonCurNasabahInLoc = "../db/json/" + to_string(noRekIn) + ".json";
      ifstream validate(pathJsonCurNasabahInLoc);
      bool statFile = validate.is_open();
      if (mode == 1)
      {
            if (statFile == 1)
            {
                  pathJsonCurNasabah = pathJsonCurNasabahInLoc;
            }
      }
      else
      {
            pathJsonCurNasabah = pathJsonCurNasabahInLoc;
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
            // Admin dikecualikan karena admin tidak memiliki histori transaksi akan membuat error
            if (nasabahExport.noRek != 459777345)
            {
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
      }
      else
      {
            cout << "[Err Opening File] - Something Seems Off in This file" << endl;
      }
      importStream.close();
}
/**
 * @brief Mengimport seluruh file json yang ada di folder database cwd kedalam struct
 *
 * @param dataNasabahOut
 * @param jumlahNasabah
 */
void importDb(nasabah *dataNasabahOut, int &jumlahNasabah)
{
      // Untuk memastikan data sebelumnya terhapus dari program
      pathNasabahJson.clear(); // Hapus dari vector
      for (int i = jumlahNasabah - 1; i >= 0; --i)
      {
            dataNasabah[i] = nasabah{}; // Reset objek
      }
      jumlahNasabah = 0;
      // Loop Buat ngemasukin semua filse json di db
      for (const auto &inputJsonNasabah : fs::directory_iterator(dbPath))
      {
            string filename = inputJsonNasabah.path().filename().string();
            // hanya memproses dengan format nama tertentu dengan minim nama 4 char
            if (filename.size() >= 13 && filename.substr(filename.size() - 5) == ".json")
            {
                  string norekStr = filename.substr(0, filename.size() - 5);
                  if ((norekStr.size() == 8 || norekStr.size() == 9) && norekStr != "459777345")
                  {
                        pathNasabahJson.push_back(inputJsonNasabah.path().string());
                  }
            }
      }
      // Akan Mengimport semua ke dalam struct dataNasabah
      for (const auto &jsonPath : pathNasabahJson)
      {
            importDataJson(jsonPath, dataNasabahOut[jumlahNasabah]);
            jumlahNasabah++;
      }
}
/**
 * @brief Fungsi untuk mengexport nasabah kedalam sebuah file JSON
 *
 * @param jsonPathOut path json untuk keluar kemana
 * @param nasabahExport struct nasabah yang mau di export
 */
void exportNasabahJson(string jsonPathOut, nasabah nasabahExport, bool mode)
{
      ofstream exportStream;
      if (mode == true)
      {
            exportStream.open(jsonPathOut, ios::trunc);
      }
      else
      {
            exportStream.open(jsonPathOut, ios::out);
      }
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

/**
 * @brief fungsi rekursif untuk mencoba login hingga maksimal 3 kali
 * @param Attempts  jumlah kesempatan login yang tersisa
 */
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
            loginStat = true;
            mainMenuLoop = false;
            cout << "\nTerima Kasih Telah Menggunakan ATM Kami:D" << endl;
            return;
      }
      // Untuk Lanjut ke Password
      else if (noRekVal(noRekIn, pathJsonCurNasabah, 1))
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
                  mainMenuLoop = true;
            }
            else // kalau bukan admin
            {
                  adminStat = false;
                  mainMenuLoop = true;
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
      cout << "\n[Login Gagal] - Kesempatan Anda Tersisa " << (Attempts - 1) << endl;
      loginAttempt(Attempts - 1);
}
// Fungsi Login
/**
 * @brief fungsi utama untuk memulai proses login user
 *
 */
void login()
{
      loginAttempt(3);
}

// Overloading Funct untuk Error Handling Pengguna
/**
 * @brief fungsi overloading untuk menangani input streing
 * @param question pertanyaan yang ditampilkan
 * @param var variabel untuk meyimpan input string
 * @param lineOr 1 untuk tanpa spasi, 2 untuk input dengan spasi
 */
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
/**
 * @brief fungsi overloading untuk menangani input integer
 * @param question pertanyaan yang ditampilkan
 * @param var variabel untuk meyimpan input integer
 */
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
/**
 * @brief fungsi overloading untuk menangani input float
 * @param question pertanyaan yang ditampilkan
 * @param var variabel untuk meyimpan input float
 */
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
/**
 * @brief fungsi overloading untuk menangani input short
 * @param question pertanyaan yang ditampilkan
 * @param var variabel untuk meyimpan input short
 */
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

// CRUD Funct
/**
 * @brief mengurutkan data nasabah berdasarkan nomor rekening secara bubble sort
 */
void sortByNoRek()
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

/**
 * @brief Mengurutkan data nasabah berdasarkan saldo secara selection sort
 */
void sortBySaldo()
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
/**
 * @brief mengurutkan histori transaksi berdasarkan nominal dan id transaksi
 * @param n struct nasabah yang akan diurutkan histori transaksinya
 */
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

/**
 * @brief menampilkan daftar semua nasabah dan detail transaksinya
 */
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
      // Pause(); //FIXME
      inputHandling("Berapa Nomor Rekeningnya? (exit: 404) \n >> ", noRekIn);
      if (noRekIn == 404)
      {
            return;
      }
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
/**
 * @brief menambahkan nasabah baru ke database
 */
void inputNasabah()
{
      system("cls");
      Pause();
      cout << "\n\n\t>-Menu Input Nasabah Baru-<\n";
      cout << "\n\nHallo Nasabah Baru!!";
      cout << "\n Data Nasabah:" << endl;
      inputHandling("\nNo Rek  : ", dataNasabah[jumlahNasabah].noRek);
      if (noRekVal(dataNasabah[jumlahNasabah].noRek, pathJsonAnotherNasabah, 0) == false)
      {
            inputHandling("\nPass    : ", dataNasabah[jumlahNasabah].pass, 1);
            inputHandling("\nNama    : ", dataNasabah[jumlahNasabah].nama, 2);
            inputHandling("\nSaldo   : ", dataNasabah[jumlahNasabah].saldo);
            dataNasabah[jumlahNasabah].jumlahTrans = 0; // Nasabah Baru Tidak Memiliki Histori Transaksi
            jumlahNasabah++;
            exportNasabahJson(pathJsonAnotherNasabah, dataNasabah[jumlahNasabah - 1], false);
            cout << "Status Pembuatan :" << noRekVal(dataNasabah[jumlahNasabah - 1].noRek, pathJsonAnotherNasabah, 0) << endl;
            pathJsonAnotherNasabah = "";
      }
      else
      {
            pathJsonAnotherNasabah = "";
            dataNasabah[jumlahNasabah].noRek = 0;
      }
}

// Fungsi untuk delete data nasabah
/**
 * @brief menghapus data nasabah dari sistem berdasarkan nomor rekening
 */
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
            if (noRekVal(noRekIn, pathJsonCurNasabah, 1))
            {
                  try
                  {
                        if (fs::exists(pathJsonCurNasabah))
                        {
                              find = true;
                              fs::remove(pathJsonCurNasabah);
                              cout << "\nFile JSON untuk rekening " << noRekIn << " berhasil dihapus.\n";
                              jumlahNasabah--;
                        }
                        else
                        {
                              cout << "\n[Warning] - File JSON tidak ditemukan.\n";
                        }
                  }
                  catch (const std::exception &e)
                  {
                        cout << "\n[Error] - Gagal menghapus file JSON: " << e.what() << endl;
                  }
            }
            else
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
/**
 * @brief menampilkan saldo nasabah yang sedang login
 */
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
/**
 * @brief melakukan penyetoran saldo ke rekening nasabah
 */
void setorSaldo()
{
      int jumlah;
      short pilihan;
      srand(time(0)); // mengatur seed pada rand() untuk menggunakan waktu saat ini memastikan tidak membawa seed bawaan ctime
      do
      {
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
                  exportNasabahJson(pathJsonCurNasabah, currentNasabah, true);
                  // FIXME - lagi Dev
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
/**
 * @brief melakukan penarikan tunai dari saldo nasabah
 */
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
                        exportNasabahJson(pathJsonCurNasabah, currentNasabah, true);
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
/**
 * @brief melakukan transfer saldo dari satu nasabah ke nasabah lain
 */
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
                  if (noRekVal(rekening_tujuan, pathJsonAnotherNasabah, 1))
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
                        exportNasabahJson(pathJsonCurNasabah, currentNasabah, true);
                        // Update Data Nasabah - Penerima
                        targetNasabah->saldo += jumlah;
                        targetNasabah->historiNasabah[targetNasabah->jumlahTrans].nominal = jumlah;
                        targetNasabah->historiNasabah[targetNasabah->jumlahTrans].idTrans = rand() % 90000000 + 10000000; // Nomor Id acak mengambil dari rentang 0 hingga 89999999 kemudian ditambahkan 10000000
                        targetNasabah->historiNasabah[targetNasabah->jumlahTrans].noRektuj = currentNasabahPtr->noRek;
                        targetNasabah->historiNasabah[targetNasabah->jumlahTrans].deskripsi = "Menerima Saldo";
                        targetNasabah->jumlahTrans++;
                        exportNasabahJson(pathJsonAnotherNasabah, *targetNasabah, true);
                        // FIXME - Dev Mode
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
/**
 * @brief Menampilkan riwayat transaksi nasabah dan analisis data
 */
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

/**
 * @brief menghitung total nominal transaksi secara rekursif
 * @param MulaiIndex indeks awal transaksi
 * @param AkhirIndex indeks akhir transaksi
 * @return total nominal transaksi dari range tersebut
 */
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
/**
 * @brief fungsi jeda (pause) program dengan  animasi
 */
void Pause()
{
      // FIXME - Developing Mode ini di Deactivate dulu yaaa
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
/**
 * @brief menampilkan teks satu per satu dengan jeda untuk efek mengetik
 * @param kata teks yang akan ditampilkan
 * @param delay delay per karakter dalam milidetik
 */
void SlowType(const string &kata, int delay) // menampilkan kata satu per satu dengan jeda waktu
{
      for (char c : kata) // membaca setiap karakter yang diinginkan
      {
            cout << c << flush;
            this_thread::sleep_for(chrono::milliseconds(delay));
      }
}
/**
 * @brief menghapus karakter dari layar dengan efek animasi
 * @param count jumlah karakter yang dihapus
 * @param delay delay per karakter dalam milidetik
 */
void DeleteText(int count, int delay) // menghapus karakter satu per satu dengan jeda waktu
{
      for (int i = 0; i < count; ++i)
      {
            cout << "\b \b" << flush;
            this_thread::sleep_for(chrono::milliseconds(delay)); // memberikan jeda pada penghapusan kata
      }
}
// !SECTION
// Function Game Ular
/**
 * @brief inisialisasi posisi ular dan buah dalam game
 */
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
/**
 * @brief menampilkan board permainan ular
 */
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
/**
 * @brief menerima input pengguna untuk arah gerakan ular
 */
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
/**
 * @brief logika pergerakan ular dan penanganan tabrakan
 */
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
/**
 * @brief menjalankan game ular dengan skor
 */
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
/**
 * @brief menampilkan splash screen sebelum game dimulai
 */
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