#include "mmextractormainwindow.h"
#include<QAudioDeviceInfo>
#include<QMessageBox>
#include<QFile>
#include<QMenuBar>
#include<QFileDialog>

void MMExtractorMainWindow::fillTree()
{
  _tree->clear();
  foreach(const QString& name, _parser.content().keys())
  {
    _tree->addTopLevelItem(new QTreeWidgetItem({name}));
  }
}

MMExtractorMainWindow::MMExtractorMainWindow(QWidget *parent)
  : QMainWindow(parent)
{
  //gui
  _tree = new QTreeWidget();
  _tree->setHeaderLabel("Element");
  _tree->setRootIsDecorated(false);
  _tree->setSelectionMode(QAbstractItemView::ExtendedSelection);
  _tree->setContextMenuPolicy(Qt::CustomContextMenu);
  setCentralWidget(_tree);
  connect(_tree,&QTreeWidget::currentItemChanged,this,&MMExtractorMainWindow::itemChanged);
  connect(_tree,&QTreeWidget::customContextMenuRequested,this,&MMExtractorMainWindow::showTreeContextMenu);
  //audio
  _audioPlayer = new AudioPlayer(this);
  //menu bar
  fileMenu = this->menuBar()->addMenu("File");
  selectFileAction = fileMenu->addAction("Select File",this,&MMExtractorMainWindow::selectFile);
  fileMenu->addSeparator();
  selectFileAction = fileMenu->addAction("Exit",this,&MMExtractorMainWindow::close);
  //tree context menu
  treeWidgetContextMenu.addAction("Export",this,&MMExtractorMainWindow::exportSelectedSounds);
}

MMExtractorMainWindow::~MMExtractorMainWindow()
{

}

void MMExtractorMainWindow::itemChanged(QTreeWidgetItem* current, QTreeWidgetItem*)
{
  if(_parser.content().contains(current->data(0,Qt::DisplayRole).toString()))
  {
    QByteArray content = _parser.content()[current->data(0,Qt::DisplayRole).toString()];
    _audioPlayer->play(content);
  }
}

void MMExtractorMainWindow::selectFile()
{
  QString selected = QFileDialog::getOpenFileName(nullptr,"Select .snd file",currentSnd,"Might and Magic .snd file (*.snd);;All files (*)");
  if(!selected.isEmpty())
  {
    currentSnd = selected;
    _parser.load(currentSnd);
    fillTree();
  }
}

void MMExtractorMainWindow::showTreeContextMenu(const QPoint& pos)
{
  if(_tree->selectedItems().size()>0)
  {
    treeWidgetContextMenu.exec(_tree->mapToGlobal(pos));
  }
}

void MMExtractorMainWindow::exportSounds(const QList<QTreeWidgetItem*>& items)
{
  if(items.size()==1)
  {
    QString location = QFileDialog::getSaveFileName(nullptr,"Save As",saveDir.absoluteFilePath(items[0]->data(0,Qt::DisplayRole).toString()+".wav"),"WAVE file (*.wav)");
    if(!location.isEmpty())
    {
      saveDir = location;
      QFile file(location);
      if(file.open(QIODevice::WriteOnly))
      {
        file.write(_parser.content()[items[0]->data(0,Qt::DisplayRole).toString()]);
        file.close();
      }
      else
      {
        QMessageBox::critical(nullptr,"Error","Error: Could not save file.");
      }
    }
  }
  else if(items.size()>1)
  {
    QString location = QFileDialog::getExistingDirectory(nullptr,"Select Directory",saveDir.absoluteFilePath("."));
    if(!location.isEmpty())
    {
      saveDir = location;
      int failCtr = 0;
      foreach(auto item, items)
      {
        QFile file(saveDir.absoluteFilePath(item->data(0,Qt::DisplayRole).toString()+".wav"));
        if(file.open(QIODevice::WriteOnly))
        {
          file.write(_parser.content()[item->data(0,Qt::DisplayRole).toString()]);
          file.close();
        }
        else
        {
          failCtr++;
        }
      }
      if(failCtr!=0)
      {
        if(failCtr==items.size())
        {
          QMessageBox::critical(nullptr,"Error","Error: Could not save files.");
        }
        else
        {
          QMessageBox::critical(nullptr,"Error",QString("Error: Could not save %1 files.").arg(QString::number(failCtr)));
        }
      }
    }
  }
}

void MMExtractorMainWindow::exportSelectedSounds()
{
  exportSounds(_tree->selectedItems());
}
