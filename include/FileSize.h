#ifndef SIMPLEAUDIOPLAYER_FILESIZE_H_
#define SIMPLEAUDIOPLAYER_FILESIZE_H_

#include <QString>

/*
A simple class parsing a file size (in bytes) into human readable data.
*/
class FileSize
{
public:
    FileSize(size_t fileSize);

    /*
    Return the human readable file size.
    */
    QString toString() const;

private:
    size_t m_convertedSize;
    QString m_unitSize;
};

#endif // SIMPLEAUDIOPLAYER_FILESIZE_H_