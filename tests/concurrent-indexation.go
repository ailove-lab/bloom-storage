package main

import (
	"fmt"
	"math/rand"
	"time"
	"runtime"
	"sync"
	"sync/atomic"
)

const SEG_LETTERS = "AB"

var rnd = rand.New(rand.NewSource(time.Now().UnixNano()))
var index map[string][]byte = make(map[string][]byte)
var indexMutex = &sync.RWMutex{}
var index_code byte = 0
var encoder_cnt int64 = 0;
var tab = ""

func randomSegment() string {
	seg := make([]byte, 8)
	for i := range seg {
		seg[i] = SEG_LETTERS[rnd.Intn(len(SEG_LETTERS))]
	}
	return string(seg)
}

func encoder(i int,in chan string) {
	for {

		val, ok := <-in
		if !ok { break }
		
		atomic.AddInt64(&encoder_cnt,1)

		indexMutex.RLock()
		code := index[val]
		indexMutex.RUnlock()

		if len(code) == 0 {

			indexMutex.Lock()

			// read again in case 
			// another write lock
			// already set the key
			code = index[val]
			if len(code) == 0 {
				code = []byte{0, index_code}
				index_code += 1
				index[val] = code
			}

			indexMutex.Unlock()
		}
		fmt.Println(val, code)
	}
	runtime.Gosched()
}

func main() {
	in := make(chan string)
	for i := 0; i < 5; i++ {
		go encoder(i, in)
	}
	go func () {
		defer func(){recover()}()
		for {
			in <- randomSegment()
		}
	}()
	time.Sleep(time.Second)
	close(in)
	fmt.Println(encoder_cnt, index_code)
	
}
