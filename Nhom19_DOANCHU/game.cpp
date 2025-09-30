#include "game.h"
#include "ve_nguoi.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cctype>
using namespace std;

#define SO_LAN_DOAN_SAI_TOI_DA 6

string chon_tu_bi_mat(const vector<string>& danh_sach_tu) {
    srand(static_cast<unsigned int>(time(0)));
    return danh_sach_tu[rand() % danh_sach_tu.size()];
}

void hien_thi_tu_hien_tai(const string& da_doan) {
    for (char c : da_doan) {
        cout << c << " ";
    }
    cout << endl;
}

bool xu_ly_doan(char ky_tu_doan, const string& tu_bi_mat, string& da_doan, int& so_chu_dung) {
    bool tim_thay = false;
    for (int i = 0; i < (int)tu_bi_mat.length(); ++i) {
        if (tu_bi_mat[i] == ky_tu_doan && da_doan[i] == '_') {
            da_doan[i] = ky_tu_doan;
            so_chu_dung++;
            tim_thay = true;
        }
    }
    return tim_thay;
}

void choi_game() {
    vector<string> danh_sach_tu = {
        "maytinh", "robot", "toan", "vatly", "dien", "congnghe",
        "chatgpt", "internet", "website", "phanmem", "phancung",
        "cambien", "solieu", "thietbi", "manhinh", "banphim",
        "chuot", "dienthoai", "chip", "mang", "wifi",
        "bluetooth", "server", "client", "database", "congcu"
    };

    string tu_bi_mat = chon_tu_bi_mat(danh_sach_tu);
    int do_dai_tu = tu_bi_mat.length();
    string da_doan(do_dai_tu, '_');
    int so_chu_dung = 0;
    int so_lan_sai = 0;
    char ky_tu_doan;

    cout << "=== GAME: NGUOI TREO CO ===\n\n";

    while (so_lan_sai < SO_LAN_DOAN_SAI_TOI_DA && so_chu_dung < do_dai_tu) {
        cout << "Tu hien tai: ";
        hien_thi_tu_hien_tai(da_doan);

        cout << "Hay doan mot chu cai: ";
        cin >> ky_tu_doan;
        ky_tu_doan = tolower(ky_tu_doan);

        if (xu_ly_doan(ky_tu_doan, tu_bi_mat, da_doan, so_chu_dung)) {
            cout << "Ban da doan dung!\n";
        }
        else {
            so_lan_sai++;
            cout << "Ban da doan sai! Con "
                << (SO_LAN_DOAN_SAI_TOI_DA - so_lan_sai) << " lan nua.\n";
        }

        ve_nguoi_treo_co(so_lan_sai);
    }

    if (so_chu_dung == do_dai_tu) {
        cout << " Chuc mung! Ban da doan dung tu: " << tu_bi_mat << "\n";
    }
    else {
        cout << " Ban da thua. Tu dung la: " << tu_bi_mat << "\n";
    }
}
