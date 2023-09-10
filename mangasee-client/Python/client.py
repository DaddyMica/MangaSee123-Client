# Client written by mica too ahlie
# nobody tryna help LOL!
# TODO: update client to wrap around new api functions
# TODO: get more devs to help with wrapper objects
# TODO: rewrite V0.0.1
# TODO: rewrite in another language once proto's done?
# imports
from __future__  import annotations
from   typing    import Union, List
from   functools import cached_property
# file imports\
# for http requests
import requests, json, base64
# VERSION -> 1.0.2 -- NOTE: Added New Api Encryption/Decryption support
class MangaSeeClient: # mainly verification shit in the constructor
    def __init__(self: MangaSeeClient, tunnel:str, auth_key: str, api_key: str=None, uuid: str=None):
        # Official MangaSee123Api Wrapper!
        if bool(auth_key):
            try:
                self.auth_key  = auth_key
                self.encryptor = MicaEncryption(16)
            except Exception as _:
                pass

        # start the handshake to fully connect & authorize client
        host_addr = requests.get("https://api.myip.com/").json()['ip']
        # if not api_key, get new api_key
        if not api_key:
            self.host    = host_addr
            # craft new payload
            payload = { # identify payload
                "client": "py-0.0.3",
                "host": self.host,
                "os": "win",
                "auth_key": auth_key
            }
            # check new headers
            res = requests.get(f"{tunnel}/api/gateway/authorize", json=payload)
            # check response
            if res.status_code < 204:
                response = res.json()
                # ya digg
                self.api_key          = response['api_key']
                self.uuid             = response['uuid']
                self.verified_session = response['authenticated']
                self.host_status      = response['host_status']
                self.tunnel           = tunnel

        elif api_key and uuid:
            self.api_key = api_key
            self.uuid    = uuid
            self.tunnel           = tunnel
            # check headers
            if self.check_api_key():
                self.verified_session = Truel
                self.host             = host_addr

    @cached_property
    def headers(self: MangaSeeClient) -> dict:
        # cached headers property
        return {"api_key": self.api_key, "uuid": self.uuid}

    def check_api_key(self: MangaSeeClient) -> bool:
        # function to check [new] api key basically
        res2 = requests.get(f"{self.tunnel}/api/ping", json=self.headers)
        # check response
        if res2.status_code < 204:
            self.test = res2.json()['test'] # for debugging shit

    def get_page(self: MangaSeeClient, title: str, chapter: int, page: int) -> Union[str, None]:
        # example function for cloning usage
        # written by nebula
        # request into the api
        res = requests.get(f"{self.tunnel}/api/mangas/{title}/chapters/{chapter}/pages/{page}", json=self.headers)
        # check response
        if res.status_code < 204:
            config = res.json()
            # get the url
            return config.get(f"{title}-{chapter}-{page}", None)

    def __str__(self: MangaSeeClient) -> str:
        # str dunder
        return f"api_key={self.api_key}, uuid={self.uuid}, proxy={self.host}"

    def __repr__(self: MangaSeeClient) -> str:
        return self.api_key
