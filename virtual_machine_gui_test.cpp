#include <QTest>
#include <QPlainTextEdit>
#include <QTableView>
#include <QLineEdit>
#include <QPushButton>

#include "virtual_machine_gui.hpp"

#include "test_config.hpp"

class VirtualMachineGUITest : public QObject {
Q_OBJECT

private slots:

    void initTestCase();

    void test_1();

    void test_2();

    void test_3();

    void test_4();


    // TODO: implement additional tests here

private:

    VirtualMachineGUI widget;
};

// this section just verifies the object names and API
void VirtualMachineGUITest::initTestCase() {

    widget.load(QString::fromStdString(TEST_FILE_DIR + "/vm/test00.asm"));

    auto textWidget = widget.findChild<QPlainTextEdit *>("text");
    QVERIFY2(textWidget, "Could not find QPLainText widget");

    auto registerViewWidget = widget.findChild<QTableView *>("registers");
    QVERIFY2(registerViewWidget, "Could not find QTableView widget for registers");

    auto memoryViewWidget = widget.findChild<QTableView *>("memory");
    QVERIFY2(memoryViewWidget, "Could not find QTableView widget for memory");

    auto statusViewWidget = widget.findChild<QLineEdit *>("status");
    QVERIFY2(statusViewWidget, "Could not find QLineEdit widget for status");

    auto stepButtonWidget = widget.findChild<QPushButton *>("step");
    QVERIFY2(stepButtonWidget, "Could not find QTableView widget for memory");
}

void VirtualMachineGUITest::test_1() {

    widget.load(QString::fromStdString(TEST_FILE_DIR + "/vm/test00.asm"));

    auto regwidget = widget.findChild<QTableView *>("registers");
    //QVERIFY2(textWidget);

    // auto reg_model2 = widget.findChild<QStandardItemModel *>("regmodel");
    // QVERIFY2(reg_model2, "Could not find QTableView widget for registers");
    auto reg = regwidget->model()->data(regwidget->model()->index(0, 2)).toString(); //pc
    //  qDebug() <<  reg_model->data(index,0);
    QCOMPARE(reg, QString("0x00000000"));
    widget.step();
    QCOMPARE(reg, QString("0x00000000"));
    widget.step();
    QCOMPARE(reg, QString("0x00000000"));
    widget.step();
    QCOMPARE(reg, QString("0x00000000"));
    widget.step();
    QCOMPARE(reg, QString("0x00000000"));

}

void VirtualMachineGUITest::test_2() {
    VirtualMachine::getVirtualMachine()->reset_mem();
    widget.load(QString::fromStdString(TEST_FILE_DIR + "/vm/test02.asm"));

    auto regwidget = widget.findChild<QTableView *>("registers");
    auto stat = widget.findChild<QLineEdit *>("status");
    auto memwidget = widget.findChild<QTableView *>("memory");
    //QVERIFY2(textWidget);


    //QModelIndex s  = regwidget->model()->index(0,2.data().toInt());
    // QModelIndex s = index(0, 2,  QModelIndex());
    auto reg = regwidget->model()->data(regwidget->model()->index(0, 2)).toString(); //pc
    auto mem = memwidget->model()->data(memwidget->model()->index(0, 1)).toString(); //pc

    auto index = stat->text();
    //  qDebug() <<  reg_model->data(index,0);
    QCOMPARE(reg, QString("0x00000000"));
    widget.step();
    widget.step();
    widget.update_reg();
    widget.update_mem();

    reg = regwidget->model()->data(regwidget->model()->index(0, 2)).toString();
    QCOMPARE(reg, QString("0x00000002"));
    reg = regwidget->model()->data(regwidget->model()->index(0, 1)).toString();
    widget.pushed_status();
    index = stat->text();
    widget.step();
    widget.step();
    widget.update_reg();
    widget.update_mem();
    reg = regwidget->model()->data(regwidget->model()->index(0, 2)).toString();
    widget.pushed_status();
    // widget.set_mem(0,"07");
    mem = memwidget->model()->data(memwidget->model()->index(0, 1)).toString(); //mem
    QVERIFY(reg == QString("0x00000004"));
    QCOMPARE(mem, QString("0x07"));
    QVERIFY(index == QString("ok"));
    widget.step();
    widget.step();
    widget.update_reg();
    widget.update_mem();

    reg = regwidget->model()->data(regwidget->model()->index(0, 2)).toString();

    widget.pushed_status();
    index = stat->text();
    QVERIFY(reg == QString("0x00000006"));


    reg = regwidget->model()->data(regwidget->model()->index(0, 2)).toString();
    widget.pushed_status();
    index = stat->text();


}

void VirtualMachineGUITest::test_3() {
    VirtualMachine::getVirtualMachine()->reset_mem();
    VirtualMachine::getVirtualMachine()->reg_init();
    widget.load(QString::fromStdString(TEST_FILE_DIR + "/vm/test03.asm"));

    auto regwidget = widget.findChild<QTableView *>("registers");
    auto stat = widget.findChild<QLineEdit *>("status");
    auto memwidget = widget.findChild<QTableView *>("memory");


    widget.fill_mem();
    widget.fill_reg();
    auto reg = regwidget->model()->data(regwidget->model()->index(0, 2)).toString(); //pc
    auto mem = memwidget->model()->data(memwidget->model()->index(0, 1)).toString();

    auto index = stat->text();
    //  qDebug() <<  reg;
    QCOMPARE(reg, QString("0x00000000"));
    widget.step();
    reg = regwidget->model()->data(regwidget->model()->index(11, 2)).toString();
    QCOMPARE(reg, QString("0x00000064"));
    widget.step();
    widget.update_reg();
    widget.update_mem();

    reg = regwidget->model()->data(regwidget->model()->index(0, 2)).toString();
    QCOMPARE(reg, QString("0x00000002"));
    reg = regwidget->model()->data(regwidget->model()->index(0, 1)).toString();
    widget.pushed_status();
    index = stat->text();
    widget.step();
    widget.step();
    widget.update_reg();
    widget.update_mem();
    reg = regwidget->model()->data(regwidget->model()->index(0, 2)).toString();
    widget.pushed_status();
    // widget.set_mem(0,"07");
    mem = memwidget->model()->data(memwidget->model()->index(0, 1)).toString(); //mem
    QVERIFY(reg == QString("0x00000004"));
    QCOMPARE(mem, QString("0x00"));
    QVERIFY(index == QString("ok"));
    widget.step();
    widget.step();
    widget.update_reg();
    widget.update_mem();

    reg = regwidget->model()->data(regwidget->model()->index(0, 2)).toString();

    widget.pushed_status();
    index = stat->text();
    QCOMPARE(reg, QString("0x00000005"));


    reg = regwidget->model()->data(regwidget->model()->index(0, 2)).toString();
    widget.pushed_status();
    index = stat->text();


}

void VirtualMachineGUITest::test_4() {
    VirtualMachine::getVirtualMachine()->reset_mem();
    VirtualMachine::getVirtualMachine()->reg_init();
    widget.load(QString::fromStdString(TEST_FILE_DIR + "/vm/test03.asm"));

    auto regwidget = widget.findChild<QTableView *>("registers");
    auto stat = widget.findChild<QLineEdit *>("status");
    auto memwidget = widget.findChild<QTableView *>("memory");
}


QTEST_MAIN(VirtualMachineGUITest)

#include "virtual_machine_gui_test.moc"

