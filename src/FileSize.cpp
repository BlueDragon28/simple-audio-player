#include "FileSize.h"

FileSize::FileSize(size_t size)
{
    // Parsing the size type.

    // Check if it's TB.
    if (size >= 1000000000000)
    {
        m_convertedSize = size / 1000000000000;
        m_unitSize = "TB";
    }
    // Check if it's GB.
    else if (size >= 1000000000)
    {
        m_convertedSize = size / 1000000000;
        m_unitSize = "GB";
    }
    // Check if it's MB.
    else if (size >= 1000000)
    {
        m_convertedSize = size / 1000000;
        m_unitSize = "MB";
    }
    // Check if it's KB.
    else if (size >= 1000)
    {
        m_convertedSize = size / 1000;
        m_unitSize = "KB";
    }
    // Otherwise, it's Octets.
    else
    {
        m_convertedSize = size;
        m_unitSize = "o";
    }
}

/*
Return the human readable file size.
*/
QString FileSize::toString() const
{
    return QString::number(m_convertedSize) + " " + m_unitSize;
}