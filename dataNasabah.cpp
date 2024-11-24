#include <iostream>
#include <map>
using namespace std;
struct identitas
{
      int noRek;
      string pass;
      string nama;
      int saldo;
      int history[100][3] = {}; //{idTrans,nominal,noRekTuj}
};
int main()
{
      map<int, identitas> data;
      identitas id;
      data[12345678] = {123345678, "Wahyu Syalalala", "Rio Meidi A", 200000, {{630000, 20000, 123456798}}};

      auto it = data.find(12345678);
      if (it != data.end())
      {
            // Mencetak key dan value
            cout << "Key: " << it->first << "\n"; // Key (nomor rekening)
            cout << "Nomor Rekening: " << it->second.noRek << "\n";
            cout << "Nama: " << it->second.nama << "\n";
            cout << "Saldo: " << it->second.saldo << "\n";

            // Mencetak histori transaksi
            cout << "Histori Transaksi:\n";
            for (int i = 0; i < 100; ++i)
            {
                  if (it->second.history[i][0] != 0)
                  {
                        cout << "  ID Transaksi: " << it->second.history[i][0]
                             << ", Nominal: " << it->second.history[i][1]
                             << ", Rekening Tujuan: " << it->second.history[i][2] << "\n";
                  }
            }
      }
      else
      {
            cout << "Data tidak ditemukan.\n";
      }
      return 0;
}