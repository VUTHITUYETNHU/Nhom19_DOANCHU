#include <iostream>
#include <vector>
#include <string>
#include "ve_nguoi.h"
using namespace std;

void ve_nguoi_treo_co(int so_lan_sai) {
    const string reset = "\033[0m";
    const string do_ = "\033[31m";// đỏ
    const string xanh = "\033[34m";// xanh dương
    const string vang = "\033[33m";// vàng
    vector<string> cac_giai_doan = {
          " ------\n |    |\n |\n |\n |\n_|_" + reset,
          " ------\n |    |\n |    " + do_ + "O" + reset + "\n |\n |\n_|_",
          " ------\n |    |\n |    " + do_ + "O" + reset + "\n |    " + vang + "|" + reset + "\n |\n_|_",
          " ------\n |    |\n |    " + do_ + "O" + reset + "\n |   " + vang + "/|" + reset + "\n |\n_|_",
          " ------\n |    |\n |    " + do_ + "O" + reset + "\n |   " + vang + "/|\\" + reset + "\n |\n_|_",
          " ------\n |    |\n |    " + do_ + "O" + reset + "\n |   " + vang + "/|\\" + reset + "\n |   " + xanh + "/" + reset + "\n_|_",
          " ------\n |    |\n |    " + do_ + "O" + reset + "\n |   " + vang + "/|\\" + reset + "\n |   " + xanh + "/ \\" + reset + "\n_|_"
    };

    cout << cac_giai_doan[so_lan_sai] << "\n\n";
}