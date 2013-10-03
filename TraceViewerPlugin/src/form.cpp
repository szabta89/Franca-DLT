#include "form.h"
#include "ui_form.h"
#include "contractvalidatorplugin.h"
#include <QDebug>
#include <QFileDialog>

Form::Form(ContractValidatorPlugin* _plugin, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    plugin = _plugin;

    QStandardItem* connectionNameHeaderItem = new QStandardItem(QString("Context id"));
    connectionListTableModel.setColumnCount(1);
    connectionListTableModel.setHorizontalHeaderItem(0, connectionNameHeaderItem);
    ui->connectionListTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->connectionListTable->setModel(&connectionListTableModel);

    QStandardItem* messageHeaderItem = new QStandardItem(QString("Message"));
    selectedConnectionTraceTableModel.setColumnCount(1);
    selectedConnectionTraceTableModel.setHorizontalHeaderItem(0, messageHeaderItem);
    ui->selectedConnectionTraceTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->selectedConnectionTraceTable->setModel(&selectedConnectionTraceTableModel);
}

Form::~Form()
{
    delete ui;
}

void Form::addContext(QString contextId)
{
    QStandardItem* row = new QStandardItem(contextId);
    row->setEditable(false);
    connectionListTableModel.setItem(connectionListTableModel.rowCount(), row);
    ui->connectionListTable->scrollToBottom();
}

QBrush Form::getBrush(int valid)
{
    if (valid == 0) {
        return QBrush(QColor(Qt::green));
    }
    else if (valid == 1) {
        return QBrush(QColor(Qt::red));
    }
    else {
        return QBrush(QColor(Qt::white));
    }
}

void Form::setMessages() {

    while (ui->selectedConnectionTraceTable->model()->rowCount() > 0)
    {
        ui->selectedConnectionTraceTable->model()->removeRow(0);
    }

    if (!currentContext.isNull()) {
        this->plugin->contextElementsLock.lock();

        // set color of context id label according to the valid flag
        ContextElement* element = plugin->contextElements.value(currentContext);
        int contextIdIndex = this->plugin->contextElements.keys().indexOf(currentContext);
        QStandardItem* item = connectionListTableModel.item(contextIdIndex);
        item->setBackground(getBrush(element->valid));

        connectionListTableModel.setItem(contextIdIndex, item);
        // add corresponding messages to the table view
        for (int i = 0;i<element->messages->keys().size();i++) {
            int messageId = element->messages->keys().at(i);
            QDltMsg message = element->messages->value(messageId);
            QStandardItem* payload = new QStandardItem(message.toStringPayload());

            int valid = -1;
            if (element->status->contains(messageId)) {
                valid = element->status->value(messageId);
            }

            payload->setBackground(getBrush(valid));

            // set tooltip at failedAt position
            if (element->expectations->contains(messageId)) {
                payload->setToolTip(element->expectations->value(messageId));
            }

            payload->setEditable(false);
            selectedConnectionTraceTableModel.setItem(i, 0, payload);
        }
        this->plugin->contextElementsLock.unlock();
    }

   // ui->selectedConnectionTraceTable->scrollToBottom();
}

void Form::on_connectionListTable_clicked(const QModelIndex &index)
{
    QVariant element = this->connectionListTableModel.data(connectionListTableModel.index(index.row(), index.column()));
    currentContext = element.toString();
    setMessages();
}

void Form::on_connectionListTable_doubleClicked(const QModelIndex &index)
{
    this->plugin->contextElementsLock.lock();
    QString contextId = this->connectionListTableModel.data(connectionListTableModel.index(index.row(), index.column())).toString();
    ContextElement* element = plugin->contextElements.value(contextId);
    QString initialPath;

    if (!element->filePath.isEmpty()) {
        initialPath = element->filePath;
        connectionListTableModel.item(index.row(), index.column())->setToolTip(initialPath);
    }
    else {
        initialPath = tr("/home");
    }

    this->plugin->contextElementsLock.unlock();

    // the file browser must be invoked after the lock is released
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), initialPath, tr("Franca Interface Definition Files (*.fidl)"));
    qDebug() << "Selected new file path for context " <<  fileName << "\n";

    this->plugin->contextElementsLock.lock();
    if (!fileName.isEmpty()) {

        // the messages must be sent again
        if (fileName.compare(initialPath) != 0) {
            element->valid = -1;
            element->status->clear();
            element->expectations->clear();
            element->unsentMessages->clear();
            for (int i = 0;i<element->messages->keys().size();i++) {
                element->unsentMessages->append(element->messages->keys().at(i));
            }
        }

        element->filePath = fileName;
        plugin->contextElements.insert(contextId, element);
    }
    this->plugin->contextElementsLock.unlock();

    this->plugin->trySendMessages();
}
