#define NOMINMAX
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
#include <algorithm> 
#include <limits>

#pragma comment(lib, "winmm.lib")
using namespace std;

// =============== HÀM PHỤ TRỢ XỬ LÝ LỖI INPUT ===============
void xu_ly_loi_input() {
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

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
// Hàm di chuyển con trỏ đến vị trí x, y trên màn hình console
void gotoXY(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
// Hàm hiệu ứng pháo hoa
void hieu_ung_phao_hoa() {
    // Không xóa màn hình ngay lập tức để giữ lại hình vẽ người treo cổ

    srand(static_cast<unsigned int>(time(0))); // Khởi tạo seed ngẫu nhiên
    auto startTime = chrono::steady_clock::now();

    // Giả định kích thước cửa sổ console (có thể điều chỉnh)
    const int consoleWidth = 80;
    const int consoleHeight = 25;

    // Vòng lặp pháo hoa trong 3 giây
    while (chrono::steady_clock::now() - startTime < chrono::seconds(3)) {

        // 1. Chọn màu ngẫu nhiên (chỉ các màu sáng)
        int mau = 9 + (rand() % 7); // Từ 9 (Blue) đến 15 (White)
        doiMau(mau);

        // 2. Chọn vị trí ngẫu nhiên
        int x = rand() % consoleWidth;
        int y = rand() % consoleHeight;

        gotoXY(x, y); // Di chuyển đến vị trí

        // 3. Chọn ký tự pháo hoa ngẫu nhiên
        char sparkle;
        int randCheck = rand() % 4;
        if (randCheck == 0) sparkle = '*';
        else if (randCheck == 1) sparkle = '+';
        else if (randCheck == 2) sparkle = '.';
        else sparkle = 'o';

        cout << sparkle << flush; // flush để đảm bảo ký tự xuất hiện ngay

        // 4. Dừng một chút
        this_thread::sleep_for(chrono::milliseconds(15));
    }

    // 5. Dọn dẹp sau khi hiệu ứng kết thúc
    system("cls"); // Xóa màn hình để chuẩn bị cho thông báo thắng
    doiMau(7);     // Reset màu về mặc định
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
        }
        else {
            tu_trong_chu_de.push_back(dong);
        }
    }

    if (!tu_trong_chu_de.empty()) {
        danh_sach_tu.push_back({ ten_chu_de, tu_trong_chu_de });
    }

    file.close();
    return danh_sach_tu;
}

string chon_do_kho() {
    doiMau(14);
    cout << "\n=== CHON DO KHO ===\n";
    doiMau(7);
    cout << "1. De (Tu dai <= 6 ky tu, 0 NGOI SAO HY VONG)\n";
    cout << "2. Trung Binh (Tu dai 7-11 ky tu, 1 NGOI SAO HY VONG)\n";
    cout << "3. Kho (Tu dai >= 12 ky tu, 2 NGOI SAO HY VONG)\n";
    cout << "Lua chon: ";

    char chon;

    // Sử dụng vòng lặp an toàn để nhập input
    while (true) {
        if (!(cin >> chon)) {
            xu_ly_loi_input();
            doiMau(12);
            cout << "Input khong hop le. Vui long nhap 1, 2, hoac 3: ";
            doiMau(7);
            continue;
        }

        if (chon == '1' || chon == '2' || chon == '3') break;

        doiMau(12);
        cout << "Lua chon khong hop le. Vui long nhap 1, 2, hoac 3: ";
        doiMau(7);
        xu_ly_loi_input();
    }

    switch (chon) {
    case '1':
        return "DE";
    case '2':
        return "TRUNG BINH";
    case '3':
        return "KHO";
    default:
        // Mặc định chọn Trung Bình
        doiMau(12);
        cout << "Lua chon khong hop le. Mac dinh chon Trung Binh.\n";
        doiMau(7);
        return "TRUNG BINH";
    }
}

string chon_tu_bi_mat(const vector<pair<string, vector<string>>>& danh_sach_tu, int n, const string& do_kho_yeu_cau) {
    srand(static_cast<unsigned int>(time(0)));
    const vector<string>& tu_goc = danh_sach_tu[n].second;
    vector<string> tu_phu_hop;

    // 1. Xác định giới hạn độ dài dựa trên độ khó
    int min_len = 0;
    int max_len = 0;

    if (do_kho_yeu_cau == "DE") {
        min_len = 1; max_len = 6;
    }
    else if (do_kho_yeu_cau == "TRUNG BINH") {
        min_len = 7; max_len = 11;
    }
    else { // KHO
        min_len = 12; max_len = 100;
    }

    // 2. Lọc từ
    for (const string& tu : tu_goc) {
        if ((int)tu.length() >= min_len && (int)tu.length() <= max_len) {
            tu_phu_hop.push_back(tu);
        }
    }

    // 3. Kiểm tra kết quả lọc và chọn từ
    if (tu_phu_hop.empty()) {
        cerr << "\nCanh bao: Khong tim thay tu nao cho do kho '" << do_kho_yeu_cau << "'. Chon ngau nhien tu goc.\n";
        return tu_goc[rand() % tu_goc.size()];
    }

    return tu_phu_hop[rand() % tu_phu_hop.size()];
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
bool check_che_do_2_nguoi(string tu_bi_mat)
{
    // Hạ toàn bộ chuỗi xuống chữ thường
    for (char& c : tu_bi_mat) c = tolower(c);

    // Chỉ cho phép chữ cái a-z, không khoảng trắng hay ký tự khác
    for (char c : tu_bi_mat) {
        if (c < 'a' || c > 'z') {
            return false;
        }
    }
    return true;
}

// =============== HÀM KHỞI TẠO GAME ===============
void khoi_tao_game(vector<pair<string, vector<string>>>& danh_sach_tu, string& tu_bi_mat,
    string& tu_thuong, string& da_doan, int& n, bool& che_do_hai_nguoi, string& do_kho_da_chon) {

    doiMau(11);
    inChamCham("=== GAME: NGUOI TREO CO ===\n", 20);
    doiMau(7);

    // BƯỚC 1: CHỌN CHẾ ĐỘ CHƠI
    cout << "\nChon che do choi:\n1. 1 nguoi (tu trong thu vien)\n2. 2 nguoi (nguoi 1 nhap tu bi mat)\nLua chon: ";
    char chon;

    // Sử dụng vòng lặp an toàn để nhập input
    while (true) {
        if (!(cin >> chon)) {
            xu_ly_loi_input();
            doiMau(12);
            cout << "Input khong hop le. Vui long nhap 1 hoac 2: ";
            doiMau(7);
            continue;
        }

        if (chon == '1' || chon == '2') break;

        doiMau(12);
        cout << "Lua chon khong hop le. Vui long nhap 1 hoac 2: ";
        doiMau(7);
        xu_ly_loi_input();
    }

    if (chon == '2') {
        che_do_hai_nguoi = true;
        cout << "Nguoi 1, hay nhap tu bi mat (chi chu cai, khong dau, khong de trong): ";
        cin.ignore();

        while (true) {
            getline(cin, tu_bi_mat);

            // Xóa khoảng trắng ở đầu và cuối chuỗi
            tu_bi_mat.erase(0, tu_bi_mat.find_first_not_of(" \t"));
            tu_bi_mat.erase(tu_bi_mat.find_last_not_of(" \t") + 1);

            // Kiểm tra chuỗi trống hoặc không hợp lệ
            if (tu_bi_mat.empty()) {
                doiMau(12);
                cout << "Tu bi mat khong duoc de trong. Vui long nhap lai: ";
                doiMau(7);
                continue;
            }

            if (!check_che_do_2_nguoi(tu_bi_mat)) {
                doiMau(12);
                cout << "Tu bi mat khong hop le (chi duoc dung chu cai a-z). Vui long nhap lai: ";
                doiMau(7);
                continue;
            }

            // Nếu hợp lệ thì thoát khỏi vòng lặp
            break;
        }

        tu_thuong = tu_bi_mat;
        for (char& c : tu_thuong) c = tolower(c);
        da_doan.assign(tu_bi_mat.length(), '_');

        // Gán độ khó/gợi ý mặc định cho chế độ 2 người (dựa trên độ dài)
        int do_dai = (int)tu_bi_mat.length();
        if (do_dai <= 6) do_kho_da_chon = "DE";
        else if (do_dai <= 11) do_kho_da_chon = "TRUNG BINH";
        else do_kho_da_chon = "KHO";

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

    // BƯỚC 2: CHỌN ĐỘ KHÓ VÀ CHỦ ĐỀ
    do_kho_da_chon = chon_do_kho(); // GỌI HÀM CHỌN ĐỘ KHÓ

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

    // BƯỚC 3: CHỌN TỪ DỰ TRÊN ĐỘ KHÓ
    tu_bi_mat = chon_tu_bi_mat(danh_sach_tu, n, do_kho_da_chon);
    tu_thuong = tu_bi_mat;
    for (char& c : tu_thuong) c = tolower(c);
    da_doan.assign(tu_bi_mat.length(), '_');
}

// =============== HÀM GỢI Ý: NGÔI SAO HY VỌNG ===============
bool su_dung_ngoi_sao_hy_vong(const string& tu_bi_mat, const string& tu_thuong, string& da_doan, int& so_lan_goi_y_con_lai) {
    // 1. Kiểm tra điều kiện sử dụng
    if (so_lan_goi_y_con_lai <= 0) {
        return false; // Không còn lượt gợi ý
    }

    doiMau(11);
    cout << "[NGOI SAO HY VONG] Ban con " << so_lan_goi_y_con_lai << " lan. Nhap 'y' de dung: ";
    doiMau(7);

    // Khai báo biến chọn gợi ý cục bộ
    char chon_goi_y;

    // Xử lý nhập liệu an toàn
    if (!(cin >> chon_goi_y)) {
        xu_ly_loi_input();
        return false; // Lỗi input, không dùng
    }

    // 2. Thực hiện gợi ý
    if (chon_goi_y == 'y' || chon_goi_y == 'Y') {
        so_lan_goi_y_con_lai--; // Giảm số lần gợi ý

        // Khai báo biến kiểm tra việc tìm thấy ký tự gợi ý
        bool da_tim_goi_y = false;

        for (int i = 0; i < (int)tu_bi_mat.length(); ++i) {
            // Chỉ gợi ý cho ký tự chưa đoán được
            if (da_doan[i] == '_') {
                doiMau(11);
                cout << "Goi y: Tu nay co chua chu cai '" << tu_thuong[i] << "'\n";
                doiMau(7);
                da_tim_goi_y = true;
                break;
            }
        }
        if (da_tim_goi_y) {
            cout << "(Ban con " << so_lan_goi_y_con_lai << " NGOI SAO HY VONG)\n";
        }
        return true; // Đã sử dụng và được gợi ý thành công
    }

    return false; // Người chơi chọn không dùng
}


// =============== HÀM CHƠI GAME CHÍNH ===============
void choi_game() {
    bool tiep_tuc = true;

    while (tiep_tuc) {
        // KHAI BÁO BIẾN TRẠNG THÁI VÒNG LẶP NGOÀI
        vector<pair<string, vector<string>>> danh_sach_tu;
        string tu_bi_mat, tu_thuong, da_doan;
        int n;
        bool che_do_hai_nguoi = false;

        // KHAI BÁO BIẾN ĐỘ KHÓ & GỢI Ý
        string do_kho;
        int so_lan_goi_y_toi_da = 0;
        int so_lan_goi_y_con_lai = 0;
        int so_lan_sai_toi_da_hien_tai = 6;

        // GỌI HÀM KHỞI TẠO GAME VỚI BIẾN ĐỘ KHÓ
        khoi_tao_game(danh_sach_tu, tu_bi_mat, tu_thuong, da_doan, n, che_do_hai_nguoi, do_kho);

        // KHAI BÁO BIẾN TRẠNG THÁI VÒNG LẶP TRONG
        int so_chu_dung = 0;
        int so_lan_sai = 0;
        int sai_lien_tiep = 0;
        char ky_tu_doan;
        string ki_tu_nhap;
        string S = "a b c d e f g h i j k l m n o p q r s t u v w x y z";

        // XÁC ĐỊNH SỐ LẦN GỢI Ý DỰ TRÊN ĐỘ KHÓ ĐÃ CHỌN
        int do_dai = (int)tu_bi_mat.length();

        // SỐ LẦN SAI CỐ ĐỊNH LÀ 6 CHO MỌI ĐỘ KHÓ
        so_lan_sai_toi_da_hien_tai = 6;

        if (do_kho == "DE") {
            so_lan_goi_y_toi_da = 0;
        }
        else if (do_kho == "TRUNG BINH") {
            so_lan_goi_y_toi_da = 1;
        }
        else { // KHO
            so_lan_goi_y_toi_da = 2;
        }
        so_lan_goi_y_con_lai = so_lan_goi_y_toi_da;

        // XUẤT RA THÔNG TIN ĐỘ KHÓ
        doiMau(14);
        cout << "\n* Do kho hien tai: " << do_kho << " (Tu dai " << do_dai << " chu cai)";
        cout << " - Ban co " << so_lan_goi_y_toi_da << " NGOI SAO HY VONG.";
        cout << " - Tong cong " << so_lan_sai_toi_da_hien_tai << " lan doan sai.\n";
        doiMau(7);

        ve_nguoi_treo_co(so_lan_sai);

        // Vòng lặp chính của trò chơi
        while (so_lan_sai < so_lan_sai_toi_da_hien_tai && so_chu_dung < tu_bi_mat.length()) {
            cout << "\nTu hien tai: ";
            hien_thi_tu_hien_tai(da_doan);
            cout << "Ky tu chua doan: " << S << endl;

            // GỌI HÀM GỢI Ý RIÊNG (Ngôi Sao Hy Vọng) BẤT KỲ LÚC NÀO
            // Cho phép người chơi tự nhập '!' để kích hoạt ngôi sao hy vọng
            cout << "Hay doan mot chu cai (hoac nhap '!' de dung NGOI SAO HY VONG): ";
            getline(cin, ki_tu_nhap);

            // Nếu người chơi nhập '!' -> kích hoạt gợi ý
            if (ki_tu_nhap == "!") {
                if (!su_dung_ngoi_sao_hy_vong(tu_bi_mat, tu_thuong, da_doan, so_lan_goi_y_con_lai)) {
                    doiMau(12);
                    cout << "Khong the su dung NGOI SAO HY VONG!\n";
                    doiMau(7);
                }
                continue; // quay lại đầu vòng lặp sau khi dùng gợi ý
            }
            char ky_tu_doan = tolower(ki_tu_nhap[0]); // lấy ký tự duy nhất và chuyển về chữ thường

            // Kiểm tra có phải là chữ cái không
            if (ky_tu_doan < 'a' || ky_tu_doan > 'z') {
                doiMau(12);
                cout << "Vui long nhap mot chu cai hop le (a-z)!\n";
                doiMau(7);
                continue;
            }

            // Kiểm tra ký tự đã đoán chưa
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
                // Sử dụng biến 'so_lan_sai_toi_da_hien_tai'
                cout << " Sai roi! Con " << (so_lan_sai_toi_da_hien_tai - so_lan_sai) << " lan nua.\n";
                doiMau(7);
                amThanhSai();
            }

            for (char& c : S) if (c == ky_tu_doan) c = ' ';
            ve_nguoi_treo_co(so_lan_sai);
        }

        // ===== KẾT QUẢ =====
        if (so_chu_dung == tu_bi_mat.length()) {
            doiMau(10);
            amThanhThang();
            hieu_ung_phao_hoa();
            inChamCham("\nChuc mung! Tu dung la: " + tu_bi_mat, 30);
            doiMau(7);
        }
        else {
            doiMau(12);
            amThanhThua();
            inChamCham("\nBan da thua. Tu dung la: " + tu_bi_mat, 30);
            doiMau(7);
        }

        cout << "\nBan co muon choi lai khong? (y/n): ";
        char choilai;
        cin >> choilai;
        tiep_tuc = (choilai == 'y' || choilai == 'Y');
    }

    doiMau(11);
    inChamCham("\nCam on ban da choi tro choi Nguoi Treo Co!\n", 40);
    doiMau(7);
}
