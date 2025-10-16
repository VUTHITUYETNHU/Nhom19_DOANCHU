#include "game.h"
#include "ve_nguoi.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <vector>
#include <string>
#include <windows.h>
#include <thread>
#include <chrono>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
using namespace std;

#define SO_LAN_DOAN_SAI_TOI_DA 6

int diem_so = 0;
int so_van_thang = 0, so_van_thua = 0;

// =============== HÀM HIỆU ỨNG ===============
void doiMau(int mau) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), mau);
}

void inChamCham(const string& text, int delayMs) {
    for (char c : text) {
        cout << c << flush;
        this_thread::sleep_for(chrono::milliseconds(delayMs));
    }
    cout << endl;
}

void amThanhDung() { Beep(1000, 100); }
void amThanhSai() { Beep(300, 200); }
void amThanhThang() {
    PlaySound(TEXT("win.wav"), NULL, SND_FILENAME | SND_ASYNC);
}
void amThanhThua() {
    PlaySound(TEXT("lose.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

// =============== HÀM ĐỌC FILE ===============
vector<pair<string, vector<string>>> doc_tu_tu_file(const string& ten_file) {
    ifstream file(ten_file);
    vector<pair<string, vector<string>>> danh_sach_tu;

    if (!file.is_open()) {
        cerr << "Khong the mo file " << ten_file << endl;
        return danh_sach_tu;
    }

    string dong;
    string ten_chu_de;
    vector<string> tu_trong_chu_de;

    while (getline(file, dong)) {
        if (dong.empty()) continue;
        if (dong[0] == '#') {
            if (!tu_trong_chu_de.empty()) {
                danh_sach_tu.push_back({ ten_chu_de, tu_trong_chu_de });
                tu_trong_chu_de.clear();
            }
            ten_chu_de = dong.substr(1); // Lấy tên sau dấu #
            if (!ten_chu_de.empty() && ten_chu_de[0] == ' ') ten_chu_de.erase(0, 1);
        } else {
            tu_trong_chu_de.push_back(dong);
        }
    }

    if (!tu_trong_chu_de.empty()) {
        danh_sach_tu.push_back({ ten_chu_de, tu_trong_chu_de });
    }

    file.close();
    return danh_sach_tu;
}

// =============== HÀM PHỤ TRỢ ===============
string chon_tu_bi_mat(const vector<pair<string, vector<string>>>& danh_sach_tu, int n) {
    srand(static_cast<unsigned int>(time(0)));
    const vector<string>& tu_trong_chu_de = danh_sach_tu[n].second;
    return tu_trong_chu_de[rand() % tu_trong_chu_de.size()];
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

// =============== HÀM KHỞI TẠO GAME ===============
void khoi_tao_game(vector<pair<string, vector<string>>>& danh_sach_tu, string& tu_bi_mat,
    string& tu_thuong, string& da_doan, int& n, bool& che_do_hai_nguoi) {
    doiMau(11);
    inChamCham("=== GAME: NGUOI TREO CO ===\n", 20);
    doiMau(7);
    cout << "\nChon che do choi:\n1. 1 nguoi (tu trong thu vien)\n2. 2 nguoi (nguoi 1 nhap tu bi mat)\nLua chon: ";
    char chon;
    cin >> chon;

    if (chon == '2') {
        che_do_hai_nguoi = true;
        cout << "Nguoi 1, hay nhap tu bi mat: ";
        cin.ignore();
        getline(cin, tu_bi_mat);
        tu_thuong = tu_bi_mat;
        for (char& c : tu_thuong) c = tolower(c);
        da_doan.assign(tu_bi_mat.length(), '_');
        system("cls");
        inChamCham("Bat dau choi nao!\n", 40);
        return;
    }

    che_do_hai_nguoi = false;
    danh_sach_tu = doc_tu_tu_file("data.txt");
    if (danh_sach_tu.empty()) {
        cerr << "Loi: Khong co du lieu trong file data.txt\n";
        exit(1);
    }

    cout << "\n=== CHU DE CO SAN ===\n";
    for (int i = 0; i < (int)danh_sach_tu.size(); ++i) {
        cout << i + 1 << ". " << danh_sach_tu[i].first << endl;
    }

    cout << "Nhap lua chon chu de: ";
    char a;
    cin >> a;
    n = (a - '0') - 1;
    if (n < 0 || n >= (int)danh_sach_tu.size()) {
        cout << "Lua chon khong hop le. Mac dinh chon chu de 1.\n";
        n = 0;
    }

    tu_bi_mat = chon_tu_bi_mat(danh_sach_tu, n);
    tu_thuong = tu_bi_mat;
    for (char& c : tu_thuong) c = tolower(c);
    da_doan.assign(tu_bi_mat.length(), '_');
}

// =============== HÀM CHƠI GAME CHÍNH ===============
void choi_game() {
    bool tiep_tuc = true;

    while (tiep_tuc) {
        vector<pair<string, vector<string>>> danh_sach_tu;
        string tu_bi_mat, tu_thuong, da_doan;
        int n;
        bool che_do_hai_nguoi = false;

        khoi_tao_game(danh_sach_tu, tu_bi_mat, tu_thuong, da_doan, n, che_do_hai_nguoi);

        int so_chu_dung = 0, so_lan_sai = 0, sai_lien_tiep = 0;
        bool da_goi_y = false;
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

            if (ky_tu_doan < 'a' || ky_tu_doan > 'z') {
                doiMau(12);
                cout << "Vui long nhap mot chu cai hop le (a-z)!\n";
                doiMau(7);
                continue;
            }
            if (S.find(ky_tu_doan) == string::npos) {
                doiMau(14);
                cout << "Ky tu da duoc doan, vui long chon ky tu khac!\n";
                doiMau(7);
                continue;
            }

            if (xu_ly_doan(ky_tu_doan, tu_thuong, da_doan, so_chu_dung)) {
                doiMau(10);
                cout << "Ban da doan dung!\n";
                amThanhDung();
                doiMau(7);
                sai_lien_tiep = 0;
            }
            else {
                so_lan_sai++;
                sai_lien_tiep++;
                doiMau(12);
                cout << " Sai roi! Con " << (SO_LAN_DOAN_SAI_TOI_DA - so_lan_sai) << " lan nua.\n";
                doiMau(7);
                amThanhSai();
            }

            for (char& c : S) if (c == ky_tu_doan) c = ' ';
            ve_nguoi_treo_co(so_lan_sai);

            // ===== GỢI Ý =====
            if (sai_lien_tiep >= 3 && !da_goi_y) {
                cout << "\nBan da doan sai 3 lan lien tiep. Ban co muon nhan goi y khong? (y/n): ";
                char chon;
                cin >> chon;
                if (chon == 'y' || chon == 'Y') {
                    for (int i = 0; i < tu_bi_mat.length(); ++i) {
                        if (da_doan[i] == '_') {
                            doiMau(11);
                            cout << "Goi y: Tu nay co chua chu cai '" << tu_thuong[i] << "'\n";
                            doiMau(7);
                            break;
                        }
                    }
                    da_goi_y = true;
                }
                sai_lien_tiep = 0;
            }
        }

        // ===== KẾT QUẢ =====
        if (so_chu_dung == tu_bi_mat.length()) {
            doiMau(10);
            amThanhThang();
            inChamCham("\nChuc mung! Tu dung la: " + tu_bi_mat, 30);
            doiMau(7);
            diem_so += 10;
            so_van_thang++;
        }
        else {
            doiMau(12);
            amThanhThua();
            inChamCham("\nBan da thua. Tu dung la: " + tu_bi_mat, 30);
            doiMau(7);
            diem_so -= 5;
            so_van_thua++;
        }

        cout << "\n===== THONG KE =====\n";
        diem_so = max(diem_so, 0);
        cout << "Diem hien tai: " << diem_so << endl;
        cout << "So van thang: " << so_van_thang << endl;
        cout << "So van thua: " << so_van_thua << endl;

        cout << "\nBan co muon choi lai khong? (y/n): ";
        char choilai;
        cin >> choilai;
        tiep_tuc = (choilai == 'y' || choilai == 'Y');
    }

    doiMau(11);
    inChamCham("\nCam on ban da choi tro choi Nguoi Treo Co!\n", 40);
    doiMau(7);
}
