# Class for securing the api ahlie
# imports
from __future__      import annotations
from   Crypto.Cipher import AES
# ya digg
import secrets
import os
import random
import base64

class MicaEncryption:
    def __init__(self: Encryption, length: int) -> None:
        # class for securing my application
        if bool(length):
            self.__setattr__("length", length)

            try:
                self.__setattr__("key", self.generate_encryption_key())
            except:
                pass


    def generate_api_key(self: Encryption) -> str:
        return secrets.token_urlsafe(self.length)

    def generate_user_id(self: Encryption) -> str:
        # ya digg
        return ''.join([random.choice([str(num) for num in range(10)]) for _ in range(18)])

    def generate_encryption_key(self: Encryption) -> bytes:
        return os.urandom(self.length)

    def encrypt_str(self: Encryption, string: str) -> tuple:
        # function to encrypt bytes -> for Manga-Api V-1.0.2
        new_key = self.key
        cipher  = AES.new(new_key, AES.MODE_EAX)
        nonce   = cipher.nonce

        ciphertext, tag = cipher.encrypt_and_digest(bytes(string, "UTF-8"))

        return nonce, ciphertext, tag

    def decrypt_bytes(self: Encryption, ciphertext: bytes, key: bytes, nonce: bytes, tag: bytes) -> str:
        # function to decrypt bytes for communication
        cipher    = AES.new(key, AES.MODE_EAX, nonce=nonce)
        plaintext = cipher.decrypt(ciphertext)

        try:
            cipher.verify(tag)
            # plaintext is verified
            return self.clean(plaintext)
        except:
            pass

    @staticmethod
    def clean(_bytes: bytes) -> str:
        return str(_bytes).strip("b'")
