#include <cstring>
#include <string>
#include <stdarg.h>
#include <sstream>
#include <iostream>
#include <fcntl.h>
//#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <utils/tc_utils.h>

using namespace tinyxml2;

void log_print(int prio, const char* tag, const char* file, int line
        , const char* func, const char* fmt, ...){ 
    std::stringstream ss;
    std::string filePath = file;
 
    if(filePath.empty()){
        ss << "["<< func << "(" << line << ")] " << fmt;
    }

    else {
        size_t pos = filePath.rfind("/");
        pos = (pos == std::string::npos ? 0 : pos  + 1);

        ss << "["<< func << "(" << filePath.substr(pos, filePath.size() - pos) <<":"<< line << ")] " << fmt;
    }

    va_list al;
    va_start(al, fmt);

#ifdef ANDROID
    __android_log_vprint(prio, tag, ss.str().c_str(), al);
#else 
    ss << "\n";
    vprintf(ss.str().c_str(), al);
#endif
    va_end(al);
} 

//------------------------------------------------------------------------------------------------------------------------
// @Function        : tc_mkpath
// @Description     :
// @Parameters      :
// @Return Value    : 0(success), -1(fail)
//------------------------------------------------------------------------------------------------------------------------
// @ID           @Date          @Name           @Description
//------------   ----------     -----------     -----------------------------------------
//
//------------------------------------------------------------------------------------------------------------------------
int32_t tc_mkpath(std::string path, mode_t mode){
    std::string tmpStr;
    char*   szBuffer = nullptr;
    int32_t pathLen = 0;
    int32_t result  = 0;
    
    szBuffer = new char[PATH_MAX];  
    memset(szBuffer, 0x00, PATH_MAX);
    size_t pos = 0;
    if((pos = path.rfind("./")) != std::string::npos){
        char*   realPath = nullptr;
        
        realPath = new char[PATH_MAX];
        memset(realPath, 0x00, PATH_MAX);
        pos += 1;
        TC_LOGV("pos:%d, substr:%s \n", pos, path.substr(0, pos).c_str());
        realpath(path.substr(0, pos).c_str(), realPath);
        
        tmpStr = realPath;
        tmpStr.append(path.substr(pos, path.size() - pos));
        if(realPath){
            delete [] realPath;
        }
    }
    else {
        tmpStr = path;
    }
    strncpy(szBuffer, tmpStr.c_str(), PATH_MAX);
    pathLen = static_cast<int32_t>(strnlen(szBuffer, PATH_MAX));
    TC_LOGV("\n    input path:%s\n    full path:%s\n", path.c_str(), szBuffer);
    for(int32_t i = 1 ; i < (pathLen + 1) ; i++){
        if((szBuffer[i] == '/') || (szBuffer[i] == '\0')){
            char ch = szBuffer[i];
            szBuffer[i] = '\0';
            result = mkdir(szBuffer, mode);
            if(result == -1){
                TC_LOGV("Failed to make \"%s\" directory \n", szBuffer);
            }
            else {
                TC_LOGV("Success to make \"%s\" directory \n", szBuffer);
            }
            szBuffer[i] = ch;
        }
    }
    if(szBuffer){
        delete [] szBuffer;
    }
    return result;
} 
 
uint8_t* convertOrder(const uint8_t* src, uint8_t* target, size_t srcSize) { 
    uint8_t* retVal = nullptr; 
 
    if((src != nullptr) && (target != nullptr)) { 
        for(uint32_t i = 0 ; i < (uint32_t)srcSize ; i++) { 
            target[i] = src[srcSize - (1 + i)]; 
        } 
        retVal = target; 
    } 
 
    return retVal; 
} 
 
XMLElement* findElement(XMLElement* pParentElem, std::string tag){ 
    XMLElement* pElem = nullptr; 
    XMLElement* pResultElem = nullptr; 
    if(pParentElem){ 
        if(tag.compare(pParentElem->Name()) == 0){ 
            pResultElem = pParentElem; 
        } 
        else { 
            pElem = pParentElem->FirstChildElement(); 
        } 
    } 
    while(pElem != nullptr){ 
        if(tag.compare(pElem->Name()) == 0) { 
#ifdef _DEV_STDOUT_LOG_ 
            std::cout << " findElement() TAG found : "<< tag << std::endl; 
#endif// _DEV_STDOUT_LOG_ 
            pResultElem = pElem; 
        } 
        else { 
            pResultElem = findElement(pElem, tag); 
        } 
        if(pResultElem != nullptr){ 
            break; 
        } 
        else { 
            pElem = pElem->NextSiblingElement(); 
        } 
    } 
    return pResultElem; 
} 
 
 
std::string getHexString(const void* mem, uint32_t len, unsigned int flag){ 
    std::stringstream ss; 
    bool newLine = (flag & AUTO_NEW_LINE); 
    bool hexFlag = (flag & HEX_FLAG); 
    const char* pMem  = (const char*)mem; 
    if(mem != nullptr) { 
        for(uint32_t i = 0 ; i < len ; i++){ 
            char buffer[3] = {0}; 
            sprintf(buffer, "%02X", pMem[i]); 
            if(hexFlag) { 
                ss << "0x"; 
            } 
            ss << buffer; 
            if(newLine && (((i + 1) % 16) == 0)) { 
                ss << "\n"; 
            } 
            else { 
                ss << " "; 
            } 
        } 
    } 
    return ss.str(); 
} 
 
 
std::string hexdump(std::string header, const void* mem, uint32_t len){ 
#if 1 
    std::stringstream ss; 
    ss << header << std::endl; 
    ss << getHexString(mem, len, HEX_FLAG | AUTO_NEW_LINE) << std::endl; 
#else 
    uint32_t i = 0; 
    std::string strBuffer; 
    char buf[4] = {0}; 
    for (i = 0; i < len; i++) { 
        (void)sprintf(buf, " %02X", ((uint8_t*)mem)[i]); 
        strBuffer.append(buf); 
        if((i % 16) == 15) { 
            strBuffer.append("\n"); 
        } 
    } 
    strBuffer.append("\n"); 
    std::cout << header.c_str() << "\n"<< strBuffer.c_str() << std::endl; 
#endif  
    std::cout << ss.str(); 
    return ss.str(); 
} 
 
 
bool tokenizer(const char* inStr, const char* delimiter,  std::vector<std::string> &tokenList){ 
    bool result = true; 
    if(inStr == nullptr || delimiter == nullptr) { 
        result = false; 
    } 
    else { 
        char* dumpStr = strdup(inStr); 
        char* tmpStr = dumpStr; 
        char* pos = nullptr; 
        int32_t delimiterLen = strlen(delimiter); 
        do { 
            pos = strstr(tmpStr, delimiter); 
            if(pos == nullptr) { 
                tokenList.push_back(std::string(tmpStr)); 
                break; 
            } 
            *pos = 0x00; 
            tokenList.push_back(std::string(tmpStr)); 
            tmpStr = pos + delimiterLen; 
        }while(1); 
        free(dumpStr); 
    } 
     
    return result; 
} 
 