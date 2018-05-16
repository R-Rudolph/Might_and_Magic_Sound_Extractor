#include "audioplayer.h"

AudioPlayer::AudioPlayer(QObject *parent) : QObject(parent)
{
  _dataBuffer = new QBuffer(&_data,this);
  _audioInputBuffer = new QBuffer(&_audioData,this);
  _audioOutputBuffer = new QBuffer(&_audioData,this);
  _audioOutput = nullptr;
  _decoder = new QAudioDecoder(this);
  _audioOutput = nullptr;
  _audioInputBuffer->open(QIODevice::WriteOnly);
  _audioOutputBuffer->open(QIODevice::ReadOnly);

  _decoder->setSourceDevice(_dataBuffer);
  QAudioFormat format;
  // Set up the format, eg.
  format.setSampleRate(48000);
  format.setChannelCount(1);
  format.setSampleSize(32);
  format.setCodec("audio/pcm");
  format.setByteOrder(QAudioFormat::LittleEndian);
  format.setSampleType(QAudioFormat::Float);
  _decoder->setAudioFormat(format);
  _timer = new QTimer(this);
  _timer->setSingleShot(true);
  _timer->setInterval(50);


  _audioOutput = new QAudioOutput(format,this);

  connect(_decoder,&QAudioDecoder::bufferReady,this,&AudioPlayer::bufferReady);
  connect(_timer,&QTimer::timeout,this,&AudioPlayer::playInternal);
}

void AudioPlayer::play(const QByteArray& data)
{
  _newData = data;
  _timer->start();
}

void AudioPlayer::playInternal()
{
  _decoder->stop();
  _audioOutput->stop();
  _dataBuffer->close();
  _audioInputBuffer->close();
  _audioOutputBuffer->close();
  _data = _newData;
  _audioData.clear();
  _dataBuffer->open(QIODevice::ReadOnly);
  _audioInputBuffer->open(QIODevice::WriteOnly);
  _audioOutputBuffer->open(QIODevice::ReadOnly);
  _audioOutput->start(_audioOutputBuffer);
  _decoder->start();
}

void AudioPlayer::bufferReady()
{
  if(_decoder->bufferAvailable() && _audioInputBuffer->isOpen())
  {
    QAudioBuffer buffer = _decoder->read();
    _audioData.append(buffer.constData<char>(),buffer.byteCount());
   // _audioInputBuffer->write(buffer.constData<char>(),buffer.byteCount());
  }
}
