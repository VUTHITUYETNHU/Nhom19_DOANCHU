#include <iostream>
#include <vector>
#include "ve_nguoi.h"
using namespace std;

void ve_nguoi_treo_co(int so_lan_sai) {
    const vector<string> cac_giai_doan = {
        " ------\n |    |\n |\n |\n |\n_|_",
        " ------\n |    |\n |    O\n |\n |\n_|_",
        " ------\n |    |\n |    O\n |    |\n |\n_|_",
        " ------\n |    |\n |    O\n |   /|\n |\n_|_",
        " ------\n |    |\n |    O\n |   /|\\\n |\n_|_",
        " ------\n |    |\n |    O\n |   /|\\\n |   /\n_|_",
        " ------\n |    |\n |    O\n |   /|\\\n |   / \\\n_|_"
    };
    cout << cac_giai_doan[so_lan_sai] << "\n\n";
}
