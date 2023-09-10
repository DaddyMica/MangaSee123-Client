using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// Http requests
using System.Net.Http;
// For Json Serialization
using Newtonsoft.Json;
using System.Runtime.CompilerServices;
using System.Text.Json.Nodes;
using Newtonsoft.Json.Linq;

namespace MicaWorkSpace
{
    public class MangaSee123Client
    { // MangaSee123Client Class!
        // Vars
        private readonly string? ApiKey;
        private readonly string? AuthKey;
        private readonly string? Tunnel;
        private readonly string? uuid;
        private readonly bool? VerifiedSession;
        // Functions that are needed for the object
        private static bool CheckApiKey(string ApiKey, string _uuid, string Tunnel)
        { // Function needed to check api keys passed into the constructor/api keys in general
            // Made by Nebula chan!
            Object payload = new
            {
                api_key = ApiKey,
                uuid    = _uuid
            };
            // serial. json data 
            string QueryString = JsonConvert.SerializeObject(payload);
            // disposable object moment
            using (HttpClient client = new HttpClient())
            {
                HttpResponseMessage response = client.GetAsync($"{Tunnel}/api/ping/data?{QueryString}").Result;
                // Parse response
                string? _response = response.Content.ReadAsStringAsync().Result;
                dynamic Parsed    = JObject.Parse(_response);

                return Parsed.test;
            }
        }

        private static Object GetHost()
        { // iykyk
            // ya digg 
            // king slime
            using (HttpClient client = new HttpClient())
            { // ma lil nga been tryna trap out of a vacant 
                HttpResponseMessage response = client.GetAsync("https://api.myip.com/").Result;
                string ResponseContent       = response.Content.ReadAsStringAsync().Result;
                dynamic ResponseJson         = JObject.Parse(ResponseContent);

                return new { host = ResponseJson.ip };
            }
        }
        public MangaSee123Client(string Tunnel, string AuthKey, string? ApiKey=null, string?uuid=null)
        { // MangaSee123 Client class for the main Api
            // Made fully by Mica & Nebula
            // Do handshake & get api key in constructor 
            if (ApiKey != null && uuid != null)
            { // check args 
                if (!CheckApiKey(ApiKey, uuid, Tunnel))
                    throw new Exception("invalid api key");
                else 
                    this.ApiKey = ApiKey; this.uuid = uuid; this.AuthKey = AuthKey; this.Tunnel = Tunnel;
            }
            // now we need to create our own api keys n shit 
            using (HttpClient client = new HttpClient())
            {   // new object for json payload
                Object IdentifyPayload = new
                {
                    auth_key = AuthKey,
                    client   = "c",
                    host     = "localhost",
                    os       = "win"
                };
                // stringify it
                string              JsonPayload  = JsonConvert.SerializeObject(IdentifyPayload);
                HttpResponseMessage response     = client.GetAsync($"{Tunnel}/api/gateway/authorize/data?{JsonPayload}").Result;
                string              ResponseCont = response.Content.ReadAsStringAsync().Result;
                dynamic             ParsedObject = JObject.Parse(ResponseCont);
                // check response json 
                if (ParsedObject.authenticated)
                {
                    this.ApiKey          = ParsedObject.api_key;
                    this.uuid            = ParsedObject.uuid;
                    this.VerifiedSession = ParsedObject.authenticated;
                    this.Tunnel          = Tunnel;
                    this.AuthKey         = AuthKey;
                }

                Console.WriteLine("[*] New Api Key: ", this.ApiKey);
                Console.WriteLine("[*] New uuid: ", this.uuid);
                Console.WriteLine("[*] Created MangaSee123-Api Connection Successfull [*]");
            }
        }
    }
}
