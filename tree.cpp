#include "tree.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QStack>
#include <QDir>
#include <algorithm>
#include <string>

void spaces(QString &out, int level){
    for (int i =0; i<level;i++){
        out+="   ";
    }
}

XML_Tree::XML_Tree(){
    Head = nullptr;
    MostRecent = nullptr;
}

//destructor
XML_Tree::~XML_Tree()
{
    Node * node;
    for(int i = 0; i< Head->Children.count();i++){
        node = Head->Children[i];
        DestroyRecursive(node);
    }
    delete Head;

}
void XML_Tree::DestroyRecursive(Node *node)
{
    if (node)
    {
        for(int i  = 0; i< node->Children.count();i++)
            DestroyRecursive(node->Children[i]);
        delete node;
    }
}

// Insert child in the tree
void XML_Tree::insertChild(Node* child){
    if(Head ==nullptr){
        Head = new Node("","");
        Head->type = "Parent";
        child->Parent = Head;
        Head->Children.push_back(child);   //Qvector of nodes
        Head->Parent = nullptr;
        MostRecent = child;
    }
    else{

        if(child->TagName == MostRecent->TagName.mid(0,MostRecent->TagName.size()-1)){
            child->repeated=true;
        }

        child->Parent = MostRecent;
        MostRecent->Children.push_back(child);
        MostRecent = child;
    }
}

void XML_Tree::DoneNode(){
    //MostRecent->Closetag = closetag;
    MostRecent = MostRecent->Parent;
}


void XML_Tree::Fill(QString text){
    int start = 0;
    int close = 0;
    int sp = 0;


    QString sub, subb;
    Node * newnode = nullptr;
    for(int i = 0; i<text.count();i++){
        if(text[i]=='<' ){

            if(text[i+1] == '/'){      //Closing tag
                if(MostRecent->Children.count()==0){
                    sub = text.mid(close+1, i-close-1);
                    newnode->Data = sub;
                }
                start = i+1;
                sp =1;
            }
            else{

              //  if(subb!= "" &&MostRecent != nullptr ) MostRecent->Data =subb;
                if(newnode != nullptr && newnode->Data != ""){
                    newnode->Closetag = "";
                    DoneNode();
                    newnode = nullptr;
                }
                newnode =new Node("", "");
                start = i+1;
            }
           // subb="";
        }

        else if(text[i]=='>'){
            close = i;
            if(sp == 1){
                sub = text.mid(start, i-start);
                if(MostRecent->TagName!= sub.mid(1,sub.size()-1) && sub.mid(1,sub.size()-1) == MostRecent->Parent->TagName){
                    MostRecent->Closetag ="";
                    DoneNode();
                    MostRecent->Closetag = sub;
                    DoneNode();
                }
                else{
                    MostRecent->Closetag = sub;
                    DoneNode();
                }
                newnode =nullptr;
                sp=0;
            }
            else{
                sub = text.mid(start,i-start);
                newnode->TagName = sub;
                insertChild(newnode);
            }
           // subb = "";
        }else{
          //  if(!(subb=="" && text[i]==' '))
           //     subb+=text[i];
        }
    }
}

/***************************HERE***************************************/
void XML_Tree::XMLtoJSON(Node *node, int lvl, QString &outfile){
    spaces(outfile, lvl);
    if(!node->repeated)
        outfile+= "\""+node->TagName+"\" : ";
    if(node->Children.count() ==0){
        outfile+= "\""+node->Data+"\"";
    }else{

        if(node->Children[0]->repeated){
            outfile+= "[\n";

            for(int i = 0; i< node->Children.count();i++){

                XMLtoJSON(node->Children[i], lvl+1, outfile);
                if(i!=node->Children.count()-1)
                    outfile+= ",\n";
                else
                    outfile+= "\n";
            }
            spaces(outfile, lvl);
            outfile+= "]";
        }
        else{
            outfile+= "{\n";

            for(int i = 0; i< node->Children.count();i++){

                XMLtoJSON(node->Children[i], lvl+1, outfile);
                if(i!=node->Children.count()-1)
                    outfile+= ",\n";
                else
                    outfile+= "\n";
            }
            spaces(outfile, lvl);
            outfile+= "}";

        }
    }

}







