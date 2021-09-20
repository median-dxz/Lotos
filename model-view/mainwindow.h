#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QEvent>
#include <QFile>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>
#include <QListView>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QPushButton>
#include <QStringListModel>
#include <QTableView>
#include "picturetable.h"

class MainWindow : public QWidget {
    Q_OBJECT

   public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

   public slots:
    void insertData();
    void deleteData();
    void showData();

   private:
};
#endif  // MAINWINDOW_H
