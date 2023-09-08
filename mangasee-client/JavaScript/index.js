// made by da boy
const axios = require('axios'); // for http reqs


class MangaSeeClient
{
 checkApiKey(apiKey, uuid)
 { // function to check api key ya digg
   axios.get(`${this.tunnel}/api/ping`, {"api_key": apiKey, "uuid": uuid})
      .then((res) => {
          if (res.status < 204)
            return true;
      })

      .catch((err) => {
        return false;
      })
 }

  constructor(tunnel, authKey, apiKey=null, uuid=null)
  { // Official manga-see api wrapper in js!
      // made by da kid
      axios.get("https://api.myip.com/")
        .then((res) => {
          // get the host addr first
          if (res.status < 204)
            this.host = res.data.ip;
        })
        // ya digg
        .catch((err) => {
            if (err)
              console.error(err);
        })

      if (!apiKey)
      {
        identifyPayload = {
            "auth_key": authKey,
            "client": "js-0.0.1",
            "host": this.host,
            "os": "win"
        }
        // send identifyPayload with axios request
        axios.get(`${tunnel}/api/gateway/authorize`, identifyPayload)
          .then((res) => {
            if (res.status < 204) {
              // add data to instance type shit
              this.apiKey          = res.data.api_key;
              this.uuid            = res.data.uuid;
              this.verifiedSession = res.data.authenticated;
              this.hostStatus      = res.data.host_status;
              this.authKey         = authKey;
              this.tunnel          = tunnel;

            }
          })

          .catch((err) => {
              if (err)
                console.error(err);
          })
      } else {
        // if they do got an api key...
        // check api key n shit
        if (uuid) {
          this.tunnel = tunnel;
          // check
          if (this.checkApiKey(apiKey, uuid))
          {
            this.apiKey          = apiKey;
            this.uuid            = uuid;
            this.verifiedSession = true;
            this.authKey         = authKey;
            this.hostStatus      = false; // unable to verify host
          }
        }
      }
  }
}
