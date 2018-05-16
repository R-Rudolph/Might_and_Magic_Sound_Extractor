#include "mmarchiveparser.h"
#include<QFile>
#include<QVector>
#include<QtEndian>

MMArchiveParser::MMArchiveParser()
{

}

MMArchiveParser::MMArchiveParser(const QString& filename) : MMArchiveParser()
{
  load(filename);
}

MMArchiveParser::MMArchiveParser(const QByteArray& data) :MMArchiveParser()
{
  load(data);
}

bool MMArchiveParser::load(const QString& filename)
{
  QFile file(filename);
  if(file.open(QIODevice::ReadOnly))
  {
    bool ret = load(file.readAll());
    file.close();
    return ret;
  }
  else
  {
    return false;
  }
}

bool MMArchiveParser::load(QByteArray data)
{
  _content.clear();
  // parse number of headers
  uint32_t numberOfEntries;
  {
    if(data.size()<(int)sizeof(uint32_t))
      return false;
    const char* ptr = data.data();
    numberOfEntries = *((uint32_t*)ptr);
  }
  // parse headers
  QVector<RawHeaderStruct> headers;
  {
    if(data.size()<(int) (numberOfEntries*sizeof(RawHeaderStruct))+(int)sizeof(uint32_t))
    {
      return false;
    }
    headers.reserve(numberOfEntries);
    RawHeaderStruct* ptr = (RawHeaderStruct*)(data.data()+sizeof(uint32_t));
    for(uint32_t i=0;i<numberOfEntries;i++)
    {
      RawHeaderStruct header = ptr[i];
      header.length = qFromLittleEndian(header.length);
      header.uncompressedLength = qFromLittleEndian(header.uncompressedLength);
      header.offset = qFromLittleEndian(header.offset);
      headers.append(ptr[i]);
    }
  }
  // read/uncompress data
  {
    foreach(const RawHeaderStruct& header, headers)
    {
      if(header.uncompressedLength > header.length)
      {
        QByteArray compressed = data.mid(header.offset,header.length);
        uint32_t ulengthBE = qToBigEndian(header.uncompressedLength);
        compressed.prepend(QByteArray((char*)&ulengthBE,sizeof(uint32_t)));
        _content.insert(QString(header.name),qUncompress(compressed));
      }
      else
      {
        _content.insert(QString(header.name),data.mid(header.offset,header.length));
      }
    }
  }
  return true;
}

const QMap<QString, QByteArray> MMArchiveParser::content()
{
  return _content;
}
