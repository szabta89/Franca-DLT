#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QStandardItem>
#include <iostream>
#include <QStandardItemModel>
#include "contextelement.h"

class ContractValidatorPlugin;

namespace Ui {
    class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(ContractValidatorPlugin* _plugin, QWidget *parent = 0);
    ~Form();

    void addContext(QString contex);
    void setMessages();

private slots:
    void on_connectionListTable_clicked(const QModelIndex &index);
    void on_connectionListTable_doubleClicked(const QModelIndex &index);
    void on_autoScrollButton_clicked(bool checked);

private:
    QBrush getBrush(int valid);
    Ui::Form *ui;
    ContractValidatorPlugin* plugin;
    QStandardItemModel connectionListTableModel;
    QStandardItemModel selectedConnectionTraceTableModel;
    QString currentContext;
    bool autoScroll;
};

#endif // FORM_H
