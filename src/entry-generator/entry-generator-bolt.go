package main

import (
	"bytes"
	"encoding/base64"
	"encoding/binary"
	"flag"
	"fmt"
	"math/rand"
	"os"
	"runtime"
	"strconv"
	"strings"
	"time"
	"log"

	"github.com/boltdb/bolt"
)

const SEG_COUNT = 200
const SEG_LETTERS = "0123456789"
const SEG_PER_ENTRY = 4

var segments [SEG_COUNT]string

var rnd = rand.New(rand.NewSource(time.Now().UnixNano()))

func randomSegment() string {
	seg := make([]byte, 8)
	for i := range seg {
		seg[i] = SEG_LETTERS[rnd.Intn(8)]
	}
	return string(seg)
}

func initSegments() {
	for i := range segments {
		segments[i] = randomSegment()
	}
}

func randomKey() string {
	buf := new(bytes.Buffer)
	for i := 0; i <= 3; i++ {
		err := binary.Write(buf, binary.LittleEndian, rnd.Uint32())
		if err != nil {
			fmt.Println("binary.Write failed:", err)
		}
	}
	return base64.RawURLEncoding.EncodeToString(buf.Bytes())
}

func randomValue() string {
	segs := rnd.Intn(SEG_PER_ENTRY*2) + 1
	value := make([]string, segs)
	for i := range value {
		value[i] = segments[rnd.Intn(SEG_COUNT)]
	}
	return "- - - -/- - - - " + strings.Join(value, "/")
}

func randomEntry() string {
	entry := []string{randomKey(), randomValue()}
	return strings.Join(entry, "\t")
}

func usage() {
	fmt.Fprintf(os.Stderr, "entry-generator [number of entries]\n")
	flag.PrintDefaults()
	os.Exit(2)
}

func main() {

	runtime.GOMAXPROCS(runtime.NumCPU())

	flag.Usage = usage
	flag.Parse()

	args := flag.Args()
	if len(args) < 1 {
		fmt.Println("Number of entries is missing")
		os.Exit(1)
	}

	initSegments()
	cnt, err := strconv.Atoi(args[0])
	if err != nil || cnt <= 0 {
		fmt.Println("Number of entries is wrong:", err)
	}

	db, err := bolt.Open("../../data/bolt.db", 0600, nil)
    if err != nil {
        log.Fatal(err)
    }
    defer db.Close()

    buff:= []byte("buff")
    err = db.Batch(func(tx *bolt.Tx) error {
        bucket, err := tx.CreateBucketIfNotExists(buff)
        if err != nil {
            return err
        }
	
		for i := 0; i < cnt; i++ {
			// fmt.Print("LPUSH buff \"", randomEntry(), "\"\r\n")
			err = bucket.Put([]byte(randomKey()), []byte(randomValue()))
			// speed capping 10K per second
			// time.Sleep(1 * time.Microsecond)

			if err != nil {
				log.Fatal(err)
				return err
			}
		}

        return nil
    })



}
