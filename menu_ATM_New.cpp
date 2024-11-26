#include <iostream>
#include <iomanip>
using namespace std;

// >> Global Var Init
bool mainMenuLoop = false;
// >> Struct Init
struct histori
{
      int idTrans;
      int nominal;
      int noRektuj;
};

struct nasabah
{
      int noRek;
      string pass;
      string nama;
      int saldo;
      int jumlahTrans;
      histori historiNasabah[100];
};
// >> Inisialisasi Global Array (Wajib Ada)
nasabah dataNasabah[100]; // menyimpan 100 data 100 nasabah
int jumlahNasabah = 0;    // Total Jumlah Data yang dimiliki
// >> Decklare Funtion
// Error handling Func (Overload)
void inputHandling(string question, string &var, short lineOr); // Untuk String
void inputHandling(string question, int &var);                  // untuk int
void inputHandling(string question, float &var);                // untuk float
void inputHandling(string question, short &var);                // untuk short
// CRUD Data Nasabah
// -(OTW)
// Main Feature
void login();
void cekSaldo(int saldo);
void setorSaldo(int &saldo, int transaksi[], int &indexTransaksi);
void tarikSaldo(int &saldo, int transaksi[], int &indexTransaksi);
void transferRekening(int &saldo, int transaksi[], int &indexTransaksi);
void riwayatTransaksi(int transaksi[], int indexTransaksi);
void pause();

int main()
{
      unsigned short opt;
      // Identitas Pembuat
      cout << "\n<Selamat Datang di ATM Program ATM> \nKarya: "
           << " \t\n1. Zaka Ahmad Ghofari             (123240144)"
           << " \t\n2. Dimas Hafid Fathoni            (123240159)"
           << " \t\n3. Erlan Rifqi Davin Darmawan     (123240173)"
           << " \t\n4. Jauza Ilham Mahardika Putra    (123240174)"
           << " \t\n5. Rio Meidi Ataurrahman          (123240175)\n\n";
      do
      {
            // Penampil Menu Utama
            cout << "\t\t <<Menu ATM>>\n";
            cout << left << setw(25) << "1> Cek Saldo" << right << setw(25) << "Transfer Rekening <4\n\n";
            cout << left << setw(25) << "2> Deposit Saldo" << right << setw(25) << "Tarik Tunai <5\n\n";
            cout << left << setw(25) << "3> Riwayat Transaksi" << right << setw(25) << "Gajadi Hehe <6\n\n";
            // Input User
            cout << "\nNasabah Mau Yang Mana?: ";
            cin >> opt;
      } while (mainMenuLoop = false);
      return 0;
}
// >> Desc Function
// Error Handling Funct
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
                  getline(cin, var);
            }
            if (cin.fail())
            {
                  cin.clear();          // Menghapus Semua Fail Flag
                  cin.ignore(30, '\n'); // Mengabaikan Input
                  cout << "\n[Error Tag] -" << "Walah Mas Nginput apa njenengan\n";
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
                  cout << "\n[Error Tag] -" << "Walah Mas Nginput apa njenengan\n";
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
                  cout << "\n[Error Tag] -" << "Walah Mas Nginput apa njenengan\n";
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
                  cout << "\n[Error Tag] -" << "Walah Mas Nginput apa njenengan\n";
                  statLoop = true;
            }
            else
            {
                  statLoop = false;
            }
      } while (statLoop == true);
};
// Main Feature Funct Desc
// (Jauza)