#include "mmextractormainwindow.h"
#include <QApplication>
#include "mmarchiveparser.h"
#include<QFile>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MMExtractorMainWindow w;
  w.show();
  return a.exec();
}
