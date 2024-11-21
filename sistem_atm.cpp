#include <iostream>
using namespace std;

void cekSaldo(int saldo)
{
    cout << "Saldo Anda saat ini adalah Rp. " << saldo << endl;
}
void setorSaldo(int &saldo, int transaksi[], int &indexTransaksi)
{
    int jumlah;
    cout << "Masukkan jumlah saldo yang ingin ditransfer : ";
    cin >> jumlah;
    saldo += jumlah;
    transaksi[indexTransaksi++] = jumlah;
}
void tarikSaldo(int &saldo, int transaksi[], int &indexTransaksi)
{
    int jumlah;
    cout << "Masukkan jumlah saldo yang ingin ditarik : ";
    cin >> jumlah;
    if (saldo < jumlah)
    {
        cout << "Saldo Anda belum mencukupi untuk melakukan penarikan.\n";
    }
    else
    {
        saldo -= jumlah;
        cout << "Saldo anda saat ini tersisa : " << saldo << endl;
    }

    transaksi[indexTransaksi++] = -jumlah;
}
void riwayatTransaksi(int transaksi[], int indexTransaksi)
{
    cout << "Riwayat Transaksi:\n";
    for (int i = 0; i < indexTransaksi; i++)
    {
        if (transaksi[i] > 0)
        {
            cout << i + 1 << ". Setor: Rp" << transaksi[i] << endl;
        }
        else
        {
            cout << i + 1 << ". Tarik: Rp" << -transaksi[i] << endl;
        }
    }
}

int main()
{
    string username;
    string password;
    string correct_usn = "ZakaKecil";
    string correct_pass = "Halo123";
    const int max_chance = 3;
    int chance = 0;
    int transaksi[100];
    int indexTransaksi = 0;
    int saldo = 0;
    int choice;
    do
    {
        cout << "Input username : ";
        getline(cin >> ws, username);
        cout << "Input password : ";
        getline(cin >> ws, password);

        if ((username == correct_usn) && (password == correct_pass))
        {
            cout << "Username dan password yang dimasukkan benar.\n";
            break;
        }
        else if (username != correct_usn)
        {
            chance++;
            cout << "Username yang dimasukkan salah !\n";
            cout << "Kesempatan Anda tersisa " << (max_chance - chance) << endl;
        }
        else if (password != correct_pass)
        {
            chance++;
            cout << "Password yang dimasukkan salah\n";
            cout << "Kesempatan Anda tersisa " << (max_chance - chance) << endl;
        }
        else if ((username != correct_usn) && (password != correct_pass))
        {
            cout << "Username dan Password yang dimasukkan salah\n";
            cout << "Kesempatan Anda tersisa " << (max_chance - chance) << endl;
        }
    } while (chance < max_chance);
    if (chance == max_chance)
    {
        cout << "Kesempatan Anda habis\n";
        return 0;
    }
    do
    {
        cout << "\t\tSistem ATM\n";
        cout << "1. Cek Saldo\n";
        cout << "2. Setor Saldo\n";
        cout << "3. Tarik Saldo\n";
        cout << "4. Riwayat Transaksi\n";
        cout << "5. Keluar\n";
        cout << "Masukkan pilihan : ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            cekSaldo(saldo);
            break;
        case 2:
            setorSaldo(saldo, transaksi, indexTransaksi);
            break;
        case 3:
            tarikSaldo(saldo, transaksi, indexTransaksi);
            break;
        case 4:
            riwayatTransaksi(transaksi, indexTransaksi);
            break;
        case 5:
            cout << "Terimakasih telah menggunakan ATM ini, Anda kena hack wkwkwk 😂😂😂😂😂😂😂😂😂😍😍😍😍😍😍😍😘😘🤣🤣🤣🤣" << endl;
            return 0;
            break;
        default:
            break;
        }
    } while (choice != 5);
    return 0;
}