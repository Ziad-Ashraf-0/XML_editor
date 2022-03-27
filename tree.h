#ifndef TREE_H
#define TREE_H
#include <QString>
#include <QVector>
#include <QFile>
#include <QTextStream>

class Node
{
public:
    QString Data;
    QString TagName;
    QString Closetag;
    QVector<Node*>Children;
    QString type;
    int repeated;
    Node* Parent;

    //Constructor
    Node( QString Name , QString DataIn )
    {
        Data = DataIn;
        TagName = Name;
        Closetag = "";
        type ="";
        Parent = NULL;
        repeated = 0;


    }
};

class XML_Tree
{
private:
    Node* Head;
    Node* MostRecent;

public:
    XML_Tree();
    ~XML_Tree();
    Node* GetHead() { return Head; }
    void insertChild(Node *child);
    void DoneNode();
    void Fill(QString text);
    void XMLtoJSON(Node * node, int lvl , QString &outfile);
    void DestroyRecursive(Node *node);
};

#endif // TREE_H
