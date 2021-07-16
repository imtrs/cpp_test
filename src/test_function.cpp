#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>./
#include <vector>
#include <set>
#include <unordered_map> 
#include <thread>
#include <memory>
#include <ctime>
#include <unistd.h>
#include <test_function.h>
#include <tc_function_test.h>
#include <utils/tc_utils.h>
#include <tinyxml2.h>
#include <jsoncpp/json/json.h>

using namespace tinyxml2;
using namespace std;

#define     CONVERT_TO_BIN(x)       ((x / 0xA) * 0x10 + (x % 0xA))
inline unsigned char convertToBin(unsigned char x){ 
    return ((x / 0xA) * 0x10 + (x % 0xA));
}
const uint8_t UDSC_ROUTING_ACTIVATION_NO_RESPONSE   = static_cast<uint8_t>(0xFF);
const int32_t FUC_FIXED_COMMAND_LENGTH              = 3;
const uint8_t s_UDS_READ_RESULT_COMMAND[FUC_FIXED_COMMAND_LENGTH] = {0x22, 0xF0, 0xA4};

static const std::string TEST_JSON_FILE_PATH    = "./data_files.json";

static uint32_t g_ID = 0;

class CTest {
    static const std::string FILE_NAME;
public:
    CTest(): mId(g_ID){ ++g_ID;};
    ~CTest(){ cout << "called CTest distructor!" << endl;};
    void print(void){
        cout << "CTest::print called, ID:" << mId << endl;
    };
private:
    uint32_t mId; 
};

const std::string CTest::FILE_NAME = "test_function.cpp";

void runThread(shared_ptr<CTest> ct){
    cout << "runThread called" << endl;
    ct->print();
}

//------------------------------------------------------------------------------------------------------------------------
// @Function        : testFunc001
void testFunc001(void){
    char* inCmdData = "TCU;F012;";
    std::vector<std::string> tokenList;
    std::vector<char*> strStack;
    char* token = nullptr;
    char* strList[5] = {"hi", "hello", "my", "name","is"};
    char* strList2[5] = {nullptr};
    int32_t offset = 0;
    char strBuffer[128] = {0};

    for(int32_t i = 0 ; i < 5 ; i++){
        sprintf(strBuffer + offset, "%s;", strList[i]);
        offset += strlen(strList[i]) + 1;
        //strList2[i] = strdup(strList[i]);
        strList2[i] = new char[strlen(strList[i]) + 1];
        strcpy(strList2[i], strList[i]);
        strStack.push_back(strList2[i]);
        printf("[%p]%s \n", strList2[i], strList2[i]);
    }

    sprintf(strBuffer + offset, "Last_value");
    printf("strBuffer: [%s]\n", strBuffer);
    //strTokenizer("hi=<memory>;hello;my;name;your;name;", ";", tokenList);
    tokenizer(strBuffer, ";", tokenList);

    for(int i = 0 ; i < tokenList.size() ; i++) {
        printf("[tokenList]  %s \n", tokenList[i].c_str());
    }

    for(auto iter = tokenList.begin() ; iter != tokenList.end() ; iter++) {
        printf("[tokenList2] %s \n", (std::string*)(&(*iter))->c_str());
    }
    for(auto iter = strStack.begin() ; iter != strStack.end() ; iter++) {
        printf("[strStack:%p] %s \n",*iter, *iter);
    }

    int32_t pos = tokenList[0].find("mem");
    printf("pos:%d \n", pos);
    pos = tokenList[0].find("hello");
    printf("pos2:%d \n", pos);

    auto iter2 = tokenList.end();
    iter2--;
    printf("last token : %s \n", (std::string*)(&(*iter2))->c_str());
    int32_t i = 0 ;
    for(auto iter : strStack) {
        char* p =  (char*)iter;
        printf("[p:%p], [strList2:%p] \n", p, strList2[i]);
        //delete strList2[i++];
        delete iter;
        //delete p;
        iter = nullptr;
        printf("[*iter:%p] \n", iter);
        //strStack.erase(iter);
    }

    strStack.clear();
}

//------------------------------------------------------------------------------------------------------------------------
// @Function        : testFunc002
void testFunc002(void){
    std::string filePath = "./test.txt";
    std::string file;
    std::string path;
    size_t offset = 0;

    if(filePath.back() == 't'){
        std::cout << "file path last ch :" << filePath.back() << endl;
    }

    offset = filePath.rfind("/");
    file = filePath.substr(offset+1, filePath.size());
    path = filePath.substr(0, offset + 1);
    cout << "# file path : " << filePath << endl;
    cout << "# file : " << file << endl;    cout << "# path : " << path << endl;

    filePath.append("/");
    if(filePath.back() != '/'){
        filePath.append("/");
    }
    std::cout << "path:" << filePath << endl;
}

//------------------------------------------------------------------------------------------------------------------------
// @Function        : testFunc003
// @Description     : Test convert to binary
void testFunc003(void){
    char dates1[6] = {0};
    unsigned char dates2[6] = {0};
    time_t t = time(nullptr);
    struct tm*  pstTime = localtime(&t);

    dates1[0] = CONVERT_TO_BIN((unsigned char)(pstTime->tm_year - 100));
    dates1[1] = CONVERT_TO_BIN((unsigned char)(pstTime->tm_mon + 1));
    dates1[2] = CONVERT_TO_BIN((unsigned char)pstTime->tm_mday);
    dates1[3] = CONVERT_TO_BIN((unsigned char)pstTime->tm_hour);
    dates1[4] = CONVERT_TO_BIN((unsigned char)pstTime->tm_min);
    dates1[5] = CONVERT_TO_BIN((unsigned char)pstTime->tm_sec);

    dates2[0] = convertToBin((unsigned char)(pstTime->tm_year - 100));
    dates2[1] = convertToBin((unsigned char)(pstTime->tm_mon + 1));
    dates2[2] = convertToBin((unsigned char)pstTime->tm_mday);
    dates2[3] = convertToBin((unsigned char)pstTime->tm_hour);
    dates2[4] = convertToBin((unsigned char)pstTime->tm_min);
    dates2[5] = convertToBin((unsigned char)pstTime->tm_sec);

    if(memcmp(dates1, dates2, 6) == 0){
        std::cout <<  " dates1 == dates2 same  " << std::endl;
    }
    else {
        std::cout <<  " dates1 != dates2" << std::endl;
    }
}

//------------------------------------------------------------------------------------------------------------------------
// @Function        : testFunc004
void testFunc004(void){

    XMLDocument doc;
    XMLElement* root = nullptr;
    XMLElement* elem = nullptr;
    int32_t ecuNum = 0;
    int32_t compNum = 0;
    std::set<std::string> ecuSet;

    if(doc.LoadFile("usb_deployment.xml") == XML_SUCCESS){
        root = doc.FirstChildElement();
    }
    if(root){
        elem = findElement(root, "content");
    }
    if(elem){
        elem = elem->FirstChildElement();
    }

    while(elem){
        compNum++;
        //ecuSet.i
        std::string objId = elem->Attribute("objectId");

        if(!objId.empty()){
            ecuSet.insert(objId.substr(0, 3));
        }
        elem = elem->NextSiblingElement();
    }

    ecuNum = ecuSet.size();
    printf("ecuNum:%d, compNum:%d\n", ecuNum, compNum);
    for(auto iter : ecuSet){
        printf("ecu id:%s \n", iter.c_str());
    }

}

//------------------------------------------------------------------------------------------------------------------------
// @Function        : testFunc005
void testFunc005(void){
    uint8_t data[FUC_FIXED_COMMAND_LENGTH] = {0};
    memcpy(data, s_UDS_READ_RESULT_COMMAND, FUC_FIXED_COMMAND_LENGTH);
    cout << getHexString(data, FUC_FIXED_COMMAND_LENGTH) << endl;
    cout << getHexString(s_UDS_READ_RESULT_COMMAND, FUC_FIXED_COMMAND_LENGTH) << endl;

    std::string hexStr;
    size_t len = 3;
    const uint8_t* pMem  = (const uint8_t*)s_UDS_READ_RESULT_COMMAND;
    printf("len:%d\n", len);
    hexStr.append("0x");
    for(uint32_t i = 0 ; i < len ; i++){
        char buffer[3] = {0};
        printf("(%d)", i);
        uint8_t ch = pMem[i];
        sprintf(buffer, "%02X", ch);
        hexStr.append(buffer);
        printf("hexStr(%d):%s, buffer:%s, ch:%x, ch2:%x\n", i, hexStr.c_str(), buffer, (uint8_t)ch, s_UDS_READ_RESULT_COMMAND[i]);
    }
}
//------------------------------------------------------------------------------------------------------------------------
// @Function        : testFunc006
void testFunc006(void){
    shared_ptr<CTest> ct = make_shared<CTest>();

    thread th(runThread, ct);
    th.join();
    std::string filename="config.bin";
    std:vector<uint8_t> pkgId;

    std::vector<uint8_t> ecu_id_str = {'T', 'C', 'U', ';'};
    std::vector<uint8_t> pre_str = {'i', 'd', '=', '"'};
    std::vector<uint8_t> mid_str = {'"', ';','f','i','l','e','n','a','m','e','=','"'};
    std::vector<uint8_t> post_str = {'"', ';'};
    pkgId.insert(pkgId.begin(), pre_str.begin(), pre_str.end());
    pkgId.insert(pkgId.end(), mid_str.begin(), mid_str.end());
    pkgId.insert(pkgId.end(), filename.begin(), filename.end());
    pkgId.insert(pkgId.end(), post_str.begin(), post_str.end());
    pkgId.insert(pkgId.begin(), ecu_id_str.begin(), ecu_id_str.end());

    printf("data :[");
    for(auto it : pkgId){
        printf("%c", it);
    }
    printf("]\n");
}

//------------------------------------------------------------------------------------------------------------------------
// @Function        : testFunc007
void testFunc007(void){
    int32_t num = 0x0CD00EF0;
    printf("0x%0X \n", (num >> 4));
    printf("0x%0X \n", (num >> 4u));
    printf("0x%0X \n", (num >> 6));
    printf("0x%0X \n", (num >> 6u));
    printf("0x%0X \n", (num << 4));
    printf("0x%0X \n", (num << 4u));
}
//------------------------------------------------------------------------------------------------------------------------
// @Function        : testFunc008
void testFunc008(void){
    std::string vin = "AIVI2HWLLGE100001";
    std::cout << "vin hex :" << getHexString(vin.c_str(), vin.size()) << std::endl;
    hexdump("VIN hex:", vin.c_str(), vin.size());
}
//------------------------------------------------------------------------------------------------------------------------
// @Function        : testFunc009
void testFunc009(void){
#if 0
    std::string newVersion = "123456789R_CUB.06.03.30.Q2M.sit.userdebug";
    std::string supplierSwNumber = "";
    std::string supplierSwVersion = "";
    size_t offset = 0;

    st_component_log compLog;
    uint8_t st = 1;
    compLog.m_ullTimeStamp  = time(0);
    compLog.m_ucStatus = 0x01;
    printf("pcompLog.m_ucStatus:%02x\n",  compLog.m_ucStatus);
    strncpy(compLog.m_szNewSWV, "123456789R_CUB.06.03.30.Q2M.sit.userdebug", 43);
    printf("#pcompLog.m_ucStatus:%02x\n",  compLog.m_ucStatus);
    printf("st: %d , sizeof:%d \n", st, sizeof(SDiagLog));

    if(st == compLog.m_ucStatus) {
        cout << "st == compLog.m_ucStatus " << endl;
    }

    fillLogStreamIn(&compLog, 7);
    offset = newVersion.find("CUB");
    
    if(offset != std::string::npos){
        char* dumpStr = strdup(newVersion.c_str() + offset);
        char* tmpStr = dumpStr;
        char* pos = nullptr;
        char* delimiter = ".";
        int32_t delimiterLen = 1;
        int32_t i = 0;
        do {
            pos = strstr(tmpStr, delimiter);
            if(pos == nullptr) {
                break;
            }
            *pos = 0x00;
            supplierSwNumber.append(tmpStr);
            tmpStr = pos + delimiterLen;
            i++;
        }while(i < 4);
        supplierSwVersion = pos + delimiterLen;
        delete dumpStr;
    }
    
    printf("supplierSwNumber:%s\n", supplierSwNumber.c_str());
    printf("supplierSwVersion:%s\n", supplierSwVersion.c_str());
#endif
}
//------------------------------------------------------------------------------------------------------------------------
// @Function        : testFunc010
void testFunc010(void){
    char versionBuffer[11];
    uint64_t FIRST_SW_VERSION = 0x23C256409;
    uint64_t MAX_VAL = 0xFFFFFFFFFFFFFFFF;
    uint32_t high_val = FIRST_SW_VERSION >> 32;
    sprintf(versionBuffer, "%X%XR", high_val, FIRST_SW_VERSION + 1);
    printf("ver origin : %XL \n", MAX_VAL);
    cout << "ver : " << versionBuffer << endl;
    std::string version = versionBuffer;
    char* endPtr = nullptr;
    
    uint64_t number =  strtoul(version.c_str(), &endPtr, 16);
    
    printf("number : 0x%x%X \n", number >> 32, number);
    printf("number2 : 0x%X \n",  strtoul(version.substr(0, version.size() -1).c_str(), &endPtr, 16));
    printf("endPtr:%p \n", endPtr);
}
//------------------------------------------------------------------------------------------------------------------------
// @Function        : testFunc011
void testFunc011(void){
    if(access("./data/usb_deployment.xml", 0) == 0){
        std::cout << "./data/usb_deployment.xml file exist!" << std::endl;
    }
    else {
        std::cout << "./data/usb_deployment.xml file does not exist!" << std::endl;
    }
}
//------------------------------------------------------------------------------------------------------------------------
// @Function        : testFunc012
void testFunc012(void){
    unsigned int accumulator = 0x12345678;
    unsigned int result01 = 0;
    unsigned int result02 = 0;
    unsigned int result03 = 0;
    char c = 64;
    result01 = (accumulator << 6) | c;
    result02 = (accumulator << 6) | static_cast<unsigned int>(c);
    result03 = (accumulator << 6) | static_cast<unsigned int>(c) ;
    printf("(accumulator << 6):%08X, static_cast<unsigned int>(c):%08X \n", (accumulator << 6), static_cast<unsigned int>(c));
    printf(" result01:%08X, result02:%08X \n", result01, result02);
}
//------------------------------------------------------------------------------------------------------------------------
// @Function        : testFunc013
void testFunc013(void){
    uint8_t data = 0x31u; 
    uint8_t sizeLen1 = (data >> 4u) & 0x0Fu;
    uint8_t sizeLen2 = (data >> 4u) & 0x0F;
    printf("data:%X, sizeLen1:%X, sizeLen2:%X \n", data, sizeLen1, sizeLen2);
}
//------------------------------------------------------------------------------------------------------------------------
// @Function        : testFunc014
void testFunc014(void){
    std::string targetFile = "/home/penta3/work_dir/cpp_test/data_files/ota_update.zip";
    ZipArchiveHandle zipHandle = nullptr;
    if(openzip(zipHandle, targetFile)){

    }
}
//------------------------------------------------------------------------------------------------------------------------
// @Function        : testFunc015
void testFunc015(void){
    std::string file;
    FILE* fp = nullptr;

    if(fp = fopen(file.c_str(), "r")){
        char* buffer = nullptr;
        size_t size = 0;

        fseek(fp, 0, SEEK_END);    // 파일 포인터를 파일의 끝으로 이동시킴
        size = ftell(fp);          // 파일 포인터의 현재 위치를 얻음

        buffer = new char[size + 1];    // 파일 크기 + 1바이트(문자열 마지막의 NULL)만큼 동적 메모리 할당
        memset(buffer, 0, size + 1);  // 파일 크기 + 1바이트만큼 메모리를 0으로 초기화

        fseek(fp, 0, SEEK_SET);

        fread(buffer, sizeof(buffer), 1, fp);

        fclose(fp);
        delete [] buffer;
    }
}
//------------------------------------------------------------------------------------------------------------------------
// @Function        : testFunc016
void testFunc016(void){
    char*   realPath = nullptr;
    
    realPath = new char[PATH_MAX];
    memset(realPath, 0x00, PATH_MAX);
    realpath("./", realPath);

    TC_LOGV("real path: %s", realPath);

    delete [] realPath;
}
//------------------------------------------------------------------------------------------------------------------------
// @Function        : testFunc017
void testFunc017(void){
    std::unordered_map<std::string, shared_ptr<CTest>> propRepo;
    shared_ptr<CTest> ct01 = make_shared<CTest>();
    std::string name = "ct1";

    propRepo.insert(make_pair(name, ct01));
    ct01->print();
    ct01 = nullptr;
    ct01 = make_shared<CTest>();

    auto it = propRepo.find(name);

    if(it != propRepo.end()){
        it->second = ct01;
    }
    it->second->print();
}
//------------------------------------------------------------------------------------------------------------------------
// @Function        : testFunc018
void testFunc018(void){
    std::string jsonStr;
    Json::Value root;
    Json::StyledWriter writer;


    root["name"] = "KKK";
    root["age"] = 12;
    root["address"] = "kor";
    root["gfriend"] = true;
 
    Json::Value family;
    family.append("mother");
    family.append("father");
    family.append("brother");
    root["family"] = family;

    jsonStr = writer.write(root);

    cout << "json string \n" << jsonStr << endl;

    std::ofstream fout(TEST_JSON_FILE_PATH);
    fout << jsonStr;
}
//------------------------------------------------------------------------------------------------------------------------
// @Function        : testFunc019
void testFunc019(void){
    std::string jsonStr;
    Json::Value root;
    Json::Reader reader;
    Json::FastWriter writer;
    std::ifstream fin(TEST_JSON_FILE_PATH);

    reader.parse(fin, root, false);

    jsonStr = writer.write(root);

#if 1
    auto family = root["family"];

    if(family.isArray()){
        for(int i = 0 ; i < family.size() ; i++){
            cout << "value:" << family[i].asString() << endl;
        }
    }
#endif
#if 1
    for(auto iter = root.begin() ; iter != root.end() ; iter++){
        cout << "key:" << *iter << endl;
    }
#endif
#if 0
    for(auto iter : root){
        cout << "key:" << iter << endl;
    }
#endif
    //cout << "json string \n" << jsonStr << endl;
} 