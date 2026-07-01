#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

using namespace std;

class AbdugrmCrypto {
private:
    unsigned char key[32]; // 256-bit key for AES
    unsigned char iv[16];  // 128-bit IV
    AES_KEY aesKey;
    RSA *rsa;

public:
    AbdugrmCrypto() {
        // Initialize with random values
        if (!RAND_bytes(key, sizeof(key))) {
            cerr << "Error generating random key" << endl;
        }
        if (!RAND_bytes(iv, sizeof(iv))) {
            cerr << "Error generating random IV" << endl;
        }
        rsa = NULL;
    }

    ~AbdugrmCrypto() {
        if (rsa) {
            RSA_free(rsa);
        }
    }

    // Encrypt message with AES-256-CBC
    vector<unsigned char> encryptAES256(const string& plaintext) {
        AES_KEY encKey;
        AES_set_encrypt_key(key, 256, &encKey);

        vector<unsigned char> ciphertext;
        unsigned char outbuf[plaintext.size() + AES_BLOCK_SIZE];
        int outlen = 0;

        AES_cbc_encrypt((unsigned char*)plaintext.c_str(), outbuf, plaintext.size(),
                       &encKey, iv, AES_ENCRYPT);

        // Add IV to beginning of ciphertext
        ciphertext.insert(ciphertext.end(), iv, iv + 16);
        ciphertext.insert(ciphertext.end(), outbuf, outbuf + plaintext.size());

        return ciphertext;
    }

    // Decrypt message with AES-256-CBC
    string decryptAES256(const vector<unsigned char>& ciphertext) {
        if (ciphertext.size() < 16) {
            cerr << "Ciphertext too short" << endl;
            return "";
        }

        AES_KEY decKey;
        AES_set_decrypt_key(key, 256, &decKey);

        unsigned char outbuf[ciphertext.size() - 16];
        unsigned char localIv[16];
        memcpy(localIv, ciphertext.data(), 16);

        AES_cbc_encrypt(ciphertext.data() + 16, outbuf, ciphertext.size() - 16,
                       &decKey, localIv, AES_DECRYPT);

        return string((char*)outbuf, ciphertext.size() - 16);
    }

    // Generate RSA key pair
    bool generateRSAKeyPair(int keySize = 4096) {
        RSA *rsa_temp = RSA_new();
        BIGNUM *e = BN_new();
        BN_set_word(e, RSA_F4);

        if (RSA_generate_key_ex(rsa_temp, keySize, e, NULL) != 1) {
            cerr << "Error generating RSA key pair" << endl;
            RSA_free(rsa_temp);
            BN_free(e);
            return false;
        }

        if (rsa) RSA_free(rsa);
        rsa = rsa_temp;
        BN_free(e);
        return true;
    }

    // RSA Encrypt
    vector<unsigned char> encryptRSA(const string& plaintext) {
        if (!rsa) {
            cerr << "RSA key not initialized" << endl;
            return vector<unsigned char>();
        }

        vector<unsigned char> ciphertext(RSA_size(rsa));
        int ciphertext_len = RSA_public_encrypt(plaintext.size(),
                                               (unsigned char*)plaintext.c_str(),
                                               ciphertext.data(), rsa,
                                               RSA_PKCS1_OAEP_PADDING);

        if (ciphertext_len == -1) {
            cerr << "RSA encryption failed" << endl;
            return vector<unsigned char>();
        }

        ciphertext.resize(ciphertext_len);
        return ciphertext;
    }

    // RSA Decrypt
    string decryptRSA(const vector<unsigned char>& ciphertext) {
        if (!rsa) {
            cerr << "RSA key not initialized" << endl;
            return "";
        }

        vector<unsigned char> plaintext(RSA_size(rsa));
        int plaintext_len = RSA_private_decrypt(ciphertext.size(),
                                               (unsigned char*)ciphertext.data(),
                                               plaintext.data(), rsa,
                                               RSA_PKCS1_OAEP_PADDING);

        if (plaintext_len == -1) {
            cerr << "RSA decryption failed" << endl;
            return "";
        }

        return string((char*)plaintext.data(), plaintext_len);
    }

    // SHA-256 Hash
    string hashSHA256(const string& text) {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256((unsigned char*)text.c_str(), text.size(), hash);

        char hashStr[65];
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            sprintf(&hashStr[i*2], "%02x", hash[i]);
        }
        hashStr[64] = '\0';
        return string(hashStr);
    }

    // Export public key
    string exportPublicKey() {
        if (!rsa) {
            cerr << "RSA key not initialized" << endl;
            return "";
        }

        BIO *bio = BIO_new(BIO_s_mem());
        if (PEM_write_bio_RSA_PUBKEY(bio, rsa) != 1) {
            cerr << "Error exporting public key" << endl;
            BIO_free(bio);
            return "";
        }

        char *buffer = NULL;
        long buffer_size = BIO_get_mem_data(bio, &buffer);
        string publicKey(buffer, buffer_size);
        BIO_free(bio);

        return publicKey;
    }

    // Get RSA key size
    int getRSAKeySize() {
        if (!rsa) return 0;
        return RSA_size(rsa) * 8;
    }
};

// C interface for JavaScript interop
extern "C" {
    AbdugrmCrypto* crypto_new() {
        return new AbdugrmCrypto();
    }

    void crypto_delete(AbdugrmCrypto* crypto) {
        delete crypto;
    }

    // AES Encryption
    const char* crypto_encrypt_aes256(AbdugrmCrypto* crypto, const char* plaintext) {
        auto ciphertext = crypto->encryptAES256(string(plaintext));
        char* result = (char*)malloc(ciphertext.size() + 1);
        memcpy(result, ciphertext.data(), ciphertext.size());
        result[ciphertext.size()] = '\0';
        return result;
    }

    // AES Decryption
    const char* crypto_decrypt_aes256(AbdugrmCrypto* crypto, const unsigned char* ciphertext, int len) {
        vector<unsigned char> ct(ciphertext, ciphertext + len);
        string plaintext = crypto->decryptAES256(ct);
        char* result = (char*)malloc(plaintext.size() + 1);
        strcpy(result, plaintext.c_str());
        return result;
    }

    // RSA Key Generation
    int crypto_generate_rsa_key(AbdugrmCrypto* crypto) {
        return crypto->generateRSAKeyPair() ? 1 : 0;
    }

    // SHA256 Hash
    const char* crypto_sha256(AbdugrmCrypto* crypto, const char* text) {
        string hash = crypto->hashSHA256(string(text));
        char* result = (char*)malloc(hash.size() + 1);
        strcpy(result, hash.c_str());
        return result;
    }

    // Get public key
    const char* crypto_get_public_key(AbdugrmCrypto* crypto) {
        string pubkey = crypto->exportPublicKey();
        char* result = (char*)malloc(pubkey.size() + 1);
        strcpy(result, pubkey.c_str());
        return result;
    }

    // Get RSA key size
    int crypto_get_rsa_key_size(AbdugrmCrypto* crypto) {
        return crypto->getRSAKeySize();
    }

    // Free allocated memory
    void crypto_free(const char* ptr) {
        free((void*)ptr);
    }
}

int main() {
    cout << "Abdugram Encryption Module" << endl;
    cout << "OpenSSL Cryptography Engine" << endl;

    AbdugrmCrypto crypto;

    // Test AES-256 encryption
    string message = "Hello, Abdugram!";
    cout << "Original: " << message << endl;

    auto encrypted = crypto.encryptAES256(message);
    cout << "Encrypted (hex): ";
    for (auto byte : encrypted) {
        printf("%02x", byte);
    }
    cout << endl;

    // Test RSA
    if (crypto.generateRSAKeyPair()) {
        cout << "RSA Key Generated: " << crypto.getRSAKeySize() << " bits" << endl;
        cout << "Public Key:" << endl;
        cout << crypto.exportPublicKey() << endl;
    }

    // Test SHA-256
    string hash = crypto.hashSHA256(message);
    cout << "SHA-256 Hash: " << hash << endl;

    return 0;
}
