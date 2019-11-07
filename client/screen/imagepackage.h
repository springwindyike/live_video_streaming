#ifndef IMAGEPACKAGE_H
#define IMAGEPACKAGE_H

typedef  struct
{
    int width;
    int height;
    int bytesPerline;
    bool isLastPack;
    int packTaken;
    char data[4096];
}ImagePackage;

struct AudioPackage
{
    char data[1024];
    int dataLen;
};
#endif // IMAGEPACKAGE_H
