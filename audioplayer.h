#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include <QByteArray>
#include <QBuffer>
#include <QAudioDecoder>
#include <QAudioOutput>
#include <QTimer>

class AudioPlayer : public QObject
{
  Q_OBJECT
  QByteArray _newData;
  QByteArray _data;
  QBuffer* _dataBuffer;
  QAudioDecoder* _decoder;
  QByteArray _audioData;
  QBuffer* _audioInputBuffer;
  QBuffer* _audioOutputBuffer;
  QAudioOutput* _audioOutput;
  QTimer* _timer;
  void playInternal();
public:
  explicit AudioPlayer(QObject *parent = nullptr);

signals:

public slots:
  void play(const QByteArray& data);
  void bufferReady();
};

#endif // AUDIOPLAYER_H
