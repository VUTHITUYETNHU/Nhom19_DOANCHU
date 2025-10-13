#include "game.h"
#include "ve_nguoi.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <vector>
#include <string>
using namespace std;

#define SO_LAN_DOAN_SAI_TOI_DA 6

int diem_so = 0;
int so_van_thang = 0, so_van_thua = 0;

// =============== HÀM ĐỌC FILE ===============
vector<vector<string>> doc_tu_tu_file(const string& ten_file) {
    ifstream file(ten_file);
    vector<vector<string>> danh_sach_tu;
    if (!file.is_open()) {
        cerr << "Khong the mo file " << ten_file << endl;
        return danh_sach_tu;
    }

    string dong;
    vector<string> chu_de_hien_tai;

    while (getline(file, dong)) {
        if (dong.empty()) continue;
        if (dong[0] == '#') {
            if (!chu_de_hien_tai.empty()) {
                danh_sach_tu.push_back(chu_de_hien_tai);
                chu_de_hien_tai.clear();
            }
        }
        else {
            chu_de_hien_tai.push_back(dong);
        }
    }
    if (!chu_de_hien_tai.empty()) danh_sach_tu.push_back(chu_de_hien_tai);
    file.close();
    return danh_sach_tu;
}

// =============== HÀM PHỤ TRỢ ===============
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

// =============== HÀM KHỞI TẠO GAME ===============
void khoi_tao_game(vector<vector<string>>& danh_sach_tu, string& tu_bi_mat,
    string& tu_thuong, string& da_doan, int& n, bool& che_do_hai_nguoi) {
    cout << "=== GAME: NGUOI TREO CO ===\n\n";
    cout << "Chon che do choi:\n1. 1 nguoi (tu trong thu vien)\n2. 2 nguoi (nguoi 1 nhap tu bi mat)\nLua chon: ";
    char chon;
    cin >> chon;
	while (chon != 1 && chon != '2')
        cin >> chon;


    if (chon == '2') {
        che_do_hai_nguoi = true;
        cout << "Nguoi 1, hay nhap tu bi mat: ";
        cin.ignore();
        getline(cin, tu_bi_mat);
        tu_thuong = tu_bi_mat;
        for (char& c : tu_thuong) c = tolower(c);
        da_doan.assign(tu_bi_mat.length(), '_');
        system("cls"); // xóa màn hình để người chơi 2 không nhìn thấy
        cout << "Bat dau choi nao!\n";
        return;
    }

    che_do_hai_nguoi = false;
    danh_sach_tu = doc_tu_tu_file("data.txt");
    if (danh_sach_tu.empty()) {
        cerr << "Loi: Khong co du lieu trong file data.txt\n";
        exit(1);
    }

    for (int i = 0; i < (int)danh_sach_tu.size(); ++i) {
        cout << i + 1 << ". Chu de " << (i == 0 ? "may tinh" : "dien tu") << endl;
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
        vector<vector<string>> danh_sach_tu;
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
                cout << "Vui long nhap mot chu cai hop le (a-z)!\n";
                continue;
            }
            if (S.find(ky_tu_doan) == string::npos) {
                cout << "Ky tu da duoc doan, vui long chon ky tu khac!\n";
                continue;
            }

            if (xu_ly_doan(ky_tu_doan, tu_thuong, da_doan, so_chu_dung)) {
                cout << "Ban da doan dung!\n";
                sai_lien_tiep = 0;
            }
            else {
                so_lan_sai++;
                sai_lien_tiep++;
                cout << "Ban da doan sai! Con "
                    << (SO_LAN_DOAN_SAI_TOI_DA - so_lan_sai)
                    << " lan nua.\n";
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
                            cout << "Goi y: Tu nay co chua chu cai '" << tu_thuong[i] << "'\n";
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
            cout << "\n Chuc mung! Tu dung la: " << tu_bi_mat << "\n";
            diem_so += 10;
            so_van_thang++;
        }
        else {
            cout << "\n Ban da thua. Tu dung la: " << tu_bi_mat << "\n";
            diem_so -= 5;
            so_van_thua++;
        }
		diem_so = max(diem_so, 0);
        cout << "\n===== THONG KE =====\n";
        cout << "Diem hien tai: " <<diem_so<< endl;
        cout << "So van thang: " << so_van_thang << endl;
        cout << "So van thua: " << so_van_thua << endl;

        cout << "\nBan co muon choi lai khong? (y/n): ";
        char choilai;
        cin >> choilai;
        tiep_tuc = (choilai == 'y' || choilai == 'Y');
    }

    cout << "\nCam on ban da choi tro choi Nguoi Treo Co!\n";
}
