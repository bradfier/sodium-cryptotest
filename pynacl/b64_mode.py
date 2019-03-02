import base64

from encrypt_decrypt import encrypt_with_symmetric_key
from encrypt_decrypt import decrypt_with_symmetric_key

if __name__ == '__main__':
    with open('../test_data', 'rb') as fp:
        data = fp.read()
        b64_data = base64.b64encode(data)

        ciphertext, key = encrypt_with_symmetric_key(b64_data)
        b64_ciphertext = base64.b64encode(ciphertext).decode('utf-8')  # base64-ed ciphertext (stringed)

        decrypted = decrypt_with_symmetric_key(base64.decodebytes(b64_ciphertext.encode('utf-8')), key)
        decrypted_bytes = base64.decodebytes(decrypted)

        # Compare
        if data == decrypted_bytes:
            print("Plaintext and decrypted buffer match.")
        else:
            print("Plaintext and decrypted buffer do not match!")

        

