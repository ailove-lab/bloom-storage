package main

import (
	// "encoding/base64"
	"fmt"
	"github.com/mediocregopher/radix.v2/redis"
	"os"
	"strings"
	"time"
	"sync"
	"bytes"
	// "net/http"
)
import _ "net/http/pprof"


var index = map[string][]byte{}
var cache = map[string][]byte{}

var index_mutex = sync.RWMutex{}
var index_code uint32 = 0

func getCode(seg string) []byte {
	
	// read lock
	index_mutex.RLock()
	code := index[seg]
	index_mutex.RUnlock()
	
	if len(code) == 0 {
		//write lock
		index_mutex.Lock()
		// check if another write lock already change this index
		code = index[seg]
		// if it still empty generate new
		if len(code) == 0 {
			code =[]byte{byte((index_code>>8) & 0xFF), byte(index_code & 0xFF)}
			index[seg] = code
			index_code += 1
		}
		index_mutex.Unlock()
	}
	return code
}

func encode(value string) []byte {
	var value_bin bytes.Buffer
	for _, s := range strings.Split(value, "/") {
		value_bin.Write(getCode(s))
	}
	return value_bin.Bytes()
}

// func binariseKey(key string) [16]byte {
// 		var key_bin [16]byte
// 		var err Error 
// 		key_bin, err = [16]byte(base64.RawURLEncoding.DecodeString(key_val[0]))
// 		if err != nil {
// 			fmt.Println("Can't decode key", err, key_val)
// 			return nil
// 		}
// 		return key_bin

// }

func loader() {

	rdb, err := redis.Dial("tcp", "localhost:6379")
	if err != nil {
		fmt.Println("Can not connect to redis db")
		os.Exit(2)
	}

	for {
		res := rdb.Cmd("BRPOP", "buff", 0)
		if res.Err != nil {
			fmt.Println("Can't BRPOP buff", res.Err)
			continue
		}
		list, err := res.List()
		if err != nil {
			fmt.Println("Can't read response", err)
			continue
		}
		entry := list[1]
		key_val := strings.Split(entry, "\t")

		// key_bin := binariseKey(key_val[0])

		values := strings.Split(key_val[1], " ")
		value := values[len(values)-1]

		value_bin := encode(value)

		// res = rdb.Cmd("HMSET", "cache", key_bin, value_bin)
		// if res.Err != nil {
		// 	fmt.Println(res.Err)
		// }

		cache[key_val[0]] = value_bin
	}
}

func main() {
	
	// go func() {
	// 	fmt.Println(http.ListenAndServe("localhost:6060", nil))
	// }()

	for i := 0; i < 100; i++ {
		go loader()
	}
	for {
		time.Sleep(60 * time.Second)
	}
}
