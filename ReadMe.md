## Thank you for using my header!

### v1.9.1

### What's new in v1.9.1

##### AKARE requires a password now, as it increases security. Further updates to AKARE algorithm which increases security.

## AES

###### I used [this](https://programmer.group/c-implementation-of-aes-encryption-algorithms.html) tutorial to understand and create the AES functions, and I must give credit where credit is due. If you want to learn more about AES-128 check them out.

### How to use


#### Types
`AESbyte` is 8 bits

`AESword` is 32 bits

Unless you're modifying the header you won't have much use for these but the AESbyte is the typedef for the key amongst other varibles. 

#### `AES::KEY::Key`

`AES::KEY::key` is the key for AES and is an array which has a length of 16 and the type is `AESbyte`. If you are assigning a key manually you can do this:

```
  AESKEY::key[0] = 0b00100010;
  == OR ==
  AESbyte value = 0b01001100;
  AESKEY::key = value;
```

#### `OPTIONS`
The enum OPTIONS is the enum to determine key generation. Use `OPTIONS::doGenerateKey` in `AES::aes_init()` if you want a randomly generated key, if you don't, then use `OPTIONS::noGenerateKey` instead.

#### `aes_init()`
You must call this function before using any encryption or decryption.

This function has 2 parameters

`AES::aes_init(OPTIONS opt, string dummykey = "")`

The first parameter is the previous section's OPTION enum. If you choose to generate a key you would put `OPTIONS::doGenerateKey` here like so: `AES::aes_init(OPTIONS::doGenerateKey)`.

If you do not want to generate a key then you will want to input a key of 16 bytes (that's 128 bits and since there's no spaces, the length of the string needs to be 128) in the form of a string with no spaces to avoid errors. That will look like:

```
string exampleKey = "00110001001100100011001100110100001101010011011000110111001110000011100100110000001100010011001000110011001101000011010100110110";

AES::aes_init(OPTIONS::noGenerateKey, exampleKey)
```

#### `encrypt()`

This is **the** encryption function for AES, it has 1 parameter and that is the string input. The function returns the encrypted string so it would be used like this:

```
string example_str = "Test String";
string encrypted_str = AES::encrypt(example_str);
```

The string it outputs is in binary due to certian issues with outputting hex or ASCII characters, I'll look into that in the future. 

#### `decrypt()`

This is **the** decrypt function for AES, it too has 1 parameter just like `AES::encrypt()` input the binary string that the encrypt function outputted and make sure the key is the same and you'll have your original text. 

```
string example_str = "0010101000110110"; //Not an actual encrypted string
string decrypted_str = "aa";
```


### Files 

#### `encryptFF()`

This has one parameter and thats the **path** to the file, make sure that is correct otherwise it will return false, if it correct it will return true. Example:

```
string path = "C://Users/me/text.txt";
encryptFF(path);
```

#### `decryptFF()`

This works the exact same way as `encryptFF()`, if the path is wrong it returns false otherwise it returns true. Example:

```
string path = "C://Users/me/text.txt";
decryptFF(path);
```

If you were wondering the FF means from file. Also, this only works with .txt file so please only use those.

### Whole File Encryption


#### `encryptFile(string path, string password, FILE_FLAGS flags)`
Works just like `encryptFF()` with parameters except this genrates a new file with extension `.aesenc`

Usage is the exact same as well, just input the path to the file you want encrypted.

This also generates a key File in the same directory, it encrypts the key with a ripoff RSA encryption I call AKARE, and it does compress the key afterwards. The first line in the keyfile is the public key, the second line is the compressed encrypted key for the AES encrypted file. It is stored in a file like so... `{original file name} + {_KEYFILE}.aeskey`

#### `decryptFile(string path, string password, string keyFilePath = "", FILE_FLAGS)`
Same as `decryptFF` except you will need to input the path to the `.aesenc` file that you want decrypted. The decrypted file will be under the same name except with the extenstion `.txt`.

The only difference is that there is a second overloaded parameter...

`decryptFile(path, keyFilePath)`

If the key file is not in the same directory as the first path/the file being decrypted, specify it in the second param. 

#### **FLAG USAGE**
To use flags on `encryptFile()` simply do this:
```
encryptFile(pathStr, AES::FILE_FLAGS::deleteInputFile);
```
The default is nothing, so if you leave the 2nd param blank it's fine

To use flags on `decryptFile()` there is a tiny bit more to it.

Delete keyfile:
`decryptFile(pathStr, AES::FILE_FLAGS::deleteKeyFile);`

Delete .aesenc file:
`decryptFile(pathStr, "", AES::FILE_FLAGS::deleteAesencFile);`

Do both:
```
decryptFile(pathStr, "", (AES::FILE_FLAGS)(AES::FILE_FLAGS::deleteAesencFile | AES::FILE_FLAGS::deleteKeyFile));
```

## Version Header

#### `VERSION_INFO::VERSION`
Gets version

#### `VERSION_INFO::REVISION_DATE`
Gets the date of the last update

#### `VERSION_INFO::GITLINK`
Returns github link to this project

##

### Notes

###### Apparently replit keeps adding a main file, that isn't the main.cpp file and im not to sure of it's purpose to please disreguard it.