#ifndef COMPRESSION_H
#define COMPRESSION_H
#include <iostream>
#include<stdio.h>
#include<queue>
#include<unordered_map>
#include<string>
#include<algorithm>
#include<map>
#include<vector>
#include<cstdlib>
#include<fstream>
#include <sstream>
#include "QMainWindow"
#include "QMessageBox"
#include "QFile"
#include "QTextStream"
#include "QDir"
#include "mainwindow.h"


using namespace std;


vector<int> encoding(string s1);
string decoding(vector<int> op);







#endif // COMPRESSION_H
