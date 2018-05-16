#ifndef MMEXTRACTORMAINWINDOW_H
#define MMEXTRACTORMAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QMediaPlayer>
#include <QBuffer>
#include <QAudioDecoder>
#include <QDir>
#include <QMenu>
#include "mmarchiveparser.h"
#include "audioplayer.h"

class MMExtractorMainWindow : public QMainWindow
{
  Q_OBJECT
  //gui
  QTreeWidget* _tree;
  //menu bar
  QMenu* fileMenu;
  QAction* selectFileAction;
  QAction* exitFileAction;
  //audio
  AudioPlayer* _audioPlayer;
  //parser
  MMArchiveParser _parser;
  //other
  QString currentSnd;
  QMenu treeWidgetContextMenu;
  QDir saveDir;

  void fillTree();
public:
  MMExtractorMainWindow(QWidget *parent = 0);
  ~MMExtractorMainWindow();
public slots:
  void itemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
  void selectFile();
  void showTreeContextMenu(const QPoint& pos);
  void exportSounds(const QList<QTreeWidgetItem*>& items);
  void exportSelectedSounds();
};

#endif // MMEXTRACTORMAINWINDOW_H
