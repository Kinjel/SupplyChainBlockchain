#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

// Qt Charts
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addBatchButton_clicked();
    void on_verifyBtn_clicked();
    void on_deleteBtn_clicked();
    void on_exportBtn_clicked();

private:
    Ui::MainWindow *ui;

    // ===== Helper functions =====
    void setupTable();
    void setupChart();
    void updateChart();
    QString calculateHash(const QString &data);

    // ===== Chart objects =====
    QChartView *chartView;
    QBarSeries *barSeries;
    QBarSet *barSet;
    QBarCategoryAxis *axisX;
    QValueAxis *axisY;
};

#endif // MAINWINDOW_H
