#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include "QFile"
#include "QFileDialog"
#include "QDebug"
#include "QTextStream"
#include "QDir"
#include <string>
#include <iostream>
#include <stack>
#include "functions.h"
#include "xml-json.h"
#include "tree.h"
#include "SyntaxHighlighter.h"
#include "compression.h"
#include "dialog.h"


using namespace std;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    this->setCentralWidget(ui->textEdit);
    SyntaxHighlighter Sh = new SyntaxHighlighter(ui->textEdit);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpen_triggered()
{

    ui->textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
    QString filter = "All Files *.*";
    QString filename = QFileDialog::getOpenFileName(this, "Open a File", QDir::currentPath(), filter);
    fpath = filename;
    QFile file(filename);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot Open File!");
        return;
    }
    else{
        QTextStream in(&file);
        QString file_text = "";
        file_text = in.readAll();
        ui->textEdit->setText(file_text);
        file.close();
        string str = file_text.toStdString();

    }

}


void MainWindow::on_actioninfo_triggered()
{
    QString info= "Yoooooo";
    QMessageBox::information(this, "Info", info);
}


void MainWindow::on_actionNew_triggered()
{
    fpath = "";
    ui->textEdit->setText("");
}


void MainWindow::on_actionSave_triggered()
{
    QString filename = fpath;
    if(filename == ""){
        on_actionSave_as_triggered();
    }
    else{
        QFile file(filename);
        if(!file.open(QFile::WriteOnly | QFile::Text)){
            QMessageBox::warning(this, "Warning", "Cannot Save File!");
            return;
        }
        else{
            QTextStream out(&file);
            QString file_text = ui->textEdit->toPlainText();
            out << file_text;
            file.flush();
            file.close();
        }
    }
}


void MainWindow::on_actionSave_as_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "Choose the Location to Save the File", QDir::currentPath());
    fpath = filename;
    QFile file(filename);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot Save File!");
        return;
    }
    else{
        QTextStream out(&file);
        QString file_text = ui->textEdit->toPlainText();
        out << file_text;
        file.flush();
        file.close();
    }
}


void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}


void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}


void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}


void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}


void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

bool checkConsistancy(string tag,stack<string> &s){
    if(tag == s.top()){
        s.pop();
        return true;
    }
    return false;
}


void MainWindow::on_actionconsistansy_triggered()
{
    string sti;
    sti = ui->textEdit->document()->toPlainText().toStdString();
    stack<string> tags;
    string tagName;

    bool isClosingTag = false ;
    bool isConsistant = true ;

    for (unsigned int i=0;i < sti.length();i++){
        // if(!isConsistant){
        //    break;
        // }
        switch (sti[i]) {
        case '<':
            tagName = "";
            break;
        case ' ':
            break;

        case '>':
            if(isClosingTag){
                if(!checkConsistancy(tagName,tags)){
                    isConsistant = false;
                }
                isClosingTag = false;
            }else{
                tags.push(tagName);
                tagName="";
            }

            break;
        case '/':
            /*  if(sti[i+1]=='>'){
              tags.push(tagName);
              tags.push(tagName);
           }
           if(sti[i-1]=='<'){
              tags.push(tagName);
           }*/
            isClosingTag = true;

            break;
        default:
            tagName += sti[i];

        }
    }

    if(isConsistant){
        QMessageBox::information(this, "Info", "is Consistant");
    }else{
        QMessageBox::information(this, "Info", "not Consistant");
    }



}


void MainWindow::on_actionminify_triggered()
{
    QString sti = ui->textEdit->toPlainText();
    QString strout = "";
    QVector<QString> lines;
    /*for (unsigned int i=0;i < sti.length();i++){
       if(sti[i] == '\n' || sti[i] == ' '  || sti[i] =='\t'){
           continue;
       }
       strout += sti[i];
    }
   ui->textEdit->document()->setPlainText(QString::fromStdString(strout));*/
    if (sti != ""){
        QString filename = QFileDialog::getSaveFileName(this, "Choose the Location to Save the Minified File", QDir::currentPath());
        fpath = filename;
        QFile file(filename);
        if(!file.open(QFile::WriteOnly | QFile::Text)){
            QMessageBox::warning(this, "Warning", "Cannot Save File!");
            return;
        }
        lines = create_xml_vector(sti);
        for (int i =0; i<lines.size();i++){
            strout += lines[i];
        }


        /*
        int i = 0;
        while (i < sti.length()){
            if(sti[i] == '<'){
                while(i < sti.length()){
                    if(sti[i] != '>'){
                        temp += sti[i];
                    }
                    else {
                        break;
                    }
                    i++;
                }
                temp += '>';
                i++;
                strout += temp;
                temp="";
            }else{
                while (i < sti.length()){
                    if(sti[i]!='<'){
                        temp += sti[i];
                    }else {
                        break;
                    }
                    i++;
                }

                bool add = false;
                for(int j=0; j<temp.length();j++){
                    if(temp[j] != ' ' && temp[j] != '\n' && temp[j] != '\t'){
                        add = true;
                        break;
                    }
                }
                if(add == true){

                    qint32 j = 0;
                    while(temp[j] == ' ' || temp[j] == '\t' || temp[j] == '\n'){
                        j += 1;
                    }
                    qint32 k = temp.length() - 1;
                    while(temp[k] == ' ' || temp[k] == '\t' || temp[k] == '\n') {
                        k -= 1;
                    }
                    QString temp1 = "";
                    for(int q = j ; q <= k; q++){
                        temp1 += temp[q];
                    }

                    strout += temp1;
                }
                temp = "";


            }
        }*/

        QTextStream outt(&file);
        outt << strout;
        file.flush();
        file.close();
        QMessageBox::information(this, "Info", "File Minified Successfully!\nThe Minified File Can Be Found at: " + filename);
        QFile f(filename);
        if(!f.open(QFile::ReadOnly | QFile::Text)){
            QMessageBox::warning(this, "Warning", "Cannot Open the Results File!");
            return;
        }else{
            QTextStream in(&f);
            QString file_text = "";
            file_text = in.readAll();
            ui->textEdit->setText(file_text);
            f.close();
        }


    }else {
        QMessageBox::warning(this,"Warning","No Text To Be Minified!");
    }





}


void MainWindow::on_actionJson_triggered()
{

    QString newtext;
    XML_Tree tt;
    QString in = ui->textEdit->toPlainText();
    QVector<QString> out = create_xml_vector(in);
    QString outt="";
    for(int i = 0;i<out.size();i++){
        outt+=out[i];
    }
    tt.Fill(outt);
    Node * node = tt.GetHead();
    newtext+="{\n";
    for(int i = 0; i < node->Children.count();i++){

        tt.XMLtoJSON(node->Children[i],1,newtext);
       // if(i!=node->Children.count()-1){
       //     newtext+=",\n";
       // }
    }
    newtext+="\n}";
    ui->textEdit->setText(newtext);


}

//====================================== Prettify XML =========================================
// Indentation: Five Spaces
// NOTE: Dependent on check_consistency()

QString Remove_Indentation(QString str, QString indent) //Remove Only 1 Identation
{
    QString temp = "";
    qint32 indLen = indent.length();
    for(int i=0; i<str.length()-indLen;i++)   // String Length - Identation Length
    {
        temp += str[i];
    }
    return temp;
}


void MainWindow::on_actionPretiffy_triggered()
{
    QString indent_char = "     "; //Five Spaces
    QVector<QString> lines;
    lines = create_xml_vector(ui->textEdit->toPlainText());
    QString filename = QFileDialog::getSaveFileName(this, "Choose the Location to Save the Prettified File", QDir::currentPath());
    fpath = filename;
    QFile file(filename);

    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot Save File!");
        return;
    }
    //check consistancy here
    if(true)
    {
        QString indent = "";
        QString final = "";
        for(int i=0; i<lines.size(); i++)
        {
            QString line = lines[i];
            if(line[0] == '<')
            {
                if(line[1] == '/')
                {
                    indent = Remove_Indentation(indent, indent_char);
                    final += indent+line;
                    final += "\n";
                }
                else if(line[1] != '!' && line[1] != '?')
                {
                    if(!(line[line.length()-2] == '/' && line[line.length()-1] == '>'))
                    {
                        final += indent+line;
                        final += "\n";
                        indent += indent_char;
                    }
                    else
                    {
                        final += indent+line;
                        final += "\n";
                    }
                }
                else
                {
                    final += indent+line;
                    final += "\n";
                }
            }
            else
            {
                final += indent+line;
                final += "\n";
            }
        }
        QTextStream out(&file);
        out << final;
        file.flush();
        file.close();
        QMessageBox::information(this, "Info", "File Prettified Successfully!\nThe Prettified File Can Be Found at: " + filename);
        ui->textEdit->setLineWrapMode(QTextEdit::NoWrap);
        QFile f(filename);
        if(!f.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox::warning(this, "Warning", "Cannot Open the Results File!");
            return;
        }
        else
        {
            QTextStream in(&f);
            QString file_text = "";
            if(f.size() > 3*1024*1024)  // file larger than 3 MB
            {
                ui->textEdit->setText("");
                QMessageBox::warning(this, "Warning", "Due to large file size > 3 MB, we will view the first 100 lines of the output file only");
                for(int q=0; q<100; q++)
                {
                    file_text = in.readLine();
                    ui->textEdit->append(file_text);
                }
            }
            else
            {
                file_text = in.readAll();
                ui->textEdit->setText(file_text);
            }
            f.close();
            ui->statusbar->showMessage("Done!");
        }
    }
    else
    {
        QMessageBox::warning(this, "Warning", "Cannot Prettify an Inconsistent XML file!");
        //ui->actionShow_Consistency_Errors->setDisabled(false);
        // ui->actionFix_Consistency_Errors->setDisabled(false);
        return;
    }


}


void MainWindow::on_actionhighleight_triggered()
{


}









//====================================== Compress =========================================


void MainWindow::on_actioncompress_triggered()
{
     string txt = ui->textEdit->toPlainText().toStdString();
     vector<int> en = encoding(txt);
     stringstream result;
     copy(en.begin(), en.end(), std::ostream_iterator<int>(result, " "));

     ui->textEdit->setText(QString::fromStdString(result.str()));



}



//====================================== Decompress =========================================
void MainWindow::on_actiondecompress_triggered(){
      string txt = ui->textEdit->toPlainText().toStdString();
      stringstream iss( txt );
      int number;
      std::vector<int> myNumbers;
      while ( iss >> number ){
          myNumbers.push_back( number );
      }
      string decom= decoding(myNumbers);
      ui->textEdit->setText(QString::fromStdString(decom));

}


//**********************************************************Fix Errors **********************************************************

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


void makeFileCoonsistent(string str,string& out) {
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
    //this->textarea->document()->setPlainText(QString::fromStdString(str));
    out = str;
}







void MainWindow::on_actionFix_Xml_triggered()
{
      string in= ui->textEdit->toPlainText().toStdString();

      string out="";
      makeFileCoonsistent(in,out);

      QString outt = QString::fromStdString(out);
      ui->textEdit->setText(outt);

}


void MainWindow::on_actionGraph_triggered()
{

    QString newtext;
    QString in = ui->textEdit->toPlainText();
    QVector<QString> out = create_xml_vector(in);
    //QString outt="";
    //for(int i = 0;i<out.size();i++){
    //    outt+=out[i];
   // }
      ofstream myfile ("C:\\Users\\zashr\\OneDrive\\Desktop\\graph.dot");
     // ofstream myfile ("graph.dot");
      if (myfile.is_open())
      {
        myfile << "digraph g{" << endl;
        myfile << "edge [dir=\"back\"]" << endl;
       // myfile << "1 -> {2,3}" << endl;
       //myfile << "2 -> {1}" << endl;
       //  myfile << "3 -> {2}" << endl;
        int follow=0;
        for(int i=0;i<out.size();i++){
              if(out[i]=="<id>"){
                  if(!follow){
                      myfile << out[i+1].toStdString()+"->{";
                      follow=1;
                  }else{
                      myfile << out[i+1].toStdString();
                      if(out[i+4]=="</followers>"){
                          myfile << "}\n";
                          follow = 0;
                      }else{
                          myfile << ",";
                      }
                  }

              }
        }
        myfile << "}";
        myfile.close();
      }
      system("dot -Tpng -O C:\\Users\\zashr\\OneDrive\\Desktop\\graph.dot");
      //system("dot -Tpng -O graph.dot");
      Dialog img_view;
      img_view.setModal(true);
      img_view.exec();
      qDebug() << out;


}

