#include "Enclave_t.h"
#include <string.h>
#include <stdio.h>

#define BUFSIZ 100

int o_printf(const char* fmt, ...)
{
    char buf[BUFSIZ] = { '\0' };
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, BUFSIZ, fmt, ap);
    va_end(ap);
    ocall_print(buf);
    return (int)strnlen(buf, BUFSIZ - 1) + 1;
}


int generate_random_number() {
    ocall_print("Processing random number generation...");
    return 42;
}

#define SIGMA_SESSION_PRIVKEY_LENGTH   32
//!  Length of Ga/Gb, in byte.
#define SIGMA_SESSION_PUBKEY_LENGTH    64
//!  Length of ICV using HMAC256in byte.
#define SIGMA_HMAC_SHA256_HASH_LENGTH  32


void test_ecdsa_sign()
{
    sgx_ecc_state_handle_t sigma_ecc_handle = NULL;
    sgx_status_t sgx_status = sgx_ecc256_open_context(&sigma_ecc_handle);
    if (SGX_ERROR_OUT_OF_MEMORY == sgx_status)
        ocall_print("create open context error \n");

    /* Get private key b and public key g^b, in little-endian format */
    uint8_t Publickey_little_endian[SIGMA_SESSION_PUBKEY_LENGTH];
    uint8_t Privatekey_b_little_endian[SIGMA_SESSION_PRIVKEY_LENGTH];
    if (SGX_SUCCESS != sgx_ecc256_create_key_pair((sgx_ec256_private_t *)Privatekey_b_little_endian,
                (sgx_ec256_public_t*)Publickey_little_endian, sigma_ecc_handle))
        ocall_print("create key pair error\n");

    const uint8_t p_data[] = "happy day";
    sgx_ec256_private_t p_private;
    sgx_ec256_signature_t p_signature;
    memset((unsigned char *)&p_private, 0, sizeof(sgx_ec256_private_t));

    //p_private all zero will make sgx_ecdsa_sign function failed
    sgx_status = sgx_ecdsa_sign( p_data, strlen((const char*)p_data), &p_private,  &p_signature, sigma_ecc_handle);
    o_printf("sgx_ecdsa_sign = %d\n", sgx_status);
    sgx_status = sgx_ecdsa_sign( p_data, strlen((const char*)p_data), (sgx_ec256_private_t *)Privatekey_b_little_endian,  &p_signature, sigma_ecc_handle);
    o_printf("sgx_ecdsa_sign = %d\n", sgx_status);

    if(sigma_ecc_handle != NULL)
    {
        sgx_ecc256_close_context(sigma_ecc_handle);
    }

    return;
}
