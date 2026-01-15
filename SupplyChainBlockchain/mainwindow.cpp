#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>
#include <QInputDialog>
#include <QDateTime>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupTable();
    setupChart();
    updateChart();

    // Disable manual editing
    ui->batchTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/////////////////////////////////////////////////////
// TABLE SETUP
/////////////////////////////////////////////////////

void MainWindow::setupTable()
{
    ui->batchTable->setColumnCount(5);
    ui->batchTable->setHorizontalHeaderLabels(
        {"Batch ID", "Material", "Quantity", "Timestamp", "Hash"}
        );

    ui->batchTable->setRowCount(3);

    ui->batchTable->setItem(0, 0, new QTableWidgetItem("B001"));
    ui->batchTable->setItem(0, 1, new QTableWidgetItem("Steel"));
    ui->batchTable->setItem(0, 2, new QTableWidgetItem("120"));
    ui->batchTable->setItem(0, 3, new QTableWidgetItem("2026-01-13"));
    ui->batchTable->setItem(0, 4, new QTableWidgetItem("A9F3X2"));

    ui->batchTable->setItem(1, 0, new QTableWidgetItem("B002"));
    ui->batchTable->setItem(1, 1, new QTableWidgetItem("Plastic"));
    ui->batchTable->setItem(1, 2, new QTableWidgetItem("300"));
    ui->batchTable->setItem(1, 3, new QTableWidgetItem("2026-01-14"));
    ui->batchTable->setItem(1, 4, new QTableWidgetItem("K8P1L9"));

    ui->batchTable->setItem(2, 0, new QTableWidgetItem("B003"));
    ui->batchTable->setItem(2, 1, new QTableWidgetItem("Copper"));
    ui->batchTable->setItem(2, 2, new QTableWidgetItem("75"));
    ui->batchTable->setItem(2, 3, new QTableWidgetItem("2026-01-15"));
    ui->batchTable->setItem(2, 4, new QTableWidgetItem("Z7M2Q8"));
}

/////////////////////////////////////////////////////
// CHART SETUP
/////////////////////////////////////////////////////

void MainWindow::setupChart()
{
    if (!ui->chartContainer->layout())
        ui->chartContainer->setLayout(new QVBoxLayout());

    barSet = new QBarSet("Quantity");
    barSeries = new QBarSeries();
    barSeries->append(barSet);

    QChart *chart = new QChart();
    chart->addSeries(barSeries);
    chart->setTitle("Supply Chain Overview");

    axisX = new QBarCategoryAxis();
    axisY = new QValueAxis();
    axisY->setTitleText("Quantity");

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    barSeries->attachAxis(axisX);
    barSeries->attachAxis(axisY);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui->chartContainer->layout()->addWidget(chartView);
}

/////////////////////////////////////////////////////
// UPDATE GRAPH
/////////////////////////////////////////////////////

void MainWindow::updateChart()
{
    barSet->remove(0, barSet->count());
    axisX->clear();

    int maxValue = 0;

    for (int i = 0; i < ui->batchTable->rowCount(); ++i) {
        QString id = ui->batchTable->item(i, 0)->text();
        int qty = ui->batchTable->item(i, 2)->text().toInt();

        *barSet << qty;
        axisX->append(id);

        if (qty > maxValue)
            maxValue = qty;
    }

    axisY->setRange(0, maxValue + 50);
}

/////////////////////////////////////////////////////
// ADD BATCH
/////////////////////////////////////////////////////

void MainWindow::on_addBatchButton_clicked()
{
    bool ok;

    QString id = QInputDialog::getText(
        this, "Add Batch", "Batch ID:", QLineEdit::Normal, "", &ok);
    if (!ok || id.isEmpty()) return;

    QString material = QInputDialog::getText(
        this, "Add Batch", "Material:", QLineEdit::Normal, "", &ok);
    if (!ok || material.isEmpty()) return;

    int qty = QInputDialog::getInt(
        this, "Add Batch", "Quantity:", 1, 1, 100000, 1, &ok);
    if (!ok) return;

    QString timestamp =
        QDateTime::currentDateTime().toString("yyyy-MM-dd");

    int row = ui->batchTable->rowCount();
    ui->batchTable->insertRow(row);

    QString prevHash =
        (row == 0)
            ? "GENESIS"
            : ui->batchTable->item(row - 1, 4)->text();

    QString blockData =
        prevHash + id + material + QString::number(qty) + timestamp;

    QString hash = calculateHash(blockData);

    ui->batchTable->setItem(row, 0, new QTableWidgetItem(id));
    ui->batchTable->setItem(row, 1, new QTableWidgetItem(material));
    ui->batchTable->setItem(row, 2, new QTableWidgetItem(QString::number(qty)));
    ui->batchTable->setItem(row, 3, new QTableWidgetItem(timestamp));
    ui->batchTable->setItem(row, 4, new QTableWidgetItem(hash));

    updateChart();
}



/////////////////////////////////////////////////////
// VERIFY CHAIN
/////////////////////////////////////////////////////

void MainWindow::on_verifyBtn_clicked()
{
    for (int i = 0; i < ui->batchTable->rowCount(); ++i) {

        QString prevHash =
            (i == 0)
                ? "GENESIS"
                : ui->batchTable->item(i - 1, 4)->text();

        QString id        = ui->batchTable->item(i, 0)->text();
        QString material  = ui->batchTable->item(i, 1)->text();
        QString qty       = ui->batchTable->item(i, 2)->text();
        QString timestamp = ui->batchTable->item(i, 3)->text();

        QString expected =
            calculateHash(prevHash + id + material + qty + timestamp);

        QString stored =
            ui->batchTable->item(i, 4)->text();

        if (expected != stored) {
            QMessageBox::critical(
                this,
                "Invalid Chain",
                "Blockchain broken at row " + QString::number(i + 1)
                );
            return;
        }
    }

    QMessageBox::information(
        this,
        "Valid Chain",
        "Blockchain is valid"
        );
}


/////////////////////////////////////////////////////
// DELETE ROW
/////////////////////////////////////////////////////

void MainWindow::on_deleteBtn_clicked()
{
    int row = ui->batchTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Delete", "Select a row first");
        return;
    }

    ui->batchTable->removeRow(row);
    updateChart();
}

/////////////////////////////////////////////////////
// HASH FUNCTION
/////////////////////////////////////////////////////

QString MainWindow::calculateHash(const QString &data)
{
    return QCryptographicHash::hash(
               data.toUtf8(),
               QCryptographicHash::Sha256
               ).toHex();
}


void MainWindow::on_exportBtn_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Export Blockchain Data",
        "",
        "CSV Files (*.csv)"
        );

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Export Failed", "Cannot write file");
        return;
    }

    QTextStream out(&file);

    // Write headers
    for (int col = 0; col < ui->batchTable->columnCount(); ++col) {
        out << ui->batchTable->horizontalHeaderItem(col)->text();
        if (col < ui->batchTable->columnCount() - 1)
            out << ",";
    }
    out << "\n";

    // Write rows
    for (int row = 0; row < ui->batchTable->rowCount(); ++row) {
        for (int col = 0; col < ui->batchTable->columnCount(); ++col) {
            out << ui->batchTable->item(row, col)->text();
            if (col < ui->batchTable->columnCount() - 1)
                out << ",";
        }
        out << "\n";
    }

    file.close();

    QMessageBox::information(
        this,
        "Export Successful",
        "Blockchain data exported successfully."
        );
}

