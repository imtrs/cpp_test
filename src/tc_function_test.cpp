#include <fcntl.h>         // O_WRONLY
#include <unistd.h>    

#include <tc_function_test.h>
#include <utils/tc_utils.h>

#if 0
int32_t strTokenizer(const char* str, std::string delimiter,std::vector<std::string> tokenList){
    
}

#endif

bool openzip(ZipArchiveHandle zipHandle, std::string fileName){
    int32_t result = 0;
    void* iteration_cookie;
    char nameBuff[64] = {'\0'};
    
    strncpy(nameBuff,"./tmp-XXXXXX",21);
    mkdtemp(nameBuff);
    TC_LOGV("created dir: %s, [%s]",  nameBuff);

    result = OpenArchive(fileName.c_str(), &zipHandle);
    
    if (StartIteration(zipHandle, &iteration_cookie, nullptr, nullptr) < 0) {
        TC_LOGE("Failed to iterate %s\n", fileName.c_str());
        return false;
    }
    ZipEntry zentry;
    ZipString zname;

    while ((result = Next(iteration_cookie, &zentry, &zname)) == 0) {
        
        char strBuf[64] = {'\0'};
        std::string fileName = nameBuff;

        memcpy(strBuf, zname.name, zname.name_length);
        fileName.append("/");
        fileName.append(strBuf);
        
        size_t pos = fileName.rfind("/");
        std::string path  = (pos != std::string::npos ? fileName.substr(0, pos) : "./");
        if(tc_mkpath(path, zentry.unix_mode)){
            TC_LOGV("make path failed!");
        }

        TC_LOGD(" znemae[%d]:%s \n", zname.name_length, fileName.c_str());
        int fd = 0;
        if( 0 < (fd = open(fileName.c_str(), O_WRONLY|O_CREAT|O_EXCL, zentry.unix_mode))) {
            int32_t err = ExtractEntryToFile(zipHandle, &zentry, fd);

            if(err != 0){
                printf("file extract failed! error :%d \n", err);
            }

            close(fd);
        }
        else {
            TC_LOGV("%s file open failed", fileName.c_str());
        }
    }
    EndIteration(iteration_cookie);
    if (result == -2) {
        TC_LOGE("Failed to iterate %s\n", fileName.c_str());
        return false;
    }

    return (result == 0 ? true : false);
}