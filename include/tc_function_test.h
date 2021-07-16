#ifndef _TC_FUNCTION_TEST_H_
#define _TC_FUNCTION_TEST_H_
#include <string>
#include <vector>
#include <ziparchive/zip_archive.h>

//int32_t strTokenizer(const char* str, std::string delimiter,std::vector<std::string> tokenList);

bool openzip(ZipArchiveHandle zipHandle, std::string fileName);

#endif //_TC_FUNCTION_TEST_H_