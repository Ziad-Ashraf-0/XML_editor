#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <QMainWindow>
using namespace std;

QString remove_spaces(QString str);
QVector<QString> create_xml_vector(QString in);
vector<string> create_xml_vectorQ(string in);
string remove_pre_post_spacesQ(string line);

#endif // FUNCTIONS_H
