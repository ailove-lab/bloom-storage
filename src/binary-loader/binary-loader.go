package main

import (
	"encoding/base64"
	"fmt"
	"github.com/mediocregopher/radix.v2/redis"
	"os"
	"strings"
	"time"
)

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

		key, err := base64.RawURLEncoding.DecodeString(key_val[0])
		if err != nil {
			fmt.Println("Can't decode key", err, key_val)
			continue
		}

		values := strings.Split(key_val[1], " ")
		value := values[len(values)-1]

		segments := strings.Replace(value, "/", " ", -1)
		res = rdb.Cmd("HMGET", "segments", segments)
		if res.Err != nil {
			fmt.Println("Get segments", res.Err)
			continue
		}
		list, err = res.List()
		fmt.Println("Segment codes", list)

		fmt.Println(key, segments)
		rdb.Cmd("HSET", "cache", key, segments)
	}

}

func main() {
	for i := 0; i < 10; i++ {
		go loader()
	}
	for {
		time.Sleep(60 * time.Second)
	}
}
