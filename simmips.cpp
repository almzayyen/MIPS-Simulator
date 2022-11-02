#include <vector>
#include <string>
#include <iostream>

#include "parser.hpp"
#include "VirtualMachine.h"
#include"../milestone2-almzayyen/Instruction.h"
#include<fstream>
#include<iomanip>
#include<iostream>
#include "virtual_machine_gui.hpp"
#include<QApplication>
#include <mqueue.h>
#include<thread>
#include<queue>
#include"message_queue.h"
#include<QFuture>
#include <QThread>
#include <QtConcurrent/qtconcurrentrun.h>
static message_queue<std::string> queue;
std::mutex mute;
void vm_thread_main()
{
    std::string s;

    while((queue.wait_and_pop(s)))
    {
        if(s ==  "step")
        { std::lock_guard<std::mutex> guard(mute);
            //std::this_thread::sleep_for( std::chrono::nanoseconds(400));
            VirtualMachine::getVirtualMachine()->step();
            //wait


        }
        else if (s == "run")
        {
            do{ std::lock_guard<std::mutex> guard(mute);

                VirtualMachine::getVirtualMachine()->step2();
                std::this_thread::sleep_for( std::chrono::milliseconds(200));
            }while(queue.try_pop(s)== false || (s !="break"));
        }

    }

}

int main(int argc, char *argv[])
{
//mqd_t mq;
//        mq_attr m_attr;

  char c;

  std::string temp;
  std::ifstream ifs;
 // std::cout << temp;

//  std::vector<std::string> args;
//
//  // check arg
//  for (int i = 0; i < argc; ++i) {
//    args.push_back(argv[i]);
//  }
//  if (args.size() < 2) {
//  // extern const char *__simmips;
//
//    std::cerr << "Error, Usage: " << argv[0] << " [file name]" << std::endl;
//    return EXIT_FAILURE;
//  }
    VirtualMachine::getVirtualMachine()->reg_init();
    VirtualMachine::getVirtualMachine()->reset_mem();
    Parser parser;


    std::string arg ;
    std::string arg2 = argv[1];
    if(arg2 != "--gui") {


        ifs.open(argv[1], std::ifstream::in);
        if (!ifs) {
            std::cout << "Error:Unable to open file";
            return EXIT_FAILURE;// terminate with error
        }
        while (ifs.get(c)) {
            temp.push_back(c);
        }

        parser.tokenize(temp);
    }
    if(arg2 == "--gui")
    {
        QApplication App(argc,argv);

       QString qstr = QString::fromStdString(argv[2]);
        VirtualMachineGUI widget;

        widget.load(qstr);
        widget.show();

        App.exec();

        //thread.waitForFinished();

    }


    if (!parser.parse_text()) {
        std::cerr << parser.get_error() << std::endl;
        return EXIT_FAILURE;
    }
    std::string prev;
    std::thread th(vm_thread_main);
if (arg2 != "--gui" && arg != "quit") {

    while (true) {

        std::cout << "simmips> ";

        getline(std::cin, arg);

        if (arg == "print $pc") { printf("0x%08x\n", VirtualMachine::getVirtualMachine()->get_pc()); }
        else if (arg == "print $hi") { printf("0x%08x\n", VirtualMachine::getVirtualMachine()->get_reg("$hi")); }
        else if (arg == "print $lo") { printf("0x%08x\n", VirtualMachine::getVirtualMachine()->get_reg("$lo")); }
        else if (arg.find("print $") == 0) {
            std::string reg_name = arg.erase(0, 6);

            // check if number between 1 and 31
            VirtualMachine::getVirtualMachine()->print(reg_name, true);


        } else if (arg.find("print &") == 0) {
            std::string addr_name = arg.erase(0, 9);


            VirtualMachine::getVirtualMachine()->print(addr_name, false);

        } else if (arg == "step") {
            queue.push("step");
            // VirtualMachine::getVirtualMachine()->step();

        } else if (arg == "run") {
            queue.push("run");
        } else if (arg == "break") {
            queue.push("break");
        } else if (arg == "status") {

            std::cout << parser.get_error();

        } else if (arg == "quit") {
            break;
        } else {

            std::cout << "Error: unknown command" << std::endl;

        }
        prev = arg;
        std::this_thread::sleep_for( std::chrono::milliseconds(200));
    }

}
  ifs.close();
    th.join();
  std::cerr << "Success" << std::endl;
  return EXIT_SUCCESS;
}
