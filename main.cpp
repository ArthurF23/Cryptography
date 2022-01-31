#include "Headers/includes.h"

int main() {
  cout << "Thank you for using my Encryption/Decryption header. \n Please visit my Github at ArthurF23" << endl << "Version: " << VERSION::ver;
  cout << " Which example would you like to use?" << endl << "1: simple example" << endl << "2: AES example" << endl << "1, 2, or 3" << endl;
  char input;
  cin >> input;
  switch (input) {
    case '1':
      encdec::start_example();
      break;
    case '2':
      AES::start_example();
      break;
    case '3':
      DUO::example();
      break;

    default:
      cout << "Err please input 1, 2, or 3" << endl;
      return main();
  };
  return main();
}