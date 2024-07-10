#pragma  once
#include  <iostream>
#include  <memory>
#include  <iomanip>
#include  <openssl/sha.h>

template < class T >
std::string createDigestArray(
        const T* data_,
        const size_t data_size_,
        std::array<u_char, SHA512_DIGEST_LENGTH>& hash_
)
{
    std::stringstream ss_digest;
    std::unique_ptr< SHA512_CTX > context
            = std::make_unique<SHA512_CTX>();

    SHA512_Init(context.get());

    SHA512_Update(
            context.get(),
            static_cast<const void *>(data_),
            data_size_
    );

    SHA512_Final(hash_.data(), context.get());

    ss_digest << std::setfill('0') << std::hex;
    for(auto it : hash_) {
        ss_digest << std::setw(2) << static_cast<int>(it);
    }

    return ss_digest.str();
}
