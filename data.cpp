#include "data.h"

std::string data::sha224(const std::string& data)
{
    unsigned char digest[SHA224_DIGEST_SIZE + 1];
    __sha224__(reinterpret_cast<const unsigned char * const>(
        data.data()),
        data.size(),
        digest);
    return std::string(reinterpret_cast<char*>(digest));
}

std::string data::sha256(const std::string& data)
{
    unsigned char digest[SHA256_DIGEST_SIZE + 1];
    __sha256__(reinterpret_cast<const unsigned char * const>(
        data.data()),
        data.size(),
        digest);
    return std::string(reinterpret_cast<char*>(digest));
}

std::string data::sha384(const std::string& data)
{
    unsigned char digest[SHA384_DIGEST_SIZE + 1];
    __sha384__(reinterpret_cast<const unsigned char * const>(
        data.data()),
        data.size(),
        digest);
    return std::string(reinterpret_cast<char*>(digest));
}

std::string data::sha512(const std::string& data)
{
    unsigned char digest[SHA512_DIGEST_SIZE + 1];
    __sha512__(reinterpret_cast<const unsigned char * const>(
        data.data()),
        data.size(),
        digest);
    return std::string(reinterpret_cast<char*>(digest));
}

static const EVP_CIPHER * get_cipher(int algorithm)
{
    const EVP_CIPHER * result;
    switch (algorithm)
    {
    case data::crypto_algorithms::AES_128_CBC:
        result = EVP_aes_128_cbc();
        break;
    case data::crypto_algorithms::AES_128_CCM:
        result = EVP_aes_128_ccm();
        break;
    case data::crypto_algorithms::AES_128_GCM:
        result = EVP_aes_128_gcm();
        break;
    case data::crypto_algorithms::AES_256_CBC:
        result = EVP_aes_256_cbc();
        break;
    case data::crypto_algorithms::AES_256_CCM:
        result = EVP_aes_256_ccm();
        break;
    case data::crypto_algorithms::AES_256_GCM:
        result = EVP_aes_256_gcm();
        break;
    case data::crypto_algorithms::CHACHA20:
        result = EVP_chacha20();
        break;
    case data::crypto_algorithms::CHACHA20_POLY1305:
        result = EVP_chacha20_poly1305();
        break;
    default:
        result = nullptr;
        break;
    }

    return result;
}

void data::crypto_context::encrypt(
    const std::vector<uint8_t>& plaintext, 
    std::vector<uint8_t>& ciphertext)
{
    // TODO: figure out what the best possible initial size is
    ciphertext.resize(2 * plaintext.size());
    int size, total;

    const EVP_CIPHER * cipher = get_cipher(_algorithm);
    
    EVP_EncryptInit(
        _ctx.get(),
        cipher,
        reinterpret_cast<const unsigned char*>(
            _key.c_str()),
        reinterpret_cast<const unsigned char*>(
            _iv.c_str()));
    
    EVP_DecryptUpdate(
        _ctx.get(),
        ciphertext.data(),
        &size,
        plaintext.data(),
        plaintext.size());

    total = size;

    EVP_EncryptFinal(
        _ctx.get(),
        ciphertext.data() + size, 
        &size);
    
    total += size;

    ciphertext.resize(total);
}

void data::crypto_context::decrypt(
    const std::vector<uint8_t>& ciphertext, 
    std::vector<uint8_t>& plaintext)
{
    // len(pt) <= len(ct)
    plaintext.resize(ciphertext.size());
    int size,total;

    const EVP_CIPHER * cipher = get_cipher(_algorithm);
    
    EVP_DecryptInit(
        _ctx.get(),
        cipher,
        reinterpret_cast<const unsigned char*>(
            _key.c_str()),
        reinterpret_cast<const unsigned char*>(
            _iv.c_str()));
    
    EVP_DecryptUpdate(
        _ctx.get(),
        plaintext.data(),
        &size,
        ciphertext.data(),
        ciphertext.size());

    total = size;

    EVP_DecryptFinal(
        _ctx.get(),
        plaintext.data() + size,
        &size);
    
    total += size;

    plaintext.resize(total);
}

data::record_manager::record_manager(
    const std::string &root_directory, 
    const std::function<std::string (const std::string &)> &hash_function) : 
        _root_directory(root_directory),
        _hash_function(hash_function)
{

}

data::default_record_manager::default_record_manager(
    const std::string &root_directory, 
    const std::function<std::string (const std::string &)> &hash_function) : 
        record_manager(root_directory,hash_function)
{

}

void data::default_record_manager::load(
    const std::string& path, 
    std::vector<uint8_t>& buffer)
{
    std::fstream file(_hash_function(path), std::ios::in);
    std::vector<uint8_t> chunk(1024);    
    std::vector<uint8_t> data;
    while(file.good())
    {
        file.read(
            reinterpret_cast<char*>(
                chunk.data()),
            chunk.size());
        data.insert(
            data.begin(),
            chunk.begin(),
            chunk.begin() + file.gcount());
    }
    buffer = std::move(data);
}

void data::default_record_manager::store(
    const std::string& path, 
    const std::vector<uint8_t>& buffer)
{
    std::fstream file(_hash_function(path), std::ios::out);
    file.write(
        reinterpret_cast<const char*>(
            buffer.data()),buffer.size());
}

data::crypto_record_manager::crypto_record_manager(
    const std::string &root_directory, 
    const std::shared_ptr<data::crypto_context>& security_profile,
    const std::function<std::string (const std::string &)> &hash_function) : 
        record_manager(root_directory,hash_function), 
        _security_profile(security_profile)
{

}

void data::crypto_record_manager::load(
    const std::string& path, 
    std::vector<uint8_t>& buffer)
{

}

void data::crypto_record_manager::store(
    const std::string& path, 
    const std::vector<uint8_t>& buffer)
{

}

data::ubiq_record_manager::ubiq_record_manager(
    const std::string &root_directory,
    const std::shared_ptr<ubiq::platform::credentials>& security_profile,   
    const std::function<std::string (const std::string &)> &hash_function) : 
        record_manager(root_directory,hash_function), 
        _security_profile(security_profile)
{

}

void data::ubiq_record_manager::load(
    const std::string& path, 
    std::vector<uint8_t>& buffer)
{
    std::fstream file(_hash_function(path), std::ios::in);
    std::vector<uint8_t> chunk(1024);    
    std::vector<uint8_t> data;
    while(file.good())
    {
        file.read(
            reinterpret_cast<char*>(
                chunk.data()),
            chunk.size());
        data.insert(
            data.begin(),
            chunk.begin(),
            chunk.begin() + file.gcount());
    }
    buffer = std::move(
        ubiq::platform::decrypt(
            *_security_profile,
            data.data(),
            data.size()));
}

void data::ubiq_record_manager::store(
    const std::string& path, 
    const std::vector<uint8_t>& buffer)
{
    std::fstream file(_hash_function(path), std::ios::out);
    std::vector<uint8_t> data;
    if(file.good())
    {
        data = std::move(
            ubiq::platform::encrypt(
                *_security_profile,
                buffer.data(),
                buffer.size()));
        file.write(
            reinterpret_cast<const char*>(
                buffer.data()),buffer.size());
    }
}