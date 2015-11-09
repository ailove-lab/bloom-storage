package main

import (
	"bufio"
	// "fmt"
	"log"
	"os"
	"strings"
	"github.com/boltdb/bolt"
)

func main() {

    db, err := bolt.Open("../../data/bolt.db", 0600, nil)
    if err != nil {
        log.Fatal(err)
    }
    defer db.Close()

	file, err := os.Open("../../data/data-sample-1G")
	if err != nil {
		log.Fatal(err)
	}
	defer file.Close()
	buff := []byte("buff")

    err = db.Batch(func(tx *bolt.Tx) error {
        bucket, err := tx.CreateBucketIfNotExists(buff)
        if err != nil {
            return err
        }

        scanner := bufio.NewScanner(file)
		for scanner.Scan() {
			key_val := strings.Split(scanner.Text(), "\t")
			if len(key_val) == 2 {
				// fmt.Print("LPUSH ", key_val[0], " \"", key_val[1], "\"\r\n")
        		err = bucket.Put([]byte(key_val[0]), []byte(key_val[1]))
			}
		}

		if err := scanner.Err(); err != nil {
			log.Fatal(err)
			return err
		}

        return nil
    })


}
