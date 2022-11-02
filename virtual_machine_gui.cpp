#include <QtCore/QChar>
#include "virtual_machine_gui.hpp"
#include <QMessageBox>
#include <QVBoxLayout>
// TODO implement the GUI class

void VirtualMachineGUI::vm_thread_main() {
    bool leave = true;
    std::string s;
    while (leave) {
        m_mqueue.wait_and_pop(s);

        if (s == "step") {

            VirtualMachine::getVirtualMachine()->step();
            leave = false;

        } else if (s == "run") {
            do {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                VirtualMachine::getVirtualMachine()->step2();


                execute();

                cursor++;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));

            } while (m_mqueue.try_pop(s) == false || (s != "break"));


            leave = false;
        }

    }
   // execute();


}

void VirtualMachineGUI::load(QString filename) {
    QFile file(filename);


    m_text->setReadOnly(true);
    std::string str = filename.toStdString();
    char c;
    std::string temp;
    std::string fileee = filename.toStdString();
    std::ifstream ifs(fileee);
    if (!ifs) {
        std::cerr << "Error:Unable to open file";
        return;// terminate with error
    }
    while (ifs.get(c)) {
        temp.push_back(c);
    }
    VirtualMachine::getVirtualMachine()->reg_init();

    parser.tokenize(temp);
    parser.parse_text();
    m_break->setEnabled(false);
// m_text->setWordWrapMode(QTextOption::NoWrap);
    QString content;
    QString line;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        do {
            line = stream.readLine();
            content.append(line);
            content.append("\n");
            //  qDebug() << line;
        } while (!line.isNull());


    } else {
        QMessageBox::information(this, tr("Error: Unable to open file"),
                                 file.errorString());
        return;
    }
    cursor = parser.get_line_num();
//    inst_list = parser.get_instructions();
    lines = m_text->document()->blockCount();


    m_text->setPlainText(content);
    file.close();
}

VirtualMachineGUI::~VirtualMachineGUI() {

}

VirtualMachineGUI::VirtualMachineGUI() {


    VirtualMachine::getVirtualMachine()->reg_init();
    m_registers = new QTableView(this);
    m_memory = new QTableView(this);
    m_status = new QLineEdit(this);
    m_run = new QPushButton("Run");
    m_break = new QPushButton("Break");
    m_button = new QPushButton("Step");
    m_text = new QPlainTextEdit(this);
    m_registers->setObjectName("registers");
    m_memory->setObjectName("memory");
    m_status->setObjectName("status");
    m_button->setObjectName("step");
    m_text->setObjectName("text");

    QGridLayout *mainLayout = new QGridLayout;

    reg_model = new QStandardItemModel(35, 3, this);
    mem_model = new QStandardItemModel(512, 2, this);


    fill_reg();
    fill_mem();
    m_status_lab = new QLabel("Status: ");
    m_registers->setModel(reg_model);
    m_memory->setModel(mem_model);
    m_registers->setMinimumHeight(600);
    m_memory->setMinimumHeight(600);
    m_text->setMinimumWidth(400);
    m_memory->setMinimumWidth(300);
    m_registers->setMinimumWidth(400);

    box = new QHBoxLayout();
    box->addWidget(m_status_lab);
    box->addWidget(m_status);

    mainLayout->addWidget(m_registers, 0, 1);
    mainLayout->addWidget(m_status, 1, 1);
    mainLayout->addWidget(m_memory, 0, 3);
    mainLayout->addWidget(m_text, 0, 0);
    mainLayout->addWidget(m_button, 2, 1);
    mainLayout->addWidget(m_status_lab, 1, 0);
    mainLayout->addWidget(m_run, 2, 2);
    mainLayout->addWidget(m_break, 2, 3);


    setLayout(mainLayout);

    connect(this->m_button, SIGNAL(clicked()), this, SLOT(pushed_status()));
    connect(this->m_button, SIGNAL(clicked()), this, SLOT(step()));
    connect(this->m_run, SIGNAL(clicked()), this, SLOT(run()));
    connect(this->m_break, SIGNAL(clicked()), this, SLOT(r_break()));
    m_text->setReadOnly(true);


    QTextCursor cursor1(m_text->document()->findBlockByLineNumber(cursor));
    ks = cursor;
    QTextBlockFormat q = cursor1.blockFormat();
    q.setBackground(QBrush(Qt::yellow));
    cursor1.setBlockFormat(q);
    m_text->setTextCursor(cursor1);

//    QTextBlockFormat frmt = textCursor->blockFormat();
//    frmt.setBackground(QBrush(Qt::yellow));
//    textCursor->setBlockFormat(frmt);
//    m_text->setTextCursor(*textCursor);
////    textCursor.select(QTextCursor::LineUnderCursor);
////
////    textEdit->setAutoFillBackground(true);
////    QPalette palette;
////    palette.setColor(QPalette::Background,QColor(200,200,200));
////    textEdit->setPalette(palette);

}

void VirtualMachineGUI::fill_reg() {
    reg_model->setHorizontalHeaderItem(0, new QStandardItem(QString("Number")));
    reg_model->setHorizontalHeaderItem(1, new QStandardItem(QString("Alias")));
    reg_model->setHorizontalHeaderItem(2, new QStandardItem(QString("Value(hex)")));

    for (int row = 0; row < 32; row++) {

//            QModelIndex index  = reg_model->index(row,col,QModelIndex());
//
//            reg_model->setData(index,0);
        QString s = "$" + QString::fromStdString(std::to_string(row));
        QStandardItem *items = new QStandardItem(QString(s));
        reg_model->setItem(row + 3, 0, items);


    }

    {
        QString s = "$";
        s.append("pc");
        QStandardItem *alias = new QStandardItem(QString(s));
        reg_model->setItem(0, 1, alias);

        s = "$";
        s.append("hi");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(1, 1, alias);
        s = "$";
        s.append("lo");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(2, 1, alias);
        s = "$";
        s.append("zero");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(3, 1, alias);
        s = "$";
        s.append("at");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(4, 1, alias);
        s = "$";
        s.append("v0");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(5, 1, alias);
        s = "$";
        s.append("v1");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(6, 1, alias);
        s = "$";
        s.append("a0");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(7, 1, alias);
        s = "$";
        s.append("a1");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(8, 1, alias);
        s = "$";
        s.append("a2");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(9, 1, alias);
        s = "$";
        s.append("a3");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(10, 1, alias);
        s = "$";
        s.append("t0");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(11, 1, alias);
        s = "$";
        s.append("t1");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(12, 1, alias);
        s = "$";
        s.append("t2");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(13, 1, alias);
        s = "$";
        s.append("t3");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(14, 1, alias);
        s = "$";
        s.append("t4");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(15, 1, alias);
        s = "$";
        s.append("t5");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(16, 1, alias);
        s = "$";
        s.append("t6");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(17, 1, alias);
        s = "$";
        s.append("t7");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(18, 1, alias);
        s = "$";
        s.append("s0");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(19, 1, alias);
        s = "$";
        s.append("s1");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(20, 1, alias);
        s = "$";
        s.append("s2");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(21, 1, alias);
        s = "$";
        s.append("s3");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(22, 1, alias);
        s = "$";
        s.append("s4");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(23, 1, alias);
        s = "$";
        s.append("s5");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(24, 1, alias);
        s = "$";
        s.append("s6");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(25, 1, alias);
        s = "$";
        s.append("s7");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(26, 1, alias);
        s = "$";
        s.append("t8");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(27, 1, alias);
        s = "$";
        s.append("t9");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(28, 1, alias);
        s = "$";
        s.append("k0");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(29, 1, alias);
        s = "$";
        s.append("k1");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(30, 1, alias);
        s = "$";
        s.append("gp");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(31, 1, alias);
        s = "$";
        s.append("sp");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(32, 1, alias);
        s = "$";
        s.append("fp");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(33, 1, alias);
        s = "$";
        s.append("ra");
        alias = new QStandardItem(QString(s));
        reg_model->setItem(34, 1, alias);


    }
    {//values
        std::string str = "0x";
        std::string temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_pc()));
        str.append(temp);
        QString pc = QString::fromStdString(str);

        QStandardItem *alias = new QStandardItem(QString(pc));
        reg_model->setItem(0, 2, alias);

        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$hi")));
        str.append(temp);

        QString hi = QString::fromStdString(str);
        alias = new QStandardItem(QString(hi));
        reg_model->setItem(1, 2, alias);

        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$lo")));
        str.append(temp);

        QString lo = QString::fromStdString(str);
        alias = new QStandardItem(QString(lo));
        reg_model->setItem(2, 2, alias);

        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$zero")));
        str.append(temp);

        QString zero = QString::fromStdString(str);
        alias = new QStandardItem(QString(zero));
        reg_model->setItem(3, 2, alias);

        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$at")));
        str.append(temp);
        QString at = QString::fromStdString(str);
        alias = new QStandardItem(QString(at));
        reg_model->setItem(4, 2, alias);

        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$v0")));
        str.append(temp);
        QString v0 = QString::fromStdString(str);
        alias = new QStandardItem(QString(v0));
        reg_model->setItem(5, 2, alias);


        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$v1")));
        str.append(temp);
        QString v1 = QString::fromStdString(str);
        alias = new QStandardItem(QString(v1));
        reg_model->setItem(6, 2, alias);

        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$a0")));
        str.append(temp);
        QString a0 = QString::fromStdString(str);
        alias = new QStandardItem(QString(a0));
        reg_model->setItem(7, 2, alias);


        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$a1")));
        str.append(temp);
        QString a1 = QString::fromStdString(str);
        alias = new QStandardItem(QString(a1));
        reg_model->setItem(8, 2, alias);


        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$a2")));
        str.append(temp);
        QString a2 = QString::fromStdString(str);
        alias = new QStandardItem(QString(a2));
        reg_model->setItem(9, 2, alias);

        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$a3")));
        str.append(temp);
        QString a3 = QString::fromStdString(str);
        alias = new QStandardItem(QString(a3));
        reg_model->setItem(10, 2, alias);

        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$t0")));
        str.append(temp);
        QString t0 = QString::fromStdString(str);
        alias = new QStandardItem(QString(t0));
        reg_model->setItem(11, 2, alias);


        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$t1")));
        str.append(temp);
        QString t1 = QString::fromStdString(str);
        alias = new QStandardItem(QString(t1));
        reg_model->setItem(12, 2, alias);

        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$t2")));
        str.append(temp);
        QString t2 = QString::fromStdString(str);
        alias = new QStandardItem(QString(t2));
        reg_model->setItem(13, 2, alias);


        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$t3")));
        str.append(temp);
        QString t3 = QString::fromStdString(str);
        alias = new QStandardItem(QString(t3));
        reg_model->setItem(14, 2, alias);


        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$t4")));
        str.append(temp);
        QString t4 = QString::fromStdString(str);
        alias = new QStandardItem(QString(t4));
        reg_model->setItem(15, 2, alias);

        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$t5")));
        str.append(temp);
        QString t5 = QString::fromStdString(str);
        alias = new QStandardItem(QString(t5));
        reg_model->setItem(16, 2, alias);


        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$t6")));
        str.append(temp);
        QString t6 = QString::fromStdString(str);
        alias = new QStandardItem(QString(t6));
        reg_model->setItem(17, 2, alias);


        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$t7")));
        str.append(temp);
        QString t7 = QString::fromStdString(str);
        alias = new QStandardItem(QString(t7));
        reg_model->setItem(18, 2, alias);

        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$s0")));
        str.append(temp);
        QString s0 = QString::fromStdString(str);
        alias = new QStandardItem(QString(s0));
        reg_model->setItem(19, 2, alias);


        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$s1")));
        str.append(temp);
        QString s1 = QString::fromStdString(str);
        alias = new QStandardItem(QString(s1));
        reg_model->setItem(20, 2, alias);


        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$s2")));
        str.append(temp);
        QString s2 = QString::fromStdString(str);
        alias = new QStandardItem(QString(s2));
        reg_model->setItem(21, 2, alias);

        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$s3")));
        str.append(temp);
        QString s3 = QString::fromStdString(str);
        alias = new QStandardItem(QString(s3));
        reg_model->setItem(22, 2, alias);


        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$s4")));
        str.append(temp);
        QString s4 = QString::fromStdString(str);
        alias = new QStandardItem(QString(s4));
        reg_model->setItem(23, 2, alias);


        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$s5")));
        str.append(temp);
        QString s5 = QString::fromStdString(str);
        alias = new QStandardItem(QString(s5));
        reg_model->setItem(24, 2, alias);

        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$s6")));
        str.append(temp);
        QString s6 = QString::fromStdString(str);
        alias = new QStandardItem(QString(s6));
        reg_model->setItem(25, 2, alias);


        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$s7")));
        str.append(temp);
        QString s7 = QString::fromStdString(str);
        alias = new QStandardItem(QString(s7));
        reg_model->setItem(26, 2, alias);


        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$t8")));
        str.append(temp);
        QString t8 = QString::fromStdString(str);
        alias = new QStandardItem(QString(t8));
        reg_model->setItem(27, 2, alias);

        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$t9")));
        str.append(temp);
        QString t9 = QString::fromStdString(str);
        alias = new QStandardItem(QString(t9));
        reg_model->setItem(28, 2, alias);


        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$k0")));
        str.append(temp);
        QString k0 = QString::fromStdString(str);
        alias = new QStandardItem(QString(k0));
        reg_model->setItem(29, 2, alias);


        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$k1")));
        str.append(temp);
        QString k1 = QString::fromStdString(str);
        alias = new QStandardItem(QString(k1));
        reg_model->setItem(30, 2, alias);

        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$gp")));
        str.append(temp);
        QString gp = QString::fromStdString(str);
        alias = new QStandardItem(QString(gp));
        reg_model->setItem(31, 2, alias);


        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$sp")));
        str.append(temp);
        QString sp = QString::fromStdString(str);
        alias = new QStandardItem(QString(sp));
        reg_model->setItem(32, 2, alias);

        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$fp")));
        str.append(temp);
        QString fp = QString::fromStdString(str);
        alias = new QStandardItem(QString(fp));
        reg_model->setItem(33, 2, alias);

        str = "0x";
        temp = std::string(7, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$ra")));
        str.append(temp);
        QString ra = QString::fromStdString(str);
        alias = new QStandardItem(QString(ra));
        reg_model->setItem(34, 2, alias);


    }
}

void VirtualMachineGUI::fill_mem() {
    mem_model->setHorizontalHeaderItem(0, new QStandardItem(QString("Address(hex)")));
    mem_model->setHorizontalHeaderItem(1, new QStandardItem(QString("Value(hex)")));


    std::stringstream stream;
    char hex[50];
    std::string str, temp;
    for (int row = 0; row < 512; row++) {

        sprintf(hex, "%x", row);

        str = "0x";
        temp = std::string(7, '0').append(hex);
        str.append(temp);
        QString s = QString::fromStdString(str);
        QStandardItem *items = new QStandardItem(QString(s));
        mem_model->setItem(row, 0, items);

        str = "0x";
        temp = std::string(1, '0').append(std::to_string(VirtualMachine::getVirtualMachine()->at_mem(row)));
        str.append(temp);
        s = QString::fromStdString(str);
        items = new QStandardItem(QString(s));
        mem_model->setItem(row, 1, items);


    }
}

void VirtualMachineGUI::pushed_status() {
    if (parser.get_error() == "")
        this->m_status->setText("ok");
    else {
        this->m_status->setText("Error:");
    }
}

void VirtualMachineGUI::step() {
    m_mqueue.push("step");


    QFuture<void> f2 = QtConcurrent::run(this, &VirtualMachineGUI::vm_thread_main);
    f2.waitForFinished();
    //std::this_thread::sleep_for( std::chrono::milliseconds(100));

        for (int a = 0; a < 20; a++) {
            QTextCursor cursor(m_text->document()->findBlockByLineNumber(a));
            QTextBlockFormat c = cursor.blockFormat();
            c.setBackground(QBrush(Qt::transparent));
            cursor.setBlockFormat(c);
        }

        QTextCursor cursor1(m_text->document()->findBlockByLineNumber(cursor));
        QTextBlockFormat q = cursor1.blockFormat();
        q.setBackground(QBrush(Qt::yellow));
        cursor1.setBlockFormat(q);
        m_text->setTextCursor(cursor1);
        cursor++;


    update_mem();
    update_reg();
}

void VirtualMachineGUI::update_mem() {
    std::stringstream stream;
    char hex[50];
    std::string str, temp;
    for (int row = 0; row < 512; row++) {


        stream << std::hex << VirtualMachine::getVirtualMachine()->at_mem(row);
        temp = stream.str();
        int n = VirtualMachine::getVirtualMachine()->at_mem(row);
        sprintf(hex, "%02X", n);
        str = "0x";
        str.append(hex);
        QString s = QString::fromStdString(str);
        QStandardItem *items = new QStandardItem(QString(s));
        mem_model->setItem(row, 1, items);


    }
}

void VirtualMachineGUI::update_reg() {
    std::string str = "0x";
    char hex[50];
    std::ostringstream stream;
    int n = VirtualMachine::getVirtualMachine()->get_pc();
    sprintf(hex, "%08x", n);

    str.append(hex);
    QString pc = QString::fromStdString(str);

    QStandardItem *alias = new QStandardItem(pc);
    reg_model->setItem(0, 2, alias);

    str = "0x";

    n = VirtualMachine::getVirtualMachine()->get_reg("$lo");
    sprintf(hex, "%08x", n);

    str.append(hex);

    QString hi = QString::fromStdString(str);
    alias = new QStandardItem(QString(hi));
    reg_model->setItem(1, 2, alias);

    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$hi");
    sprintf(hex, "%08x", n);

    str.append(hex);
    QString lo = QString::fromStdString(str);
    alias = new QStandardItem(QString(lo));
    reg_model->setItem(2, 2, alias);

    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$zero");
    sprintf(hex, "%08x", n);

    str.append(hex);

    QString zero = QString::fromStdString(str);
    alias = new QStandardItem(QString(zero));
    reg_model->setItem(3, 2, alias);

    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$at");
    sprintf(hex, "%08x", n);

    str.append(hex);

    QString at = QString::fromStdString(str);
    alias = new QStandardItem(QString(at));
    reg_model->setItem(4, 2, alias);

    str = "0x";

    n = VirtualMachine::getVirtualMachine()->get_reg("$v0");
    sprintf(hex, "%08x", n);

    str.append(hex);

    QString v0 = QString::fromStdString(str);
    alias = new QStandardItem(QString(v0));
    reg_model->setItem(5, 2, alias);


    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$v1");
    sprintf(hex, "%08x", n);

    str.append(hex);
    QString v1 = QString::fromStdString(str);
    alias = new QStandardItem(QString(v1));
    reg_model->setItem(6, 2, alias);

    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$a0");
    sprintf(hex, "%08x", n);

    str.append(hex);

    QString a0 = QString::fromStdString(str);
    alias = new QStandardItem(QString(a0));
    reg_model->setItem(7, 2, alias);


    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$a1");
    sprintf(hex, "%08x", n);

    str.append(hex);
    QString a1 = QString::fromStdString(str);
    alias = new QStandardItem(QString(a1));
    reg_model->setItem(8, 2, alias);


    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$a2");
    sprintf(hex, "%08x", n);

    str.append(hex);
    QString a2 = QString::fromStdString(str);
    alias = new QStandardItem(QString(a2));
    reg_model->setItem(9, 2, alias);

    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$a3");
    sprintf(hex, "%08x", n);

    str.append(hex);
    QString a3 = QString::fromStdString(str);
    alias = new QStandardItem(QString(a3));
    reg_model->setItem(10, 2, alias);

    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$t0");

    sprintf(hex, "%08x", n);

    // temp =  std::string( 7, '0').append( std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$t0")));
    str.append(hex);

    QString t0 = QString::fromStdString(str);
    alias = new QStandardItem(QString(t0));
    reg_model->setItem(11, 2, alias);


    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$t1");
    sprintf(hex, "%08x", n);

    // temp =  std::string( 7, '0').append( std::to_string(VirtualMachine::getVirtualMachine()->get_reg("$t1")));
    str.append(hex);

    QString t1 = QString::fromStdString(str);
    alias = new QStandardItem(QString(t1));
    reg_model->setItem(12, 2, alias);

    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$t2");
    sprintf(hex, "%08x", n);

    str.append(hex);
    QString t2 = QString::fromStdString(str);
    alias = new QStandardItem(QString(t2));
    reg_model->setItem(13, 2, alias);


    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$t3");
    sprintf(hex, "%08x", n);

    str.append(hex);
    QString t3 = QString::fromStdString(str);
    alias = new QStandardItem(QString(t3));
    reg_model->setItem(14, 2, alias);


    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$t4");
    sprintf(hex, "%08x", n);

    str.append(hex);

    QString t4 = QString::fromStdString(str);
    alias = new QStandardItem(QString(t4));
    reg_model->setItem(15, 2, alias);

    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$t5");
    sprintf(hex, "%08x", n);

    str.append(hex);

    QString t5 = QString::fromStdString(str);
    alias = new QStandardItem(QString(t5));
    reg_model->setItem(16, 2, alias);


    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$t6");
    sprintf(hex, "%08x", n);

    str.append(hex);

    QString t6 = QString::fromStdString(str);
    alias = new QStandardItem(QString(t6));
    reg_model->setItem(17, 2, alias);


    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$t7");
    sprintf(hex, "%08x", n);

    str.append(hex);
    QString t7 = QString::fromStdString(str);
    alias = new QStandardItem(QString(t7));
    reg_model->setItem(18, 2, alias);

    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$s0");
    sprintf(hex, "%08x", n);

    str.append(hex);
    QString s0 = QString::fromStdString(str);
    alias = new QStandardItem(QString(s0));
    reg_model->setItem(19, 2, alias);

    std::string temp;
    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$s1");
    sprintf(hex, "%08x", n);

    str.append(hex);
    QString s1 = QString::fromStdString(str);
    alias = new QStandardItem(QString(s1));
    reg_model->setItem(20, 2, alias);


    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$s2");
    sprintf(hex, "%08x", n);

    str.append(hex);
    QString s2 = QString::fromStdString(str);
    alias = new QStandardItem(QString(s2));
    reg_model->setItem(21, 2, alias);

    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$s3");
    sprintf(hex, "%08x", n);

    str.append(hex);
    QString s3 = QString::fromStdString(str);
    alias = new QStandardItem(QString(s3));
    reg_model->setItem(22, 2, alias);


    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$s4");
    sprintf(hex, "%08x", n);

    str.append(hex);
    QString s4 = QString::fromStdString(str);
    alias = new QStandardItem(QString(s4));
    reg_model->setItem(23, 2, alias);


    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$s5");
    sprintf(hex, "%08x", n);

    str.append(hex);
    QString s5 = QString::fromStdString(str);
    alias = new QStandardItem(QString(s5));
    reg_model->setItem(24, 2, alias);

    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$s6");
    sprintf(hex, "%08x", n);

    str.append(hex);
    QString s6 = QString::fromStdString(str);
    alias = new QStandardItem(QString(s6));
    reg_model->setItem(25, 2, alias);


    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$s7");
    sprintf(hex, "%08x", n);

    str.append(hex);
    QString s7 = QString::fromStdString(str);
    alias = new QStandardItem(QString(s7));
    reg_model->setItem(26, 2, alias);


    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$t8");
    sprintf(hex, "%08x", n);

    str.append(hex);
    QString t8 = QString::fromStdString(str);
    alias = new QStandardItem(QString(t8));
    reg_model->setItem(27, 2, alias);

    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$t9");
    sprintf(hex, "%08x", n);

    str.append(hex);
    QString t9 = QString::fromStdString(str);
    alias = new QStandardItem(QString(t9));
    reg_model->setItem(28, 2, alias);


    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$k0");
    sprintf(hex, "%08x", n);

    str.append(hex);
    QString k0 = QString::fromStdString(str);
    alias = new QStandardItem(QString(k0));
    reg_model->setItem(29, 2, alias);


    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$k1");
    sprintf(hex, "%08x", n);

    str.append(hex);
    QString k1 = QString::fromStdString(str);
    alias = new QStandardItem(QString(k1));
    reg_model->setItem(30, 2, alias);

    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$gp");
    sprintf(hex, "%08x", n);

    str.append(hex);
    QString gp = QString::fromStdString(str);
    alias = new QStandardItem(QString(gp));
    reg_model->setItem(31, 2, alias);


    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$sp");
    sprintf(hex, "%08x", n);

    str.append(hex);
    QString sp = QString::fromStdString(str);
    alias = new QStandardItem(QString(sp));
    reg_model->setItem(32, 2, alias);

    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$fp");
    sprintf(hex, "%08x", n);

    str.append(hex);
    QString fp = QString::fromStdString(str);
    alias = new QStandardItem(QString(fp));
    reg_model->setItem(33, 2, alias);

    str = "0x";
    n = VirtualMachine::getVirtualMachine()->get_reg("$ra");
    sprintf(hex, "%08x", n);

    str.append(hex);
    QString ra = QString::fromStdString(str);
    alias = new QStandardItem(QString(ra));
    reg_model->setItem(34, 2, alias);
}

void VirtualMachineGUI::set_mem(int index, std::string item) {
    std::string str = "0x";
    std::string temp = item;
    str.append(temp);

    QString ra = QString::fromStdString(str);
    QStandardItem *alias = new QStandardItem(QString(ra));
    mem_model->setItem(index, 1, alias);
}

//VirtualMachineGUI::VirtualMachineGUI(message_queue<std::string> &queue): m_mqueue(queue) {
//
//
//    VirtualMachine::getVirtualMachine()->reg_init();
//    m_registers = new QTableView(this);
//    m_memory = new QTableView(this);
//    m_status = new QLineEdit(this);
//    m_run = new QPushButton("Run");
//    m_break = new QPushButton("Break");
//    m_button = new QPushButton("Step");
//    m_text = new QPlainTextEdit(this);
//    m_registers->setObjectName("registers");
//    m_memory->setObjectName("memory");
//    m_status->setObjectName("status");
//    m_button->setObjectName("step");
//    m_text->setObjectName("text");
//
//    QGridLayout *mainLayout = new QGridLayout;
//
//    reg_model = new QStandardItemModel(35, 3, this);
//    mem_model = new QStandardItemModel(512, 2, this);
//
//
//    fill_reg();
//    fill_mem();
//    m_status_lab = new QLabel("Status: ");
//    m_registers->setModel(reg_model);
//    m_memory->setModel(mem_model);
//    m_registers->setMinimumHeight(600);
//    m_memory->setMinimumHeight(600);
//    m_text->setMinimumWidth(400);
//    m_memory->setMinimumWidth(300);
//    m_registers->setMinimumWidth(400);
//
//    box = new QHBoxLayout();
//    box->addWidget(m_status_lab);
//    box->addWidget(m_status);
//
//    mainLayout->addWidget(m_registers, 0, 1);
//    mainLayout->addWidget(m_status, 1, 1);
//    mainLayout->addWidget(m_memory, 0, 3);
//    mainLayout->addWidget(m_text, 0, 0);
//    mainLayout->addWidget(m_button, 2, 1);
//    mainLayout->addWidget(m_status_lab, 1, 0);
//    mainLayout->addWidget(m_run,2,2);
//    mainLayout->addWidget(m_break,2,3);
//    //  mainLayout->addLayout (box, 1, 0);
//
//    // m_status->setFixedWidth(1000);
//    setLayout(mainLayout);
//    // m_registers->show();
//    connect(this->m_button, SIGNAL(clicked()), this, SLOT(pushed_status()));
//    connect(this->m_button, SIGNAL(clicked()), this, SLOT(step()));
//    connect(this->m_run, SIGNAL(clicked()), this, SLOT(run()));
//    connect(this->m_break, SIGNAL(clicked()), this, SLOT(r_break()));
//

//}

void VirtualMachineGUI::r_break() {
    m_mqueue.push("break");
    m_break->setEnabled(false);
    f2.waitForFinished();
    //std::this_thread::sleep_for( std::chrono::seconds(1));

    pushed_status();
    update_mem();
    update_reg();
}

void VirtualMachineGUI::run() {
    m_mqueue.push("run");

     f2 = QtConcurrent::run(this, &VirtualMachineGUI::vm_thread_main);

    m_break->setEnabled(true);

    //std::this_thread::sleep_for( std::chrono::seconds(3));
    pushed_status();


    //VirtualMachine::getVirtualMachine()->step();
    update_mem();
    update_reg();

}

void VirtualMachineGUI::execute() {

    for (int a = 0; a < 15; a++) {
        QTextCursor cursor(m_text->document()->findBlockByLineNumber(a));
        QTextBlockFormat c = cursor.blockFormat();
        c.setBackground(QBrush(Qt::transparent));
        cursor.setBlockFormat(c);
    }

    QTextCursor cursor1(m_text->document()->findBlockByLineNumber(cursor));
    QTextBlockFormat q = cursor1.blockFormat();
    q.setBackground(QBrush(Qt::yellow));
    cursor1.setBlockFormat(q);
    m_text->setTextCursor(cursor1);


}



