# import the module
from __future__ import annotations
# expand
import sys
# ya digg
sys.path.append("../")
# file imports
from client             import MangaSeeClient
from modules.encryption import MicaEncryption

class Main:
    def main(self: Main):
        # main example function
        auth_key  = "rYY8UAPTYf9ieRnqdS8Rlg" # 16 byte long example key
        tunnel    = "http://127.0.0.1:80"  # example tunnel but normally it would be an ngrok tunnel
        # new client
        ms_client = MangaSeeClient(tunnel, auth_key)
        # type shit
        verified = ms_client.verified_session
        api_key  = ms_client.api_key
        uuid     = ms_client.uuid
        key      = ms_client.encryption_key

        # DBEUG: print(verified, api_key, uuid)

if __name__ == "__main__":
    Main().main()
