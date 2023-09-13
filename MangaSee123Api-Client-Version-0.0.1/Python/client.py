# Python MangaSee123Api Wrapper
# TODO: update client to wrap around new api functions
# TODO: get more devs to help with wrapper objects
# imports
from __future__  import annotations
from   typing    import Union, List, Iterator, Iterable, Any, overload
from   functools import cached_property
# file imports
import builtins 
import requests
import json
import base64
import itertools
# UPGRADED on -> 9/10/2023
# VERSION     -> 1.0.3 --

class MicaDict(dict): # custom dict class
    def iterate(self: MicaDict) -> Iterator:
        return zip(self.keys(), self.values())

    def mica_update(self: MicaDict, *args: Iterable[Any], **kwargs: Iterable[Any]) -> None:
        # Needed a better dict class yfm
        if args:
            for obj in args:
                for [key, value] in obj.items():
                    self.update({key: value})

        if kwargs:
            for [key, value] in kwargs.items():
                self.update({key: value})
# Setup
builtins.dict = MicaDict

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
            self.tunnel  = tunnel
            # check headers
            if self.check_api_key():
                self.verified_session = True
                self.host             = host_addr

    @cached_property
    def __headers(self: MangaSeeClient) -> dict:
        # cached headers property
        return {"api_key": self.api_key, "uuid": self.uuid}

    def get_page(self: MangaSeeClient, title: str, chapter: int, page: int) -> Union[str, None]:
        # example function for cloning usage
        # written by nebula
        # request into the api
        res = requests.get(f"{self.tunnel}/api/mangas/{title}/chapters/{chapter}/pages/{page}", json=self.__headers)
        # check response
        if res.status_code < 204:
            config = res.json()
            # get the url
            return config.get("url", None)

    def __str__(self: MangaSeeClient) -> str:
        # str dunder
        return f"api_key={self.api_key}, uuid={self.uuid}, proxy={self.host}"

    def __repr__(self: MangaSeeClient) -> str:
        return self.api_key
