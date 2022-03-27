#include "consistancy.h"
#include <QLabel>
#include <QShortcut>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QGuiApplication>
#include <QSaveFile>
#include <iostream>
#include <QSyntaxHighlighter>
#include "mainwindow.h"
#include "ui_mainwindow.h"






void fixClosingTag(std::string& str, std::string& tag,unsigned int& i, std::stack<std::string>& tags) {
    int startOfClosingTag = i - tags.top().length() - 2;
    int endOfClosingTag = startOfClosingTag + tags.top().length() + 2;
    tags.pop(); // pop the not correct tag
    str = str.substr(0 , startOfClosingTag) + "</" + tags.top() + str.substr(endOfClosingTag, str.length()) + ">";
    if (!tags.empty()) {
        if (tags.top().length() + 1 < str.length())
            i += tags.top().length() + 1;
        tags.pop();
    }
}


void fixMissingTag(std::string& str, std::string& tag, unsigned int& i, std::stack<std::string>& tags) {
    //std::string firstInStack = tags.top();
    //tags.pop();
    std::string openingTagWithNoClosing = tags.top();
    int startOfOpeningTag = str.std::string::rfind(openingTagWithNoClosing.c_str(), i , openingTagWithNoClosing.length());
    int beforeStartOfNextTag = str.std::string::find('<', startOfOpeningTag);

    //int startOfClosingTag = i - tags.top().length() - 2;
    //int endOfClosingTag = startOfClosingTag + tags.top().length() + 2;

    //tags.pop();
    str = str.substr(0, beforeStartOfNextTag) + "</" + openingTagWithNoClosing + ">" + str.substr(beforeStartOfNextTag, str.length());
    if (!tags.empty()) {
        if (tags.top().length() + 1 < str.length())
            i -= openingTagWithNoClosing.length() - 3;
        tags.pop();
    }
}

bool checkConsistency(std::string tag , std::stack<std::string>& s) {
    if (tag == s.top()) {
        s.pop();
        return true;
    }
    return false;
}

bool hasMissingTag(std::string tag, std::stack<std::string>& s) {
    std::string firstInStack = s.top();
    s.pop();
    if (tag == s.top()) {
        //s.pop();
        s.push(firstInStack);
        return true;
    }
    s.push(firstInStack);
    //s.push(tag);
    return false;
}


/*void makeFileCoonsistent() {
   // QString in = ui->textEdit->toPlainText();
   // std::string str = this->textarea->document()->toPlainText().toStdString();
    std::stack<std::string> tags;
    std::string tagName;
    bool isClosingTag = false;
    bool isConsistant = false;

    while (!isConsistant) {
        for (unsigned int i = 0; i < str.length(); i++) {
            switch (str[i]) {
            case '<':
                tagName = "";
                break;
            case '\t':
                break;
            case ' ':
                break;
            case '>':
                if (isClosingTag) {
                    if (!checkConsistency(tagName, tags)) {
                        if (hasMissingTag(tagName, tags)) {
                            fixMissingTag(str, tags.top(), i, tags);
                            //tags.push(tagName);
                        }
                        else {
                            tags.push(tagName);
                            fixClosingTag(str, tags.top(), i, tags);
                        }
                        isConsistant = true;
                    }
                    else {
                        isConsistant = true;
                    }
                    isClosingTag = false;
                }
                else {
                    tags.push(tagName);
                    tagName = "";
                }
                break;
            case '/':
                isClosingTag = true;
                break;
            default:
                tagName += str[i];
            }
        }
    }
   // this->textarea->document()->setPlainText(QString::fromStdString(str));
}

