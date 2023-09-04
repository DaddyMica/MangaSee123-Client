# Client written by mica too ahlie
# nobody tryna help LOL!
# TODO: update client to wrap around new api functions
# TODO: get more devs to help with wrapper objects
# TODO: rewrite V0.0.1
# TODO: rewrite in another language once proto's done?
# imports
from __future__ import annotations
from   typing   import Union
# file imports
from modules.encryption import MicaEncryption
# for http requests
import requests, json, base64
# VERSION -> 1.0.2 -- NOTE: Added New Api Encryption/Decryption support
class MangaSeeClient: # mainly verification shit in the constructor
    def __init__(self: MangaSeeClient, tunnel:str, auth_key: str, api_key: str=None):
        # Official MangaSee123Api Wrapper!
        if bool(auth_key):
            try:
                self.auth_key  = auth_key
                self.encryptor = MicaEncryption(16)
            except Exception as _:
                pass
        # 2 way handshake to get new api key back
        if not api_key:
            if tunnel:
                try: # request into gateway endpoint for new api key
                    res  = requests.get(f"{tunnel}/api/debug/encryption-tunnel", json={"auth_key": self.auth_key})
                    # check resposne
                    if res.status_code < 204:
                        config = res.json()
                        # ya digg
                        encryption_key    = base64.b64decode(bytes(config['encoded_key'],     "UTF-8")) # tedious as fucc 
                        nonce             = base64.b64decode(bytes(config['encoded_nonce'],   "UTF-8"))
                        encrypted_payload = base64.b64decode(bytes(config['encoded_payload'], "UTF-8"))
                        tag               = base64.b64decode(bytes(config['encoded_tag'],     "UTF-8"))
                        # decrypt payload now
                        decrypted_palyoad = json.loads(self.encryptor.decrypt_bytes(encrypted_payload, encryption_key, nonce, tag))

                        if bool(decrypted_palyoad.get("test")):
                            print("[*] Decrypted Initial Handshake Payload [*]")
                            self.__setattr__("encryption_key", encryption_key) # add key to instance for future decryption
                            # now check gateway endpoint for encrypted api key
                            res2 = requests.get(f"{tunnel}/api/gateway/authorize", json={"auth_key": self.auth_key})
                            # check res
                            if res2.status_code < 204:
                                config = res2.json()
                                # ya digg
                                encrypted_api_key = base64.b64decode(bytes(config['encoded_api_key'], "UTF-8"))
                                nonce             = base64.b64decode(bytes(config['encryption']['nonce'], "UTF-8"))
                                tag               = base64.b64decode(bytes(config['encryption']['tag'], "UTF-8"))
                                decrypted_api_key = self.encryptor.decrypt_bytes(encrypted_api_key, self.encryption_key, nonce, tag)

                                if decrypted_api_key:
                                    print(f"[*] Decrypted New Api key: {decrypted_api_key} [*]")
                                # add to instance
                                self.verified_session = config['authenticated']
                                self.uuid             = config['user_id']
                                self.api_key          = decrypted_api_key
                except:
                    pass

        if self.api_key:
            res2 = requests.get(f"{tunnel}/api/ping", json={"api_key": self.api_key, "uuid": self.uuid})
            # check response
            if res2.status_code < 204:
                self.test = res2.json()['test'] # for debugging shit

    def get_page(self: MangaSeeClient, title: str, chapter: int, page: int) -> Union[str, None]:
        # example function for cloning usage
        # written by nebula
        # request into the api
        res = requests.get(f"{self.tunnel}/api/mangas/{title}/chapters/{chapter}/pages/{page}", json={"api_key": self.api_key, "uuid": self.uuid})
        # check response
        if res.status_code < 204:
            return res.json().get(f"{title}-{chapter}-{page}", None)

    def download_page(self: MangaSeeClient, title: str, chapter: int, page: int, path: str) -> None:
        # example function                                                      # can be .png, .jpeg, .jpg
        # written by mica
        res = requests.get(f"{self.tunnel}/api/mangas/{title}/chapters/{chapter}/pages/{page}", json={"api_key": self.api_key, "uuid": self.uuid})
        # check
        if res.status_code < 204:
            url = res.json()[f"{title}-{chapter}-{page}"]
            # request into url for byte content
            res2 = requests.get(url)
            # write to disc
            with open(path, "wb") as manga_page:
                manga_page.write(res2.content)
        # for ez access
        return path
