#include "form.h"
#include "ui_form.h"
#include "traceviewerplugin.h"
#include <QDebug>
#include <QFileDialog>

Form::Form(TraceViewerPlugin* _plugin, QWidget *parent) :
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
}

void Form::setMessages() {

    while (ui->selectedConnectionTraceTable->model()->rowCount() > 0)
    {
        ui->selectedConnectionTraceTable->model()->removeRow(0);
    }

    if (!currentContext.isNull()) {
        this->plugin->contextElementsLock.lock();
        ContextElement* element = plugin->contextElements.value(currentContext);

        //selectedConnectionTraceTableModel.clear();
        for (int i = 0;i<element->messages.keys().size();i++) {
            int messageId = element->messages.keys().at(i);
            QDltMsg message = element->messages[messageId];
            QStandardItem* payload = new QStandardItem(message.toStringPayload());

            if (element->valid == 0) {
                // full green
                payload->setBackground(QBrush(QColor(Qt::green)));
            }
            else if (element->valid == 1) {
                if (messageId < element->failedAt) {
                    payload->setBackground(QBrush(QColor(Qt::green)));
                }
                else {
                    payload->setBackground(QBrush(QColor(Qt::red)));
                }

                // set tooltip at failedAt position
                if (messageId == element->failedAt) {
                    payload->setToolTip(element->failedAtExpectation);
                }
            }

            payload->setEditable(false);
            selectedConnectionTraceTableModel.setItem(i, 0, payload);
        }
        this->plugin->contextElementsLock.unlock();
    }
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
    QVariant selection = this->connectionListTableModel.data(connectionListTableModel.index(index.row(), index.column()));
    ContextElement* element = plugin->contextElements.value(selection.toString());
    QString initialPath;
    if (!element->filePath.isEmpty()) {
        initialPath = element->filePath;
        connectionListTableModel.item(index.row(), index.column())->setToolTip(initialPath);
    }
    else {
        initialPath = tr("/home");
    }

    this->plugin->contextElementsLock.unlock();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), initialPath, tr("Franca Interface Definition Files (*.fidl)"));
    this->plugin->contextElementsLock.lock();
    qDebug() << fileName << "\n";
    if (!fileName.isEmpty()) {
        element->filePath = fileName;
        plugin->contextElements.insert(selection.toString(), element);
    }
    this->plugin->contextElementsLock.unlock();
}
