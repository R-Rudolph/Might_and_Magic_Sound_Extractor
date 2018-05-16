#ifndef MMARCHIVEPARSER_H
#define MMARCHIVEPARSER_H

#include<QMap>
#include<QString>
#include<QByteArray>

class MMArchiveParser
{
  typedef struct
  {
    char name[40];
    uint32_t offset;
    uint32_t length;
    uint32_t uncompressedLength;
  } RawHeaderStruct;

  QMap<QString,QByteArray> _content;
public:
  MMArchiveParser();
  MMArchiveParser(const QString& filename);
  MMArchiveParser(const QByteArray& data);
  bool load(const QString& filename);
  bool load(QByteArray data);
  const QMap<QString,QByteArray> content();
};

#endif // MMARCHIVEPARSER_H
