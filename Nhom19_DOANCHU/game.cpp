#include "game.h"
#include "ve_nguoi.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <vector>
using namespace std;

#define SO_LAN_DOAN_SAI_TOI_DA 6

string chon_tu_bi_mat(const vector<vector<string>>& danh_sach_tu, int n) {
    srand(static_cast<unsigned int>(time(0)));
    const vector<string>& chu_de = danh_sach_tu[n];
    return chu_de[rand() % chu_de.size()];
}

void hien_thi_tu_hien_tai(const string& da_doan) {
    for (char c : da_doan) cout << c << " ";
    cout << endl;
}

bool xu_ly_doan(char ky_tu_doan, const string& tu_thuong, string& da_doan, int& so_chu_dung) {
    bool tim_thay = false;
    for (int i = 0; i < (int)tu_thuong.length(); ++i) {
        if (tu_thuong[i] == ky_tu_doan && da_doan[i] == '_') {
            da_doan[i] = tu_thuong[i];
            so_chu_dung++;
            tim_thay = true;
        }
    }
    return tim_thay;
}

void choi_game() {
    vector<vector<string>> danh_sach_tu(3);
    danh_sach_tu[1] = { "Computer", "Monitor", "Keyboard", "Mouse", "Processor",
                        "Memory", "Hard drive", "Software", "Hardware",
                        "Operating system", "Program", "Application", "Network",
                        "Internet", "Download", "Upload", "File", "Folder",
                        "Data", "Database" };
    danh_sach_tu[2] = { "Electronics", "Telecommunications", "Circuit", "Resistor",
                        "Capacitor", "Inductor", "Transistor", "Diode",
                        "Microcontroller", "Signal", "Frequency", "Antenna",
                        "Modulation", "Amplifier", "Oscillator", "Bandwidth",
                        "Transmission", "Receiver", "Sensor", "Wireless" };

    cout << "=== GAME: NGUOI TREO CO ===\n\n";
    cout << "1. Chu de may tinh\n2. Chu de dien tu\nNhap lua chon (1 hoac 2): ";

    int n; char a;
    cin >> a;
    if (a != '1' && a != '2') {
        cout << "Lua chon khong hop le. Mac dinh chon chu de 1.\n";
        n = 1;
    }
	else n = int(a) - '0';

    string tu_bi_mat = chon_tu_bi_mat(danh_sach_tu, n);
    string tu_thuong = tu_bi_mat;
    for (char& c : tu_thuong) c = tolower(c);

    string da_doan(tu_bi_mat.length(), '_');
    int so_chu_dung = 0, so_lan_sai = 0;
    char ky_tu_doan;
    string S = "a b c d e f g h i j k l m n o p q r s t u v w x y z";
    ve_nguoi_treo_co(so_lan_sai);
    while (so_lan_sai < SO_LAN_DOAN_SAI_TOI_DA && so_chu_dung < tu_bi_mat.length()) {
        cout << "\nTu hien tai: ";
        hien_thi_tu_hien_tai(da_doan);
        cout << "Ky tu chua doan: " << S << endl;
        cout << "Hay doan mot chu cai: ";
        cin >> ky_tu_doan;
        ky_tu_doan = tolower(ky_tu_doan);

        if (S.find(ky_tu_doan) == string::npos) {
            cout << "Ky tu khong hop le\n";
            continue;
        }

        if (xu_ly_doan(ky_tu_doan, tu_thuong, da_doan, so_chu_dung)) {
            cout << "Ban da doan dung!\n";
        }
        else {
            so_lan_sai++;
            cout << "Sai roi con vo! Phai chiuuu! Con " << (SO_LAN_DOAN_SAI_TOI_DA - so_lan_sai) << " lan nua.\n";
        }

        ve_nguoi_treo_co(so_lan_sai);
        for (char& c : S) {
            if (c == ky_tu_doan) c = ' ';
        }
    }

    if (so_chu_dung == tu_bi_mat.length()) {
        cout << "\n Chuc mung! Tu dung la: " << tu_bi_mat << "\n";
    }
    else {
        cout << "\n Ban da thua. Tu dung la: " << tu_bi_mat << "\n";
    }
}
