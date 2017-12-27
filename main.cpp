#include <fstream>
#include <iostream>
#include <string>
#include <memory>

#define BUF_LEN  1000

void encode(const std::string & words, char *buf, std::size_t len) {
  auto iWordsLen = words.length();
  std::size_t idx = 0;
  for (; idx < len; ++buf, ++idx) {
    *buf ^= words[idx % iWordsLen];
  }

}


int main(int argc, char** argv) {
  int iRc = 0;
  if (argc < 2) { std::cerr << "filename required" << std::endl; return 1; }
  std::string  inFName = argv[1];
  std::string  outFName = inFName + ".new";
  
  std::ifstream  inStream(inFName, std::ios::binary);
  if (!inStream.is_open()) {
    std::cerr << "Couldn't open " << inFName << std::endl;
    return 1;
  }
  
  inStream.seekg(0, inStream.end);
  int length = inStream.tellg();
  inStream.seekg(0, inStream.beg);

  std::ofstream  ofStream(outFName, std::ios::binary);
  if (!ofStream.is_open()) {
    std::cerr << "Couldn't open " << outFName << std::endl;
    return 1;
  }

  std::cout << "Type some words:";
  std::string  sWords;
  std::cin >> sWords;
  if (sWords.size() < 10) {
    std::cerr << "Words are too short" << std::endl;
    return 1;
  }

  std::size_t bufLen, maxLen = BUF_LEN*sWords.length();
  std::unique_ptr<char[]> pBuf = std::unique_ptr<char[]>(new char[maxLen]);
  bool finished = false;
  for (; true;) {
    inStream.read(pBuf.get(), maxLen);

    if (inStream) {
      bufLen = maxLen;
    }
    else {
      bufLen = inStream.gcount();
      finished = true;
      std::cout << "error: only " << inStream.gcount() << " could be read";
    }
    encode(sWords, pBuf.get(), bufLen);
    ofStream.write(pBuf.get(), bufLen);
    if (finished) break;
  }

  inStream.close();
  ofStream.close();



  return iRc;
}