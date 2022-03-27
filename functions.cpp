#include "functions.h"


QString remove_spaces(QString str){
    int i = 0;
    while(str[i] == ' ' || str[i] == '\t' || str[i] == '\n'){
            i += 1;
        }
        int k = str.length() - 1;
        while(str[k] == ' ' || str[k] == '\t' || str[k] == '\n') {
            k -= 1;
        }
        QString no_space = "";
        for(int q = i ; q <= k; q++){
            no_space += str[q];
        }
        return no_space;
}


QVector<QString> create_xml_vector(QString in){
    QVector<QString> out;
    QString temp = "";
    int i = 0;
    while (i< in.length()){
        if(in[i] == '<'){
            while (i < in.length()) {
                if(in[i] != '>'){
                    temp += in[i];
                }else{
                    break;
                }
                i++;

            }
            temp += '>';
            temp = remove_spaces(temp);
            i++;
            out.push_back(temp);
            temp="";


        }else{
            while (i < in.length()) {
                if(in[i] != '<'){
                    temp += in[i];
                }else{
                    break;
                }
                i++;
            }
            bool add = false;
            for(int j= 0; j<temp.length();j++){
                if(temp[j] != ' ' && temp[j] != '\n' && temp[j] != '\t'){
                    add = true;
                    break;
                }
            }
            if(add){
                temp = remove_spaces(temp);
                out.push_back(temp);
            }
            temp = "";
        }
    }
    return out;
}


string remove_pre_post_spacesQ(string line){
    int j = 0;
    while(line[j] == ' ' || line[j] == '\t' || line[j] == '\n'){
        j += 1;
    }
    int k = line.length() - 1;
    while(line[k] == ' ' || line[k] == '\t' || line[k] == '\n') {
        k -= 1;
    }
    string line_without_pre_or_post_spaces = "";
    for(int q = j ; q <= k; q++){
        line_without_pre_or_post_spaces += line[q];
    }
    return line_without_pre_or_post_spaces;
}

vector<string> create_xml_vectorQ(string in){
    vector<string> res;
    string temp = "";
    unsigned int i = 0;
    while(i < in.length()){
        if(in[i] == '<'){
            while(i < in.length()){
                if(in[i] != '>'){
                    temp += in[i];
                }
                else{
                    break;
                }
                i++;
            }
            temp += '>';
            temp = remove_pre_post_spacesQ(temp);
            i++;
            res.push_back(temp);
            temp = "";
        }
        else{
            while(i < in.length()) {
                if(in[i] != '<'){
                    temp += in[i];
                }
                else{
                    break;
                }
                i++;
            }
            bool add = false;
            for(unsigned int j=0; j<temp.length(); j++){
                if(temp[j] != ' ' && temp[j] != '\n' && temp[j] != '\t'){
                    add = true;
                    break;
                }
            }
            if(add == true){
                temp = remove_pre_post_spacesQ(temp);
                res.push_back(temp);
            }
            temp = "";
        }
    }
    return res;
}







