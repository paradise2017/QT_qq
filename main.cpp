//#include "CCMainWindow.h"
#include "userlogin.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   /* CCMainWindow w;
    w.show();*/
    //设置当窗口关闭时，不退出
    a.setQuitOnLastWindowClosed(false);

    UserLogin* userLogin = new UserLogin;
    userLogin->show();
    return a.exec();
}
