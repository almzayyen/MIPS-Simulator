#ifndef VIRTUAL_MACHINE_GUI_HPP
#define VIRTUAL_MACHINE_GUI_HPP
#include"message_queue.h"
#include <QString>
#include <QWidget>
#include <QPlainTextEdit>
#include <QTableView>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QMainWindow>
#include"parser.hpp"
#include"VirtualMachine.h"
#include <QLabel>
#include <QItemDelegate>
#include <QStandardItemModel>
#include<QMainWindow>
#include<QHBoxLayout>
#include<string>
#include <stdio.h>
#include <stdlib.h>
#include<QFile>
#include <QTextStream>
#include <QDebug>
#include<thread>
#include<iomanip>
#include<QFuture>
#include <QThread>
#include <QtConcurrent/qtconcurrentrun.h>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include<QTextBlock>
#include<QTextBlockFormat>
#include<QTextCursor>
#include<QTextBrowser>
#include <QTextEdit>
// TODO define the GUI class

class VirtualMachineGUI: public QWidget{
    Q_OBJECT
public:
    VirtualMachineGUI();
    //VirtualMachineGUI(const message_queue<std::string>& queue);
    void load(QString filename );
    ~VirtualMachineGUI();
    void vm_thread_main();
    void execute();
  //  signals:
  void fill_reg();
    void fill_mem();
    void update_mem();
    void update_reg();
    void set_mem(int index,std::string item);

 public slots:
    void pushed_status();
    void run();
    void r_break();
    void step();


private:
    QFuture<void> f2;
    int lines ;

         int cursor ;
         int ks;
    Parser parser;
    QStandardItemModel *reg_model;
    QStandardItemModel *mem_model ;
    std::mutex mute;
// VirtualMachine vm; // have to implement operater = in vm, or figure out how to use singleton
    QTableView * m_registers;
    //  m_registers->setObjectName("registers");
    QTableView * m_memory;
    QPlainTextEdit * m_text;
    QHBoxLayout *box ;
    QLineEdit * m_status;
    QPushButton * m_button;
    QPushButton * m_run;
    QPushButton * m_break;
    QLabel * m_status_lab ;

    message_queue <std::string> m_mqueue;

};


#endif
