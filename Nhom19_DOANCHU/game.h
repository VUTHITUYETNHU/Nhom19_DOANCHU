#pragma once
#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
using namespace std;

string chon_tu_bi_mat(const vector<string>& danh_sach_tu);
void hien_thi_tu_hien_tai(const string& da_doan);
bool xu_ly_doan(char ky_tu_doan, const string& tu_bi_mat, string& da_doan, int& so_chu_dung);
void choi_game();

#endif
