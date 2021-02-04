#include <fstream>
#include <functional>
#include <string>
#include <unordered_set>
#include <vector>
#include <ubiq/platform.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include "sha2.h"

namespace data
{
    std::string sha224(const std::string& data);

    std::string sha256(const std::string& data);
    
    std::string sha384(const std::string& data);
    
    std::string sha512(const std::string& data);

    class record_manager
    {
    public:
        record_manager(
            const std::string& root_directory,
            const std::function<std::string(const std::string&)>& hash_function = sha256);
        
        virtual void load(const std::string& path, std::vector<uint8_t>& buffer) = 0;
        
        virtual void store(const std::string& path, const std::vector<uint8_t>& buffer) = 0;

    protected:
        std::string _root_directory;
        std::function<std::string(const std::string&)> _hash_function;
    };

    class default_record_manager : public record_manager
    {
    public:
        default_record_manager(
            const std::string& root_directory,
            const std::function<std::string(const std::string&)>& hash_function = sha256);

        void load(const std::string& path, std::vector<uint8_t>& buffer) override;
        
        void store(const std::string& path, const std::vector<uint8_t>& buffer) override;
    };

    enum crypto_algorithms
    {
        AES_128_GCM,
        AES_128_CBC,
        AES_128_CCM,
        AES_256_GCM,
        AES_256_CBC,
        AES_256_CCM,
        CHACHA20,
        CHACHA20_POLY1305
    };

    class crypto_context
    {
    public:
        crypto_context( 
            const std::string& key, 
            const std::string& iv,
            int algorithm = AES_256_GCM);

        ~crypto_context();

        void encrypt(
            const std::vector<uint8_t>& plaintext, 
            std::vector<uint8_t>& ciphertext);

        void decrypt(
            const std::vector<uint8_t>& ciphertext, 
            std::vector<uint8_t>& plaintext);

    private:
        std::unique_ptr<EVP_CIPHER_CTX,std::function<void(EVP_CIPHER_CTX*)>> _ctx;
        std::string _key;
        std::string _iv;
        int _algorithm;
    };

    class crypto_record_manager : public record_manager
    {
    public:
        crypto_record_manager(
            const std::string& root_directory,
            const std::shared_ptr<crypto_context>& security_profile,
            const std::function<std::string(const std::string&)>& hash_function = sha256);

        void load(const std::string& path, std::vector<uint8_t>& buffer) override;
        
        void store(const std::string& path, const std::vector<uint8_t>& buffer) override;
    private:
        std::shared_ptr<crypto_context> _security_profile;
    };
    
    class ubiq_record_manager : public record_manager
    {
    public:
        ubiq_record_manager(
            const std::string& root_directory,
            const std::shared_ptr<ubiq::platform::credentials>& security_profile,
            const std::function<std::string(const std::string&)>& hash_function = sha256);

        void load(const std::string& path, std::vector<uint8_t>& buffer) override;
        
        void store(const std::string& path, const std::vector<uint8_t>& buffer) override;
    private:
        std::shared_ptr<ubiq::platform::credentials> _security_profile;
    };
}