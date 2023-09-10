package main

// Made by King Slime
// Nebula helped out a lil icl
// Golang Wrapper around MangaSee123Api -- !!
// --> request to /api/gateway/authorize to get a new api key
// --> request to /api/ping to test api key
// imports
import (
	"bytes"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
)

// you cant get close...only ma squad
// for general shit
type dict map[string]string // I get killed its an inside job...
// struct for new client
type MangaSee123Client struct {
	tunnel          string
	authKey         string
	apiKey          string
	uuid            string
	host            string
	verifiedSession bool
}

// -- Http Request Function --
func GetContent(Uri string, JsonPayload []byte) map[string]any {
	// function to get content
	req, err := http.NewRequest("GET", Uri, bytes.NewBuffer(JsonPayload))
	// set the request headers
	req.Header.Set("Content-Type", "application/json; charset=UTF-8")
	// checks
	if err != nil {
		panic(err)
	}
	// new httpclient ptr
	client := &http.Client{}
	// get response
	response, err := client.Do(req)
	// more checks
	if err != nil {
		panic(err)
	}
	// get response buffer back as json
	resBuffer, _ := ioutil.ReadAll(response.Body)

	var jsonRes map[string]interface{}
	// unmarshal it
	json.Unmarshal(resBuffer, &jsonRes)
	// ya digg
	return jsonRes
}

// -- Map functions --
// needed functions to mutate maps
// This Portion Made by Mica
func update(_map dict, _update dict) dict {
	// ya digg
	for key, value := range _update {
		_map[key] = value
	}

	return _map
}

// got my first gun on my own...
func JsonDump(Args ...dict) string {
	// function to dump json data back into a string buffer
	// make result map to store every arg in
	result := make(map[string]string)
	// iterate through them now
	for _, arg := range Args {
		result = update(result, arg)
	}
	// convert from map back to a string or buffer
	_result, _ := json.Marshal(result)

	return string(_result)
}

// -- Main Client Functions --
// Made by Nebula
func CheckApiKey(apiKey string, uuid string, tunnel string) bool {
	// function to check  apiKey validity
	// made by neb
	// for res-json parsing
	var JsonResonse map[string]any
	var StringContent string = JsonDump(dict{"api_key": apiKey, "uuid": uuid})
	// --> request to /api/ping to test api key
	// --> request to /api/gateway/authorize to get a new api key
	req, err := http.NewRequest(http.MethodGet, fmt.Sprintf("%s/api/ping", tunnel), bytes.NewBuffer([]byte(StringContent)))
	// set the request headers
	req.Header.Set("Content-Type", "application/json; charset=UTF-8")
	// shush
	if err != nil {
		panic(err)
	}
	// see thats that problem
	client := &http.Client{}
	res, _ := client.Do(req)
	body, err := ioutil.ReadAll(res.Body)

	err = json.Unmarshal(body, &JsonResonse)

	if err != nil {
		// Debug: log.Print(string(body))
		panic(err)
	}

	// check json
	if JsonResonse["test"].(bool) {
		return true
	}
	// u ain kno nothin
	return false
}

func GetHost() string {
	// last function of today
	req, err := http.NewRequest(http.MethodGet, "https://api.myip.com/", bytes.NewBuffer([]byte("")))

	if err != nil {
		panic(err)
	}

	client := &http.Client{}
	res, _ := client.Do(req)
	body, err := ioutil.ReadAll(res.Body)
	// new var
	var response map[string]any
	json.Unmarshal(body, &response)

	// u see da monster in me
	return response["ip"].(string)
}

// Official Wrapper Around MangaSee123Api!
func MangaSeeClient(tunnel string, authKey string) MangaSee123Client {
	// create a client
	// start by getting new apikey & uuid
	// ma bruddas
	var host string = GetHost()
	var StringContent string = JsonDump(dict{"auth_key": authKey, "client": "go-0.0.1", "host": host, "os": "win"})
	// make the initial handshake request now
	req, err := http.NewRequest(http.MethodGet, fmt.Sprintf("%s/api/gateway/authorize", tunnel), bytes.NewBuffer([]byte(StringContent)))
	// set the request headers
	req.Header.Set("Content-Type", "application/json; charset=UTF-8")

	if err != nil {
		panic(err)
	}
	// new client pointer
	client := &http.Client{}
	response, _ := client.Do(req)

	// get the data from the initial handshake
	ResponseContent, err := ioutil.ReadAll(response.Body)
	// new var for json parsing
	var Response1 map[string]any
	// parse!!!
	json.Unmarshal(ResponseContent, &Response1)

	// new data!
	var NewApiKey string = Response1["api_key"].(string)
	var uuid string = Response1["uuid"].(string)

	if CheckApiKey(NewApiKey, uuid, tunnel) {
		return MangaSee123Client{
			tunnel:          tunnel,
			authKey:         authKey,
			apiKey:          NewApiKey,
			uuid:            Response1["uuid"].(string),
			host:            host,
			verifiedSession: true,
		}
	}

	return MangaSee123Client{} // empty interface
}

// -- Main Function --
func main() {
	// main function
	// example showcase function
	// Done: initial 2 step handshake and api key gen verification.
	// TODO: write manga wrapped functions
	var tunnel string = "http://127.0.0.1:80" // normally would be a host tunnel but i was debugging on localhost
	var authKey string = "tb_is_johns_son"    // cant give you this lols

	msClient := MangaSeeClient(tunnel, authKey)
	// display new data
	log.Print("[*] New Api Key: ", msClient.apiKey)
	log.Print("[*] New uuid: ", msClient.uuid)
	log.Print("[*] Verified Session: ", msClient.verifiedSession)
}
