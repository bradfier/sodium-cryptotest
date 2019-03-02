def encrypt_with_symmetric_key(plaintext):
    import nacl.secret
    import nacl.utils

    # Generate a random key
    key = nacl.utils.random(nacl.secret.SecretBox.KEY_SIZE)

    box = nacl.secret.SecretBox(key)
    return box.encrypt(plaintext), key


def decrypt_with_symmetric_key(ciphertext, key):
    import nacl.secret
    import nacl.utils

    box = nacl.secret.SecretBox(key)
    return box.decrypt(ciphertext)


if __name__ == '__main__':
    with open('../test_data', 'rb') as fp:
        data = fp.read()
        ciphertext, key = encrypt_with_symmetric_key(data)
        plaintext = decrypt_with_symmetric_key(ciphertext, key)

        # Compare
        if data == plaintext:
            print("Plaintext and decrypted buffer match.")
        else:
            print("Plaintext and decrypted buffer do not match!")
