#ifndef _TC_UTILS_H_
#define _TC_UTILS_H_
#include <cstdlib>
#include <string>
#include <vector>
#include <tinyxml2.h>
#ifdef ANDROID 
#include <android/log.h>
#endif //

#ifndef LOG_TAG
#define LOG_TAG     "Test_Code"
#endif

const int32_t AUTO_NEW_LINE = 0x01;
const int32_t HEX_FLAG      = 0x02;

void log_print(int prio, const char* tag, const char* file, int line
        , const char* func, const char* fmt, ...);

int32_t tc_mkpath(std::string path, mode_t mode = 0644);

uint8_t* convertOrder(const uint8_t* src, uint8_t* target, size_t srcSize);
tinyxml2::XMLElement* findElement(tinyxml2::XMLElement* pParentElem, std::string tag);
std::string getHexString(const void* mem, uint32_t len, unsigned int flag = 0);
std::string hexdump(std::string header, const void* mem, uint32_t len);
bool tokenizer(const char* inStr, const char* delimiter,  std::vector<std::string> &tokenList);

#ifdef ANDROID 
#define TC_LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define TC_LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define TC_LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#else 
#define TC_LOGV(...) log_print(2, LOG_TAG, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define TC_LOGD(...) log_print(3, LOG_TAG, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define TC_LOGI(...) log_print(4, LOG_TAG, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define TC_LOGW(...) log_print(5, LOG_TAG, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define TC_LOGE(...) log_print(6, LOG_TAG, __FILE__, __LINE__, __func__, __VA_ARGS__)
#endif //
#endif //_TC_UTILS_H_