#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QStandardItem>
#include <iostream>
#include <QStandardItemModel>
#include "contextelement.h"

class TraceViewerPlugin;

namespace Ui {
    class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(TraceViewerPlugin* _plugin, QWidget *parent = 0);
    ~Form();

    void addContext(QString contex);
    void setMessages();

private slots:
    void on_connectionListTable_clicked(const QModelIndex &index);
    void on_connectionListTable_doubleClicked(const QModelIndex &index);

private:
    Ui::Form *ui;
    TraceViewerPlugin* plugin;
    QStandardItemModel connectionListTableModel;
    QStandardItemModel selectedConnectionTraceTableModel;
    QString currentContext;
};

#endif // FORM_H
