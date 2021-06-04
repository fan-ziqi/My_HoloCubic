#include <SD.h>

#ifndef SD_CARD_H
#define SD_CARD_H

#include "FS.h"
#include "SD.h"
#include "SPI.h"
#define IMAGE_FILE_NUM 50
#define IMAGE_FILE_NAME_MAX_LEN 20

extern int photo_file_num;
extern char file_name_list[IMAGE_FILE_NUM][IMAGE_FILE_NAME_MAX_LEN];

class SdCard
{
  private:
    char buf[128];

  public:
    void init();
    void listDir(const char* dirname, uint8_t levels);

    void createDir( const char* path);

    void removeDir(  const char* path);

    void readFile(  const char* path);

    String readFileLine( const char* path, int num);

    void writeFile(const char* path, const char* message1,const char* message2,
        const char* message3, const char* message4, const char* message5);

    void appendFile(  const char* path, const char* message);

    void renameFile(  const char* path1, const char* path2);

    void deleteFile(  const char* path);

    void readBinFromSd(const char* path, uint8_t* buf);

    void writeBinToSd(const char* path, uint8_t* buf);

    void fileIO(  const char* path);

};

extern SdCard tf;

#endif
